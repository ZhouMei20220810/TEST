#include "addauthorizationdialog.h"
#include "ui_addauthorizationdialog.h"
#include <QDateTime>
#include "messagetipsdialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "policydialog.h"

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

    QString strPhoneName = ui->lineEditPhoneNum->text();
    int iAuthDay = ui->lineEditDay->text().toInt();
    qDebug()<<"instance="<<m_phoneInfo.strInstanceNo<<"strPhoneName="<<strPhoneName<<"iDay="<<iAuthDay<<"readonly="<<m_bIsReadOnly<<"accountAuth="<<m_bIsAccountAuth;
    if(m_bIsAccountAuth && strPhoneName.isEmpty())
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
    if (iDay <= 0)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("请输入正确天数", this);
        dialog->show();
        return;
    }

    //根据单选弹框
    if(ui->radioButtonAccount->isChecked())
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
        
        ui->labelUseDay_2->setText(ui->lineEditDay->text() + "天");

        ui->label->setText("授权信息");
        ui->stackedWidget->setCurrentWidget(ui->pageAccount);
    }
    else
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

        ui->labelUseDay->setText(ui->lineEditDay->text() + "天");

        ui->label->setText("授权信息");
        ui->stackedWidget->setCurrentWidget(ui->pageCode);
    }
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

