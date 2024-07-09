#include "addauthorizationdialog.h"
#include "ui_addauthorizationdialog.h"
#include <QDateTime>
#include "messagetipsdialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "policydialog.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"

AddAuthorizationDialog::AddAuthorizationDialog(S_PHONE_INFO phoneInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAuthorizationDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    m_bIsAccountAuth = false;
    m_bIsReadOnly = true;
    m_phoneInfo = phoneInfo;

    QRegularExpression regExp("[0-9]*");
    QValidator* validator = new QRegularExpressionValidator(regExp, this);
    ui->lineEditDay->setValidator(validator);
    ui->lineEditPhoneNum->setValidator(validator);

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->radioButtonAccount);
    m_btnGroup->addButton(ui->radioButtonAuthorCode);

    ui->labelPhoneName->setText(phoneInfo.strName);
    ui->labelPhoneInstance->setText(phoneInfo.strInstanceNo);

    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime = QDateTime::fromString(phoneInfo.strExpireTime, "yyyy-MM-dd hh:mm:ss");
    qint64 mseconds = 0; 
    mseconds = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();
    m_iDay = mseconds / (1000 * 60 * 60 * 24);
    QString strTime = strTime.asprintf("%d天%d小时", m_iDay, (mseconds / (1000 * 60 * 60)) % 24);
    qDebug() << "strTime=" << strTime;
    ui->labelDay->setText(strTime);

    //默认,没有授权和被授权
    if (phoneInfo.iAuthStatus == 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->pageAddAuth);
    }
    else if(phoneInfo.iAuthStatus == 1)//已授权
    {
        ui->stackedWidget->setCurrentWidget(ui->pageAddAuth);
    }
    else if(phoneInfo.iAuthStatus == 2)//被授权
    {
        ui->stackedWidget->setCurrentWidget(ui->pageAccount);
    }
}

AddAuthorizationDialog::~AddAuthorizationDialog()
{
    delete ui;
}

void AddAuthorizationDialog::on_btnClose_clicked()
{
    this->close();
}


void AddAuthorizationDialog::on_btnOk_clicked()
{
    //确定授权
    //发送请求给服务器
    if (!ui->checkBoxPolicy->isChecked())
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("请先阅读授权协议", this);
        dialog->show();
        return;
    }

    QString strPhone = ui->lineEditPhoneNum->text();
    int iAuthDay = ui->lineEditDay->text().toInt();
    qDebug()<<"instance="<<m_phoneInfo.strInstanceNo<<"strPhone="<<strPhone<<"iDay="<<iAuthDay<<"readonly="<<m_bIsReadOnly<<"accountAuth="<<m_bIsAccountAuth;
    if(m_bIsAccountAuth && strPhone.isEmpty())
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("指定账号授权，账号不能为空", this);
        dialog->show();
        return;
    }

    int iDay = ui->lineEditDay->text().toInt();
    if (m_iDay < iDay)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权天数不能大于云手机剩余天数", this);
        dialog->show();
        return;
    }
    if (iDay > 999)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权最大值999天", this);
        dialog->show();
        return;
    }
    if (iDay < 1)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权最小值1天", this);
        dialog->show();
        return;
    }

    //根据单选弹框
    if(ui->radioButtonAccount->isChecked())
    {
        HttpPostAuthAccountByPhone(m_bIsReadOnly, m_phoneInfo.iId, iDay, strPhone);
    }
    else
    {
        //生成授权码
        HttpPostGeneratorAuthCode(m_bIsReadOnly, m_phoneInfo.iId, iDay);
    }
}

void AddAuthorizationDialog::HttpPostGeneratorAuthCode(bool bIsReadOnly, int iUserInstanceId, int iUseDay)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_GENERATOR_AUTH_CODE;

    QString strGrantControl = "CONTROL";
    if (bIsReadOnly)
        strGrantControl = "WATCH";
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("grantControl", strGrantControl);
    obj.insert("useDay", iUseDay);
    obj.insert("userInstanceId",iUserInstanceId);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
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
                QString strData = obj["data"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << strData << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    ui->labelName->setText(m_phoneInfo.strName);
                    if (ui->radioButtonReadOnly->isChecked())
                    {
                        ui->labelQuanxian->setText("仅观看");
                    }
                    else
                    {
                        ui->labelQuanxian->setText("可操控");
                    }

                    if (m_phoneInfo.bUsed)
                    {
                        ui->labelUseStatus->setText("已使用");
                    }
                    else
                    {
                        ui->labelUseStatus->setText("未使用");
                    }
                    QDateTime currentTime = QDateTime::currentDateTime();
                    QDateTime newDateTime = currentTime.addDays(ui->lineEditDay->text().toInt());
                    ui->labelUseDay->setText(newDateTime.toString("yyyy-MM-dd hh:mm:ss"));

                    ui->label->setText("授权信息");
                    ui->label_18->setText("授权码："+ strData);
                    ui->stackedWidget->setCurrentWidget(ui->pageCode);
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


void AddAuthorizationDialog::HttpPostAuthAccountByPhone(bool bIsReadOnly, int iUserInstanceId, int iUseDay, QString strPhoneNum)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_AUTH_ACCOUNT_BY_PHONE;

    QString strGrantControl = "CONTROL";
    if (bIsReadOnly)
        strGrantControl = "WATCH";
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("grantControl", strGrantControl);
    obj.insert("useDay", iUseDay);
    obj.insert("userInstanceId", iUserInstanceId);
    obj.insert("mobile",strPhoneNum);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << "postData=" << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
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
                QString strData = obj["data"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << strData << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    ui->labelName_2->setText(m_phoneInfo.strName);
                    if (ui->radioButtonReadOnly->isChecked())
                    {
                        ui->labelQuanxian_2->setText("仅观看");
                    }
                    else
                    {
                        ui->labelQuanxian_2->setText("可操控");
                    }

                    if (m_phoneInfo.bUsed)
                    {
                        ui->labelUseStatus_2->setText("已使用");
                    }
                    else
                    {
                        ui->labelUseStatus_2->setText("未使用");
                    }

                    QDateTime currentTime = QDateTime::currentDateTime();
                    QDateTime newDateTime = currentTime.addDays(ui->lineEditDay->text().toInt());
                    ui->labelUseDay_2->setText(newDateTime.toString("yyyy-MM-dd hh:mm:ss"));

                    ui->label->setText("授权信息");
                    ui->stackedWidget->setCurrentWidget(ui->pageAccount);
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
void AddAuthorizationDialog::on_toolBtnTips_clicked()
{
    //打开温馨提示
    MessageTipsDialog* dialog = new MessageTipsDialog("温馨提示：\n授权码的泄露可能会导致你的手机以及云手机内登录的游戏出现风险，谨慎\n保管好。若不慎泄露请尽快在设置-->授权管理中取消云手机授权",this);
    dialog->show();
}


void AddAuthorizationDialog::on_radioButtonAccount_clicked(bool checked)
{
    //账号授权
    if(checked)
    {
        m_bIsAccountAuth= true;
    }
}


void AddAuthorizationDialog::on_radioButtonAuthorCode_clicked(bool checked)
{
    //授权码授权
    if(checked)
    {
        m_bIsAccountAuth = false;
    }
}


void AddAuthorizationDialog::on_radioButtonReadOnly_clicked(bool checked)
{
    //仅浏览
    if(checked)
    {
        m_bIsReadOnly = true;
    }
}


void AddAuthorizationDialog::on_radioButtonControl_clicked(bool checked)
{
    //控制
    if(checked)
    {
        m_bIsReadOnly = false;
    }
}


void AddAuthorizationDialog::on_btnAuthorPolicy_clicked()
{
    //易舜授权协议
    PolicyDialog* policy = new PolicyDialog("易舜授权协议", "https://www.ysyos.com/deal/Authorization.html", this);
    //policy->show();
    policy->exec();
}


void AddAuthorizationDialog::on_btnCancel_3_clicked()
{
    this->close();
}


void AddAuthorizationDialog::on_btnCancelAuthAccount_3_clicked()
{
    //取消授权，向服务器发送请求

}


void AddAuthorizationDialog::on_btnCancel_2_clicked()
{
    this->close();
}


void AddAuthorizationDialog::on_btnCancelAuthAccount_2_clicked()
{
    //取消授权，向服务器发送请求

}

