#include "recentcopycutcontentdialog.h"
#include "ui_recentcopycutcontentdialog.h"
#include <QGraphicsDropShadowEffect>

RecentCopyCutContentDialog::RecentCopyCutContentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecentCopyCutContentDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("最近复制/剪切过的文字");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);
}

RecentCopyCutContentDialog::~RecentCopyCutContentDialog()
{
    delete ui;
}

void RecentCopyCutContentDialog::on_btnCopyToPhone_clicked()
{
    //粘贴到云手机中
}


void RecentCopyCutContentDialog::on_btnDirectCopy_clicked()
{
    //直接拷贝
}


void RecentCopyCutContentDialog::on_btnCopyByOrder_clicked()
{
    //按顺序依次拷贝
}


void RecentCopyCutContentDialog::on_btnClose_clicked()
{
    this->close();
}

