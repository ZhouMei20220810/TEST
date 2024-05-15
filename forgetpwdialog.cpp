#include "forgetpwdialog.h"
#include "ui_forgetpwdialog.h"
#include "messagetipsdialog.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>

ForgetPWDialog::ForgetPWDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgetPWDialog)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
}

ForgetPWDialog::~ForgetPWDialog()
{
    delete ui;
}

void ForgetPWDialog::on_btnReturn_clicked()
{
    this->accept();
}


void ForgetPWDialog::on_btnClose_clicked()
{
    this->reject();
}

void ForgetPWDialog::on_btnGetCode_clicked()
{
    //获取验证码
    qDebug() << "重置密码 - 获取验证码";
    QString strPhone = ui->lineEditPhone->text();
    if(strPhone.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("手机号码不能为空!",this);
        tips->show();
        return;
    }
    if(strPhone.length() < 11)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请输入正确的手机!",this);
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


void ForgetPWDialog::on_btnOK_clicked()
{
    QString strPhone = ui->lineEditPhone->text();
    if(strPhone.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("手机号码不能为空!",this);
        tips->show();
        return;
    }
    if(strPhone.length() < 11)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请输入正确的手机!",this);
        tips->show();
        return;
    }
    QString strSMSCode = ui->lineEditSMSCode->text();
    if(strSMSCode.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("短信验证码不能为空",this);
        tips->show();
        return;
    }
    QString strPassword = ui->lineEditNewPW->text();
    if(strPassword.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("密码不能为空",this);
        tips->show();
        return;
    }
    QString strConfirmPW = ui->lineEditConfirmPW->text();
    if(strConfirmPW.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("确定密码不能为空",this);
        tips->show();
        return;
    }
    if(strPassword != strConfirmPW)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("两次密码输入不一致！",this);
        tips->show();
        return;
    }

    if(strPassword.length() < 6 || strPassword.length() > 16)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("密码错误,密码须为6-16位字母和数字组成!",this);
        tips->show();
        return;
    }

    //请求服务器接口,暂未
}

