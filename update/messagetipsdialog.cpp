#include "messagetipsdialog.h"
#include "ui_messagetipsdialog.h"

MessageTipsDialog::MessageTipsDialog(QString strShow,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageTipsDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose,true);

    ui->labelText->setText(strShow);
}

MessageTipsDialog::~MessageTipsDialog()
{
    qDebug()<<"delete MessageTipsDialog";
    delete ui;
}

void MessageTipsDialog::on_pushButton_2_clicked()
{
    this->close();
}

void MessageTipsDialog::on_btnOK_clicked()
{
    accept();
}


void MessageTipsDialog::on_btnCancel_clicked()
{
    reject();
}

