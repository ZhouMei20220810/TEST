#include "systemsettingwidget.h"
#include "ui_systemsettingwidget.h"
#include "messagetips.h"
#include <QDesktopServices>
#include <QSettings>

SystemSettingWidget::SystemSettingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SystemSettingWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);

    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    m_enQuality = (ENUM_PICTURE_QUALITY)setting.value("PictureQuality", TYPE_QUALITY_HIGH_SPEED).toInt();
    m_bVerticalScreen = setting.value("VerticalScreen", true).toBool();
    m_bPhoneInstanceCenter = setting.value("PhoneInstanceCenter",true).toBool();

    switch (m_enQuality)
    {
    case TYPE_QUALITY_AUTO:
        ui->radioButton_5->setChecked(true);
        break;
    case TYPE_QUALITY_TOPSPEED:
        ui->radioButton_6->setChecked(true);
        break;
    case TYPE_QUALITY_HIGH_SPEED:
        ui->radioButton_7->setChecked(true);
        break;
    case TYPE_QUALITY_NORMAL:
        ui->radioButton_8->setChecked(true);
        break;
    case TYPE_QUALITY_HIGH_DEFINITION:
        ui->radioButton_9->setChecked(true);
        break;
    }

    if (m_bVerticalScreen)
    {
        ui->radioButton_11->setChecked(true);
    }
    else
    {
        ui->radioButton_10->setChecked(true);
    }

    if (m_bPhoneInstanceCenter)
    {
        ui->radioButton_13->setChecked(true);
    }
    else
    {
        ui->radioButton_12->setChecked(true);
    }

    m_qualityGroup = new QButtonGroup(this);
    m_qualityGroup->addButton(ui->radioButton_5, TYPE_QUALITY_AUTO);
    m_qualityGroup->addButton(ui->radioButton_6, TYPE_QUALITY_TOPSPEED);
    m_qualityGroup->addButton(ui->radioButton_7, TYPE_QUALITY_HIGH_SPEED);
    m_qualityGroup->addButton(ui->radioButton_8, TYPE_QUALITY_NORMAL);
    m_qualityGroup->addButton(ui->radioButton_9, TYPE_QUALITY_HIGH_DEFINITION);
    connect(m_qualityGroup,&QButtonGroup::buttonToggled,this,[this](QAbstractButton* button, bool checked)
        {
        if (checked) 
        {
            // 这里可以获取按钮的状态和ID
            m_enQuality = (ENUM_PICTURE_QUALITY)m_qualityGroup->id(button);
            bool isChecked = button->isChecked();
            qDebug() << "Button:" << m_enQuality << (isChecked ? "checked" : "unchecked");
        }
    });
}

SystemSettingWidget::~SystemSettingWidget()
{
    delete ui;
}

void SystemSettingWidget::on_toolBtnGeneralSetting_clicked()
{
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageGeneralSetting);
}


void SystemSettingWidget::on_toolBtnUISetting_clicked()
{
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageUISetting);
}


void SystemSettingWidget::on_toolBtnAbout_clicked()
{
    ui->toolBtnAbout->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnGeneralSetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnUISetting->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->frame_7->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->pageAbout);
}


void SystemSettingWidget::on_pushButton_clicked()
{
    this->close();
}


void SystemSettingWidget::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void SystemSettingWidget::on_btnSave_clicked()
{
    //获取界面数据并保存
    qDebug()<<"m_enQuality="<<m_enQuality;
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    setting.setValue("PictureQuality", m_enQuality);
    setting.setValue("VerticalScreen", m_bVerticalScreen);
    setting.setValue("PhoneInstanceCenter", m_bPhoneInstanceCenter);
    //保存
    MessageTips* tips = new MessageTips("数据已保存",this);
    tips->show();
}


void SystemSettingWidget::on_pushButton_3_clicked()
{
    QString strUrl = "https://www.ysyos.com/";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_4_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/Disclaimer.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_5_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/UserPrivacy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_6_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/PrivacyPolicy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_pushButton_7_clicked()
{
    QString strUrl = "https://www.ysyos.com/deal/VipPrivacy.html";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void SystemSettingWidget::on_radioButton_10_clicked(bool checked)
{
    if(checked)
    {
        m_bVerticalScreen = false;
    }
}


void SystemSettingWidget::on_radioButton_11_clicked(bool checked)
{
    if(checked)
    {
        m_bVerticalScreen = true;
    }
}


void SystemSettingWidget::on_radioButton_12_clicked(bool checked)
{
    if(checked)
    {
        //记录上次的位置
        m_bPhoneInstanceCenter = false;
    }
}


void SystemSettingWidget::on_radioButton_13_clicked(bool checked)
{
    if(checked)
    {
        //居中
        m_bPhoneInstanceCenter = true;
    }
}

