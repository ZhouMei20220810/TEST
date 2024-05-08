#include "smsloginpage.h"
#include "ui_smsloginpage.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

SMSLoginPage::SMSLoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SMSLoginPage)
{
    ui->setupUi(this);
    ui->lineEditPhone->setText("15019445205");
}

SMSLoginPage::~SMSLoginPage()
{
    delete ui;
}

void SMSLoginPage::on_btnPasswordLogin_clicked()
{
    emit showPageType(TYPE_PASSWORDLOGIN_PAGE);
}


void SMSLoginPage::on_btnGetSMSCode_clicked()
{
    //获取验证码
    qDebug() << "获取验证码";
    QString strPhone = ui->lineEditPhone->text();
    if(strPhone.isEmpty())
    {
        QMessageBox::warning(this,"提示","输入手机号码不能为空");
        return;
    }



    //没有响应
   
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_GET_SMS_CODE;
    strUrl += "/?mobiles=" + strPhone;
    //创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    
    QUrl url(strUrl);    
    QUrlQuery query;
    query.addQueryItem("mobiles", strPhone);
    url.setQuery(query.query(QUrl::FullyEncoded));
    //创建请求对象
    QNetworkRequest request;
    request.setRawHeader("Authorization", strPhone.toUtf8());
    request.setUrl(url);

    QNetworkReply* reply = manager->get(request);
    QNetworkReply::NetworkError error = reply->error();
    qDebug() << "reply error=" << error;
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        if (reply->error())
        {
            qDebug() << "获取验证码报错:" << reply->error();
        }
        else
        {
            //读取响应数据
            QByteArray response = reply->readAll();
            qDebug() << response;

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
            if (parseError.error != QJsonParseError::NoError)
            {
                qDebug() << response;
                qWarning() << "Json parse error:" << parseError.errorString();
            }
            else
            {
                if (doc.isObject())
                {
                    QJsonObject obj = doc.object();
                    int iCode = obj["code"].toInt();
                    QString strMessage = obj["message"].toString();
                    qDebug() << "Code=" << iCode << "message=" << strMessage;
                }
            }
        }
        reply->deleteLater();
        });
}

