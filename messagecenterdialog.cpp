#include "messagecenterdialog.h"
#include "ui_messagecenterdialog.h"

MessageCenterDialog::MessageCenterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageCenterDialog)
{
    ui->setupUi(this);
}

MessageCenterDialog::~MessageCenterDialog()
{
    delete ui;
}
