#include "systemsettingwidget.h"
#include "ui_systemsettingwidget.h"
#include "messagetips.h"
#include <QDesktopServices>
#include <QSettings>
#include <QDir>
#include <QProcess>
//#include <QShortcut>

SystemSettingWidget::SystemSettingWidget(QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::SystemSettingWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);    
    setWindowTitle("系统设置");
    m_enQuality = GlobalData::enPictrueQuality;
    m_bVerticalScreen = GlobalData::bVerticalPhoneInstance;
    m_bCloseMainWindowExit = GlobalData::bCloseMainWindowExit;
    m_bShowTrayIcon = GlobalData::bShowSystemTrayIcon;
    m_bIsPageMode = GlobalData::bIsPageMode;
    ui->checkBox_3->setChecked(m_bShowTrayIcon);
    //更新软件版本
    ui->labelVersion->setText(QString("当前版本: ")+CURRENT_APP_VERSION);
    ui->labelVersion2->setText(QString("当前版本: ")+CURRENT_APP_VERSION);
    switch (m_enQuality)
    {
    case TYPE_QUALITY_AUTO:
        ui->radioButton_5->setChecked(true);
        break;
    case TYPE_QUALITY_TOPSPEED:
        ui->radioButton_6->setChecked(true);
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

    if (GlobalData::bVerticalPhoneInstanceCenter)
    {
        ui->radioButton_13->setChecked(true);
    }
    else
    {
        ui->radioButton_12->setChecked(true);
    }
    if (m_bCloseMainWindowExit)
    {
        ui->radioButton_2->setChecked(true);
    }
    else
    {
        ui->radioButton->setChecked(true);
    }
    if (m_bIsPageMode)
    {
        ui->radioButton_4->setChecked(true);
    }
    else
    {
        ui->radioButton_3->setChecked(true);
    }
    ui->checkBox->setChecked(GlobalData::bBootstrapAutoStart);
    ui->checkBox_2->setChecked(GlobalData::bIsTopWindow);

    m_qualityGroup = new QButtonGroup(this);
    m_qualityGroup->addButton(ui->radioButton_5, TYPE_QUALITY_AUTO);
    m_qualityGroup->addButton(ui->radioButton_6, TYPE_QUALITY_TOPSPEED);
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

    /*QShortcut* bossKey = new QShortcut(QKeySequence("Ctrl+Shift+Q"),this);
    QObject::connect(bossKey, &QShortcut::activated, this, [=](){
        if (this->isVisible()) {
            this->hide(); // 隐藏窗口
        } else {
            this->showNormal(); // 显示窗口
        }
    });*/

    InitListWidget();
    getLoggedInUserInfo();
}

void SystemSettingWidget::InitListWidget()
{
    //设置QListWidget的显示模式    
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(ITEM_WIDGET_SPACING);
    ui->listWidget->setFixedHeight(128);
    //设置能移动
    ui->listWidget->setMovement(QListWidget::Free);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);   

    //QToolButton* btn;
    //QLabel* label;
    QListWidgetItem* item = NULL;
    QString strIcon;    
    int iType = 0;
    ui->listWidget->setIconSize(QSize(ICON_WIDTH, ICON_HEIGHT));
    /*ui->listWidget->setGridSize(QSize(128, 462));
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setWrapping(true);*/
    
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
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    GlobalData::enPictrueQuality = m_enQuality;
    GlobalData::bVerticalPhoneInstance = m_bVerticalScreen;
    GlobalData::bVerticalPhoneInstanceCenter = m_bPhoneInstanceCenter;
    GlobalData::bCloseMainWindowExit = m_bCloseMainWindowExit;
    GlobalData::bShowSystemTrayIcon = m_bShowTrayIcon;
    GlobalData::bIsPageMode = m_bIsPageMode;
    setting.setValue("PictureQuality", m_enQuality);    
    setting.setValue("VerticalScreen", m_bVerticalScreen);
    setting.setValue("PhoneInstanceCenter", m_bPhoneInstanceCenter);
    setting.setValue("CloseMainWindowExit", m_bCloseMainWindowExit);
    setting.setValue("ShowSystemTrayIcon", m_bShowTrayIcon);
    setting.setValue("PageMode", m_bIsPageMode);

    //获取工具栏按钮设置
        
    GlobalData::strToolButtonList = ui->listWidget->SaveToolsOrder();
    qDebug() << "strToolList = " << GlobalData::strToolButtonList;
    setting.setValue("ToolButtonList", GlobalData::strToolButtonList);
    GlobalData::bBootstrapAutoStart = ui->checkBox->isChecked();
    setting.setValue("BootstrapAutoStart", GlobalData::bBootstrapAutoStart);
    GlobalData::bIsTopWindow = ui->checkBox_2->isChecked();
    setting.setValue("MainWindowTopWindow", GlobalData::bIsTopWindow);
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (GlobalData::bBootstrapAutoStart) {
        //设置开机自启注册表
        QString strAppPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        reg.setValue("YiShunYun", strAppPath);
    }
    else {
        //取消开机自启注册表
        reg.remove("YiShunYun");
    }
    //保存
    MessageTips* tips = new MessageTips("数据已保存",this);
    tips->show();

    this->accept();
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


void SystemSettingWidget::on_pushButton_2_clicked()
{
    ui->listWidget->ResetArray();    
}


void SystemSettingWidget::on_radioButton_clicked(bool checked)
{
    m_bCloseMainWindowExit = false;
}


void SystemSettingWidget::on_radioButton_2_clicked(bool checked)
{
    m_bCloseMainWindowExit = true;
}

void SystemSettingWidget::getLoggedInUserInfo()
{
    QProcess process;

    // 获取当前登录的用户名
    process.start("whoami");
    process.waitForFinished(); // 等待命令执行完成
    QString userName = process.readAllStandardOutput();
    userName = userName.trimmed(); // 去除可能的空白字符

    // 获取计算机名
    process.start("hostname");
    process.waitForFinished(); // 等待命令执行完成
    QString computerName = process.readAllStandardOutput();
    computerName = computerName.trimmed(); // 去除可能的空白字符

    qDebug() << "Logged in user:" << userName;
    qDebug() << "Computer name:" << computerName;
    ui->labelLoginDevice->setText("当前登录设备:"+ computerName);
}

void SystemSettingWidget::on_radioButton_3_clicked(bool checked)
{
    m_bIsPageMode = false;
}


void SystemSettingWidget::on_radioButton_4_clicked(bool checked)
{
    m_bIsPageMode = true;
}

