#include "phoneitemnodatawidget.h"
#include "ui_phoneitemnodatawidget.h"

PhoneItemNoDataWidget::PhoneItemNoDataWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneItemNoDataWidget)
{
    ui->setupUi(this);
}

PhoneItemNoDataWidget::~PhoneItemNoDataWidget()
{
    delete ui;
}
