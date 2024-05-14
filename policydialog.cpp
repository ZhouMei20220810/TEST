#include "policydialog.h"
#include "ui_policydialog.h"
#include <QWebEngineView>

PolicyDialog::PolicyDialog(QString strTitle,QString strUrl,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PolicyDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    setWindowFlag(Qt::FramelessWindowHint);

    ui->labelTitle->setText(strTitle);
    qDebug()<<"url:"<<strUrl;
    ui->webView->load(QUrl(strUrl));
}

PolicyDialog::~PolicyDialog()
{
    delete ui;
}

void PolicyDialog::on_btnClose_clicked()
{
    this->close();
}

