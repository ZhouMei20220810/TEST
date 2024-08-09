#include "registerpage.h"
#include "ui_registerpage.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include "mainwindow.h"
#include "messagetipsdialog.h"
#include "policydialog.h"
#include "messagetips.h"

RegisterPage::RegisterPage(QMoveWidget *parent)
    : QMoveWidget(parent)
    , ui(new Ui::RegisterPage)
{
    ui->setupUi(this);
    //无边框
    setWindowFlags(Qt::FramelessWindowHint);
    //透明背景
    //setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_Hover, true);
    setWindowTitle("注册");
    //setStyleSheet("QWidget{backgroud:#FFFFFFFF;border:1px solid #A0A0A0; padding10px;box-shadow:5px 5px 10px rgba(0,0,0,0.5);}");
    //setStyleSheet("QWidget{backgroud:#FFFFFFFF;border:none; padding:10px;box-shadow:5px 5px 10px rgba(0,0,0,0.8);}");

    ui->btnCustomerService->setVisible(false);

    //设置屏幕居中显示
    //QGuiApplication* desktop = QApplication::primaryScreen(); // =qApp->desktop();也可以
    //QScreen *primaryScreen = QGuiApplication::primaryScreen();
    //this->setGeometry(0,0,260,100);
    //qDebug("十进制%d",this->width());
    //this->move((primaryScreen->geometry().width() - this->width())/2, (primaryScreen->geometry().height() - this->height())/2);
    /*QList<QScreen*> screen = QGuiApplication::screens();
    if(screen.size()>=1)
    {
        // 窗口居中
        int iNewPosX = (screen[0]->geometry().width()-this->width())/2;
        int iNewPosY = (screen[0]->geometry().height()-this->height())/2;
        qDebug()<<"setSceneNameAndTagsList move : iNewPosX="<< iNewPosX<<"iNewPosY="<<iNewPosY;
        this->move(iNewPosX,iNewPosY);
    }*/
}

RegisterPage::~RegisterPage()
{
    delete ui;
}

void RegisterPage::on_btnRegister_clicked()
{
    //注册
    QString strPhone = ui->lineEditPhone->text();
    if(strPhone.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("手机号码不能为空!",this);
        tips->show();
        return;
    }

    QString strAccount = ui->lineEditAccount->text();
    if(strAccount.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("账号不能为空!",this);
        tips->show();
        return;
    }

    QString strNickname = ui->lineEditNickname->text();
    if(strNickname.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("昵称不能为空!",this);
        tips->show();
        return;
    }
    QString strSMSCode = ui->lineEditSMSCode->text();
    if(strSMSCode.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("短信验证码不能为空!",this);
        tips->show();
        return;
    }
    QString strPassword = ui->lineEditPassword->text();
    if(strPassword.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("密码不能为空!",this);
        tips->show();
        return;
    }
    QString strConfirmPW = ui->lineEditConfirmPW->text();
    if(strConfirmPW.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("确定密码不能为空!",this);
        tips->show();
        return;
    }
    if(strPassword != strConfirmPW)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("两次密码输入不一致!",this);
        tips->show();
        return;
    }

    if(strPassword.length() < 6 || strPassword.length() > 16)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("密码错误,密码须为6-16位字母和数字组成!",this);
        tips->show();
        return;
    }

    bool bAgree = ui->checkBoxPolicy->isChecked();
    if(!bAgree)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("注册前须阅读并同意,服务协议和隐私保护声明!",this);
        tips->show();
        return;
    }

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
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
        
    strPassword = GlobalData::md5(strPassword);
    qDebug() << "password = " << strPassword;
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("account", strAccount);
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    obj.insert("nickName", strNickname);
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
                    if(HTTP_SUCCESS_CODE == iCode)
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

                            MainWindow* mainWindow = new MainWindow();
                            connect(mainWindow, &MainWindow::logoutSignals, this, &RegisterPage::logoutSignals);
                            mainWindow->show();
                            /*YsyMainWindow* mainWindow = new YsyMainWindow();
                            mainWindow->show();*/
                        }
                    }
                    else
                    {
                        MessageTips* tips = new MessageTips(strMessage, this);
                        tips->show();
                    }
                }
            }
        }
        reply->deleteLater();
    });
}


void RegisterPage::on_btnReturn_clicked()
{
    this->close();
    emit showPageType(TYPE_PASSWORDLOGIN_PAGE);
}


void RegisterPage::on_pushButton_clicked()
{
    //获取验证码
    qDebug() << "获取验证码";
    QString strPhone = ui->lineEditPhone->text();
    if(strPhone.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("手机号码不能为空!",this);
        tips->show();
        return;
    }
    if(strPhone.length() < 11)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请输入正确的手机号!",this);
        tips->show();
        return;
    }


    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_GET_SMS_CODE;
    strUrl += "/?mobiles=" + strPhone;
    //创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    QUrl url(strUrl);
    /*QUrlQuery query;
    query.addQueryItem("mobiles", strPhone);
    url.setQuery(query.query(QUrl::FullyEncoded));*/
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


void RegisterPage::on_pushButton_2_clicked()
{
    this->close();
}


void RegisterPage::on_btnUserPolicy_clicked()
{
    //用户使用协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/UserPrivacy.html",this);
    //policy->show();
    policy->exec();
}


void RegisterPage::on_btnPrivacyPolicy_clicked()
{
    //隐私政策
    PolicyDialog* policy = new PolicyDialog("隐私政策","https://www.ysyos.com/deal/PrivacyPolicy.html");
    //policy->show();
    policy->exec();
}


void RegisterPage::on_btnDisclaimer_clicked()
{
    //免责申明
    PolicyDialog* policy = new PolicyDialog("免责声明","https://www.ysyos.com/deal/Disclaimer.html");
    //policy->show();
    policy->exec();
}
