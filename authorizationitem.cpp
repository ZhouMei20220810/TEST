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

    if(authInfo.strInstanceName.isEmpty())
        ui->labelPhone->setText(authInfo.strInstanceNo);
    else
        ui->labelPhone->setText(authInfo.strInstanceName);
    QDateTime currentDataTime = QDateTime::fromString(authInfo.strCreateTime);
    ui->labelExpirationTime->setText(authInfo.strExpireTime);
    //ui->labelActiveStatus->setText(getAuthStatusString(authInfo.iStatus));
    /*switch (authInfo.iType)
    {
    case EN_AUTHORIZATION:
        ui->toolBtnOper->setText("管理");
        break;
    case EN_BE_AUTHORIZATION:
        ui->toolBtnOper->setText("详情");
        break;
    default:
        break;
    }*/
}

authorizationItem::~authorizationItem()
{
    delete ui;
}

void authorizationItem::on_toolBtnOper_clicked()
{
    //管理
    S_PHONE_INFO phoneInfo;
    //phoneInfo.iType = m_authInfo.iType;
    AddAuthorizationDialog* dialog = new AddAuthorizationDialog(phoneInfo);
    connect(dialog, &AddAuthorizationDialog::notifyMainWindowRefreshGroupListSignals, this, &authorizationItem::notifyMainWindowRefreshGroupListSignals);
    dialog->InitWidget(m_authInfo);
    dialog->exec();
}

