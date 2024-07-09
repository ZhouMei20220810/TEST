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

    QString strTime = strTime.asprintf("%d天%d小时", mseconds / (1000 * 60 * 60 * 24), (mseconds / (1000 * 60 * 60)) % 24);
    qDebug() << "strTime=" << strTime;
    ui->labelDay->setText(strTime);
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
        return;
    }

    QString strPhoneName = ui->lineEditPhoneNum->text();
    int iAuthDay = ui->lineEditDay->text().toInt();
    qDebug()<<"instance="<<m_phoneInfo.strInstanceNo<<"strPhoneName="<<strPhoneName<<"iDay="<<iAuthDay<<"readonly="<<m_bIsReadOnly<<"accountAuth="<<m_bIsAccountAuth;
    if(m_bIsAccountAuth)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("请先阅读授权协议", this);
        dialog->show();
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

