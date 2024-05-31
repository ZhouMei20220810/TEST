#include "phoneinstancehorwidget.h"
#include "ui_phoneinstancehorwidget.h"

PhoneInstanceHorWidget::PhoneInstanceHorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneInstanceHorWidget)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlag(Qt::FramelessWindowHint);

    ui->frameTool->setVisible(false);
}

PhoneInstanceHorWidget::~PhoneInstanceHorWidget()
{
    delete ui;
}

void PhoneInstanceHorWidget::on_toolBtnMore_clicked()
{
    bool bVisible = ui->frameTool->isVisible();
    ui->frameTool->setVisible(!bVisible);
}

