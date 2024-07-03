#include "authorizationmanagedialog.h"
#include "ui_authorizationmanagedialog.h"
#include "policydialog.h"
#include "messagetipsdialog.h"

AuthorizationManageDialog::AuthorizationManageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthorizationManageDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
}

AuthorizationManageDialog::~AuthorizationManageDialog()
{
    delete ui;
}

void AuthorizationManageDialog::on_btnClose_clicked()
{
    this->close();
}


void AuthorizationManageDialog::on_toolBtnAddAuthorization_clicked()
{
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAddAuthorization);

}


void AuthorizationManageDialog::on_toolBtnAuthorized_clicked()
{
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAuthorized);
}


void AuthorizationManageDialog::on_toolBtnBeAuthorized_clicked()
{
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageAuthorized);
}


void AuthorizationManageDialog::on_btnAuthorPolicy_clicked()
{
    //易舜授权协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/Authorization.html",this);
    //policy->show();
    policy->exec();
}

void AuthorizationManageDialog::on_btnUserPolicy_clicked()
{
    //用户使用协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/UserPrivacy.html",this);
    //policy->show();
    policy->exec();
}


void AuthorizationManageDialog::on_btnOk_clicked()
{
    bool bCheck = ui->checkBoxPolicy->isChecked();
    if(!bCheck)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("请先阅读并勾选",this);
        dialog->show();
        return;
    }
}


void AuthorizationManageDialog::on_btnCancel_clicked()
{
    this->close();
}

