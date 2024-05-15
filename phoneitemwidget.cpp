#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"

PhoneItemWidget::PhoneItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
}

PhoneItemWidget::~PhoneItemWidget()
{
    delete ui;
}

void PhoneItemWidget::on_btnAddPhone_clicked()
{
    qDebug()<<"添加设备";
}

