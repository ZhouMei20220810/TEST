#include "phoneinstancewidget.h"
#include "ui_phoneinstancewidget.h"
#include <QDir>
#include <QClipboard>
#include "messagetips.h"

PhoneInstanceWidget::PhoneInstanceWidget(S_PHONE_INFO sPhoneInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneInstanceWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlag(Qt::FramelessWindowHint);

    m_PhoneInfo = sPhoneInfo;
    ui->toolBtnShow->setVisible(false);

    ui->toolBtnPhoneInstance->setText(sPhoneInfo.strInstanceNo);
    //ui->frame_2->setVisible(true);    
}

PhoneInstanceWidget::~PhoneInstanceWidget()
{
    delete ui;
}

void PhoneInstanceWidget::on_toolBtnPhoneInstance_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->toolBtnPhoneInstance->text());

    MessageTips* tips = new MessageTips("已复制云号至剪贴板", this);
    tips->show();
}

void PhoneInstanceWidget::on_toolBtnPictureQuality_clicked()
{

}


void PhoneInstanceWidget::on_toolBtnTopLevel_clicked()
{

}


void PhoneInstanceWidget::on_toolBtnMin_clicked()
{
    this->showMinimized();
}


void PhoneInstanceWidget::on_toolBtnMax_clicked()
{
    this->showMaximized();
}


void PhoneInstanceWidget::on_toolBtnClose_clicked()
{
    this->close();
}


void PhoneInstanceWidget::on_toolBtnShow_clicked()
{
    ui->toolBtnShow->setVisible(false);
    ui->frame_2->setVisible(true);
}


void PhoneInstanceWidget::on_toolBtnHide_clicked()
{
    ui->toolBtnShow->setVisible(true);
    ui->frame_2->setVisible(false);
}


void PhoneInstanceWidget::on_toolBtnSignal_clicked()
{
    //信号
}


void PhoneInstanceWidget::on_toolBtnVerOrHor_clicked()
{
    //横竖屏
}


void PhoneInstanceWidget::on_toolBtnScreenshot_clicked()
{
    //截图
}


void PhoneInstanceWidget::on_toolBtnAddVolume_clicked()
{
    //音量调大
}


void PhoneInstanceWidget::on_toolBtnSubVolume_clicked()
{
    //音量调小
}


void PhoneInstanceWidget::on_toolBtnClipboard_clicked()
{
    //剪贴板
}


void PhoneInstanceWidget::on_toolBtnRestart_clicked()
{
    //重启
}


void PhoneInstanceWidget::on_toolBtnFactoryDataReset_clicked()
{
    //恢复出厂设置
}



void PhoneInstanceWidget::showEvent(QShowEvent *event)
{
    qDebug() << "showEvent = ";
    m_strPicturePath = QDir::tempPath() + "/" + SCREENSHOT_PICTRUE_FLODER + "/" + m_PhoneInfo.strInstanceNo + ".png";
    QFile file1(m_strPicturePath);
    QString strUrl;
    if (!file1.exists())
        strUrl = ":/main/resource/main/defaultSceenShot.png";
    else
        strUrl = m_strPicturePath;
    ui->labelPhone->setPixmap(QPixmap(strUrl).scaled(QSize(ui->labelPhone->width(), ui->labelPhone->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QSize actualSize = ui->labelPhone->size();
    ui->labelPhone->setPixmap(QPixmap(strUrl).scaled(QSize(ui->labelPhone->width(), ui->labelPhone->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    qDebug() << "Size after showing:" << actualSize;
}


