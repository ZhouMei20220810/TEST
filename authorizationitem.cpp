#include "authorizationitem.h"
#include "ui_authorizationitem.h"
#include <QDateTime>
#include "addauthorizationdialog.h"
authorizationItem::authorizationItem(S_AUTHOR_INFO authInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::authorizationItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_authInfo = authInfo;

    ui->labelPhone->setText(authInfo.strInstanceName);
    QDateTime currentDataTime = QDateTime::fromString(authInfo.strCreateTime);
    ui->labelExpirationTime->setText(authInfo.strExpireTime);
    //ui->labelActiveStatus->setText(getAuthStatusString(authInfo.iStatus));
}

authorizationItem::~authorizationItem()
{
    delete ui;
}

void authorizationItem::on_toolBtnOper_clicked()
{
    //管理
    S_PHONE_INFO phoneInfo;
    AddAuthorizationDialog* dialog = new AddAuthorizationDialog(phoneInfo);
    dialog->InitWidget(m_authInfo);
    dialog->exec();
}

