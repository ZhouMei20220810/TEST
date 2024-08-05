#include "buyhistoryitemwidget.h"
#include "ui_buyhistoryitemwidget.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"

BuyHistoryItemWidget::BuyHistoryItemWidget(S_ORDER_INFO orderInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BuyHistoryItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    m_orderInfo = orderInfo;

    ui->labelOrderNo->setText(orderInfo.strOutTradeNo);
    ui->labelMeal->setText(orderInfo.strOrderTitle);
    //实际支付
    ui->labelAmount->setText(QString::asprintf("%.2f元",orderInfo.fActualAmount));
    ui->labelCreateTime->setText(orderInfo.strCreateTime);
    ui->labelCloudId->setText(QString::asprintf("%d",orderInfo.iId));
    QString strStatus;//0:未支付 1:已支付 2:已完成
    switch (orderInfo.iStatus) {
    case 0:
        strStatus="未支付";
        break;
    case 1:
        strStatus="已支付";
        break;
    case 2:
        strStatus="已完成";
        break;
    default:
        break;
    }
    ui->labelStatus->setText(strStatus);
    ui->labelBuyNum->setText(QString("%1").arg(orderInfo.iBuyNum));
}

BuyHistoryItemWidget::~BuyHistoryItemWidget()
{
    delete ui;
}

void BuyHistoryItemWidget::on_btnClear_clicked()
{
    //删除订单
    HttpDeleteOrder(m_orderInfo.iId);
}

//删除
void BuyHistoryItemWidget::HttpDeleteOrder(int iOrderId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(iOrderId);
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

    QNetworkReply* reply = manager->post(request, "");
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
                bool bData = obj["data"].toBool();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (bData)
                        qDebug() << "删除订单成功";
                    else
                        qDebug() << "删除订单失败";
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