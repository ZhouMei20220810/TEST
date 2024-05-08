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

PasswordLoginPage::PasswordLoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PasswordLoginPage)
{
    ui->setupUi(this);

    //设置默认值
    ui->lineEditPhone->setText("15019445205");
    ui->lineEditPassword->setText("Zysy888888");
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
    //登录
    qDebug() << "登录被点击";
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
    obj.insert("account", strAccount);
    obj.insert("password", strPassword);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager.post(request, postData);//manager.get(request);
    QNetworkReply::NetworkError error = reply->error();
    qDebug()<<"reply error="<<error;
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished,this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

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

