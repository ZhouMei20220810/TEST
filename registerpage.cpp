#include "registerpage.h"
#include "ui_registerpage.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::on_btnRegister_clicked()
{
    //注册
    QString strPhone = ui->lineEditPhone->text();
    QString strSMSCode = ui->lineEditSMSCode->text();
    QString strPassword = ui->lineEditPassword->text();

    //创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_REGISTER;
    qDebug() << "注册：" << strUrl;
    QUrl url(strUrl);
    //创建请求对象
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("account", strPhone);
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    obj.insert("nickName", strPhone);
    obj.insert("password", strPassword);
    obj.insert("photoUrl", "");
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);//manager.get(request);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished,this, [=] {
        if (reply->error())
        {
            qDebug() << "注册响应:" << reply->error();
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
                    qDebug() << "Code=" << iCode << "message=" << strMessage <<"response:"<<response;
                    if(200 == iCode)
                    {
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
                            QString strPhotoUrl = userDetailVO["photoUrl"].toString();
                            qDebug() << "跳转到主页面"<<"id="<<id<<"name="<<strName<<"account="<<strAccount<<"mobile="<<strMobile<<"MaxExpirationDate"<<strMaxExpirationDate<<"photoUrl="<<strPhotoUrl<<"token="<<strToken;

                            m_mainWindow = new MainWindow(this);
                            m_mainWindow->show();
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this, "错误提示", strMessage);
                    }
                }
            }
        }
        reply->deleteLater();
    });
}


void RegisterPage::on_btnReturn_clicked()
{
    emit showPageType(TYPE_PASSWORDLOGIN_PAGE);
}

