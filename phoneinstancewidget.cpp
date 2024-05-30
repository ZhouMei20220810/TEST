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

    m_toolObject = new ToolObject(this);
    m_PhoneInfo = sPhoneInfo;
    m_strPhoneList.clear();
    m_strPhoneList << sPhoneInfo.strInstanceNo;//同步操作时，同时传入所有选中的item
    ui->toolBtnShow->setVisible(false);
    m_strPicturePath = QDir::tempPath() + "/" + SCREENSHOT_PICTRUE_FLODER + "/" + m_PhoneInfo.strInstanceNo + ".png";

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
    //切换窗口状态
    if (window()->isMaximized()) {
        return;
    }
    // 获取当前窗口状态
    Qt::WindowFlags flags = window()->windowFlags();

    // 切换置顶状态
    flags ^= Qt::WindowStaysOnTopHint;

    // 更新窗口状态
    window()->setWindowFlags(flags);

    // 检查窗口是否可见
    if (!window()->isVisible()) {
        // 窗口不可见，尝试重新显示窗口
        window()->show();
    }
    if (flags & Qt::WindowStaysOnTopHint) {
        ui->toolBtnTopLevel->setText("➴");
        QString styleSheet = QString("QToolButton{background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
        ui->toolBtnTopLevel->setStyleSheet(styleSheet);
    }
    else {
        ui->toolBtnTopLevel->setText("↘");
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
        ui->toolBtnTopLevel->setStyleSheet(styleSheet);
    }
}

void PhoneInstanceWidget::on_toolBtnMin_clicked()
{
    this->showMinimized();
}


void PhoneInstanceWidget::on_toolBtnMax_clicked()
{
    //最大化、常态
    QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
    ui->toolBtnTopLevel->setStyleSheet(styleSheet);
    if (window()->isMaximized()) {
        ui->toolBtnMax->setIcon(QIcon(":/instance/resource/instance/b3.png"));
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);margin-bottom:3px;}QToolButton:hover {color: rgb(255, 255, 255);}");
        ui->toolBtnMax->setStyleSheet(styleSheet);
        this->showNormal();
    }
    else {
        ui->toolBtnMax->setIcon(QIcon(":/instance/resource/instance/b5.png"));
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);margin-bottom:2px;}QToolButton:hover {color: rgb(255, 255, 255);}");
        ui->toolBtnMax->setStyleSheet(styleSheet);
        this->showMaximized();
    }
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
    //只需要截图，不需要获取当前页面
    this->m_toolObject->HttpPostInstanceScreenshot(m_strPhoneList);
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
    this->m_toolObject->HttpPostInstanceReboot(m_strPhoneList);
}


void PhoneInstanceWidget::on_toolBtnFactoryDataReset_clicked()
{
    //恢复出厂设置
    this->m_toolObject->HttpPostInstanceReset(m_strPhoneList);
}

void PhoneInstanceWidget::showEvent(QShowEvent *event)
{
    qDebug() << "PhoneInstanceWidget showEvent = ";    
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


