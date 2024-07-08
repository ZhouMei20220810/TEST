#include "activecoderenewitem.h"
#include "ui_activecoderenewitem.h"

ActiveCodeRenewItem::ActiveCodeRenewItem(S_PHONE_INFO phoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeRenewItem)
{
    ui->setupUi(this);
    m_phoneInfo = phoneInfo;
    m_iStatus = -1;
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
    m_iStatus = -1;
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

void ActiveCodeRenewItem::do_activeCodeStatusSignals(QString strRenewAcitiveCode,bool bSuccess,QString strStatus)
{
    QString labelRenewActiveCode = ui->labelRenewActiveCode->text();
    if (!labelRenewActiveCode.isEmpty())
    {
        qDebug() << "strRenewAcitiveCode=" << strRenewAcitiveCode << "strStatus=" << strStatus;
        //if (labelRenewActiveCode == strRenewAcitiveCode)
        {
            if (!bSuccess)
                ui->labelStatus->setStyleSheet("border:none;background:transparent;color:#F96D6F");
            else
                ui->labelStatus->setStyleSheet("border:none;background:transparent;color:#505465");
            m_iStatus = bSuccess?0:1;
            ui->labelStatus->setText(strStatus);
        }
    }
}

int ActiveCodeRenewItem::getStatus()
{
    return m_iStatus;
}
void ActiveCodeRenewItem::on_toolBtnDel_clicked()
{
    //删除项
    emit deleteActiveItemSignals(m_phoneInfo);
}
