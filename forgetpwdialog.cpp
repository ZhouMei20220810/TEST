#include "forgetpwdialog.h"
#include "ui_forgetpwdialog.h"

ForgetPWDialog::ForgetPWDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ForgetPWDialog)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
}

ForgetPWDialog::~ForgetPWDialog()
{
    delete ui;
}

void ForgetPWDialog::on_btnReturn_clicked()
{
    this->accept();
}


void ForgetPWDialog::on_btnClose_clicked()
{
    this->reject();
}

