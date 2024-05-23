#include "phoneinstancewidget.h"
#include "ui_phoneinstancewidget.h"

PhoneInstanceWidget::PhoneInstanceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneInstanceWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    ui->toolBtnShow->setVisible(false);
    //ui->frame_2->setVisible(true);
}

PhoneInstanceWidget::~PhoneInstanceWidget()
{
    delete ui;
}

void PhoneInstanceWidget::on_toolBtnPictureQuality_clicked()
{

}


void PhoneInstanceWidget::on_toolBtnTopLevel_clicked()
{

}


void PhoneInstanceWidget::on_toolBtnMin_clicked()
{
    this->showMinimized();
}


void PhoneInstanceWidget::on_toolBtnMax_clicked()
{
    this->showMaximized();
}


void PhoneInstanceWidget::on_toolBtnClose_clicked()
{
    this->close();
}


void PhoneInstanceWidget::on_toolBtnShow_clicked()
{
    ui->toolBtnShow->setVisible(false);
    ui->frame_2->setVisible(true);
}


void PhoneInstanceWidget::on_toolBtnHide_clicked()
{
    ui->toolBtnShow->setVisible(true);
    ui->frame_2->setVisible(false);
}


void PhoneInstanceWidget::on_toolBtnSignal_clicked()
{
    //信号
}


void PhoneInstanceWidget::on_toolBtnVerOrHor_clicked()
{
    //横竖屏
}


void PhoneInstanceWidget::on_toolBtnScreenshot_clicked()
{
    //截图
}


void PhoneInstanceWidget::on_toolBtnAddVolume_clicked()
{
    //音量调大
}


void PhoneInstanceWidget::on_toolBtnSubVolume_clicked()
{
    //音量调小
}


void PhoneInstanceWidget::on_toolBtnClipboard_clicked()
{
    //剪贴板
}


void PhoneInstanceWidget::on_toolBtnRestart_clicked()
{
    //重启
}


void PhoneInstanceWidget::on_toolBtnFactoryDataReset_clicked()
{
    //恢复出厂设置
}

