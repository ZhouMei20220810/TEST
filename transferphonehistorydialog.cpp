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

TransferPhoneHistoryDialog::TransferPhoneHistoryDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferPhoneHistoryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("登录");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

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
                            /*S_ORDER_INFO orderInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                orderInfo.fActualAmount = recordObj["actualAmount"].toDouble();
                                orderInfo.iCreateBy = recordObj["createBy"].toInt();
                                orderInfo.strCreateTime = recordObj["createTime"].toString();
                                orderInfo.iId = recordObj["id"].toInt();
                                orderInfo.bIsDelete = recordObj["isDelete"].toBool();
                                orderInfo.iMemberId = recordObj["memberId"].toInt();
                                orderInfo.strOrderTitle = recordObj["name"].toString();
                                orderInfo.iBuyNum = recordObj["num"].toInt();
                                orderInfo.strOutTradeNo = recordObj["outTradeNo"].toString();
                                orderInfo.strPayTime = recordObj["payTime"].toString();
                                orderInfo.iPayType = recordObj["payType"].toInt();
                                orderInfo.fPreferentialAmount = recordObj["preferentialAmount"].toDouble();
                                orderInfo.strRelateid = recordObj["relateId"].toString();
                                orderInfo.strRemark = recordObj["remark"].toString();
                                orderInfo.iStatus = recordObj["status"].toInt();
                                orderInfo.fTotalAmount = recordObj["totalAmount"].toDouble();
                                orderInfo.strTradeNo = recordObj["tradeNo"].toString();
                                orderInfo.iOrderType = recordObj["type"].toInt();
                                m_mapOrderInfo.insert(i, orderInfo);
                            }*/

                            //显示数据
                            //ShowOrderInfoList();
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
void TransferPhoneHistoryDialog::ShowTransferInfoList()
{
    /*if (m_mapOrderInfo.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("没有购买记录!");
        tips->show();
        return;
    }

    QListWidgetItem* item;
    BuyHistoryItemWidget* itemWidget;
    QMap<int, S_ORDER_INFO>::iterator iter = m_mapOrderInfo.begin();
    for (; iter != m_mapOrderInfo.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetBuyHistory);
        ui->listWidgetBuyHistory->addItem(item);

        itemWidget = new BuyHistoryItemWidget(iter.value(), this);
        item->setSizeHint(QSize(ui->listWidgetBuyHistory->width() - 50, itemWidget->height()));
        ui->listWidgetBuyHistory->setItemWidget(item, itemWidget);
    }*/
}
