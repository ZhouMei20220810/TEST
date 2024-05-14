#include "passwordloginpage.h"
#include "ui_passwordloginpage.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include "mainwindow.h"

PasswordLoginPage::PasswordLoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordLoginPage)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    //设置默认值
    //ui->lineEditPhone->setText("15019445205");
    //ui->lineEditPassword->setText("123456");
    //隐藏返回和客服按钮
    ui->btnReturn->setVisible(false);
    ui->btnCustomerService->setVisible(false);

    ui->lineEditPhone->setText("18774660070");
    ui->lineEditPassword->setText("123456");
}

PasswordLoginPage::~PasswordLoginPage()
{
    delete ui;
}

void PasswordLoginPage::on_btnReturn_clicked()
{
    emit showPageType(TYPE_SMSLOGIN_PAGE);
}

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
        QMessageBox::warning(this, "错误提示", "账号不能为空");
        return;
    }
    QString strPassword = ui->lineEditPassword->text();
    if (strPassword.isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "密码不能为空");
        return;
    }

    bool bRemmemberPW = ui->checkBoxRemberPW->isChecked();
    bool bAutoLogin = ui->checkBoxAutoLogin->isChecked();

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_PASSWORD_LOGIN;
    qDebug()<<"登录 url="<<strUrl <<"account="<<strAccount<<"strPassword="<<strPassword;
    //创建网络访问管理器
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "15019445205");//strAccount.toUtf8());
    request.setUrl(url);
    
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("account", strAccount);
    obj.insert("password", strPassword);
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
                            this->close();

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
                        QMessageBox::warning(this, tr("提示"), strMessage);
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

