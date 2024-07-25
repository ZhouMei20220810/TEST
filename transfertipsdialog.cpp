#include "transfertipsdialog.h"
#include "ui_transfertipsdialog.h"
#include <QGraphicsDropShadowEffect>

TransferTipsDialog::TransferTipsDialog(QString strPhoneOrAccount, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferTipsDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("确认转移");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    ui->labelContent->setText(strPhoneOrAccount);
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

