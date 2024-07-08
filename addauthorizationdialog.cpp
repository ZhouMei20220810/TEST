#include "addauthorizationdialog.h"
#include "ui_addauthorizationdialog.h"

AddAuthorizationDialog::AddAuthorizationDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddAuthorizationDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
}

AddAuthorizationDialog::~AddAuthorizationDialog()
{
    delete ui;
}

void AddAuthorizationDialog::on_btnClose_clicked()
{
    this->close();
}


void AddAuthorizationDialog::on_btnOk_clicked()
{
    //确定授权

}

