#include "policydialog.h"
#include "ui_policydialog.h"

PolicyDialog::PolicyDialog(QString strTitle,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PolicyDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    setWindowFlag(Qt::FramelessWindowHint);

    ui->labelTitle->setText(strTitle);
}

PolicyDialog::~PolicyDialog()
{
    delete ui;
}

void PolicyDialog::on_btnClose_clicked()
{
    this->close();
}

