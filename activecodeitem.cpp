#include "activecodeitem.h"
#include "ui_activecodeitem.h"

ActiveCodeItem::ActiveCodeItem(QString strActiveCode,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeItem)
{
    ui->setupUi(this);
    ui->labelActiveCode->setText(strActiveCode);
    ui->labelActiveStatus->setText("");
}

ActiveCodeItem::~ActiveCodeItem()
{
    delete ui;
}
