#include "systemsettingwidget.h"
#include "ui_systemsettingwidget.h"
#include "messagetips.h"
#include <QDesktopServices>

SystemSettingWidget::SystemSettingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SystemSettingWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);
}

SystemSettingWidget::~SystemSettingWidget()
{
    delete ui;
}

void SystemSettingWidget::on_toolBtnGeneralSetting_clicked()
{
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageGeneralSetting);
}


void SystemSettingWidget::on_toolBtnUISetting_clicked()
{
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageUISetting);
}


void SystemSettingWidget::on_toolBtnAbout_clicked()
{
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->pageAbout);
}


void SystemSettingWidget::on_pushButton_clicked()
{
    this->close();
}


void SystemSettingWidget::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void SystemSettingWidget::on_btnSave_clicked()
{
    //保存
    MessageTips* tips = new MessageTips("数据已保存",this);
    tips->show();
}


void SystemSettingWidget::on_pushButton_3_clicked()
{
    QString strUrl = "https://www.ysyos.com/";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_4_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/Disclaimer.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_5_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/UserPrivacy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_6_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/PrivacyPolicy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_7_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/VipPrivacy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}

