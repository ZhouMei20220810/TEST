#include "phoneitemwidget.h"
#include "ui_phoneitemwidget.h"

PhoneItemWidget::PhoneItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
}

PhoneItemWidget::~PhoneItemWidget()
{
    qDebug()<<"delete PhoneItemWidget";
    delete ui;
}

void PhoneItemWidget::on_btnAddPhone_clicked()
{
    qDebug()<<"添加设备";
}

