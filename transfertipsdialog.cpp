#include "transfertipsdialog.h"
#include "ui_transfertipsdialog.h"

TransferTipsDialog::TransferTipsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferTipsDialog)
{
    ui->setupUi(this);
}

TransferTipsDialog::~TransferTipsDialog()
{
    delete ui;
}

void TransferTipsDialog::on_btnSave_clicked()
{
    this->accept();
}


void TransferTipsDialog::on_btnCancel_clicked()
{
    this->reject();
}

