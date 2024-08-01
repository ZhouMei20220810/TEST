#include "oneclicknewmachinedialog.h"
#include "ui_oneclicknewmachinedialog.h"
#include <QGraphicsDropShadowEffect>

OneClickNewMachineDialog::OneClickNewMachineDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OneClickNewMachineDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("一键新机");

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);
}

OneClickNewMachineDialog::~OneClickNewMachineDialog()
{
    delete ui;
}

void OneClickNewMachineDialog::on_btnClose_clicked()
{
    this->close();
}


void OneClickNewMachineDialog::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void OneClickNewMachineDialog::on_btnOk_clicked()
{
    //确定修改

}

