#include "replacecloudphonedialog.h"
#include "ui_replacecloudphonedialog.h"

ReplaceCloudPhoneDialog::ReplaceCloudPhoneDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceCloudPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
}

ReplaceCloudPhoneDialog::~ReplaceCloudPhoneDialog()
{
    delete ui;
}

void ReplaceCloudPhoneDialog::on_btnClose_clicked()
{
    this->close();
}


void ReplaceCloudPhoneDialog::on_btnOk_clicked()
{
    qDebug()<<"确定更换";
}

