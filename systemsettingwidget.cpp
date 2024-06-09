#include "systemsettingwidget.h"
#include "ui_systemsettingwidget.h"
#include "messagetips.h"
#include <QDesktopServices>
#include <QSettings>
#include <QDir>

SystemSettingWidget::SystemSettingWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SystemSettingWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);    
    m_enQuality = GlobalData::enPictrueQuality;
    m_bVerticalScreen = GlobalData::bVerticalPhoneInstance;
    m_bPhoneInstanceCenter = GlobalData::bVerticalPhoneInstanceCenter;
    m_bCloseMainWindowExit = GlobalData::bCloseMainWindowExit;
    m_bShowTrayIcon = GlobalData::bShowSystemTrayIcon;
    ui->checkBox_3->setChecked(m_bShowTrayIcon);
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
    if (m_bCloseMainWindowExit)
    {
        ui->radioButton_2->setChecked(true);
    }
    else
    {
        ui->radioButton->setChecked(true);
    }

    ui->checkBox->setChecked(GlobalData::bBootstrapAutoStart);

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

    InitListWidget();
}

void SystemSettingWidget::InitListWidget()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidget->sortItems(Qt::DescendingOrder);
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    //ui->listWidget->setResizeMode(QListWidget::Adjust);
    //ui->listWidget->setSizePolicy(QSizePolicy.f)
    ui->listWidget->setFixedHeight(128);
    //设置能移动
    ui->listWidget->setMovement(QListWidget::Free);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setDragEnabled(true);
    //设置接受拖放
    ui->listWidget->setSortingEnabled(true);
    ui->listWidget->viewport()->setAcceptDrops(true);
    ui->listWidget->setDropIndicatorShown(true);
    ui->listWidget->setDragDropMode(QAbstractItemView::DragDrop);
    //ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);


    //设置排序为倒序
/*ui->listWidget->sortItems(Qt::DescendingOrder);
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置选择模式为单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //启用拖动
    ui->listWidget->setDragEnabled(true);
    //设置接受拖放
    ui->listWidget->viewport()->setAcceptDrops(true);
    //设置要显示将要被放置的位置
    ui->listWidget->setDropIndicatorShown(true);
    //设置拖放模式为移动项目，如果不设置，默认为复制项目
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);*/

    QSize size(40, 46);
    //QToolButton* btn;
    QLabel* label;
    QListWidgetItem* item = NULL;
    QString strIcon;    
    if (!GlobalData::strToolButtonList.isEmpty())
    {
        QStringList strList = GlobalData::strToolButtonList.split(',');
        for (int i = 0; i < strList.size(); i++)
        {
            item = new QListWidgetItem(ui->listWidget);
            item->setData(Qt::UserRole, i);
            item->setSizeHint(size);
            //btn = new QToolButton(this);
            label = new QLabel(this);
            strIcon = QString::asprintf(":/resource/setting/%d.png", strList.at(i).toInt());
            //btn->setIcon(QIcon(strIcon));
            //btn->setIconSize(size);
            label->setPixmap(QPixmap(strIcon));
            label->resize(size);
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, label);
        }
    }
    else
    {
        for (int i = 1; i <= 12; i++)
        {
            item = new QListWidgetItem(ui->listWidget);
            item->setData(Qt::UserRole, i);
            item->setSizeHint(size);
            //btn = new QToolButton(this);
            label = new QLabel(this);
            strIcon = QString::asprintf(":/resource/setting/%d.png", i);
            //btn->setIcon(QIcon(strIcon));
            //btn->setIconSize(size);
            label->setPixmap(QPixmap(strIcon));
            label->resize(size);
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, label);
        }
    }
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
    setting.setValue("PictureQuality", m_enQuality);    
    setting.setValue("VerticalScreen", m_bVerticalScreen);
    setting.setValue("PhoneInstanceCenter", m_bPhoneInstanceCenter);
    setting.setValue("CloseMainWindowExit", m_bCloseMainWindowExit);
    setting.setValue("ShowSystemTrayIcon", m_bShowTrayIcon);
    

    //获取工具栏按钮设置
    QListWidgetItem* item= NULL;
    int iCount = ui->listWidget->count();
    int iType = 0;
    QString strToolList ="";
    //strToolList.clear();
    for(int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        iType = item->data(Qt::UserRole).toInt();
        if (strToolList.isEmpty())
        {
            strToolList = QString::asprintf("%d", iType);
        }
        else
        {
            strToolList += QString::asprintf(",%d", iType);
        }
    }
    qDebug() << "strToolList = " << strToolList;
    GlobalData::strToolButtonList = strToolList;
    setting.setValue("ToolButtonList", strToolList);
    GlobalData::bBootstrapAutoStart = ui->checkBox->isChecked();
    setting.setValue("BootstrapAutoStart", GlobalData::bBootstrapAutoStart);
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
    //设置排序    
    ui->listWidget->sortItems(Qt::AscendingOrder);
}


void SystemSettingWidget::on_radioButton_clicked(bool checked)
{
    m_bCloseMainWindowExit = false;
}


void SystemSettingWidget::on_radioButton_2_clicked(bool checked)
{
    m_bCloseMainWindowExit = true;
}


void SystemSettingWidget::on_checkBox_3_clicked(bool checked)
{
    m_bShowTrayIcon = checked;
}

