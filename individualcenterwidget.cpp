#include "individualcenterwidget.h"
#include "ui_individualcenterwidget.h"
#include "forgetpwdialog.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include "messagetips.h"
#include "buyhistorywidget.h"
#include "authorizationmanagedialog.h"
#include "activecodehistorydialog.h"
#include <QGraphicsDropShadowEffect>

IndividualCenterWidget::IndividualCenterWidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::IndividualCenterWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    QFontMetrics fontWidth(ui->labelAccount->font());
    QString strElideNote = fontWidth.elidedText(GlobalData::strAccount, Qt::ElideRight, 90);
    ui->labelAccount->setText(strElideNote);
    //ui->labelAccount->setText(GlobalData::strAccount);
    ui->labelAccount->setToolTip(GlobalData::strAccount);
    ui->labelID->setText(QString("%1").arg(GlobalData::id));

}

IndividualCenterWidget::~IndividualCenterWidget()
{
    delete ui;
}

void IndividualCenterWidget::on_toolBtnIndividualCenter_clicked()
{
    //个人中心,修改密码
    ForgetPWDialog* forgetPW = new ForgetPWDialog();
    forgetPW->setModal(true);
    forgetPW->show();

    this->hide();
}

void IndividualCenterWidget::on_toolBtnLogout_clicked()
{
    HttpLogout();
}


void IndividualCenterWidget::on_toolBtnBuyHistory_clicked()
{
    BuyHistoryWidget* widget = new BuyHistoryWidget();
    widget->setModal(true);
    widget->show();

    this->hide();
}

void IndividualCenterWidget::on_toolBtnAuthorizationManage_clicked()
{
    AuthorizationManageDialog* dialog = new AuthorizationManageDialog();
    connect(dialog, &AuthorizationManageDialog::notifyMainWindowRefreshGroupListSignals, this, &IndividualCenterWidget::notifyMainWindowRefreshGroupListSignals);
    dialog->setModal(true);
    dialog->show();

    this->hide();
}

void IndividualCenterWidget::HttpLogout()
{
    //关闭窗口并且退出登录
    qDebug() << "注销";
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGOUT;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
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
                qDebug() << "Code=" << iCode << "message=" << strMessage <<"response:"<<response;
                if(HTTP_SUCCESS_CODE == iCode)
                {
                    /*if (obj["data"].isObject())
                        {
                            QJsonObject data = obj["data"].toObject();
                            QString strToken = data["token"].toString();
                            QString strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            int id = userDetailVO["id"].toInt();
                            QString strName = userDetailVO["name"].toString();
                            QString strAccount = userDetailVO["account"].toString();
                            QString strMobile = userDetailVO["mobile"].toString();
                            qDebug() << "跳转到主页面"<<"id="<<id<<"name="<<strName<<"account="<<strAccount<<"mobile="<<strMobile<<"MaxExpirationDate"<<strMaxExpirationDate<<"token="<<strToken;
                        }*/
                    qDebug()<<"注销成功";
                    this->hide();
                    emit logoutSignals();
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

void IndividualCenterWidget::on_toolBtnActiveCodeHistory_clicked()
{
    //激活码记录
    ActiveCodeHistoryDialog* dialog = new ActiveCodeHistoryDialog();
    dialog->exec();
}

