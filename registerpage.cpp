#include "registerpage.h"
#include "global.h"
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
    qDebug()<<"注册被点击";

    //创建网络访问管理器
    QNetworkAccessManager manager;

    //创建请求对象
    QNetworkRequest request;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_REGISTER;
    QUrl url(strUrl);
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("username", "zhoumei");
    obj.insert("password", "123456");
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager.post(request, postData);//manager.get(request);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, [&] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        reply->deleteLater();
    });
}


void RegisterPage::on_btnReturn_clicked()
{
    emit showPageType(TYPE_PASSWORDLOGIN_PAGE);
}

