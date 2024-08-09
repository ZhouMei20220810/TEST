#include "transferphonehistorydialog.h"
#include "ui_transferphonehistorydialog.h"
#include <QGraphicsDropShadowEffect>
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "messagetips.h"

void TransferHistoryListItem::setupUI(S_TRANSFER_INFO transferInfo)
{
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QString strStyleSheet = "QLabel{color:#4A4A4A;font-size:12px;}";
    /*m_LabelTransferType = new QLabel(this);
    m_LabelTransferType->setStyleSheet(strStyleSheet);
    m_LabelTransferType->setText(transferInfo.strPhoneNum);
    m_LabelTransferType->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(m_LabelTransferType);*/

    m_LabelTransferAccount = new QLabel(this);
    m_LabelTransferAccount->setStyleSheet(strStyleSheet);
    m_LabelTransferAccount->setText(transferInfo.strTransferAccount);
    m_LabelTransferAccount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(m_LabelTransferAccount);

    m_LabelTransferTime = new QLabel(this);
    m_LabelTransferTime->setStyleSheet(strStyleSheet);
    m_LabelTransferTime->setText(transferInfo.strCreateTime);
    m_LabelTransferTime->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(m_LabelTransferTime);

    m_LabelTransferCount = new QLabel(this);
    m_LabelTransferCount->setStyleSheet(strStyleSheet);
    m_LabelTransferCount->setText(transferInfo.strTransferCount);
    m_LabelTransferCount->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(m_LabelTransferCount);

    m_LabelTransferStatus = new QLabel(this);
    m_LabelTransferStatus->setStyleSheet(strStyleSheet);
    m_LabelTransferStatus->setText(transferInfo.strTransferStatus);
    m_LabelTransferStatus->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hLayout->addWidget(m_LabelTransferStatus);
    hLayout->setSpacing(22);

    //添加到垂直布局
    vLayout->addLayout(hLayout);
};

TransferPhoneHistoryDialog::TransferPhoneHistoryDialog(QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::TransferPhoneHistoryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("转移记录");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidgetTransferHistory->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetTransferHistory->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING*2);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetTransferHistory->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetTransferHistory->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetTransferHistory->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_mapOrderInfo.clear();
    //获取转移记录
    HttpGetTransferHistory(1, 1000);
}

TransferPhoneHistoryDialog::~TransferPhoneHistoryDialog()
{
    delete ui;
}

void TransferPhoneHistoryDialog::on_btnClose_clicked()
{
    this->close();
}

// 订单接口-我的支付订单
void TransferPhoneHistoryDialog::HttpGetTransferHistory(int iPage, int iPageSize)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_TRANSFER_PHONE_HISTORY;
    strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iTotal = data["total"].toInt();
                        int iMaxLimit = data["maxLimit"].toInt();
                        int iCountId = data["countId"].toInt();
                        bool bSearchCount = data["searchCount"].toBool();
                        bool bOptimizeCountSql = data["optimizeCountSql"].toBool();

                        QJsonArray records = data["records"].toArray();
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_TRANSFER_INFO transferInfo;
                            QMap<int, S_TRANSFER_INFO> map;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                //transferInfo.fActualAmount = recordObj["id"].toDouble();
                                transferInfo.strTransferAccount = recordObj["mobile"].toString();
                                transferInfo.strCreateTime = recordObj["createTime"].toString();
                                transferInfo.strTransferCount = QString("%1台").arg(recordObj["snum"].toInt());
                                transferInfo.strTransferStatus = "成功";
                                map.insert(i, transferInfo);
                            }

                            //显示数据
                            ShowTransferInfoList(map);
                        }
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//显示数据
void TransferPhoneHistoryDialog::ShowTransferInfoList(QMap<int, S_TRANSFER_INFO> map)
{
    if (map.size() <= 0)
    {
        MessageTips* tips = new MessageTips("没有转移记录!");
        tips->show();
        return;
    }

    QListWidgetItem* item = NULL;
    TransferHistoryListItem* widget = NULL;
    QMap<int, S_TRANSFER_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetTransferHistory);
        ui->listWidgetTransferHistory->addItem(item);

        widget = new TransferHistoryListItem(iter.value(), this);
        item->setSizeHint(QSize(620, LISTMODE_ITEM_HEGITH));
        ui->listWidgetTransferHistory->setItemWidget(item, widget);
    }
}
