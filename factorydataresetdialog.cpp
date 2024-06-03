#include "factorydataresetdialog.h"
#include "ui_factorydataresetdialog.h"

FactoryDataResetDialog::FactoryDataResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FactoryDataResetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

FactoryDataResetDialog::~FactoryDataResetDialog()
{
    delete ui;
}

void FactoryDataResetDialog::on_btnOk_clicked()
{
    this->accept();
}


void FactoryDataResetDialog::on_btnCancel_clicked()
{
    this->reject();
}

