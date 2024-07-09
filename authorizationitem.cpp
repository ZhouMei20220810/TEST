#include "authorizationitem.h"
#include "ui_authorizationitem.h"
#include <QDateTime>
authorizationItem::authorizationItem(S_AUTHOR_INFO authInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::authorizationItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    ui->labelPhone->setText(authInfo.strInstanceName);
    QDateTime currentDataTime = QDateTime::fromString(authInfo.strCreateTime);
    QDateTime newDataTime = currentDataTime.addDays(authInfo.iUseDay);
    ui->labelExpirationTime->setText(newDataTime.toString("yyyy-MM-dd hh:mm:ss"));
    //ui->labelActiveStatus->setText(getAuthStatusString(authInfo.iStatus));
}

authorizationItem::~authorizationItem()
{
    delete ui;
}

void authorizationItem::on_toolBtnOper_clicked()
{
    //管理

}

