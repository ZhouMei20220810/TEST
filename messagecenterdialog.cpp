#include "messagecenterdialog.h"
#include "ui_messagecenterdialog.h"

MessageCenterDialog::MessageCenterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageCenterDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
}

MessageCenterDialog::~MessageCenterDialog()
{
    delete ui;
}
