#include "smsloginpage.h"
#include "ui_smsloginpage.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include "mainwindow.h"
#include "messagetipsdialog.h"

SMSLoginPage::SMSLoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SMSLoginPage)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

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
        MessageTipsDialog* tips = new MessageTipsDialog("输入手机号不能为空!", this);
        tips->show();
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
        MessageTipsDialog* tips = new MessageTipsDialog("手机不能为空!", this);
        tips->show();
        return;
    }
    QString strSMSCode = ui->lineEditSMSCode->text();
    if (strSMSCode.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("验证码不能为空!", this);
        tips->show();
        return;
    }

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_SMS_LOGIN;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
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
                    QString strMessage = obj["message"].toString();
                    qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
                    S_USER_LOGIN_INFO userInfo;
                    if (HTTP_SUCCESS_CODE == iCode)
                    {
                        if (obj["data"].isObject())
                        {
                            QJsonObject data = obj["data"].toObject();
                            userInfo.strToken = data["token"].toString();
                            userInfo.strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            userInfo.id = userDetailVO["id"].toInt();
                            userInfo.strName = userDetailVO["name"].toString();
                            userInfo.strAccount = userDetailVO["account"].toString();
                            userInfo.strMobile = userDetailVO["mobile"].toString();
                            userInfo.strPhotoUrl = userDetailVO["photoUrl"].toString();

                            qDebug() << "登录成功：" << "id=" << userInfo.id << "name=" << userInfo.strName << "account=" << userInfo.strAccount << "mobile=" << userInfo.strMobile << "MaxExpirationDate" << userInfo.strMaxExpirationDate << "token=" << userInfo.strToken;

                            //关闭
                            //this->close();
                            emit closeWindowSignals();

                            //去掉父窗口
                            MainWindow* mainWindow = new MainWindow();
                            mainWindow->setUserInfo(userInfo);
                            mainWindow->show();

                            //使用新的主窗口
                            /*YsyMainWindow* mainWindow = new YsyMainWindow();
                            mainWindow->setUserInfo(userInfo);
                            mainWindow->show();*/

                        }
                    }
                    else
                    {
                        MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                        tips->show();
                    }
                }
            }
        }
        reply->deleteLater();
    });
}
