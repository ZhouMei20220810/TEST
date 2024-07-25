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
    setWindowTitle("ȷ��ת��");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//��Ӱģ���뾶
    shadow->setXOffset(0);//ˮƽƫ��
    shadow->setYOffset(0); //��ֱƫ��
    shadow->setColor(Qt::gray);//��Ӱ��ɫ
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

