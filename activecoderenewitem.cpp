#include "activecoderenewitem.h"
#include "ui_activecoderenewitem.h"

ActiveCodeRenewItem::ActiveCodeRenewItem(S_PHONE_INFO phoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeRenewItem)
{
    ui->setupUi(this);
    m_phoneInfo = phoneInfo;
    m_iStatus = -1;
    if (phoneInfo.strName.isEmpty())
        ui->checkBox->setText(phoneInfo.strInstanceNo);
    else
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

void ActiveCodeRenewItem::clearStatus()
{
    ui->labelRenewActiveCode->clear();
    ui->labelStatus->clear();
}

void ActiveCodeRenewItem::do_UpdateRenewActiveCodeSignals(int iPhoneId,QString strRenewActiveCode)
{
    if (m_phoneInfo.iId == iPhoneId)
    {
        ui->labelRenewActiveCode->setText(strRenewActiveCode);
    }
}

void ActiveCodeRenewItem::do_activeCodeStatusSignals(QMap<QString, bool> mapActiveCodeStatus)
{
    QString strRenewActiveCode = ui->labelRenewActiveCode->text();
    if (strRenewActiveCode.isEmpty())
    {
        return;
    }
    QMap<QString, bool>::iterator iterFind = mapActiveCodeStatus.find(strRenewActiveCode);
    if (iterFind != mapActiveCodeStatus.end())
    {
        bool bSuccess = iterFind.value();
        QString strMessage,strStyleSheet;
        if (bSuccess)
        {
            strMessage = "激活成功";
            strStyleSheet = "border:none;background:transparent;color:#505465";
        }            
        else
        {
            strStyleSheet = "border:none;background:transparent;color:#F96D6F";
            strMessage = "激活码不存在";
        }
        m_iStatus = bSuccess ? 0 : 1;
        ui->labelStatus->setStyleSheet(strStyleSheet);
        ui->labelStatus->setText(strMessage);            
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
