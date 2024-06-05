#include "systemsettingwidget.h"
#include "ui_systemsettingwidget.h"

SystemSettingWidget::SystemSettingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SystemSettingWidget)
{
    ui->setupUi(this);
}

SystemSettingWidget::~SystemSettingWidget()
{
    delete ui;
}

void SystemSettingWidget::on_toolBtnGeneralSetting_clicked()
{

}


void SystemSettingWidget::on_toolBtnUISetting_clicked()
{

}


void SystemSettingWidget::on_toolBtnAbout_clicked()
{

}

