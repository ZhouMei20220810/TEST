#include "passwordloginpage.h"
#include "ui_passwordloginpage.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include "mainwindow.h"
#include <QSettings>
#include "messagetipsdialog.h"
#include "messagetips.h"

PasswordLoginPage::PasswordLoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordLoginPage)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    //设置默认值
    //ui->lineEditPhone->setText("15019445205");
    //ui->lineEditPassword->setText("123456");
    //隐藏返回、客服按钮、隐藏验证码登录功能
    //ui->btnSMSLogin->setVisible(false);
    //隐藏忘记密码功能
    ui->btnForgetPW->setVisible(false);

    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    bool bRemmemberPW = setting.value("IsRemmemberPW",false).toBool();
    bool bAutoLogin = setting.value("IsAutoLogin", false).toBool();
    //自动登录
    if(bRemmemberPW)
    {
        ui->checkBoxRemberPW->setChecked(true);
        QString strAccount = setting.value("account","").toString();
        ui->lineEditPhone->setText(strAccount);
        QString strPassword = setting.value("password","").toString();
        ui->lineEditPassword->setText(strPassword);        
    }

    if(bAutoLogin)
    {
        ui->checkBoxAutoLogin->setChecked(true);
        on_btnLogin_clicked();
    }
}

PasswordLoginPage::~PasswordLoginPage()
{
    delete ui;
}

/*void PasswordLoginPage::on_btnReturn_clicked()
{
    emit showPageType(TYPE_SMSLOGIN_PAGE);
}*/

void PasswordLoginPage::on_btnRegister_clicked()
{
    emit showPageType(TYPE_REGISTER_PAGE);
}


void PasswordLoginPage::on_btnSMSLogin_clicked()
{
    emit showPageType(TYPE_SMSLOGIN_PAGE);
}

void PasswordLoginPage::on_btnLogin_clicked()
{
    //账号密码登录
    QString strAccount = ui->lineEditPhone->text();
    if (strAccount.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("账号不能为空!",this);
        tips->show();
        return;
    }
    //判断是否为手机号
    if(strAccount.length() < 11)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请输入正确的手机!",this);
        tips->show();
        return;
    }

    QString strPassword = ui->lineEditPassword->text();
    if (strPassword.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("密码不能为空!",this);
        tips->show();
        return;
    }
    QString strMd5Password = GlobalData::md5(strPassword);
    qDebug()<<"strPassword = "<<strPassword<<"md5 password= " << strMd5Password <<"length="<< strMd5Password.length();

    bool bRemmemberPW = ui->checkBoxRemberPW->isChecked();
    bool bAutoLogin = ui->checkBoxAutoLogin->isChecked();

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_PASSWORD_LOGIN;
    qDebug()<<"登录 url="<<strUrl <<"account="<<strAccount<<"strPassword="<<strMd5Password;
    //创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    //request.setRawHeader("Authorization", strAccount.toUtf8());
    request.setUrl(url);
    
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("account", strAccount);
    obj.insert("password", strMd5Password);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);//manager.get(request);
    connect(reply, &QNetworkReply::finished,this, [=] {
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
                    if (HTTP_SUCCESS_CODE == iCode)
                    {
                        if (obj["data"].isObject())
                        {                             
                            QJsonObject data = obj["data"].toObject();
                            GlobalData::strToken = data["token"].toString();
                            GlobalData::strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            GlobalData::id = userDetailVO["id"].toInt();
                            GlobalData::strName = userDetailVO["name"].toString();
                            GlobalData::strAccount = userDetailVO["account"].toString();
                            GlobalData::strMobile = userDetailVO["mobile"].toString();
                            GlobalData::strPhotoUrl = userDetailVO["photoUrl"].toString();

                            qDebug() << "登录成功：" << "id=" << GlobalData::id << "name=" << GlobalData::strName << "account=" << GlobalData::strAccount << "mobile=" << GlobalData::strMobile << "MaxExpirationDate" << GlobalData::strMaxExpirationDate << "token=" << GlobalData::strToken;
                            if (bRemmemberPW)
                            {
                                QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
                                setting.setValue("IsRemmemberPW", bRemmemberPW);
                                setting.setValue("IsAutoLogin", bAutoLogin);
                                setting.setValue("id", GlobalData::id);
                                setting.setValue("name", GlobalData::strName);
                                setting.setValue("account", GlobalData::strAccount);
                                setting.setValue("password",strPassword);
                                setting.setValue("photoUrl", GlobalData::strPhotoUrl);
                                setting.setValue("mobile", GlobalData::strMobile);
                            }
                            //关闭
                            //this->close();
                            emit closeWindowSignals();

                            //去掉父窗口
                            MainWindow* mainWindow = new MainWindow();
                            connect(mainWindow, &MainWindow::logoutSignals, this, &PasswordLoginPage::logoutSignals);
                            mainWindow->show();
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


void PasswordLoginPage::on_btnForgetPW_clicked()
{
    //忘记密码
    qDebug()<<"忘记密码被点击";
    //this->hide();
    emit showLoginWindow(TYPE_WINDOWS_HIDE);
    m_forgetPWDialog = new ForgetPWDialog();
    if(m_forgetPWDialog->exec() == QDialog::Accepted)
    {
        //this->show();
        emit showLoginWindow(TYPE_WINDOWS_SHOW);
    }
    else
    {
        emit showLoginWindow(TYPE_WINDOWS_CLOSE);
        //this->close();
    }
}


void PasswordLoginPage::on_checkBoxAutoLogin_clicked(bool checked)
{
    //自动登录勾选
    if(checked)
    {
        ui->checkBoxRemberPW->setChecked(true);
    }
}

