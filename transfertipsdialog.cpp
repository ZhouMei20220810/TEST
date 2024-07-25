#include "transfertipsdialog.h"
#include "ui_transfertipsdialog.h"
#include <QGraphicsDropShadowEffect>

TransferTipsDialog::TransferTipsDialog(int iPhoneCount,QString strPhoneOrAccount, QWidget *parent)
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

    QString strContent = QString("<font color='#4A4A4A'>是否将%1台云手机转移至账号</font><font color='#F96D6F'>%2</font><font color='#4A4A4A'>,信号确认无误后,将确认转移</font>").arg(iPhoneCount).arg(strPhoneOrAccount);
    ui->labelContent->setText(strContent);
    //自动换行
    ui->labelContent->setWordWrap(true);
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


void TransferTipsDialog::on_btnClose_clicked()
{
    this->reject();
}

