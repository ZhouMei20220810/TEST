#include "activecoderenewitem.h"
#include "ui_activecoderenewitem.h"

ActiveCodeRenewItem::ActiveCodeRenewItem(S_PHONE_INFO phoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeRenewItem)
{
    ui->setupUi(this);
    m_phoneInfo = phoneInfo;
    m_bSuccessed = false;

    ui->checkBox->setText(phoneInfo.strName);
}

ActiveCodeRenewItem::~ActiveCodeRenewItem()
{
    delete ui;
}

void ActiveCodeRenewItem::setCheckBoxStatus(bool bCheck)
{
    ui->checkBox->setChecked(bCheck);
}

bool ActiveCodeRenewItem::getCheckBoxStatus()
{
    return ui->checkBox->isChecked();
}

void ActiveCodeRenewItem::setRenewActiveCode(QString strRenewActiveCode)
{
    m_bSuccessed = false;
    ui->labelRenewActiveCode->setText(strRenewActiveCode);
}

QString ActiveCodeRenewItem::getRenewActiveCode()
{
    return ui->labelRenewActiveCode->text();
}

void ActiveCodeRenewItem::do_UpdateRenewActiveCodeSignals(int iPhoneId,QString strRenewActiveCode)
{
    if (m_phoneInfo.iId == iPhoneId)
    {
        ui->labelRenewActiveCode->setText(strRenewActiveCode);
    }
}

void ActiveCodeRenewItem::do_UpdateActiveCodeStatusSignals(QString strRenewAcitiveCode,bool bSuccess)
{
    QString labelRenewActiveCode = ui->labelRenewActiveCode->text();
    if (!labelRenewActiveCode.isEmpty())
    {
        if (labelRenewActiveCode == strRenewAcitiveCode)
        {
            m_bSuccessed = bSuccess;
            ui->labelStatus->setText(bSuccess ? "success" : "failed");
        }
    }
}
