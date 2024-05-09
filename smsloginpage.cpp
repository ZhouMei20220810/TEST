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


void SMSLoginPage::on_btnSMSLogin_clicked()
{
    //验证码登录
    qDebug() << "验证码登录";
    QString strPhone = ui->lineEditPhone->text();
    if (strPhone.isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "手机不能为空");
        return;
    }
    QString strSMSCode = ui->lineEditSMSCode->text();
    if (strSMSCode.isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "验证码不能为空");
        return;
    }

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGIN;
    //创建网络访问管理器
    QNetworkAccessManager manager;
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager.post(request, postData);//manager.get(request);
    QNetworkReply::NetworkError error = reply->error();
    qDebug()<<"reply error="<<error;
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished,this, [=] {
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
                    if(200 == iCode)
                    {
                        QString strMessage = obj["message"].toString();
                        qDebug() << "Code=" << iCode << "message=" << strMessage;
                        if (obj["data"].isObject())
                        {
                            QJsonObject data = obj["data"].toObject();
                            QString strToken = data["token"].toString();
                            QString strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            int id = userDetailVO["id"].toInt();
                            QString strName = userDetailVO["name"].toString();
                            QString strAccount = userDetailVO["account"].toString();
                            QString strMobile = userDetailVO["mobile"].toString();
                            qDebug() << "跳转到主页面";
                        }                                                 
                    }
                    else
                    {
                        qWarning()<<"code="<<iCode <<"json:"<<response;
                    }
                }
            }
        }
        reply->deleteLater();
    });
}

