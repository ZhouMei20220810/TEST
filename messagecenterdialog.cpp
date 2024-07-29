#include "messagecenterdialog.h"
#include "ui_messagecenterdialog.h"
#include "messagetips.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsDropShadowEffect>

MessageCenterDialog::MessageCenterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageCenterDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("消息中心");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    HttpGetNoticeListInfo(true, 1, 1000);
}

MessageCenterDialog::~MessageCenterDialog()
{
    delete ui;
}

void MessageCenterDialog::on_btnAnnouncement_clicked()
{
    qDebug() << "公告列表";
    ui->btnAnnouncement->setStyleSheet("QPushButton{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->btnActivity->setStyleSheet("QPushButton:hover{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}QPushButton{color:#A9ADB6;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->labelAnnouncement->setStyleSheet("background-color:#505465;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");
    ui->labelActivity->setStyleSheet("background-color:#F4F6FA;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");

    ui->listWidget->clear();
    QListWidgetItem* item = NULL;
    QMap<int, S_NOTICE_INFO>::iterator iter = m_mapNotice.begin();
    for (; iter != m_mapNotice.end(); iter++)
    {
        if (NOTICE_SYSTEM_ANNOUNCEMENT == iter->iType)
        {
            item = new QListWidgetItem(iter->strTitle, ui->listWidget);
            ui->listWidget->addItem(item);
        }
    }
}

void MessageCenterDialog::on_btnActivity_clicked()
{
    qDebug() << "活动列表";
    ui->btnActivity->setStyleSheet("QPushButton{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->btnAnnouncement->setStyleSheet("QPushButton:hover{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}QPushButton{color: #A9ADB6;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->labelActivity->setStyleSheet("background-color:#505465;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");
    ui->labelAnnouncement->setStyleSheet("background-color:#F4F6FA;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");

    ui->listWidget->clear();
    QListWidgetItem* item = NULL;
    QMap<int, S_NOTICE_INFO>::iterator iter = m_mapNotice.begin();
    for (; iter != m_mapNotice.end(); iter++)
    {
        if (NOTICE_ACTIVE == iter->iType)
        {
            item = new QListWidgetItem(iter->strTitle, ui->listWidget);
            ui->listWidget->addItem(item);
        }
    }
}

void MessageCenterDialog::HttpGetNoticeListInfo(bool bIsAuth, int iPage, int iPageSize)
{
    //已授权列表
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_NOTICE_LIST;
    //level不传值,返回该 组下面所有的level
    strUrl += QString::asprintf("?page=%d&pageSize=%d", iPage, iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    m_mapNotice.clear();
    //发出GET请求
    QNetworkReply* reply = manager->get(request);
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
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();                        
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_NOTICE_INFO noticeInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                noticeInfo.iId = recordObj["id"].toInt();
                                noticeInfo.iType = recordObj["type"].toInt();
                                noticeInfo.strTitle = recordObj["title"].toString();
                                noticeInfo.strCreateTime = recordObj["createTime"].toString();
                                noticeInfo.strRemark = recordObj["remark"].toString();
                                noticeInfo.iCreateBy = recordObj["createBy"].toInt();
                                noticeInfo.bIsRead = recordObj["isRead"].toBool();
                                m_mapNotice.insert(i, noticeInfo);
                            }
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

void MessageCenterDialog::on_btnClose_clicked()
{
    this->close();
}

