#include "activecodeitem.h"
#include "ui_activecodeitem.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

ActiveCodeItem::ActiveCodeItem(QString strActiveCode,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeItem)
{
    ui->setupUi(this);
    ui->labelActiveCode->setText(strActiveCode);
    ui->labelActiveStatus->setText("");
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

void ActiveCodeItem::do_activeCodeStatusSignals(QString strStatus)
{
    ui->labelActiveStatus->setText(strStatus);
}