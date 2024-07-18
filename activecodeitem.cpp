#include "activecodeitem.h"
#include "ui_activecodeitem.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ActiveCodeItem::ActiveCodeItem(QString strActiveCode, QString strStatus,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeItem)
{
    ui->setupUi(this);
    ui->labelActiveCode->setText(strActiveCode);
    ui->labelActiveStatus->setText(strStatus);
    m_strActiveCode = strActiveCode;
}

QString ActiveCodeItem::getActiveCode()
{
    return m_strActiveCode;
}

ActiveCodeItem::~ActiveCodeItem()
{
    delete ui;
}

void ActiveCodeItem::do_activeCodeStatusSignals(QMap<QString, bool> mapActiveCodeStatus)
{
    QString strRenewActiveCode = ui->labelActiveCode->text();
    if (strRenewActiveCode.isEmpty())
    {
        return;
    }
    QMap<QString, bool>::iterator iterFind = mapActiveCodeStatus.find(strRenewActiveCode);
    if (iterFind != mapActiveCodeStatus.end())
    {
        bool bSuccess = iterFind.value();
        QString strMessage, strStyleSheet;
        if (bSuccess)
        {
            strMessage = "激活成功";
            strStyleSheet = "border:none;background-color:#F5F7FB;color:#505465";
        }
        else
        {
            strStyleSheet = "border:none;background-color:#F5F7FB;color:#F96D6F";
            strMessage = "激活码不存在";
        }
        ui->labelActiveStatus->setStyleSheet(strStyleSheet);
        ui->labelActiveStatus->setText(strMessage);
    }
}