#include "transferphonedialog.h"
#include "ui_transferphonedialog.h"
#include <QGraphicsDropShadowEffect>

TransferPhoneDialog::TransferPhoneDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TransferPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("转移云机");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

}

TransferPhoneDialog::~TransferPhoneDialog()
{
    delete ui;
}

void TransferPhoneDialog::on_btnClose_clicked()
{
    this->close();
}

