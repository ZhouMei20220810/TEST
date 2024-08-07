#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QTreeWidget>
#include "updategroupwidget.h"
//#include "levelitemwidget.h"
#include <QAbstractItemView>
#include "vipitemwidget.h"
#include "Logoutdialog.h"
#include "messagetipsdialog.h"
#include "phoneitemwidget.h"
#include "phonelistmodeitemwidget.h"
#include <QScrollBar>
#include <QFile>
#include <QDir>
#include "qrencode.h"
#include <QPainter>
#include <QClipboard>
#include "messagetips.h"
#include <QThread>
#include "uploadfiledialog.h"
#include "renewitemwidget.h"
#include "filedownloader.h"
#include "factorydataresetdialog.h"
#include <QDesktopServices>
#include "messagetips.h"
#include "policydialog.h"
#include <QGraphicsDropShadowEffect>
#include "addactivecodedialog.h"
#include "activecodeitem.h"
#include "activecoderenewitem.h"
#include "customtoolbutton.h"
#include "addauthorizationdialog.h"
#include "replacecloudphonedialog.h"
#include "transferphonedialog.h"

extern QSystemTrayIcon* g_trayIcon;

#define         LEVEL_TYPE_TOOLBUTTON_WIDTH        (80)
#define         LEVEL_TYPE_TOOLBUTTON_HEIGHT       (32)

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose,true);
    setWindowFlag(Qt::WindowStaysOnTopHint, GlobalData::bIsTopWindow);
    setWindowTitle("易舜云手机");
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    ui->centralwidget->setGraphicsEffect(shadow);

    this->setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);
    //ui->frame->setMouseTracking(true);
    //ui->frame_9->setMouseTracking(true);
    //ui->frame_2->setMouseTracking(true);
    dir = NONE;
    m_oldSize = this->size();
    m_globalPoint = this->pos();
    this->setCursor(QCursor(Qt::ArrowCursor));

    m_MainPhoneInstanceWidget = NULL;
    m_createGroupWidget = NULL;
    m_systemSettingWidget = NULL;
    m_SyncOperListWidget = NULL;
    m_individualCenterWidget = NULL;

    m_pCurItem = NULL;
    m_PayTimer = NULL;
    m_Timer = NULL;

    m_toolObject = new ToolObject(this);
    connect(m_toolObject, &ToolObject::startTimerShowScreenshotSignals, this,[=]
            {
        m_Timer->start(DOWNLOAD_SCREENSHOT_INTERVAL);
    });
    connect(m_toolObject, &ToolObject::getScreenshortSignals, this, [=](QMap<QString, S_TASK_INFO> mapScreenshotTask) {
        m_mapTask = mapScreenshotTask;
        ShowTaskInfo();
        });
    connect(m_toolObject, &ToolObject::ShowAuthDetailSignals, this, [=](S_AUTHOR_INFO authInfo)
        {
            AddAuthorizationDialog* dialog = new AddAuthorizationDialog(m_CurSelMenuPhoneInfo);
            connect(dialog, &AddAuthorizationDialog::notifyMainWindowRefreshGroupListSignals, this, &MainWindow::on_btnGroupRefresh_clicked);
            dialog->InitWidget(authInfo);
            dialog->exec();
        });

    ui->labelAccount->setText(GlobalData::strAccount);

    m_PayTimer = new QTimer();
    connect(m_PayTimer, &QTimer::timeout, this, [=]() {
        if (m_iPayCount > 0)
        {
            ui->labelTimer->setText(QString::asprintf("%ds",m_iPayCount));
            m_iPayCount--;
        }
        else
        {
            ui->labelTimer->setText(QString::asprintf("%ds", m_iPayCount));
            ui->labelPayTimeoutTip->setVisible(true);
            ui->btnRefreshQrCode->setVisible(true);
            m_PayTimer->stop();
        }
        });

    m_TaskTimer = new QTimer();
    connect(m_TaskTimer, &QTimer::timeout, this, &MainWindow::do_timeoutRefreshPicture);

    m_Timer = new QTimer();
    connect(m_Timer, &QTimer::timeout, this, [this]() 
        {
        m_Timer->stop();
        this->m_toolObject->HttpPostInstanceScreenshot(m_listInstanceNo);
        });

    //加载等级列表
    HttpLevelList();

    on_btnGroupRefresh_clicked();
    //初始化Tab云手机
    InitCloudPhoneTab();
    //初始化Tab激活码
    InitActiveCodeTab(); 
    //初始化Tab购买
    InitBuyTab();

    //第一版暂时屏蔽功能
    ui->frame_4->setVisible(false);
    ui->frame_8->setVisible(false);
    ui->toolBtnMacroscheme->setVisible(false);
    ui->toolBtnAppInstall->setVisible(false);
    //暂时屏蔽翻页功能
    ui->frame_PageMode->setVisible(false);
    //暂时屏蔽激活码分组的功能
    ui->comboBoxGroupName->setVisible(false);

    if (!GlobalData::bCloseMainWindowExit && NULL == g_trayIcon)
    {
        g_trayIcon = new QSystemTrayIcon(QIcon(":/main/resource/main/aboutlogo.png"));
        g_trayIcon->setToolTip("易舜云手机");
        g_trayIcon->setVisible(true);
        g_trayIcon->show();        
    }
    if (NULL != g_trayIcon)
    {
        QMenu* menu = new QMenu();
        menu->setStyleSheet("QMenu{ background-color:#FFFFFF;border:1px solid rgba(255,255,255,1);width:200; padding-left:20px; } \
        QMenu::item{ min-width:50px; font-size:12px; color:#505465;width:200;height:20; background:rgba(255,255,255,0.5); border:0px solid rgba(82,130,164,1); padding:1px 1px; margin:1px 1px; }\
        QMenu::item:selected{ background:#F5F7FB;font-weight:bold;width:200;height:20; border:0px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
        QMenu::item:pressed{ background:#F5F7FB;font-weight:bold;width:200;height:20; border:0px solid rgba(82,130,164,1);/*摁下状态*/ }");

        QAction* pActionExit = new QAction("退出");
        QObject::connect(pActionExit, &QAction::triggered, QApplication::instance(), &QApplication::quit);
        menu->addAction(pActionExit);
        g_trayIcon->setContextMenu(menu);
    }
    connect(g_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::handleTrayIconActivated);
    this->setContextMenuPolicy(Qt::PreventContextMenu);    

    ui->stackedWidget->setCurrentWidget(ui->page);
}

MainWindow::~MainWindow()
{
    if (m_TaskTimer->isActive())
    {
        m_TaskTimer->stop();
    }
    if (m_Timer->isActive())
    {
        m_Timer->stop();
    }
    delete ui;
}

//组菜单
void MainWindow::do_DeleteGroupAction(bool bChecked)
{
    QTreeWidgetItem* item = ui->treeWidget->currentItem();
    S_GROUP_INFO groupInfo = item->data(0, Qt::UserRole).value<S_GROUP_INFO>();
    HttpDeleteGroup(groupInfo.iGroupId);
}

void MainWindow::do_EditGroupNameAction(bool bChecked)
{
    if (NULL == m_createGroupWidget)
    {
        m_createGroupWidget = new CreateGroupWidget(TYPE_UPDATE_GROUP_WIDGET);
    }
        
    connect(m_createGroupWidget, &CreateGroupWidget::createGroupSignals, this, &MainWindow::do_createGroupSignals);
    connect(m_createGroupWidget, &CreateGroupWidget::destroyed, this, [=]() {
        m_createGroupWidget = NULL;
        });
    m_createGroupWidget->setModal(true);
    m_createGroupWidget->show();
}

//实例重命名
void MainWindow::HttpPostInstanceRename(int iId, QString strName)
{
    //实例名称修改
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_SET_INSTANCE_NAME;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("id", iId);
    obj.insert("name", strName);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode && data)
                {
                    //phoneid=id;
                    QTreeWidgetItemIterator it(ui->treeWidget);
                    //遍历所有选中项的迭代器
                    QTreeWidgetItem* item = NULL;
                    S_PHONE_INFO phoneInfo;                    
                    bool bFind = false;
                    QTreeWidgetItem* child = NULL;
                    int count = 0;
                    int i=0;                    
                    while (*it)
                    {
                        item = *it;
                        qDebug() << item->text(0);
                        //获取组的所有子节点
                        count = item->childCount();                        
                        for (i = 0; i < count; ++i)
                        {
                            child = item->child(i);
                            phoneInfo = child->data(0, Qt::UserRole).value<S_PHONE_INFO>();  
                            if (phoneInfo.iId == iId)
                            {
                                phoneInfo.strName = strName;
                                child->setData(0, Qt::UserRole, QVariant::fromValue(phoneInfo));
                                child->setText(0, m_mapLevelList.find(phoneInfo.iLevel).value().strLevelName+" "+strName);
                                bFind = true;
                                break;
                            }
                        }
                        if (bFind)
                        {
                            break;
                        }
                        ++it;
                    }
                                  
                    int iListCount = 0;
                    int iRow = 0;
                    QListWidgetItem* phoneItem = NULL;                    
                    //重新显示listWidget
                    if (m_isIconMode)
                    {
                        PhoneItemWidget* widget = NULL;
                        iListCount = ui->listWidget->count();
                        for (iRow = 0; iRow < iListCount; iRow++)
                        {
                            phoneItem = ui->listWidget->item(iRow);
                            phoneInfo = phoneItem->data(Qt::UserRole).value<S_PHONE_INFO>();
                            if (phoneInfo.iId == iId)
                            {
                                phoneInfo.strName = strName;
                                phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                                widget = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(phoneItem));
                                if (widget != NULL)
                                {
                                    widget->setPhoneName(strName);                                    
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        PhoneListModeItemWidget* widget2 = NULL;
                        //listWidget2
                        iListCount = ui->listWidget2->count();
                        for (iRow = 0; iRow < iListCount; iRow++)
                        {
                            phoneItem = ui->listWidget2->item(iRow);
                            phoneInfo = phoneItem->data(Qt::UserRole).value<S_PHONE_INFO>();
                            if (phoneInfo.iId == iId)
                            {
                                phoneInfo.strName = strName;
                                phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                                widget2 = static_cast<PhoneListModeItemWidget*>(ui->listWidget2->itemWidget(phoneItem));
                                if (widget2 != NULL)
                                {
                                    widget2->setPhoneName(strName);
                                }
                                break;
                            }
                        }
                    }
                    /*if (m_pCurItem != NULL)
                    {
                        m_pCurItem->setText(0, strName);
                    }*/
                    //MessageTipsDialog* tips = new MessageTipsDialog("实例重命名成功!", this);
                    //tips->show();
                    //HttpQueryAllGroup();//否则改名后要遍历树节点去更改名称
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
//设置实例分组
void MainWindow::HttpPostInstanceSetGroup(int iGroupId, QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_SET_INSTANCE_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonObject jsonObj;
    jsonObj["groupId"] = iGroupId;
    
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i).toInt());
    }
    //doc.setObject(listArray);
    jsonObj["ids"] = listArray;
    //doc.setArray(listArray);
    QJsonDocument doc(jsonObj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool bData = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage <<"json=" << response;
                if (HTTP_SUCCESS_CODE == iCode && bData)
                {
                    on_btnGroupRefresh_clicked();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
//手机菜单
void MainWindow::do_ActionBeginControl(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;
    
    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();*/
    on_ShowPhoneInstanceWidgetSignals(m_CurSelMenuPhoneInfo,false);
}
void MainWindow::do_ActionCopyCloudId(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;

    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();*/
    QClipboard* clipboard = QApplication::clipboard();
    if (NULL != clipboard)
    {
        clipboard->setText(m_CurSelMenuPhoneInfo.strInstanceNo);
    }    

    MessageTips* tips = new MessageTips("复制成功", this);
    if (NULL != tips)
    {
        tips->show();
    }    
}
void MainWindow::do_ActionRename(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;

    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();*/
    if (NULL == m_createGroupWidget)
    {
        m_createGroupWidget = new CreateGroupWidget(TYPE_PHONE_RENAME_WIDGET, m_CurSelMenuPhoneInfo.iId, m_CurSelMenuPhoneInfo.strName);
    }

    connect(m_createGroupWidget, &CreateGroupWidget::createGroupSignals, this, &MainWindow::do_createGroupSignals);
    connect(m_createGroupWidget, &CreateGroupWidget::destroyed, this, [=]() {
        m_createGroupWidget = NULL;
        });
    m_createGroupWidget->setModal(true);
    m_createGroupWidget->show();
}
void MainWindow::do_ActionRestartCloudPhone(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;

    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();*/

    QStringList strList;    
    strList << m_CurSelMenuPhoneInfo.strInstanceNo;
    this->m_toolObject->HttpPostInstanceReboot(strList);
}

void MainWindow::do_ActionFactoryDataReset(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;*/

    FactoryDataResetDialog* dialog = new FactoryDataResetDialog();
    if (QDialog::Accepted == dialog->exec())
    {
        //qDebug() << "确定出厂";
        //S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();

        QStringList strList;
        strList << m_CurSelMenuPhoneInfo.strInstanceNo;
        this->m_toolObject->HttpPostInstanceReset(strList);
    }
}
void MainWindow::do_ActionUploadFile(bool bChecked)
{
    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;

    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();
    */
    QStringList strList;
    strList << m_CurSelMenuPhoneInfo.strInstanceNo;
    UploadFileDialog* upload = new UploadFileDialog(strList);
    upload->exec();
}
void MainWindow::do_ActionMoveGroup(bool bChecked)
{
    QAction* pAction = qobject_cast<QAction*>(sender());
    S_GROUP_INFO groupInfo;
    if (pAction != NULL)
    {
        groupInfo = pAction->data().value<S_GROUP_INFO>();
        qDebug() << "当前选中 分组名称" << groupInfo.strGroupName;
    }

    /*m_pCurItem = ui->treeWidget->currentItem();
    if (m_pCurItem == NULL)
        return;

    S_PHONE_INFO phoneInfo = m_pCurItem->data(0, Qt::UserRole).value<S_PHONE_INFO>();*/

    QStringList strList;
    strList << QString("%1").arg(m_CurSelMenuPhoneInfo.iId);
    HttpPostInstanceSetGroup(groupInfo.iGroupId, strList);
}
void MainWindow::do_ActionBatchMoveGroup(bool bChecked)
{
    QAction* pAction = qobject_cast<QAction*>(sender());
    S_GROUP_INFO groupInfo;
    if (pAction != NULL)
    {
        groupInfo = pAction->data().value<S_GROUP_INFO>();
        qDebug() << "当前选中 分组名称" << groupInfo.strGroupName;
    }

    //获取当前列表所有选中的项
    QStringList strPhoneList = getCheckedPhoneInstance(true);
    HttpPostInstanceSetGroup(groupInfo.iGroupId, strPhoneList);
}
void MainWindow::do_ActionRenewCloudPhone(bool bChecked)
{
    on_toolBtnBuy_clicked();
    //手机续费
    on_toolBtnRenewPhone_clicked();
}

void MainWindow::do_ActionAuthorization(bool bChecked)
{
    if (!m_CurSelMenuPhoneInfo.bIsAuth && m_CurSelMenuPhoneInfo.iAuthType == EN_BE_AUTHORIZATION)
    {
        return;
    }
    if (m_CurSelMenuPhoneInfo.bIsAuth || m_CurSelMenuPhoneInfo.iAuthType == EN_BE_AUTHORIZATION)
    {
        qDebug() << "已授权或者被授权id" << m_CurSelMenuPhoneInfo.iId;
        m_toolObject->HttpPostAuthDetail(m_CurSelMenuPhoneInfo.iId);
    }
    else
    {
        //授权
        AddAuthorizationDialog* dialog = new AddAuthorizationDialog(m_CurSelMenuPhoneInfo);
        connect(dialog, &AddAuthorizationDialog::notifyMainWindowRefreshGroupListSignals, this, &MainWindow::on_btnGroupRefresh_clicked);
        dialog->exec();
    }
    
}

void MainWindow::do_ActionReplaceCloudPhone(bool bChecked)
{
    //更换云机
    ReplaceCloudPhoneDialog* dialog = new ReplaceCloudPhoneDialog(m_CurSelMenuPhoneInfo,m_mapLevelList);
    dialog->exec();
}

//获取当前所有选中的item,根据enum判断是返回树形控件，预览模式还是列表模式
QMap<int, S_PHONE_INFO> MainWindow::getCurrentAllSelectItem(EN_RIGHT_CLICK_TYPE enType)
{
    QMap<int, S_PHONE_INFO> map;
    S_PHONE_INFO phoneInfo;
    switch (GlobalData::enRightClickType)
    {
    case EN_GROUP_TREEWIDGET:
        //获取树形控件选中项
    {
        QTreeWidgetItemIterator it(ui->treeWidget);
        //遍历所有选中项的迭代器
        QTreeWidgetItem* item = NULL;        
        QTreeWidgetItem* child = NULL;
        int count = 0;
        int i = 0;
        Qt::CheckState checkState;
        while (*it)
        {
            item = *it;
            //获取组的所有子节点
            count = item->childCount();
            for (i = 0; i < count; ++i)
            {
                child = item->child(i);
                checkState = child->checkState(0);
                phoneInfo = child->data(0, Qt::UserRole).value<S_PHONE_INFO>();
                if (checkState == Qt::Checked)
                {
                    if (phoneInfo.bIsAuth || phoneInfo.iAuthType == EN_BE_AUTHORIZATION)
                    {
                        qDebug() << "已授权或者被授权 name=" << phoneInfo.strName << "No=" << phoneInfo.strInstanceNo;
                        continue;
                    }
                    map.insert(phoneInfo.iId, phoneInfo);
                }
            }
            ++it;
        }
    }
    break;
    case EN_ICON_MODE_WIDGET:
        //获取预览模式选中项
    {
        int iCount = ui->listWidget->count();
        if (iCount > 0)
        {
            QListWidgetItem* item = NULL;
            QListWidgetItem* phoneItem = NULL;
            QMap<int, S_LEVEL_INFO>::iterator iterFind;
            for (int i = 0; i < iCount; i++)
            {
                item = ui->listWidget->item(i);
                if (item != NULL)
                {
                    //是否选中
                    if (((PhoneItemWidget*)ui->listWidget->itemWidget(item))->getCheckBoxStatus())
                    {
                        phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                        if (phoneInfo.bIsAuth || phoneInfo.iAuthType == EN_BE_AUTHORIZATION)
                        {
                            qDebug() << "已授权或者被授权 name=" << phoneInfo.strName << "No=" << phoneInfo.strInstanceNo;
                            continue;
                        }
                        map.insert(phoneInfo.iId, phoneInfo);
                    }
                }
            }
        }
    }
        break;
    case EN_LIST_MODE_WIDGET:
        //获取列表模式选中项
    {
        int iCount = ui->listWidget2->count();
        if (iCount > 0)
        {
            QListWidgetItem* item = NULL;
            for (int i = 0; i < iCount; i++)
            {
                item = ui->listWidget2->item(i);
                if (item != NULL)
                {
                    //是否选中
                    if (((PhoneListModeItemWidget*)ui->listWidget2->itemWidget(item))->getCheckBoxStatus())
                    {
                        phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                        if (phoneInfo.bIsAuth || phoneInfo.iAuthType == EN_BE_AUTHORIZATION)
                        {
                            qDebug() << "已授权或者被授权 name=" << phoneInfo.strName << "No=" << phoneInfo.strInstanceNo;
                            continue;
                        }
                        map.insert(phoneInfo.iId, phoneInfo);
                    }
                }
            }
        }
    }
        break;
    default:
        break;
    }

    return map;
}
void MainWindow::do_ActionTransferCloudPhone(bool bChecked)
{
    //分别获取分组、预览模式、列表模式选中项
    QMap<int, S_PHONE_INFO> currentSelMap = getCurrentAllSelectItem(GlobalData::enRightClickType);
    if (currentSelMap.size() <= 0)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("当前没有选中可转移设备");
        dialog->show();
        return;
    }
    TransferPhoneDialog* dialog = new TransferPhoneDialog(currentSelMap, m_mapLevelList);
    connect(dialog, &TransferPhoneDialog::TransferSuccessRefreshInstanceListSignals, this, &MainWindow::do_TransferSuccessRefreshInstanceListSignals);
    dialog->exec();
}

void MainWindow::do_TransferSuccessRefreshInstanceListSignals()
{
    on_btnGroupRefresh_clicked();
}

void MainWindow::InitGroupMenu()
{
    //组右键菜单
    m_menu = new QMenu(ui->treeWidget);
    m_menu->setStyleSheet("QMenu{ background-color:#FFFFFF;font-size:14px;border:1px solid rgba(255,255,255,1);padding-left:20px; } \
        QMenu::item{ min-width:150px; font-size:12px; color:#505465;height:20; background:rgba(255,255,255,0.5); border:0px solid rgba(82,130,164,1); padding:1px 1px; margin:1px 1px; }\
        QMenu::item:selected{ background:#F5F7FB;font-weight:bold;height:20; border:0px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
        QMenu::item:pressed{ background:#F5F7FB;font-weight:bold;height:20; border:0px solid rgba(82,130,164,1);/*摁下状态*/ }");
    QAction* pActionDeleteGroup = new QAction("删除分组");
    QAction* pActionEditGroupName = new QAction("编辑分组名称");
    connect(pActionDeleteGroup, &QAction::triggered, this, &MainWindow::do_DeleteGroupAction);
    connect(pActionEditGroupName, &QAction::triggered, this, &MainWindow::do_EditGroupNameAction);
    m_menu->addAction(pActionDeleteGroup);
    m_menu->addAction(pActionEditGroupName);
}
void MainWindow::InitPhoneMenu()
{
    //手机右键菜单
    m_PhoneMenu = new QMenu(ui->treeWidget);
    m_PhoneMenu->setStyleSheet("QMenu{ background-color:#FFFFFF;font-size:14px;border:1px solid rgba(255,255,255,1);} \
        QMenu::item{ min-width:50px; font-size:12px;height:20; color:#505465;background:rgba(255,255,255,0.5); border:0px solid rgba(82,130,164,1); padding:1px 1px; margin:1px 1px; }\
        QMenu::item:selected{ background:#F5F7FB;font-weight:bold;height:20;border:0px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
        QMenu::item:pressed{ background:#F5F7FB;font-weight:bold;height:20; border:0px solid rgba(82,130,164,1);/*摁下状态*/ }");
    //m_PhoneMenu->setStyleSheet("QMenu{ background-color:#FFFFFF;border:1px solid rgba(255,255,255,1);width:200; padding-left:20px; } \
        QMenu::item{ min-width:50px; font-size:12px; color:#505465;width:200;height:20; background:rgba(255,255,255,0.5); border:0px solid rgba(82,130,164,1); padding:1px 1px; margin:1px 1px; }\
        QMenu::item:selected{ background:#F5F7FB;font-weight:bold;width:200;height:20; border:0px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
        QMenu::item:pressed{ background:#F5F7FB;font-weight:bold;width:200;height:20; border:0px solid rgba(82,130,164,1);/*摁下状态*/ }");
    pActionBeginControl = new QAction("开始控制", ui->treeWidget);
    pActionCopyCloudId = new QAction("复制云号", ui->treeWidget);
    pActionRename = new QAction("重命名", ui->treeWidget);
    pActionRestartCloudPhone = new QAction("重启云手机", ui->treeWidget);
    pActionFactoryDataReset = new QAction("恢复出厂设置", ui->treeWidget);
    pActionUploadFile = new QAction("上传文件", ui->treeWidget);
    //pActionMoveGroup = new QAction("移动分组", ui->treeWidget);    
    pActionRenewCloudPhone = new QAction("续费云手机", ui->treeWidget);
    pActionAuthorization = new QAction("授权", ui->treeWidget);
    pActionReplaceCloudPhone = new QAction("更换云机",ui->treeWidget);
    pActionTransferCloudPhone = new QAction("转移云手机", ui->treeWidget);
    connect(pActionBeginControl, &QAction::triggered, this, &MainWindow::do_ActionBeginControl);
    connect(pActionCopyCloudId, &QAction::triggered, this, &MainWindow::do_ActionCopyCloudId);
    connect(pActionRename, &QAction::triggered, this, &MainWindow::do_ActionRename);
    connect(pActionRestartCloudPhone, &QAction::triggered, this, &MainWindow::do_ActionRestartCloudPhone);
    connect(pActionFactoryDataReset, &QAction::triggered, this, &MainWindow::do_ActionFactoryDataReset);
    connect(pActionUploadFile, &QAction::triggered, this, &MainWindow::do_ActionUploadFile);
    //connect(pActionMoveGroup, &QAction::triggered, this, &MainWindow::do_ActionMoveGroup);
    connect(pActionRenewCloudPhone, &QAction::triggered, this, &MainWindow::do_ActionRenewCloudPhone);
    connect(pActionAuthorization, &QAction::triggered, this, &MainWindow::do_ActionAuthorization);
    connect(pActionReplaceCloudPhone, &QAction::triggered, this, &MainWindow::do_ActionReplaceCloudPhone);
    connect(pActionTransferCloudPhone, &QAction::triggered, this, &MainWindow::do_ActionTransferCloudPhone);
    m_PhoneMenu->addAction(pActionBeginControl);
    m_PhoneMenu->addAction(pActionCopyCloudId);
    m_PhoneMenu->addAction(pActionRename);
    m_PhoneMenu->addAction(pActionRestartCloudPhone);
    m_PhoneMenu->addSeparator();
    m_PhoneMenu->addAction(pActionFactoryDataReset);
    m_PhoneMenu->addAction(pActionUploadFile);
    m_SubPhoneMenu = m_PhoneMenu->addMenu("移动分组");
    //m_PhoneMenu->addAction(pActionMoveGroup);
    m_PhoneMenu->addSeparator();
    m_PhoneMenu->addAction(pActionRenewCloudPhone);
    m_PhoneMenu->addAction(pActionAuthorization);
    m_PhoneMenu->addAction(pActionReplaceCloudPhone);
    m_PhoneMenu->addAction(pActionTransferCloudPhone);
}

//云手机
void MainWindow::InitCloudPhoneTab()
{
    InitGroupMenu();
    InitPhoneMenu();
    InitBatchOperatorMenu();
    InitComboBox();
    //设置TreeWidget相关属性
    //ui->treeWidget->resize(200, 600);
    //ui->treeWidget->move(100, 80);
    //打开右键菜单属性
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);       

    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->setColumnWidth(0, 200);
    ui->treeWidget->setColumnWidth(1, 50);
    //ui->treeWidget->setHeaderLabel("分组列表");
    //ui->treeWidget->setHeaderLabels("分组列表");
    //隐藏表头
    ui->treeWidget->setHeaderHidden(true);
    //屏蔽拖拽功能
    /*ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDragDropMode(QAbstractItemView::InternalMove);*/

    //设置复选框
    //ui->treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->treeWidget->setCheckBoxes(true);

    //隐藏展开功能按钮
    ui->toolBtnExpansionFunction->setVisible(false);

    //初始化设备列表
    InitPhoneList();

    //初始化QComboBox
    ui->comboBoxSystem->addItem("安卓","andrios");
    ui->comboBoxSystem->addItem("苹果","ios");

    ui->comboBoxLevel->addItem("标准型","vip");
    ui->comboBoxLevel->addItem("增强型","GVIP");
    ui->comboBoxLevel->addItem("尊享型","KVIP");

    //隐藏切换竖屏按钮
    ui->toolBtnChangeVerScreen->setVisible(false);

    m_toolBtnContraction = new QToolButton(this);
    QString strStyleSheet = QString("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/control_contraction.png);}");
    m_toolBtnContraction->setStyleSheet(strStyleSheet);
    m_toolBtnContraction->setFixedSize(QSize(14, 56));
    m_toolBtnContraction->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_toolBtnContraction, &QToolButton::clicked, this, &MainWindow::do_ContractionOrExpansion);
    m_toolBtnExpansion = new QToolButton(this);
    strStyleSheet = QString("QToolButton {border:none;background:transparent;background-image: url(:/main/resource/main/control_expansion.png);}");
    m_toolBtnExpansion->setStyleSheet(strStyleSheet);
    m_toolBtnExpansion->setFixedSize(QSize(14, 56));
    m_toolBtnExpansion->setCursor(QCursor(Qt::PointingHandCursor));
    connect(m_toolBtnExpansion, &QToolButton::clicked, this, &MainWindow::do_ContractionOrExpansion);
    m_IsContraction = true;
    m_toolBtnContraction->setVisible(m_IsContraction);
    m_toolBtnExpansion->setVisible(!m_IsContraction);    

    m_isIconMode = true;
 
    m_iCheckCount = 0;

    ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageIconNoData);
	connect(ui->listWidget, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
	connect(ui->listWidget2, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->listWidgetActiveCode, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->listWidgetRenewActiveCode, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->listWidgetRenewList, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->listWidgetVIP, &QMouseListWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->treeWidget, &QMouseTreeWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_3, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_4, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_6, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_7, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_8, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_14, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_16, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->frame_12, &QMouseFrame::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->pageIconNoData, &QMouseWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->pageListNoData, &QMouseWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->pageQrCode, &QMouseWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->page_EmptyMeal,&QMouseWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
    connect(ui->widget, &QMouseWidget::hideIndividualCenterWidgetSignals, this, &MainWindow::do_hideIndividualCenterWidgetSignals);
}

//激活码
void MainWindow::InitActiveCodeTab()
{
    InitActiveCodeRenewList();
}

//购买
void MainWindow::InitBuyTab()
{
    //隐藏微信支付
    ui->toolBtnPayWechat->setVisible(false);    
    //加载等级数据
    HttpMemberLevelListData();
    
    InitVipList();
    InitVipRenewList();

    //隐藏续费列表
    ui->frame_Renew->setHidden(true);

    //默认显示
    ui->stackedWidget_2->setCurrentWidget(ui->page_Meal);

    QRegularExpression regExp("[0-9]*");
    QValidator* validator = new QRegularExpressionValidator(regExp, this);
    ui->lineEditBuyNumber->setValidator(validator);
}

//初始化列表
void MainWindow::InitPhoneList()
{    
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->listWidget2->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget2->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget2->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget2->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget2->setSelectionMode(QAbstractItemView::SingleSelection);

    //默认显示竖屏
    on_toolBtnChangeVerScreen_clicked();
}

void MainWindow::InitBatchOperatorMenu()
{
    m_BatchOperMenu = new QMenu(this);
    m_BatchOperMenu->setStyleSheet("QMenu{ background-color:#FFFFFF;font-size:14px;border:1px solid rgba(255,255,255,1);padding-left:20px;} \
        QMenu::item{ min-width:150px; font-size:12px;height:20; color:#505465;background:rgba(255,255,255,0.5); border:0px solid rgba(82,130,164,1); padding:1px 1px; margin:1px 1px; }\
        QMenu::item:selected{ background:#F5F7FB;font-weight:bold;height:20;border:0px solid rgba(82,130,164,1); }  /*选中或者说鼠标滑过状态*/\
        QMenu::item:pressed{ background:#F5F7FB;font-weight:bold;height:20; border:0px solid rgba(82,130,164,1);/*摁下状态*/ }");
    pActionBatchReboot = new QAction("批量重启", this);
    pActionBatchUploadFile = new QAction("批量上传文件", this);
    pActionBatchFactoryReset = new QAction("批量恢复出厂", this);
    pActionBatchAuth = new QAction("批量授权", this);
    //pActionMoveGroup = new QAction("移动分组", ui->treeWidget);
    connect(pActionBatchReboot, &QAction::triggered, this, &MainWindow::do_ActionBatchReboot);
    connect(pActionBatchUploadFile, &QAction::triggered, this, &MainWindow::do_ActionBatchUploadFile);
    connect(pActionBatchFactoryReset, &QAction::triggered, this, &MainWindow::do_ActionBatchFactoryReset);
    connect(pActionBatchAuth, &QAction::triggered, this, &MainWindow::do_ActionBatchAuth);

    //connect(pActionMoveGroup, &QAction::triggered, this, &MainWindow::do_ActionMoveGroup);
    m_BatchOperMenu->addAction(pActionBatchReboot);
    m_BatchOperMenu->addSeparator();
    m_BatchOperMenu->addAction(pActionBatchUploadFile);
    m_BatchOperMenu->addAction(pActionBatchFactoryReset);
    m_BatchOperMenu->addAction(pActionBatchAuth);
    m_BatchOperSubMenu = m_BatchOperMenu->addMenu("批量移动分组");
}

void MainWindow::InitComboBox()
{
    ui->comboBoxView->addItem("视图100%",100);
    ui->comboBoxView->addItem("视图80%",80);
    ui->comboBoxView->addItem("视图60%",60);
    ui->comboBoxView->addItem("视图50%",50);
    ui->comboBoxView->addItem("视图40%",40);
    ui->comboBoxView->addItem("视图30%",30);
}

QStringList MainWindow::getCheckedPhoneInstance(bool IsPhoneId)
{
    QStringList strPhoneIdList;
    QStringList strPhoneList;
    strPhoneIdList.clear();
    strPhoneList.clear();
    S_PHONE_INFO phoneInfo;
    GlobalData::mapSyncPhoneList.clear();
    //预览模式
    if (m_isIconMode)
    {
        int iCount = ui->listWidget->count();
        if (iCount <= 0)
            return strPhoneList;

        QListWidgetItem* item = NULL;
        PhoneItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget->item(i);
            if (item != NULL)
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                phoneItem = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(item));
                if (phoneItem != NULL && !phoneInfo.strInstanceNo.isEmpty() && phoneItem->getCheckBoxStatus())
                {
                    strPhoneIdList << QString("%1").arg(phoneInfo.iId);
                    strPhoneList << phoneInfo.strInstanceNo;
                    GlobalData::mapSyncPhoneList.insert(phoneInfo.iId, phoneInfo);
                }
            }
        }
    }
    else
    {
        int iCount = ui->listWidget2->count();
        if (iCount <= 0)
        {
            return strPhoneList;
        }

        QListWidgetItem* item = NULL;
        PhoneListModeItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget2->item(i);
            if (item != NULL)
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                phoneItem = static_cast<PhoneListModeItemWidget*>(ui->listWidget2->itemWidget(item));
                if (phoneItem != NULL && phoneItem->getCheckBoxStatus())
                {
                    strPhoneIdList << QString("%1").arg(phoneInfo.iId);
                    strPhoneList << phoneInfo.strInstanceNo;
                    GlobalData::mapSyncPhoneList.insert(phoneInfo.iId, phoneInfo);
                }
            }
        }
    }
    
    if (IsPhoneId)
        return strPhoneIdList;
    return strPhoneList;
}

void MainWindow::do_ActionBatchReboot(bool bChecked)
{
    //获取列表选中项
    QStringList strPhoneList = getCheckedPhoneInstance();
    if (strPhoneList.size() > 0)
    {
        m_toolObject->HttpPostInstanceReboot(strPhoneList);
    }    
}

void MainWindow::do_ActionBatchUploadFile(bool bChecked)
{
    //获取列表选中项
    QStringList strPhoneList = getCheckedPhoneInstance();
    for (int i = 0; i < strPhoneList.size(); i++)
    {
        qDebug() << "批处理设备No("<<i<<"):" << strPhoneList.at(i);
    }
    
    if (strPhoneList.size() > 0)
    {
        UploadFileDialog* upload = new UploadFileDialog(strPhoneList);
        upload->exec();
    }    
}

void MainWindow::do_ActionBatchFactoryReset(bool bChecked)
{
    //获取列表选中项
    QStringList strPhoneList = getCheckedPhoneInstance();
    if (strPhoneList.size() > 0)
    {
        m_toolObject->HttpPostInstanceReset(strPhoneList);
    }
}

void MainWindow::do_ActionBatchAuth(bool bChecked)
{
    QStringList strPhoneList = getCheckedPhoneInstance();
    if (strPhoneList.size() > 0)
    {
        //批量授权
        AddAuthorizationDialog* dialog = new AddAuthorizationDialog(GlobalData::mapSyncPhoneList);
        connect(dialog, &AddAuthorizationDialog::notifyMainWindowRefreshGroupListSignals, this, &MainWindow::on_btnGroupRefresh_clicked);
        dialog->exec();
    }
}

void MainWindow::InitLevelList()
{
    if (m_mapLevelList.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("暂无数据,请联系后台", this);
        tips->show();
        return;
    }
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    /*ui->listWidgetLevel->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetLevel->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    //ui->listWidgetLevel->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetLevel->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetLevel->setSelectionMode(QAbstractItemView::SingleSelection);

    //初始化level listwidget
    //ui->listWidgetLevel->setFixedHeight(254);
    ui->listWidgetLevel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidgetLevel->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //ui->listWidgetLevel->setFixedSize(QSize())
    ui->listWidgetLevel->verticalScrollBar()->setVisible(false);
    ui->listWidgetLevel->setFixedWidth(3*(ITEM_WIDGET_LEVEL_WIDTH+15));*/

    //初始化列表云手机购买列表+云手机续费 等级列表
    //LevelItemWidget* widget = NULL;
    //QListWidgetItem* item = NULL;
    //int iCount = map.size();
    //qDebug() << "map size() =" << iCount;
    //QMap<int, S_ItemWidgetData*>::const_iterator iter = map.constBegin();
    //for (; iter != map.constEnd(); iter++)
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QString strImage;
    //QWidget* containerWidget = new QWidget();
    ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    //containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    QHBoxLayout* horizontalLayout = new QHBoxLayout(ui->scrollAreaWidgetContents);
    horizontalLayout->setSpacing(50);

    //初始化激活页面等级按钮
    //int会员等级
    QMap<int, S_LEVEL_INFO>::iterator iterLevelList = m_mapLevelList.begin();
    m_hBoxLevelTypeToolBtn = new QHBoxLayout(ui->frameLevelTab);
    CustomToolButton* toolBtn;
    QSize size(LEVEL_TYPE_TOOLBUTTON_WIDTH, LEVEL_TYPE_TOOLBUTTON_HEIGHT);
    for (; iterLevelList != m_mapLevelList.end(); iterLevelList++)
    {
        //激活码续费ToolBtn
        toolBtn = new CustomToolButton(this);
        toolBtn->setText(iterLevelList->strLevelName);
        //toolBtn->resize(size);
        toolBtn->setFixedSize(size);
        toolBtn->setData(Qt::UserRole,QVariant::fromValue(*iterLevelList));
        toolBtn->setStyleSheet("QToolButton {border:none;background-color:#F4F6FA;color:#A9ADB6;border-top-left-radius:2px;border-top-right-radius:2px;padding-left:0px;font-size:14px;}QToolButton:hover{background-color:#E6E9F2;color:#505465;border-top-left-radius:2px;border-top-right-radius:2px;padding-left:0px;font-size:14px;}");
        connect(toolBtn, &QToolButton::clicked, this, &MainWindow::on_toolBtnLevelVIP_clicked);
        m_hBoxLevelTypeToolBtn->addWidget(toolBtn);

        //购买页面等级列表
        levelItem = new LevelItemWidget(*iterLevelList, ui->scrollArea);
        //levelItem->setFixedSize(QSize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT));
        //根据点击的level实时显示最新的会员数据
        connect(levelItem, &LevelItemWidget::refreshMemberListSignals, this, &MainWindow::do_refreshMemberListSignals);
        //在全量数据中查找并显示会员数据
        //connect(levelItem, &LevelItemWidget::selectLevelTypeSignals, this, &MainWindow::do_selectLevelTypeSignals);
        horizontalLayout->addWidget(levelItem);
    }
    m_hBoxLevelTypeToolBtn->addStretch();
    m_hBoxLevelTypeToolBtn->setContentsMargins(24, 8, 0, 0);
    m_hBoxLevelTypeToolBtn->setSpacing(4);
    ui->frameLevelTab->setLayout(m_hBoxLevelTypeToolBtn);
}
void MainWindow::InitVipList()
{
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidgetVIP->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetVIP->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetVIP->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetVIP->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetVIP->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::InitVipRenewList()
{
    ui->listWidgetRenewList->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetRenewList->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetRenewList->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetRenewList->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetRenewList->setSelectionMode(QAbstractItemView::SingleSelection);
}

void MainWindow::InitActiveCodeRenewList()
{
    ui->listWidgetActiveCode->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    //ui->listWidgetActiveCode->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    ui->listWidgetActiveCode->setSpacing(0);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetActiveCode->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetActiveCode->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetActiveCode->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->listWidgetRenewActiveCode->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    //ui->listWidgetActiveCode->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    ui->listWidgetRenewActiveCode->setSpacing(0);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetRenewActiveCode->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetRenewActiveCode->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetRenewActiveCode->setSelectionMode(QAbstractItemView::SingleSelection);
}
void MainWindow::HttpQueryAllGroup()//查询全部分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_QUERY_ALL_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    qDebug() << "strToken=" << strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        int iGroupSize = dataArray.size();
                        if(0 == iGroupSize)
                        {
                            return;
                        }
                        else
                        {
                            m_mapGroupInfo.clear();
                            ui->treeWidget->clear();
                            //树形列表显示
                            S_GROUP_INFO groupInfo;
                            QJsonObject dataObj;
                            for(int i= 0; i < iGroupSize; i++)
                            {
                                dataObj = dataArray[i].toObject();
                                groupInfo.iGroupId = dataObj["id"].toInt();
                                groupInfo.iGroupNum = dataObj["num"].toInt();
                                groupInfo.strGroupName = dataObj["name"].toString();
                                qDebug() << "iGroupId="<< groupInfo.iGroupId <<"strGroupName="<< groupInfo.strGroupName <<"iGroupCount="<< groupInfo.iGroupNum;

                                m_mapGroupInfo.insert(i,groupInfo);
                            }

                            //调用UI接口显示数据
                            ShowGroupInfo();
                        }
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::ShowGroupInfo()
{
    if(m_mapGroupInfo.size() <= 0)
        return;

    m_SubPhoneMenu->clear();
    m_BatchOperSubMenu->clear();
    ui->comboBoxGroupName->clear();
    QTreeWidgetItem* item = NULL;
    QTreeWidgetItem* child = NULL;
    QMap<int, S_GROUP_INFO>::iterator iter = m_mapGroupInfo.begin();
    QString strNewGroupName;
    QAction* pAction = NULL;
    QAction* pBatchAction = NULL;
    for( ; iter != m_mapGroupInfo.end(); iter++)
    {
        item = new QTreeWidgetItem(ui->treeWidget);
        //strNewGroupName = QString("%1(%2)").arg(iter->strGroupName).arg(iter->iGroupNum);
        strNewGroupName = QString("%1(%2)").arg(iter->strGroupName).arg(0);
        item->setText(0, strNewGroupName);
        //存储GroupId
        qDebug() << "写入iGroupId=" << iter->iGroupId;
        item->setData(0, Qt::UserRole, QVariant::fromValue(*iter));
        //item->setData(0, Qt::UserRole, iter->iGroupId);
        //item->setIcon(0, QIcon(":/login/resource/login/option_normal.png"));
        item->setCheckState(0, Qt::Unchecked);
        ui->treeWidget->addTopLevelItem(item);

        //初始化子菜单
        pAction = new QAction(iter->strGroupName, ui->treeWidget);
        pAction->setData(QVariant::fromValue(*iter));
        connect(pAction, &QAction::triggered, this, &MainWindow::do_ActionMoveGroup);
        pBatchAction = new QAction(iter->strGroupName, this);
        pBatchAction->setData(QVariant::fromValue(*iter));
        connect(pBatchAction, &QAction::triggered, this, &MainWindow::do_ActionBatchMoveGroup);
        m_SubPhoneMenu->addAction(pAction);
        m_BatchOperSubMenu->addAction(pBatchAction);
        ui->comboBoxGroupName->addItem(iter->strGroupName);
        if (iter->iGroupNum > 0)
        {
            qDebug() << "查询手机列表";
            HttpGetMyPhoneInstance(iter->iGroupId, 1, 1000, 0);
        }        

        /*child = new QTreeWidgetItem(item);
        child->setText(0, iter->strGroupName + "子节点1");
        //将节点插入到第一个位置
        item->addChild(child);
        //同上
        ui->treeWidget->insertTopLevelItem(0, child);*/
    }

    //展开所有
    //ui->treeWidget->expandAll();
}

//实例列表
void MainWindow::ShowPhoneInfo(int iGroupId, QMap<int, S_PHONE_INFO> mapPhoneInfo)
{
    if (mapPhoneInfo.size() <= 0)
        return;    
    
    int iId = 0;
    QTreeWidgetItem* item;
    QTreeWidgetItem* phoneItem;
    QTreeWidgetItemIterator it(ui->treeWidget);
    S_GROUP_INFO sGroupInfo;
    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime;
    QString strTime;
    QString strLevelName = "";
    qint64 mseconds = 0;
    while (*it) 
    {
        item = *it;
        if (item->parent() == nullptr) 
        { // 判断是否为根节点
            // 在这里处理根节点
            // 例如：
            sGroupInfo = item->data(0, Qt::UserRole).value<S_GROUP_INFO>();
            //只显示未过期的设备，修改父窗口的值            
            if (iGroupId == sGroupInfo.iGroupId)
            {
                item->setText(0, QString("%1(%2)").arg(sGroupInfo.strGroupName).arg(mapPhoneInfo.size()));
                QMap<int, S_LEVEL_INFO>::iterator iterFind; 
                QMap<int, S_PHONE_INFO>::iterator iter = mapPhoneInfo.begin();
                for (; iter != mapPhoneInfo.end(); iter++)
                {
                    iterFind = m_mapLevelList.find(iter->iLevel);
                    strLevelName = "";
                    if (iterFind != m_mapLevelList.end())
                    {
                        strLevelName = iterFind->strLevelName;
                    }

                    phoneItem = new QTreeWidgetItem(item);
                    qDebug() << "phone id" << iter->iId << " name=" << iter->strName;
                    phoneItem->setData(0, Qt::UserRole, QVariant::fromValue(*iter));
                    phoneItem->setIcon(0, QIcon(QString(":/main/resource/main/%1.png").arg(strLevelName)));

                    expireTime = QDateTime::fromString(iter->strExpireTime, "yyyy-MM-dd hh:mm:ss");
                    mseconds = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();

                    strTime = strTime.asprintf("%d天%d小时", mseconds / (1000 * 60 * 60 * 24), (mseconds / (1000 * 60 * 60)) % 24);
                    qDebug() << "strTime=" << strTime;
                    if(iter->strName.isEmpty())
                        phoneItem->setText(0, strLevelName + " " + iter->strInstanceNo);
                    else
                        phoneItem->setText(0, strLevelName + " " + iter->strName);
                    phoneItem->setText(1, strTime);
                    phoneItem->setCheckState(0, Qt::Unchecked);
                    item->addChild(phoneItem);
                }
                break;
            }
        }
        ++it;
    }
}

//显示续费列表数据
void MainWindow::ShowActiveCodeItemInfo(int iLevelId, QMap<int, S_PHONE_INFO> mapPhoneInfo)
{
    if (mapPhoneInfo.size() <= 0)
        return;

    ui->listWidgetRenewActiveCode->clear();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    QMap<int, S_PHONE_INFO>::iterator iter = mapPhoneInfo.begin();
    for (; iter != mapPhoneInfo.end(); iter++)
    {
        if (iter->iLevel == iLevelId)
        {
            item = new QListWidgetItem(ui->listWidgetRenewActiveCode);
            item->setData(Qt::UserRole, QVariant::fromValue(*iter));
            item->setSizeHint(QSize(RENEW_ITEM_WIDTH, 30));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
            widget = new ActiveCodeRenewItem(*iter, this);
            widget->setCheckBoxStatus(ui->checkBoxActiveCodeRenew->isChecked());
            connect(this, &MainWindow::activeCodeStatusSignals, widget, &ActiveCodeRenewItem::do_activeCodeStatusSignals);
            connect(widget, &ActiveCodeRenewItem::deleteActiveItemSignals, this, &MainWindow::do_deleteActiveItemSignals);
            ui->listWidgetRenewActiveCode->addItem(item);
            ui->listWidgetRenewActiveCode->setItemWidget(item, widget);
        }
    }
}

//显示任务
void MainWindow::ShowTaskInfo()
{
    if (m_mapTask.size() <= 0)
        return;

    int iCount = ui->listWidget->count();
    if (iCount <= 0)
    {
        qDebug() << "ShowTaskInfo ui->listWidget 已清空";
        return;
    }
        
    QListWidgetItem* item = NULL;
    PhoneItemWidget* phoneItem = NULL;
    S_PHONE_INFO phoneInfo;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        if (item != NULL)
        {
            phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
            phoneItem = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(item));
            if (phoneItem != NULL && !phoneInfo.strInstanceNo.isEmpty())
            {
                phoneItem->startRequest(m_mapTask.find(phoneInfo.strInstanceNo).value().strUrl);
            }
        }        
    }
}
void MainWindow::HttpCreateGroup(QString strGroupName)//创建分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("name", strGroupName);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json="<<response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //界面添加该组
                    on_btnGroupRefresh_clicked();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::HttpUpdateGroup(int iGroupId, QString strNewName)//修改分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_UPDATE_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("createBy", "");
    obj.insert("id", iGroupId);
    obj.insert("name", strNewName);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        on_btnGroupRefresh_clicked();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::HttpDeleteGroup(int iGroupId)//删除分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_GROUP;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        on_btnGroupRefresh_clicked();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//获取serverToken
/*void MainWindow::HttpGetServerToken()
{
    int iGroupId = 0;
    QTreeWidgetItem* selectItem = ui->treeWidget->currentItem();
    if (selectItem != NULL)
    {
        iGroupId = selectItem->data(0, Qt::UserRole).toInt();
    }

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_SERVER_TOKEN;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + m_userInfo.strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    qDebug() << "操作成功";
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}*/

//会员级别接口
void MainWindow::HttpLevelList()
{    
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_LEVEL_LIST;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strCode);
    obj.insert("password", strPassword);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    QByteArray postData = "";
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();                
                qDebug() << "Code=" << iCode << "message=" << strMessage <<"json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        QJsonObject objData;
                        int iSize = dataArray.size();
                        m_mapLevelList.clear();
                        S_LEVEL_INFO info;
                        for (int i = 0; i < iSize; i++)
                        {
                            objData = dataArray.at(i).toObject();
                            info.iLevelId = objData["id"].toInt();
                            info.strLevelName = objData["name"].toString();
                            info.strColorIcon = objData["colorIcon"].toString();
                            /*if (!info.strColorIcon.isEmpty())
                            {
                                startDownload(info.strColorIcon);
                            }*/
                            info.strAshIcon = objData["ashIcon"].toString();
                            /*if (!info.strAshIcon.isEmpty())
                            {
                                startDownload(info.strAshIcon);
                            }*/
                            //isEnabled true能用;false禁用
                            info.bIsEnabled = objData["isEnable"].toBool();
                            info.strLevelRemark = objData["remark"].toString();
                            qDebug() << "等级" << info.iLevelId << " name=" << info.strLevelName;
                            if (info.bIsEnabled)
                            {
                                m_mapLevelList.insert(info.iLevelId, info);
                            }                                                       
                        }
                        InitLevelList();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//会员相关接口
void MainWindow::HttpMemberLevelListData()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_MEMBER_LEVEL_LIST_DATA;
    //strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        QMap<int,S_LEVEL_DATA_INFO> mapData;
                        S_LEVEL_DATA_INFO sLevelData;
                        QJsonObject data;

                        QJsonArray memberList;
                        QJsonObject member;
                        int iMemberIndex =0;
                        for(int i =0 ; i < dataArray.size();i++)
                        {
                            data = dataArray[i].toObject();
                            sLevelData.iLevelId = data["level"].toInt();
                            memberList = data["memberList"].toArray();
                            mapData.clear();
                            for(iMemberIndex = 0; iMemberIndex < memberList.size(); iMemberIndex++)
                            {
                                member = memberList[iMemberIndex].toObject();
                                sLevelData.iMemberId = member["id"].toInt();
                                sLevelData.strMemberName = member["name"].toString();
                                sLevelData.fPrice = member["price"].toDouble();
                                sLevelData.fActivityPrice = member["activityPrice"].toDouble();
                                sLevelData.strUrl = member["url"].toString();
                                sLevelData.strRemark = member["remark"].toString();
                                sLevelData.strInstanceLevel = member["instanceLevel"].toString();
                                sLevelData.iUseDay = member["useDay"].toInt();
                                sLevelData.strLevelName = member["levelName"].toString();
                                sLevelData.strColorIcon = member["colorIcon"].toString();
                                sLevelData.strAshIcon = member["ashIcon"].toString();
                                sLevelData.strLevelRemark = member["levelRemark"].toString();
                                mapData.insert(sLevelData.iMemberId, sLevelData);
                            }
                            m_mapLevel.insert(sLevelData.iLevelId, mapData);
                        }

                        //初始化界面数据
                        //InitLevelList();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

QImage generateAlipayQRCode(const QString& data) 
{
    // 使用QRcode库生成二维码
    QByteArray qrData = data.toUtf8();
    QRcode* qrcode = QRcode_encodeString(qrData.constData(), 0, QR_ECLEVEL_L, QR_MODE_8, 1);

    if (qrcode == nullptr) {
        return QImage(); // 生成失败，返回空图像
    }

    int width = qrcode->width;
    QImage image(width, width, QImage::Format_ARGB32);
    image.fill(qRgba(255, 255, 255, 0));

    for (int y = 0; y < qrcode->width; y++) {
        for (int x = 0; x < qrcode->width; x++) {
            unsigned char b = qrcode->data[y * qrcode->width + x];
            if (b & 0x01) {
                image.setPixel(x, y, qRgba(0, 0, 0, 255));
            }
        }
    }

    QRcode_free(qrcode); // 释放QRcode结构
    return image;
}

void MainWindow::gernerateQRCode(const QString& text, QPixmap& qrPixmap, int scale)
{
    if (text.isEmpty()) {
        return;
    }

    //二维码数据
    QRcode* qrCode = nullptr;

    //这里二维码版本传入参数是2,实际上二维码生成后，它的版本是根据二维码内容来决定的
    qrCode = QRcode_encodeString(text.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    if (nullptr == qrCode) {
        return;
    }

    int qrCode_Width = qrCode->width > 0 ? qrCode->width : 1;
    int width = scale * qrCode_Width;
    int height = scale * qrCode_Width;

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);

    QPainter mPainter(&image);
    QColor background(Qt::white);
    mPainter.setBrush(background);
    mPainter.setPen(Qt::NoPen);
    mPainter.drawRect(0, 0, width, height);
    QColor foreground(Qt::black);
    mPainter.setBrush(foreground);
    for (int y = 0; y < qrCode_Width; ++y) {
        for (int x = 0; x < qrCode_Width; ++x) {
            unsigned char character = qrCode->data[y * qrCode_Width + x];
            if (character & 0x01) {
                QRect rect(x * scale, y * scale, scale, scale);
                mPainter.drawRects(&rect, 1);
            }
        }
    }

    qrPixmap = QPixmap::fromImage(image);
    QRcode_free(qrCode);
}

//订单接口-创建订单
void MainWindow::HttpCreateOrder(int iChannel,int iMemberId,int iNum, int iPayType,QString strRelateId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_ORDER;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;

    obj.insert("channel", iChannel);
    obj.insert("memberId", iMemberId);
    obj.insert("num", iNum);
    obj.insert("payType", iPayType);
    if (!strRelateId.isEmpty())
    {
        obj.insert("relateId", strRelateId);
    }
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() <<"response="<< response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    QString strData = obj["data"].toString();
                    qDebug() << strData;

                    doc = QJsonDocument::fromJson(strData.toUtf8(), &parseError);
                    if (parseError.error != QJsonParseError::NoError)
                    {
                        qDebug() << response;
                        qWarning() << "Json parse error:" << parseError.errorString();
                    }
                    else
                    {
                        obj = doc.object();
                        QJsonObject objResponse = obj["alipay_trade_precreate_response"].toObject();
                        QString strCode = objResponse["code"].toString();
                        QString strMsg = objResponse["msg"].toString();
                        QString strOutTradeNo = objResponse["out_trade_no"].toString();
                        QString strQrCode = objResponse["qr_code"].toString();

                        //生成QRCode
                        /*QString text = ui->lineEdit_content->text();

                        QPixmap qrPixmap;
                        int width = ui->label_code->width();
                        int height = ui->label_code->height();
                        gernerateQRCode(text, qrPixmap, 10);
                        qrPixmap = qrPixmap.scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

                        ui->label_code->setPixmap(qrPixmap);*/
                        qDebug() << strQrCode;
                        QImage qrImage = generateAlipayQRCode(strQrCode);
                        if (!qrImage.isNull())
                        {
                            QDir dir;
                            GlobalData::strQrcode = dir.tempPath() + "/alipay_qrcode.png";
                            qrImage.save(GlobalData::strQrcode);
                            int width = ui->labelQrCode->width();
                            int height = ui->labelQrCode->height();
                            ui->labelQrCode->setPixmap(QPixmap(GlobalData::strQrcode).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
                            //正常情况
                            ui->stackedWidget->setCurrentWidget(ui->pageQrCode);
                            ui->labelPayTimeoutTip->setVisible(false);
                            ui->btnRefreshQrCode->setVisible(false);
                            m_iPayCount = 59;
                            m_PayTimer->start(1000);
                        }
                    }                    
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//关闭订单
void MainWindow::HttpCloseOrder(QString strOutTradeNo)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += "/api/order/closeOrder";// HTTP_CLOSE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(strOutTradeNo);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                bool bData = obj["data"].toBool();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if(bData)
                        qDebug() << "关闭订单成功";
                    else
                        qDebug() << "关闭订单失败";
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//获取我的手机实例
void MainWindow::HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_PHONE_INSTANCE;
    //level不传值,返回该 组下面所有的level
    if(iLevel != 0)
        strUrl += QString::asprintf("?level=%d&page=%d&pageSize=%d", iLevel, iPage, iPageSize);
    else
        strUrl += QString::asprintf("?groupId=%d&page=%d&pageSize=%d", iGroupId, iPage, iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())       
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iTotal = data["total"].toInt();
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages<<"iSize="<<iSize;
                        QJsonArray records = data["records"].toArray();
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                memset(&phoneInfo, 0, sizeof(S_PHONE_INFO));
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iAuthType = recordObj["type"].toInt();
                                phoneInfo.strGrantControl = recordObj["grantControl"].toString();
                                phoneInfo.bIsAuth = recordObj["isAuth"].toBool();
                                m_mapPhoneInfo.insert(phoneInfo.iId, phoneInfo);
                                qDebug() << "name" << phoneInfo.strName << "strInstanceNo=" << phoneInfo.strInstanceNo<<"phoneInfo.strCreateTime="<< phoneInfo.strCreateTime<< "phoneInfo.strCurrentTime=" << phoneInfo.strCurrentTime <<"phoneInfo.strExpireTime="<< phoneInfo.strExpireTime << "id=" << phoneInfo.iId << "authType=" << phoneInfo.iAuthType<<"level="<< phoneInfo.iLevel;
                            }
                            if(iLevel != 0)
                                ShowActiveCodeItemInfo(iLevel, m_mapPhoneInfo);
                            else
                                ShowPhoneInfo(iGroupId, m_mapPhoneInfo);
                            
                        }
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//获取我的实例级别
void MainWindow::HttpGetMyInstanceLevel(int iPhoneId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MYINSTANCE_LEVEL;
    strUrl += QString("/%1").arg(iPhoneId);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();                        
                        int iDataSize = dataArray.size();
                        if (0 == iDataSize)
                        {
                            return;
                        }
                        else
                        {                            
                            QJsonObject data;
                            for (int i = 0; i < iDataSize; i++)
                            {
                                data = dataArray[i].toObject();
                                QString strAshIcon = data["ashIcon"].toString();
                                QString strColorIcon = data["colorIcon"].toString();
                                int iId = data["id"].toInt();
                                int isEnabled = data["isEnabled"].toInt();
                                QString strlevelName = data["name"].toString();
                                QString strRemark = data["remark"].toString();
                                qDebug() << "获取我的实例级别 id=" << iId << "strlevelName" << strlevelName << "remark=" << strRemark;
                            }
                        }
                        
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//云手机
void MainWindow::on_toolBtnCloudPhone_clicked()
{
    if(ui->page == ui->stackedWidget->currentWidget())
    {
        return;
    }
    m_toolBtnContraction->setVisible(m_IsContraction);
    m_toolBtnExpansion->setVisible(!m_IsContraction);

    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;color:#6296FF;font-size:12px;background:transparent;}QToolButton:pressed{border:none;background:transparent;}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");
    ui->toolBtnCloudPhone->setIcon(QIcon(":/main/resource/main/cloudPhone_select.png"));
    ui->toolBtnActiveCode->setIcon(QIcon(":/main/resource/main/activeCode_normal.png"));
    ui->toolBtnBuy->setIcon(QIcon(":/main/resource/main/buy_normal.png"));
    ui->stackedWidget->setCurrentWidget(ui->page);
}

//激活码
void MainWindow::on_toolBtnActiveCode_clicked()
{
    if(ui->pageActiveCode == ui->stackedWidget->currentWidget())
    {
        return;
    }
    m_toolBtnContraction->setVisible(false);
    m_toolBtnExpansion->setVisible(false);
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;color:#6296FF;font-size:12px;background:transparent;}QToolButton:pressed{border:none;background:transparent;}");
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");

    ui->toolBtnCloudPhone->setIcon(QIcon(":/main/resource/main/cloudPhone_normal.png"));
    ui->toolBtnActiveCode->setIcon(QIcon(":/main/resource/main/activeCode_select.png"));
    ui->toolBtnBuy->setIcon(QIcon(":/main/resource/main/buy_normal.png"));
    ui->stackedWidget->setCurrentWidget(ui->pageActiveCode);
}

//购买
void MainWindow::on_toolBtnBuy_clicked()
{
    if(ui->pageBuy == ui->stackedWidget->currentWidget())
    {
        return;
    }
    m_toolBtnContraction->setVisible(false);
    m_toolBtnExpansion->setVisible(false);
    ui->toolBtnBuy->setStyleSheet("QToolButton{border:none;color:#6296FF;font-size:12px;background:transparent;}QToolButton:pressed{border:none;background:transparent;}");
    ui->toolBtnActiveCode->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");
    ui->toolBtnCloudPhone->setStyleSheet("QToolButton{border:none;background:transparent;color:#BCC5E2;}QToolButton:pressed{border:none;background:transparent;color:#6296FF;}");
    ui->toolBtnCloudPhone->setIcon(QIcon(":/main/resource/main/cloudPhone_normal.png"));
    ui->toolBtnActiveCode->setIcon(QIcon(":/main/resource/main/activeCode_normal.png"));
    ui->toolBtnBuy->setIcon(QIcon(":/main/resource/main/buy_select.png"));

    ui->stackedWidget->setCurrentWidget(ui->pageBuy);
}

//菜单栏
void MainWindow::on_btnPhotoUrl_clicked()
{

}

void MainWindow::on_btnMin_clicked()
{
    this->showMinimized();
}


void MainWindow::on_btnMax_clicked()
{
    if(this->isMaximized())
        this->showNormal();
    else
        this->showMaximized();
}

void MainWindow::HttpLogout()
{
    //关闭窗口并且退出登录
    qDebug() << "注销";
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGOUT;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished,this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage <<"response:"<<response;
                if(HTTP_SUCCESS_CODE == iCode)
                {
                    /*if (obj["data"].isObject())
                        {
                            QJsonObject data = obj["data"].toObject();
                            QString strToken = data["token"].toString();
                            QString strMaxExpirationDate = data["maxExpirationDate"].toString();

                            QJsonObject userDetailVO = data["userDetailVO"].toObject();
                            int id = userDetailVO["id"].toInt();
                            QString strName = userDetailVO["name"].toString();
                            QString strAccount = userDetailVO["account"].toString();
                            QString strMobile = userDetailVO["mobile"].toString();
                            qDebug() << "跳转到主页面"<<"id="<<id<<"name="<<strName<<"account="<<strAccount<<"mobile="<<strMobile<<"MaxExpirationDate"<<strMaxExpirationDate<<"token="<<strToken;
                        }*/
                    qDebug()<<"注销成功";
                    this->close();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

void MainWindow::on_btnClose_clicked()
{    
    this->close();
}

void MainWindow::do_createGroupSignals(ENUM_CREATE_OR_UPDATA type, QString strGroupName, int id)
{
    switch (type)
    {
    case TYPE_CREATE_GROUP_WIDGET:
    {
        //创建分组
        qDebug() << " 创建分组 strGroupName=" << strGroupName;
        HttpCreateGroup(strGroupName);
    }
    break;
    case TYPE_UPDATE_GROUP_WIDGET:
    {
        //修改分组
        //获取当前选中的item
        QTreeWidgetItem* item = ui->treeWidget->currentItem();
        S_GROUP_INFO groupInfo = item->data(0, Qt::UserRole).value<S_GROUP_INFO>();
        qDebug() << " 编辑分组 id=" << groupInfo.iGroupId << "strGroupName=" << strGroupName;
        HttpUpdateGroup(groupInfo.iGroupId, strGroupName);
    }
    break;
    case TYPE_PHONE_RENAME_WIDGET:
    {
        //手机重命名
        HttpPostInstanceRename(id, strGroupName);
    }
    break;
    default:
        break;
    }
}

void MainWindow::on_btnCreateGroup_clicked()
{
    //新建分组
    if (NULL == m_createGroupWidget)
    {
        m_createGroupWidget = new CreateGroupWidget(TYPE_CREATE_GROUP_WIDGET);
    }

    connect(m_createGroupWidget, &CreateGroupWidget::createGroupSignals, this, &MainWindow::do_createGroupSignals);
    connect(m_createGroupWidget, &CreateGroupWidget::destroyed, this, [=]() {
        m_createGroupWidget = NULL;
        });
    m_createGroupWidget->setModal(true);
    m_createGroupWidget->show();

    //调试修改分组接口
    //HttpUpdateGroup(2, "新名称");

    //调试删除分组接口
    //HttpDeleteGroup(0);    
}

//激活码接口
/*
void MainWindow::HttpPostActivateCode(QStringList strActiveCodeList, int iRelateId)
{
    if (strActiveCodeList.size() <= 0)
        return;

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_ACTIVE_CODE;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray jsonArray;
    QJsonObject rootObject;
    
    for (const QString& strActiveCode : strActiveCodeList)
    {
        obj.insert("code", strActiveCode);
        //obj.insert("relateId", iRelateId);
        jsonArray.append(obj);
    }


    rootObject.insert("activateList", jsonArray);
    doc.setObject(rootObject);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                qDebug() << "response = " << response;
                QString strMessage = obj["message"].toString();
                if (obj["data"].isArray())
                {
                    QJsonArray dataArray = obj["data"].toArray();
                    int iSize = dataArray.size();
                    QString strCode;
                    bool bSuccess = false;
                    for (int i = 0; i < iSize; i++)
                    {
                        strCode = dataArray[i].toString();
                        bSuccess = dataArray[i].toBool();
                        qDebug() << "strCode =" << strCode << "bSuccess=" << bSuccess;
                        emit activeCodeStatusSignals(strMessage);
                    }
                    //
                }
            }
        }
        reply->deleteLater();
    });
}
*/

//激活码接口
void MainWindow::HttpPostActivateCode(QMap<int, S_ACTIVE_CODE_INFO> mapActiveCode)
{
    if (mapActiveCode.size() <= 0)
        return;

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_ACTIVE_CODE;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray jsonArray;
    QJsonObject rootObject;

    QMap<int, S_ACTIVE_CODE_INFO>::iterator iter = mapActiveCode.begin();
    //for (const QString& strActiveCode : strActiveCodeList)
    for(;iter != mapActiveCode.end(); iter++)
    {
        obj.insert("code", iter->strRenewActiveCode);
        if (iter->iRelateId != 0)
        {
            obj.insert("relateId", iter->iRelateId);
        }        
        //obj.insert("relateId", iRelateId);
        jsonArray.append(obj);
    }


    rootObject.insert("activateList", jsonArray);
    doc.setObject(rootObject);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qDebug() << response;
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                qDebug() << "response = " << response;
                QString strMessage = obj["message"].toString();
                if (obj["data"].isArray())
                {
                    QJsonArray dataArray = obj["data"].toArray();
                    int iSize = dataArray.size();
                    QString strCode;
                    bool bSuccess = false;
                    QJsonObject obj;
                    QMap<QString, bool> mapStatus;
                    for (int i = 0; i < iSize; i++)
                    {
                        obj = dataArray[i].toObject();
                        strCode = obj["code"].toString();
                        bSuccess = obj["isSuccess"].toBool();                        
                        mapStatus.insert(strCode, bSuccess);                        
                    }
                    emit activeCodeStatusSignals(mapStatus);
                }
            }
        }
        reply->deleteLater();
        });
}

void MainWindow::on_toolBtnBuyPhone_clicked()
{
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}QToolButton:hover{color:rgb(255,255,255);background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: rgb(255, 255, 255);border-radius:1px;padding-left:8px;}");
    ui->frame_Renew->setHidden(true);
}

void MainWindow::on_toolBtnRenewPhone_clicked()
{
    ui->toolBtnRenewPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}QToolButton:hover{color:rgb(255,255,255);background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnBuyPhone->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: rgb(255, 255, 255);border-radius:1px;padding-left:8px;}");
    ui->frame_Renew->setHidden(false);
}


void MainWindow::on_btnBeginPay_clicked()
{
    if (0 == m_curLevelDataInfo.iMemberId)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请选择VIP类型!", this);
        tips->show();
        return;
    }
    //确定支付
    bool bAgree = ui->checkBox->isChecked();
    if(!bAgree)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请勾选协议!",this);
        tips->show();
        return;
    }

    int iCount = ui->listWidgetRenewList->count();
    QListWidgetItem* item = NULL;
    renewItemWidget* widget = NULL;
    bool bChecked = false;
    S_PHONE_INFO phoneInfo;
    m_strPayRelateId = "";
    for(int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewList->item(i);
        if(item != NULL)
        {
            widget = static_cast<renewItemWidget*>(ui->listWidgetRenewList->itemWidget(item));
            bChecked = widget->getCheckBoxStatus();
            if(bChecked)
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                qDebug()<<"选中iRow="<<i <<";No="<<phoneInfo.strInstanceNo;
                if (m_strPayRelateId.isEmpty())
                {
                    m_strPayRelateId = QString::asprintf("%d", phoneInfo.iId);
                }
                else
                {
                    m_strPayRelateId += QString::asprintf(",%d", phoneInfo.iId);
                }
            }
        }
    }

    qDebug() <<"m_strPayRelateId=" << m_strPayRelateId;
    QString strBuyNum = ui->lineEditBuyNumber->text();
    m_iBuyNum = strBuyNum.toInt();

    HttpCreateOrder(4, m_curLevelDataInfo.iMemberId, m_iBuyNum, 1, m_strPayRelateId);
}

//level item 
void MainWindow::do_selectLevelTypeSignals(S_LEVEL_INFO levelInfo)
{
    qDebug() << "click do_selectLevelTypeSignals level Type="<< levelInfo.iLevelId;
    //获取QScrollBar的所有列表
    QList<LevelItemWidget*> levelItemList = ui->scrollAreaWidgetContents->findChildren<LevelItemWidget*>();
    foreach(LevelItemWidget* levelItem, levelItemList) 
    {
        if (levelItem->getLevelInfo().iLevelId != levelInfo.iLevelId)
        {
            levelItem->setLabelCheckStatus(false);
        }
    }
    
    //设置显示
    /*LevelItemWidget* levelItemWidget = NULL;
    QListWidgetItem* levelItem=NULL;
    LEVEL_TYPE currentType;
    int iCount = ui->listWidgetLevel->count();    
    for(int iRow=0; iRow < iCount;iRow++)
    {
        levelItem = ui->listWidgetLevel->item(iRow);
        currentType = (LEVEL_TYPE)levelItem->data(Qt::UserRole).toInt();
        if (currentType != enType)
        {
            levelItemWidget = static_cast<LevelItemWidget*>(ui->listWidgetLevel->itemWidget(levelItem));
            //enType = (LEVEL_TYPE)item->data(Qt::UserRole).toInt();
            qDebug()<<"iRow="<<iRow<<"currentType="<<currentType<<"选中状态：";
            levelItemWidget->setLabelCheckStatus(false);

            //续费的列表
            //ui->listWidgetRenewList
        }
    }*/

    //加载vip列表
    loadVipType(levelInfo);
}

void MainWindow::do_refreshMemberListSignals(int iLevelId, QMap<int, S_LEVEL_DATA_INFO> mapData)
{
    qDebug() << "click do_refreshMemberListSignals level Type=" << iLevelId;
    //获取QScrollBar的所有列表
    QList<LevelItemWidget*> levelItemList = ui->scrollAreaWidgetContents->findChildren<LevelItemWidget*>();
    foreach(LevelItemWidget * levelItem, levelItemList)
    {
        if (levelItem->getLevelInfo().iLevelId != iLevelId)
        {
            levelItem->setLabelCheckStatus(false);
        }
    }

    //清空列表
    ui->listWidgetVIP->clear();

    if (mapData.size() <= 0)
    {
        qDebug() << "无套餐";
        ui->stackedWidget_2->setCurrentWidget(ui->page_EmptyMeal);
        return;
    }
    qDebug() << "刷新会员列表";
    //加载套餐列表
    //ui->listWidgetVIP
    //ui->label_2->setText(QString("%1套餐").arg(levelInfo.strLevelName));
    ui->stackedWidget_2->setCurrentWidget(ui->page_Meal);
    int iVIPType = 0;
    QListWidgetItem* vipItem = NULL;
    VIPItemWidget* vipWidget = NULL;
    ui->widget->setVisible(true);

    QMap<int, S_LEVEL_DATA_INFO>::iterator iter = mapData.begin();
    for (; iter != mapData.end(); iter++)
    {
        vipItem = new QListWidgetItem(ui->listWidgetVIP);
        vipItem->setSizeHint(QSize(ITEM_WIDGET_VIP_WIDTH, ITEM_WIDGET_VIP_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        vipItem->setData(Qt::UserRole, iter->iMemberId);
        ui->listWidgetVIP->addItem(vipItem);

        qDebug() << "vip=" << iter->iMemberId;
        vipWidget = new VIPItemWidget(*iter, iter->strLevelName, this);
        connect(vipWidget, &VIPItemWidget::selectVIPTypeSignals, this, &MainWindow::do_selectVIPTypeSignals);
        ui->listWidgetVIP->setItemWidget(vipItem, vipWidget);

        //设置默认第一个被选中
        if (iter == mapData.begin())
        {
            m_curLevelDataInfo = *iter;
            vipWidget->setLabelCheckStatus(true);
            //更新支付金额
            calcNeedPayMoney();
        }
    }

    //判断是否可见
    if (ui->frame_Renew->isVisible())
    {
        //初始化续费列表listWidgetRenewList
        //加载数据并显示
        ui->listWidgetRenewList->clear();
        int iCount = m_mapPhoneInfo.size();
        if (iCount > 0)
        {
            //初始化续费列表
            QListWidgetItem* renewListItem = NULL;
            renewItemWidget* widget = NULL;
            QMap<int, S_PHONE_INFO>::iterator iter = m_mapPhoneInfo.begin();
            for (; iter != m_mapPhoneInfo.end(); iter++)
            {
                qDebug() << "phone level id=" << iter->iLevel;
                if (iter->iLevel == iLevelId)
                {
                    renewListItem = new QListWidgetItem(ui->listWidgetRenewList);
                    renewListItem->setData(Qt::UserRole, QVariant::fromValue(*iter));
                    renewListItem->setSizeHint(QSize(RENEW_ITEM_WIDTH, RENEW_ITEM_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
                    widget = new renewItemWidget(*iter, this);
                    ui->listWidgetRenewList->addItem(renewListItem);
                    ui->listWidgetRenewList->setItemWidget(renewListItem, widget);
                }
            }
        }
    }
}

//初始化vip列表
void MainWindow::loadVipType(S_LEVEL_INFO levelInfo)
{
    //清空列表
    ui->listWidgetVIP->clear();

    QMap<int, QMap<int, S_LEVEL_DATA_INFO>>::iterator iterFind = m_mapLevel.find(levelInfo.iLevelId);
    if (iterFind != m_mapLevel.end())
    {
        qDebug() << "加载vip列表 levelId=" << levelInfo.iLevelId;
        //加载套餐列表
        //ui->listWidgetVIP
        ui->label_2->setText(QString("%1套餐").arg(levelInfo.strLevelName));
        ui->stackedWidget_2->setCurrentWidget(ui->page_Meal);
        int iVIPType = 0;
        QListWidgetItem* vipItem = NULL;
        VIPItemWidget* vipWidget = NULL;
        ui->widget->setVisible(true);

        QMap<int, S_LEVEL_DATA_INFO>::iterator iter = iterFind->begin();
        for (; iter != iterFind->end(); iter++)
        {            
            vipItem = new QListWidgetItem(ui->listWidgetVIP);
            vipItem->setSizeHint(QSize(ITEM_WIDGET_VIP_WIDTH, ITEM_WIDGET_VIP_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
            vipItem->setData(Qt::UserRole, iter->iMemberId);
            ui->listWidgetVIP->addItem(vipItem);

            qDebug() << "vip=" << iter->iMemberId;
            vipWidget = new VIPItemWidget(*iter,levelInfo.strLevelName, this);
            connect(vipWidget, &VIPItemWidget::selectVIPTypeSignals, this, &MainWindow::do_selectVIPTypeSignals);
            ui->listWidgetVIP->setItemWidget(vipItem, vipWidget);

            //设置默认第一个被选中
            if (iter == iterFind->begin())
            {
                m_curLevelDataInfo = *iter;
                vipWidget->setLabelCheckStatus(true);
                //更新支付金额
                calcNeedPayMoney();
            }
        }

        //判断是否可见
        if (ui->frame_Renew->isVisible())
        {
            //初始化续费列表listWidgetRenewList
            //加载数据并显示
            ui->listWidgetRenewList->clear();
            int iCount = m_mapPhoneInfo.size();
            if (iCount > 0)
            {
                //初始化续费列表
                QListWidgetItem* renewListItem = NULL;
                renewItemWidget* widget = NULL;
                QMap<int, S_PHONE_INFO>::iterator iter = m_mapPhoneInfo.begin();
                for (; iter != m_mapPhoneInfo.end(); iter++)
                {
                    qDebug() << "phone level id=" << iter->iLevel;
                    if (iter->iLevel == levelInfo.iLevelId)
                    {
                        renewListItem = new QListWidgetItem(ui->listWidgetRenewList);
                        renewListItem->setData(Qt::UserRole, QVariant::fromValue(*iter));
                        renewListItem->setSizeHint(QSize(RENEW_ITEM_WIDTH, RENEW_ITEM_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
                        widget = new renewItemWidget(*iter, this);
                        ui->listWidgetRenewList->addItem(renewListItem);
                        ui->listWidgetRenewList->setItemWidget(renewListItem, widget);
                    }
                }
            }
        }
    }
    else
    {
        qDebug() << "无套餐";
        ui->stackedWidget_2->setCurrentWidget(ui->page_EmptyMeal);
    }    
}

//vip item
void MainWindow::do_selectVIPTypeSignals(S_LEVEL_DATA_INFO levelInfo)
{
    m_curLevelDataInfo = levelInfo;
    qDebug() << "click do_selectVIPTypeSignals memberId=" << levelInfo.iMemberId;
    calcNeedPayMoney();

    //设置显示
    VIPItemWidget* vipItemWidget = NULL;
    QListWidgetItem* vipItem = NULL;
    int iMemberId = 0;
    int iCount = ui->listWidgetVIP->count();
    for (int iRow = 0; iRow < iCount; iRow++)
    {
        vipItem = ui->listWidgetVIP->item(iRow);
        iMemberId = vipItem->data(Qt::UserRole).toInt();
        if (iMemberId != levelInfo.iMemberId)
        {
            vipItemWidget = static_cast<VIPItemWidget*>(ui->listWidgetVIP->itemWidget(vipItem));
            qDebug() << "当前选中：等级" << iMemberId;
            vipItemWidget->setLabelCheckStatus(false);         
        }
    }
}


void MainWindow::on_toolBtnSub_clicked()
{
    //减少
    QString strBuyNum = ui->lineEditBuyNumber->text();
    int iBuyNum = strBuyNum.toInt();
    if(iBuyNum > 1)
        strBuyNum = strBuyNum.asprintf("%d",--iBuyNum);
    ui->lineEditBuyNumber->setText(strBuyNum);
}


void MainWindow::on_toolBtnAdd_clicked()
{
    //增加
    QString strBuyNum = ui->lineEditBuyNumber->text();
    int iBuyNum = strBuyNum.toInt();
    strBuyNum = strBuyNum.asprintf("%d",++iBuyNum);
    ui->lineEditBuyNumber->setText(strBuyNum);
}

void MainWindow::calcNeedPayMoney()
{
    int iBuyNum = ui->lineEditBuyNumber->text().toInt();
    QString str;
    str = str.asprintf("%.2f", iBuyNum * m_curLevelDataInfo.fActivityPrice);
    QStringList strValueList = str.split('.');
    if (strValueList.size() > 1)
    {
        ui->labelPayMoneyInteger->setText(strValueList.at(0));
        ui->labelPayMoney->setText(QString(".%1").arg(strValueList.at(1)));
    }
}
void MainWindow::on_lineEditBuyNumber_textChanged(const QString &arg1)
{
    //文本框值改变
    int iBuyNum = arg1.toInt();//ui->lineEditBuyNumber->text().toInt();
    if (iBuyNum > 200)
    {
        MessageTips* tips = new MessageTips("单次限购200台");
        tips->show();
        ui->lineEditBuyNumber->setText("200");
        iBuyNum = 200;
        //return;
    }
    calcNeedPayMoney();
}
void MainWindow::CalculateBorderIndex(QMouseEvent* ev) {
    static QPoint rightTop;
    static QPoint leftBottom;
    static QPoint leftTop;
    static QPoint rightBottom;

    int MOUSE_GAP = 5;

    if (!m_bResizeIng)
    {
        //右下角
        if (ev->x() > this->width() - MOUSE_GAP && ev->y() > this->height() - MOUSE_GAP)
        {
            // 右下角
            this->setCursor(Qt::SizeFDiagCursor);
            dir = RIGHTBOTTOM;
        }
        else if (ev->x() > this->width() - MOUSE_GAP && ev->y() > MOUSE_GAP && ev->y() < this->height() - MOUSE_GAP)
        {
            // 右边
            this->setCursor(Qt::SizeHorCursor);
            dir = RIGHT;
        }
        else if (ev->x() > MOUSE_GAP && ev->y() < MOUSE_GAP)
        {
            // 上边
            this->setCursor(Qt::SizeVerCursor);
            dir = UP;
        }
        else if (ev->x() < MOUSE_GAP && ev->y() > MOUSE_GAP && ev->y() < this->height() - MOUSE_GAP)
        {
            // 左边
            this->setCursor(Qt::SizeHorCursor);
            dir = LEFT;
        }
        else if (ev->x() < MOUSE_GAP && ev->y() < this->height() - MOUSE_GAP)
        {
            // 左上角
            this->setCursor(Qt::SizeBDiagCursor);
            dir = LEFTTOP;
        }
        else if (ev->x() < this->width() - MOUSE_GAP && ev->y() > this->height() - MOUSE_GAP)
        {
            // 下边
            this->setCursor(Qt::SizeVerCursor);
            dir = DOWN;
        }
        else if (ev->x() > MOUSE_GAP && ev->y() > this->height() - MOUSE_GAP)
        {
            // 右下角
            this->setCursor(Qt::SizeFDiagCursor);
            dir = RIGHTBOTTOM;
        }
        else
        {
            // 其他位置
            this->setCursor(Qt::ArrowCursor);
            dir = NONE;
        }
    }

    if (m_bCanResize && dir != NONE)
    {
        qDebug() << "可以改变尺寸";
        if (dir == RIGHT)
        {
            if (m_globalPoint.x() < ev->globalPos().x() && ev->globalPos().x() > rightTop.x())
            {
                m_bResizeIng = true;
                qDebug() << "向右拉大";
                this->resize(m_oldSize.width() + (ev->globalX() - m_globalPoint.x()), this->height());
            }
            else if (ev->globalPos().x() < m_globalPoint.x())
            {
                m_bResizeIng = true;
                qDebug() << "向右拉小";
                this->resize(m_oldSize.width() - (m_globalPoint.x() - ev->globalX()), this->height());
            }
        }
        else if (dir == DOWN)
        {
            if (ev->globalY() > m_globalPoint.y() && ev->globalY() > this->y() + this->height())
            {
                qDebug() << "向下拉大";
                m_bResizeIng = true;
                this->resize(this->width(), this->height() + ev->globalY() - m_globalPoint.y());
            }
            else if (ev->globalY() < m_globalPoint.y())
            {
                qDebug() << "向下拉小";
                m_bResizeIng = true;
                this->resize(this->width(), this->height() - (m_globalPoint.y() - ev->globalY()));
            }
        }
        else if (dir == LEFT)
        {
            if (m_globalPoint.x() > ev->globalX() && leftTop.x() > ev->globalX())
            {
                m_bResizeIng = true;
                qDebug() << "向左拉大";
                this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                this->move(this->x() - (m_globalPoint.x() - ev->globalPos().x()), this->y());
            }
            else if (m_globalPoint.x() < ev->globalX())
            {
                if (this->width() != this->minimumWidth())
                {
                    m_bResizeIng = true;
                    qDebug() << "向左拉小";
                    this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                    this->move(rightTop.x() - this->width(), this->y());
                }
            }
        }
        else if (dir == UP)
        {
            if (m_globalPoint.y() > ev->globalY() && ev->globalY() < this->y())
            {
                m_bResizeIng = true;
                qDebug() << "向上拉大";

                this->resize(this->width(), this->height() + (m_globalPoint.y() - ev->globalY()));
                this->move(this->x(), this->y() - (m_globalPoint.y() - ev->globalY()));
            }
            else if (m_globalPoint.y() < ev->globalY())
            {
                m_bResizeIng = true;
                qDebug() << "向上拉小";
                this->resize(this->width(), this->height() - (ev->globalY() - m_globalPoint.y()));
                this->move(this->x(), leftBottom.y() - this->height());
            }
        }
        else if (dir == RIGHTBOTTOM)
        {
            //拉大
            if (ev->globalX() > m_globalPoint.x() || ev->globalY() > m_globalPoint.y())
            {
                if (ev->globalX() > rightBottom.x())
                {
                    m_bResizeIng = true;
                    qDebug() << "RIGHTBOTTOM 拉大 x";
                    this->resize(m_oldSize.width() + (ev->globalX() - m_globalPoint.x()), this->height());
                }
                if (ev->globalY() > rightBottom.y())
                {
                    qDebug() << "RIGHTBOTTOM 拉大 y";
                    m_bResizeIng = true;
                    this->resize(this->width(), this->height() + ev->globalY() - m_globalPoint.y());
                }
            }
            //缩小
            else if (ev->globalX() < m_globalPoint.x() || ev->globalY() < m_globalPoint.y())
            {
                m_bResizeIng = true;
                qDebug() << "右下拉小";
                this->resize(m_oldSize.width() - (m_globalPoint.x() - ev->globalX()), this->height());
                this->resize(this->width(), this->height() - (m_globalPoint.y() - ev->globalY()));
            }
        }
        else if (dir == RIGHTTOP)
        {
            if (ev->globalX() > m_globalPoint.x() || ev->globalY() < m_globalPoint.y())
            {
                qDebug() << "右上拉大";
                if (ev->globalX() > rightTop.x())
                {
                    m_bResizeIng = true;
                    this->resize(m_oldSize.width() + (ev->globalX() - m_globalPoint.x()), this->height());
                }
                if (ev->globalY() < rightTop.y())
                {
                    m_bResizeIng = true;
                    this->resize(this->width(), this->height() + (m_globalPoint.y() - ev->globalY()));
                    this->move(leftTop.x(), this->y() - (m_globalPoint.y() - ev->globalY()));
                }

            }
            else if (ev->globalX() < m_globalPoint.x() || ev->globalY() > m_globalPoint.y())
            {
                m_bResizeIng = true;
                qDebug() << "右上拉小";

                this->resize(this->width(), this->height() - (ev->globalY() - m_globalPoint.y()));
                this->resize(m_oldSize.width() - (m_globalPoint.x() - ev->globalX()), this->height());
                this->move(leftTop.x(), leftBottom.y() - this->height());
            }
        }
        else if (dir == LEFT)
        {
            if (ev->globalX() < m_globalPoint.x() || ev->globalY() < m_globalPoint.y())
            {
                qDebug() << "左上拉大";
                if (ev->globalX() < leftTop.x())
                {
                    m_bResizeIng = true;
                    this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                    this->move(this->x() - (m_globalPoint.x() - ev->globalPos().x()), leftTop.y());
                }
                if (ev->globalY() < leftTop.y())
                {
                    m_bResizeIng = true;
                    this->resize(this->width(), this->height() + (m_globalPoint.y() - ev->globalY()));
                    this->move(this->x(), this->y() - (m_globalPoint.y() - ev->globalY()));
                }
            }
            else if (ev->globalX() > m_globalPoint.x() || ev->globalY() > m_globalPoint.y())
            {
                m_bResizeIng = true;
                qDebug() << "左上拉小";
                this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                this->move(rightTop.x() - this->width(), this->y());

                if (ev->globalY() > leftTop.y())
                {
                    qDebug() << ev->globalY() << leftTop.y();
                    this->resize(this->width(), this->height() - (ev->globalY() - m_globalPoint.y()));
                    this->move(this->x(), leftBottom.y() - this->height());
                }
            }
        }
        else if (dir == LEFTBOTTOM)
        {
            if (ev->globalX() < m_globalPoint.x() || ev->globalY() > m_globalPoint.y())
            {
                qDebug() << "左下拉大";
                if (ev->globalX() < leftTop.x())
                {
                    m_bResizeIng = true;
                    this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                    this->move(this->x() - (m_globalPoint.x() - ev->globalPos().x()), this->y());
                }
                if (ev->globalY() > leftBottom.y())
                {
                    m_bResizeIng = true;
                    this->resize(this->width(), this->height() + ev->globalY() - m_globalPoint.y());
                }
            }
            else if (ev->globalX() > m_globalPoint.x() || ev->globalY() < m_globalPoint.y())
            {
                m_bResizeIng = true;
                qDebug() << "左下拉小";

                this->resize(this->width(), this->height() - (m_globalPoint.y() - ev->globalY()));
                this->resize(m_oldSize.width() + m_globalPoint.x() - ev->globalX(), this->height());
                this->move(rightTop.x() - this->width(), this->y());
            }
        }
    }

    leftTop = this->pos();
    leftBottom.setX(this->pos().x());
    leftBottom.setY(this->y() + this->height());
    rightTop.setX(this->x() + this->width());
    rightTop.setY(this->y());
    rightBottom.setX(this->x() + this->width());
    rightBottom.setY(this->y() + this->height());
    m_oldSize = this->size();
    m_globalPoint = ev->globalPos();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if (dir != NONE) {
            this->mouseGrabber();
            m_bCanResize = true;
        }
        else 
        {
            if (event->pos().x() >= ui->frame->width() && event->pos().y() >= 0 && event->pos().x() <= this->width() && event->pos().y() <= ui->frame_9->height())
            {
                m_bMoving = true;
                dragPosition = event->globalPos() - this->frameGeometry().topLeft();
            }
        }
        break;
    case Qt::RightButton:
        //this->close();
        break;
    default:
        QMainWindow::mousePressEvent(event);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //qDebug() << "mouseReleaseEvent LeftButton";
        isLeftPressDown = false;
        m_bMoving = false;
        //if (dir != NONE) 
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
            dir = NONE;
            m_bCanResize = false;
            m_bResizeIng = false;
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPosition = event->globalPosition().toPoint();
    if (m_bMoving)
    {
        if ((event->buttons() & Qt::LeftButton)
            && (!window()->isMaximized())
            && (globalPosition - m_LastPos - pos()).manhattanLength() > QApplication::startDragDistance())
        {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
        return QMainWindow::mouseMoveEvent(event);
    }
    
    CalculateBorderIndex(event);

    /*if (isLeftPressDown) {
        if (dir == NONE) {
            if (!(window()->isMaximized())) {
                move(event->globalPos() - dragPosition);
                event->accept();
            }
        }
    }*/
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::do_ContractionOrExpansion(bool bChecked)
{
    ui->groupWidget->setVisible(!m_IsContraction);
    m_toolBtnContraction->setVisible(!m_IsContraction);
    m_toolBtnExpansion->setVisible(m_IsContraction);
    m_IsContraction = !m_IsContraction;
}

void MainWindow::on_toolBtnDropFunction_clicked()
{
    //收起功能
    ui->frame_3->setVisible(false);
    ui->frame_4->setVisible(false);
    ui->frame_8->setVisible(false);

    ui->toolBtnDropFunction->setVisible(false);
    ui->toolBtnExpansionFunction->setVisible(true);
}


void MainWindow::on_toolBtnExpansionFunction_clicked()
{
    //展开功能
    //第一版暂时屏蔽功能
    ui->frame_3->setVisible(true);
    //ui->frame_4->setVisible(true);
    //ui->frame_8->setVisible(true);

    ui->toolBtnDropFunction->setVisible(true);
    ui->toolBtnExpansionFunction->setVisible(false);
}


void MainWindow::on_toolBtnChangeHorScreen_clicked()
{
    qDebug()<<"切换到横屏";
    GlobalData::bVerticalScreen = false;
    ui->toolBtnChangeVerScreen->setVisible(true);
    ui->toolBtnChangeHorScreen->setVisible(false);

    int iIndex = ui->comboBoxView->currentIndex();
    int i = ui->comboBoxView->itemData(iIndex).toInt();
    GlobalData::iPhoneItemWidth = ITEM_PHONE_HORIZONTAL_WIDTH * (i / 100.0);
    GlobalData::iPhoneItemHeight = ITEM_PHONE_HORIZONTAL_HEIGHT * (i / 100.0);
    
    on_treeWidget_itemPressed(m_pCurItem, NULL);
}


void MainWindow::on_toolBtnChangeVerScreen_clicked()
{
    qDebug()<<"切换到竖屏";
    GlobalData::bVerticalScreen = true;
    ui->toolBtnChangeVerScreen->setVisible(false);
    ui->toolBtnChangeHorScreen->setVisible(true);
    int iIndex = ui->comboBoxView->currentIndex();
    int i = ui->comboBoxView->itemData(iIndex).toInt();
    GlobalData::iPhoneItemWidth = ITEM_PHONE_VERTICAL_WIDTH * (i / 100.0);
    GlobalData::iPhoneItemHeight = ITEM_PHONE_VERTICAL_HEIGHT * (i / 100.0);
    
    on_treeWidget_itemPressed(m_pCurItem, NULL);
}


void MainWindow::on_toolBtnPayZhifubao_clicked()
{
    ui->toolBtnPayZhifubao->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_select.png);padding-left:10px;}");
    ui->toolBtnPayWechat->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_normal.png);padding-left:10px;}");
}


void MainWindow::on_toolBtnPayWechat_clicked()
{
    ui->toolBtnPayWechat->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_select.png);padding-left:10px;}");
    ui->toolBtnPayZhifubao->setStyleSheet("QToolButton{border-image:url(:/main/resource/main/border_normal.png);padding-left:10px;}");
}


void MainWindow::on_btnReturn_clicked()
{
    //支付返回
    ui->stackedWidget->setCurrentWidget(ui->pageBuy);
}


void MainWindow::on_btnFinish_clicked()
{
    //支付完成
    ui->stackedWidget->setCurrentWidget(ui->pageBuy);
}


void MainWindow::on_toolBtnPhoto_clicked()
{
    //个人中心
    if (NULL == m_individualCenterWidget)
    {
        m_individualCenterWidget = new IndividualCenterWidget(this);
        connect(m_individualCenterWidget, &IndividualCenterWidget::notifyMainWindowRefreshGroupListSignals, this, &MainWindow::on_btnGroupRefresh_clicked);
        m_individualCenterWidget->setWindowModality(Qt::NonModal);
        connect(m_individualCenterWidget, &IndividualCenterWidget::logoutSignals, this, [=]()
            {
                if (m_TaskTimer->isActive())
                {
                    m_TaskTimer->stop();
                }
                this->close();
                emit logoutSignals();
            });
    }
    if (m_individualCenterWidget->isVisible())
    {
        GlobalData::bHideIndividualCenterWidget = false;
        m_individualCenterWidget->hide();
    }
    else
    {
        GlobalData::bHideIndividualCenterWidget = true;
        QRect rect = ui->toolBtnPhoto->geometry();
        m_individualCenterWidget->move(QPoint(rect.x(), ui->frame_9->height()));
        m_individualCenterWidget->show();
    }    
}

void MainWindow::on_treeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if(qApp->mouseButtons() == Qt::RightButton) // 只针对鼠标右键
    {
        if (item->parent() != NULL)
        {
            GlobalData::enRightClickType = EN_GROUP_TREEWIDGET;
            m_CurSelMenuPhoneInfo = item->data(0, Qt::UserRole).value<S_PHONE_INFO>();
            pActionCopyCloudId->setText("复制云号[" + m_CurSelMenuPhoneInfo.strInstanceNo+"]");
            m_PhoneMenu->exec(QCursor::pos());
        }            
        else
        {
            S_GROUP_INFO groupInfo = item->data(0, Qt::UserRole).value<S_GROUP_INFO>();
            if ("默认分组" != groupInfo.strGroupName)
            {
                m_menu->exec(QCursor::pos());
            }
        }            
    }
    else
    {        
        if (m_isIconMode)
        {            
            ui->listWidget->clear();
            m_iCheckCount = 0;
            m_listInstanceNo.clear();
            if (m_TaskTimer->isActive())
            {
                m_TaskTimer->stop();
            }
            
            BianliTreeWidgetSelectItem(item);
            if (m_listInstanceNo.size() > 0)
            {
                m_TaskTimer->start(TIMER_INTERVAL);
                this->m_toolObject->HttpPostInstanceScreenshotRefresh(m_listInstanceNo);
            }
        }
        else
        {
            ui->listWidget2->clear();
            m_iCheckCount = 0;
            BianliTreeWidgetSelectItem(item);
        }
    }
}

//一分钟响应一次
void MainWindow::do_timeoutRefreshPicture()
{
    //QDateTime dateTime = QDateTime::currentDateTime();
    //qDebug() << dateTime.toString("yyyy-MM-dd HH:mm:ss");    
    //请求刷新函数
    //qDebug() << "请求生成图片函数";
    //获取选中分组的所有手机
    if (m_listInstanceNo.size() > 0)
    {
        this->m_toolObject->HttpPostInstanceScreenshotRefresh(m_listInstanceNo);
    }
    else
    {
        if (m_TaskTimer->isActive())
        {
            m_TaskTimer->stop();
        }
    }
    
}


void MainWindow::on_checkBoxRenew_clicked(bool checked)
{
    //选中状态
    int iCount = ui->listWidgetRenewList->count();
    QListWidgetItem* item = NULL;
    renewItemWidget* widget = NULL;
    for(int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewList->item(i);
        if(item != NULL)
        {
            widget = static_cast<renewItemWidget*>(ui->listWidgetRenewList->itemWidget(item));
            widget->setCheckBoxStatus(checked);
        }
    }
}


void MainWindow::on_btnCancelSelect_clicked()
{
    //取消选择
    if (m_isIconMode)
    {
        int iCount = ui->listWidget->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(false);
                }
            }
        }
    }
    else
    {
        int iCount = ui->listWidget2->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneListModeItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget2->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneListModeItemWidget*>(ui->listWidget2->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(false);
                }
            }
        }
    }
}

void MainWindow::on_checkBoxAllSelect_clicked(bool checked)
{
    //全选
    if (m_isIconMode)
    {
        int iCount = ui->listWidget->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(checked);
                }
            }
        }
    }
    else
    {
        int iCount = ui->listWidget2->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneListModeItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget2->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneListModeItemWidget*>(ui->listWidget2->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(checked);
                }
            }
        }
    }
}


void MainWindow::on_checkBoxFanSelect_clicked(bool checked)
{
    int iCount = 0;
    //反选
    if (m_isIconMode)
    {
        iCount = ui->listWidget->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneItemWidget*>(ui->listWidget->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(!phoneItem->getCheckBoxStatus());
                }
            }
        }
    }
    else
    {
        iCount = ui->listWidget2->count();
        if (iCount <= 0)
        {
            return;
        }

        QListWidgetItem* item = NULL;
        PhoneListModeItemWidget* phoneItem = NULL;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget2->item(i);
            if (item != NULL)
            {
                phoneItem = static_cast<PhoneListModeItemWidget*>(ui->listWidget2->itemWidget(item));
                if (phoneItem != NULL)
                {
                    phoneItem->setCheckBoxStatus(!phoneItem->getCheckBoxStatus());
                }
            }
        }
    }
}

void MainWindow::on_toolBtnListMode_clicked()
{
    ui->toolBtnListMode->setStyleSheet("QToolButton {border:none;color:rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}");
    ui->toolBtnPreviewMode->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:0px;padding-left:8px;background-color:#E6E9F2;}");

    ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageListMode);
    m_isIconMode = false;
    //初始化数据
    if (m_TaskTimer->isActive())
    {
        m_TaskTimer->stop();
    }
    ui->listWidget2->clear();
    m_iCheckCount = 0;
    PhoneListModeItemWidget* widget2 = NULL;
    QListWidgetItem* item = NULL;
    S_PHONE_INFO phoneInfo;
    QListWidgetItem* phoneItem = NULL;
    int iCount = ui->listWidget->count();
    if (iCount > 0)
    {
        QMap<int, S_LEVEL_INFO>::iterator iterFind;
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget->item(i);
            if (item != NULL)
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                //获取对应level name
                iterFind = m_mapLevelList.find(phoneInfo.iLevel);
                if (iterFind != m_mapLevelList.end())
                {
                    phoneInfo.strLevelName = iterFind->strLevelName;
                }

                widget2 = new PhoneListModeItemWidget(phoneInfo, this);
                widget2->setCheckBoxStatus(((PhoneItemWidget*)ui->listWidget->itemWidget(item))->getCheckBoxStatus());
                connect(widget2, &PhoneListModeItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
                connect(widget2, &PhoneListModeItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
                phoneItem = new QListWidgetItem(ui->listWidget2);
                phoneItem->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
                phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                ui->listWidget2->addItem(phoneItem);
                ui->listWidget2->setItemWidget(phoneItem, widget2);
            }
        }
    }
    else
    {
        ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageListNoData);
    }
    ui->listWidget->clear();    
}

void MainWindow::on_toolBtnPreviewMode_clicked()
{
    ui->toolBtnPreviewMode->setStyleSheet("QToolButton {border:none;color:rgb(204, 204, 204);background-color:#FF9092A4;border-radius:1px;padding-left:8px;}");
    ui->toolBtnListMode->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:0px;padding-left:8px;background-color:#E6E9F2;}");

    ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageIconMode);
    m_isIconMode = true;
    m_iCheckCount = 0;
    ui->listWidget->clear();
    PhoneItemWidget* widget = NULL;
    QListWidgetItem* item = NULL;
    S_PHONE_INFO phoneInfo;
    QListWidgetItem* phoneItem = NULL;
    m_listInstanceNo.clear();
    int iCount = ui->listWidget2->count();
    if (iCount > 0)
    {
        for (int i = 0; i < iCount; i++)
        {
            item = ui->listWidget2->item(i);
            if (item != NULL)
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();

                m_listInstanceNo << phoneInfo.strInstanceNo;
                qDebug() << "树上节点信息 name" << phoneInfo.strName << "strInstanceNo=" << phoneInfo.strInstanceNo << "phoneInfo.strCreateTime=" << phoneInfo.strCreateTime << "phoneInfo.strCurrentTime=" << phoneInfo.strCurrentTime << "phoneInfo.strExpireTime=" << phoneInfo.strExpireTime << "id=" << phoneInfo.iId << "authType=" << phoneInfo.iAuthType << "level=" << phoneInfo.iLevel;

                //重新显示listWidget
                widget = new PhoneItemWidget(phoneInfo, this);
                widget->setCheckBoxStatus(((PhoneListModeItemWidget*)ui->listWidget2->itemWidget(item))->getCheckBoxStatus());
                connect(widget, &PhoneItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
                connect(widget, &PhoneItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
                phoneItem = new QListWidgetItem(ui->listWidget);
                phoneItem->setSizeHint(QSize(GlobalData::iPhoneItemWidth, GlobalData::iPhoneItemHeight));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
                phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                ui->listWidget->addItem(phoneItem);
                ui->listWidget->setItemWidget(phoneItem, widget);
            }
        }
    }
    else
    {
        ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageIconNoData);
    }
    ui->listWidget2->clear();

    if (m_listInstanceNo.size() > 0)
    {
        m_TaskTimer->start(TIMER_INTERVAL);
        this->m_toolObject->HttpPostInstanceScreenshotRefresh(m_listInstanceNo);
    }
}

void MainWindow::startDownload(QString strUrl) 
{
    FileDownloader* downloader = new FileDownloader(this);    
    downloader->setUrl(strUrl, "xxx.png");
    downloader->start();
}

void MainWindow::on_toolBtnBatchOperation_clicked(bool checked)
{
    QStringList strPhoneList = getCheckedPhoneInstance();
    if (strPhoneList.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请先选择云手机后再进行操作");
        tips->show();
        return;
    }

    QPoint globalPos = ui->toolBtnBatchOperation->mapToGlobal(QPoint(0, 0));
    m_BatchOperMenu->exec(QPoint(globalPos.x(), globalPos.y()+ui->toolBtnBatchOperation->height()));
}


void MainWindow::on_toolBtnSetting_clicked()
{
    //系统设置
    if(NULL == m_systemSettingWidget)
        m_systemSettingWidget = new SystemSettingWidget();
    m_systemSettingWidget->setModal(true);
    m_systemSettingWidget->show();
    connect(m_systemSettingWidget, &SystemSettingWidget::destroyed, this, [this]() {
        m_systemSettingWidget = NULL;
        });
}


void MainWindow::on_toolBtnCustomer_clicked()
{
    //客服
    //打开官网客服
    //https://ccc-v2.aliyun.com/v-chat?token=9XYGTGWtq2yxzD3cm38yM7T_QkVSJzQAOkOb_XIk-aVrFVf7Dx49UKN0ym7bYIPjKaJ5Gkt1onRIX_6726a9reskIUuwRp_cdgYLRzd2lpE%3D
    QString strUrl = "https://ccc-v2.aliyun.com/v-chat?token=9XYGTGWtq2yxzD3cm38yM7T_QkVSJzQAOkOb_XIk-aVrFVf7Dx49UKN0ym7bYIPjKaJ5Gkt1onRIX_6726a9reskIUuwRp_cdgYLRzd2lpE%3D";
    QDesktopServices::openUrl(QUrl(strUrl));
}


void MainWindow::on_comboBoxView_currentIndexChanged(int index)
{
    int i = ui->comboBoxView->itemData(index).toInt();
    qDebug()<<"click i="<<i <<"old width="<< GlobalData::iPhoneItemWidth<<"old height="<< GlobalData::iPhoneItemHeight;
    if (GlobalData::bVerticalScreen)
    {
        GlobalData::iPhoneItemWidth = ITEM_PHONE_VERTICAL_WIDTH * (i / 100.0);
        GlobalData::iPhoneItemHeight = ITEM_PHONE_VERTICAL_HEIGHT * (i / 100.0);
    }
    else
    {
        GlobalData::iPhoneItemWidth = ITEM_PHONE_HORIZONTAL_WIDTH * (i / 100.0);
        GlobalData::iPhoneItemHeight = ITEM_PHONE_HORIZONTAL_HEIGHT * (i / 100.0);
    }    
    qDebug() << "click i=" << i << "new width=" << GlobalData::iPhoneItemWidth << "new height=" << GlobalData::iPhoneItemHeight;
    on_treeWidget_itemPressed(m_pCurItem, NULL);
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (GlobalData::bCloseMainWindowExit)
    {
		//注销
    	HttpLogout();
    }
    else
    {
        this->hide();
        //g_trayIcon->showMessage("程序在运行", "程序已最小化到系统托盘", QIcon(":/main/resource/main/aboutlogo.png"));
        event->ignore(); // 忽略关闭事件，防止程序退出
    }
}

void MainWindow::handleTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        this->showNormal();
        this->activateWindow();
    }
}

void MainWindow::on_radioButtonSyncOperation_clicked(bool checked)
{
    GlobalData::bIsSyncOperation = checked;
    qDebug() << "同步=" << GlobalData::bIsSyncOperation;

    QStringList strPhoneList = getCheckedPhoneInstance();
    if (strPhoneList.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("请先选择云手机后再进行操作", this);
        tips->show();
        return;
    }    

    on_ShowPhoneInstanceWidgetSignals(m_CurSelMenuPhoneInfo, false);
}

//显示实例
void MainWindow::on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu)
{
    if (NULL != m_MainPhoneInstanceWidget)
    {     
        emit closePhoneInstanceWidgetSignals();
        //m_MainPhoneInstanceWidget->close();
        delete m_MainPhoneInstanceWidget;
        m_MainPhoneInstanceWidget = NULL;
    }

    m_CurSelMenuPhoneInfo = sPhoneInfo;
    if (bShowMenu)
    {
        pActionCopyCloudId->setText("复制云号[" + m_CurSelMenuPhoneInfo.strInstanceNo + "]");
        m_PhoneMenu->exec(QCursor::pos());
        return;
    }

    GlobalData::mapSyncPhoneList.insert(sPhoneInfo.iId, sPhoneInfo);
    if (GlobalData::mapSyncPhoneList.size() <= 0)
        return;

    QMap<int, S_PHONE_INFO>::iterator iter = GlobalData::mapSyncPhoneList.begin();
    if (m_SyncOperListWidget != NULL)
        m_SyncOperListWidget->clear();
    else
    {
        m_SyncOperListWidget = new QListWidget(this);
        m_SyncOperListWidget->setViewMode(QListView::IconMode);
        //设置QListWidget中单元项的图片大小
        //ui->imageList->setIconSize(QSize(100,100));
        //设置QListWidget中单元项的间距
        m_SyncOperListWidget->setSpacing(ITEM_WIDGET_SPACING);
        //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
        m_SyncOperListWidget->setResizeMode(QListWidget::Adjust);
        //设置不能移动
        m_SyncOperListWidget->setMovement(QListWidget::Static);
        //设置单选
        m_SyncOperListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    m_SyncOperListWidget->show();
    QListWidgetItem* item = NULL;
    int height = PHONE_INSTANCE_VERTICAL_HEIGHT;
    int iwidth = calculateWidth(height);
    PhoneInstanceWidget* phoneWidget = NULL;
    for (; iter != GlobalData::mapSyncPhoneList.end(); iter++)
    {
        if (!GlobalData::bIsSyncOperation && iter->iId != sPhoneInfo.iId)
            continue;
        qDebug() << "同步实例id=" << iter.value().iId;
        //HttpGetInstanceSession(iter.value().iId);
        phoneWidget = new PhoneInstanceWidget(*iter);
        connect(phoneWidget, &PhoneInstanceWidget::TouchEventSignals, this, &MainWindow::paifaTouchEventSignals);
        connect(this, &MainWindow::paifaTouchEventSignals, phoneWidget, &PhoneInstanceWidget::dealTouchEventSignals);

        connect(phoneWidget, &PhoneInstanceWidget::ReturnSignals, this, &MainWindow::returnSignals);
        connect(this, &MainWindow::returnSignals, phoneWidget, &PhoneInstanceWidget::do_ReturnSignals);

        connect(phoneWidget, &PhoneInstanceWidget::HomeSignals, this, &MainWindow::homeSignals);
        connect(this, &MainWindow::homeSignals, phoneWidget, &PhoneInstanceWidget::do_HomeSignals);

        connect(phoneWidget, &PhoneInstanceWidget::ChangePageSignals, this, &MainWindow::changePageSignals);
        connect(this, &MainWindow::changePageSignals, phoneWidget, &PhoneInstanceWidget::do_ChangePageSignals);

        connect(phoneWidget, &PhoneInstanceWidget::RebootSignals, this, &MainWindow::RebootSignals);
        connect(this, &MainWindow::RebootSignals, phoneWidget, &PhoneInstanceWidget::do_RebootSignals);
        connect(phoneWidget, &PhoneInstanceWidget::FactoryDataResetSignals, this, &MainWindow::FactoryDataResetSignals);
        connect(this, &MainWindow::FactoryDataResetSignals, phoneWidget, &PhoneInstanceWidget::do_FactoryDataResetSignals);
        connect(phoneWidget, &PhoneInstanceWidget::ScreenshotsSignals, this, &MainWindow::ScreenshotsSignals);
        connect(this, &MainWindow::ScreenshotsSignals, phoneWidget, &PhoneInstanceWidget::do_ScreenshotsSignals);

        connect(phoneWidget, &PhoneInstanceWidget::VolumeUpSignals,this, &MainWindow::VolumeUpSignals);
        connect(this, &MainWindow::VolumeUpSignals, phoneWidget, &PhoneInstanceWidget::do_VolumeUpSignals);
        connect(phoneWidget, &PhoneInstanceWidget::VolumeDownSignals,this, &MainWindow::VolumeDownSignals);
        connect(this, &MainWindow::VolumeDownSignals, phoneWidget, &PhoneInstanceWidget::do_VolumeDownSignals);
        
        connect(phoneWidget, &PhoneInstanceWidget::HorizontalSignals, this, &MainWindow::HorizontalSignals);
        connect(this, &MainWindow::HorizontalSignals, phoneWidget, &PhoneInstanceWidget::do_HorizontalSignals);
        connect(phoneWidget, &PhoneInstanceWidget::SharkSignals, this, &MainWindow::SharkSignals);
        connect(this, &MainWindow::SharkSignals, phoneWidget, &PhoneInstanceWidget::do_SharkSignals);
        connect(phoneWidget, &PhoneInstanceWidget::GPSSignals, this, &MainWindow::GPSSignals);
        connect(this, &MainWindow::GPSSignals, phoneWidget, &PhoneInstanceWidget::do_GPSSignals);

        connect(phoneWidget, &PhoneInstanceWidget::closePhoneInstanceWidgetSignals, this, &MainWindow::closePhoneInstanceWidgetSignals);
        connect(this, &MainWindow::closePhoneInstanceWidgetSignals, phoneWidget, &PhoneInstanceWidget::do_closePhoneInstanceWidgetSignals);

        if (iter->iId != sPhoneInfo.iId)
        {            
            item = new QListWidgetItem(m_SyncOperListWidget);
            item->setSizeHint(QSize(iwidth, height));
            item->setData(Qt::UserRole, QVariant::fromValue(*iter));
            m_SyncOperListWidget->setItemWidget(item, phoneWidget);
        }
        else
        {
            m_MainPhoneInstanceWidget = phoneWidget;
            connect(m_MainPhoneInstanceWidget, &PhoneInstanceWidget::closePhoneInstanceWidgetSignals, this, [this]() {                
                delete m_MainPhoneInstanceWidget;
                m_MainPhoneInstanceWidget = NULL;
                });
            m_MainPhoneInstanceWidget->setModal(false);
            m_MainPhoneInstanceWidget->show();
        }
    }

    //m_SyncOperListWidget->move(0, 0);
    //m_SyncOperListWidget->resize(500,500);
    m_SyncOperListWidget->setVisible(false);

}

void MainWindow::on_btnAddPhone_clicked()
{
    on_toolBtnBuy_clicked();
}



void MainWindow::showEvent(QShowEvent *event)
{
    QRect rect = ui->groupWidget->geometry();
    m_toolBtnContraction->move(rect.x() + rect.width() + ui->frame->width() + 5, (rect.height() - m_toolBtnContraction->height()) / 2);
    m_toolBtnExpansion->move(rect.x() + ui->frame->width() + 5, (rect.height() - m_toolBtnContraction->height()) / 2);
}

void MainWindow::on_btnVipServerPolicy_clicked()
{
    //VIP服务协议
    PolicyDialog* policy = new PolicyDialog("会员服务协议","https://www.ysyos.com/deal/VipPrivacy.html");
    //policy->show();
    policy->exec();
}


void MainWindow::on_btnRefreshQrCode_clicked()
{
    //刷新支付
    HttpCreateOrder(4, m_curLevelDataInfo.iMemberId, m_iBuyNum, 1, m_strPayRelateId);
}

void MainWindow::on_btnAddActiveCode_clicked()
{
    ui->btnAddActiveCode->setStyleSheet("QPushButton {border:none;color: #407AFF;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:bold;font-size:16px;}");
    ui->labelAddActiveCode->setStyleSheet("background-color:#407AFF;max-height:4px;max-width:64px;min-height:4px;min-width:64px;border:none;");
    ui->btnRenewActiveCode->setStyleSheet("QPushButton:hover{border:none;color: #407AFF;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:bold;font-size:16px;}QPushButton{color: #A9ADB6;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:0;font-size:16px;}");
    ui->labelRenewActiveCode->setStyleSheet("background-color:#FFFFFF;max-height:4px;max-width:64px;min-height:4px;min-width:64px;border:none;");

    ui->stackedWidgetActiveCode->setCurrentWidget(ui->pageActive);
}


void MainWindow::on_btnRenewActiveCode_clicked()
{
    ui->btnRenewActiveCode->setStyleSheet("QPushButton {border:none;color: #407AFF;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:bold;font-size:16px;}");
    ui->labelRenewActiveCode->setStyleSheet("background-color:#407AFF;max-height:4px;max-width:64px;min-height:4px;min-width:64px;border:none;");
    ui->btnAddActiveCode->setStyleSheet("QPushButton:hover{border:none;color: #407AFF;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:bold;font-size:16px;}QPushButton{color: #A9ADB6;background-color:#FFFFFFFF;border-radius:1px;padding-left:0px;font-weight:0;font-size:16px;}");
    ui->labelAddActiveCode->setStyleSheet("background-color:#FFFFFF;max-height:4px;max-width:64px;min-height:4px;min-width:64px;border:none;");

    ui->stackedWidgetActiveCode->setCurrentWidget(ui->pageRenewActive);
}


void MainWindow::on_toolBtnClearList_clicked()
{
    ui->listWidgetActiveCode->clear();
}


void MainWindow::on_toolBtnAddActiveCode_clicked()
{
    AddActiveCodeDialog* dialog = new AddActiveCodeDialog();
    connect(dialog, &AddActiveCodeDialog::addActiveCodeSignals, this, [this](QStringList strActiveCodeList)
        {
            int iCount = strActiveCodeList.size();
            if (iCount > 0)
            {
                ActiveCodeItem* widget = NULL;
                QListWidgetItem* item = NULL;
                for(int i = 0 ; i < iCount;i++)
                {
                    item = new QListWidgetItem(ui->listWidgetActiveCode);
                    item->setSizeHint(QSize(RENEW_ITEM_WIDTH, 30));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果

                    widget = new ActiveCodeItem(strActiveCodeList.at(i),"",this);
                    connect(this, &MainWindow::activeCodeStatusSignals, widget, &ActiveCodeItem::do_activeCodeStatusSignals);
                    ui->listWidgetActiveCode->addItem(item);
                    ui->listWidgetActiveCode->setItemWidget(item, widget);
                }
            }
        });
    dialog->setModal(true);
    dialog->exec();
}

void MainWindow::on_btnActiveCode_clicked()
{
    /*QString strActiveCode = ui->lineEditActiveCode->text();
    if(strActiveCode.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("激活码不能为空!",this);
        tips->show();
        return;
    }
    qDebug()<<"点击激活" << strActiveCode;*/

    //激活码
    int iCount = ui->listWidgetActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeItem* widget = NULL;
    QString strActiceCode="";
    QString strPostActiveCode="";
    S_PHONE_INFO phoneInfo;
    QString strRelateId = "";
    QMap<int, S_ACTIVE_CODE_INFO> mapActiveCode;
    //QStringList strActiveCodeList;
   // strActiveCodeList.clear();
    S_ACTIVE_CODE_INFO activeInfo;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetActiveCode->item(i);
        if (item != NULL)
        {
            widget = static_cast<ActiveCodeItem*>(ui->listWidgetActiveCode->itemWidget(item));
            strActiceCode = widget->getActiveCode();
            if (!strActiceCode.isEmpty())
            {
                activeInfo.iRelateId = 0;
                activeInfo.strRenewActiveCode = strActiceCode;
                mapActiveCode.insert(i, activeInfo);
                //strActiveCodeList << strActiceCode;
            }
        }
    }

    if (mapActiveCode.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("激活码列表不能为空!", this);
        tips->show();
        return;
    }
    //qDebug() <<"strPostActiveCode=" << strPostActiveCode;

    //relateId
    //HttpPostActivateCode(strActiveCodeList, 1);
    HttpPostActivateCode(mapActiveCode);
}

void MainWindow::on_btnGroupRefresh_clicked()
{
    //重新加载列表
    m_mapPhoneInfo.clear();
    HttpQueryAllGroup();
}

void MainWindow::checkAllChildren(QTreeWidgetItem* parent, bool checked)
{
    Qt::CheckState state = checked? Qt::Checked : Qt::Unchecked;
    for (int i = 0; i < parent->childCount(); ++i) 
    {
        QTreeWidgetItem* child = parent->child(i);
        child->setCheckState(0, state);  // 设置子节点为选中状态
        checkAllChildren(child, state);              // 递归遍历子节点的子节点
    }
}

void MainWindow::on_checkBoxGroup_clicked(bool checked)
{
    QTreeWidgetItem* parent = ui->treeWidget->invisibleRootItem();
    checkAllChildren(parent, checked);

    QStringList strList;
    strList.clear();
    m_iCheckCount = 0;
    ui->listWidget->clear();
    ui->listWidget2->clear();
    int iSelCount = 0;
    QMap<int, S_LEVEL_INFO>::iterator iterFind;
    if (checked)
    {
        //获取所有选中项的迭代器
        QTreeWidgetItemIterator it(ui->treeWidget);
        //遍历所有选中项的迭代器
        QTreeWidgetItem* item = NULL;
        S_PHONE_INFO phoneInfo;
        PhoneItemWidget* widget=NULL;
        QListWidgetItem* phoneItem = NULL;
        PhoneListModeItemWidget* widget2 = NULL;
        while (*it)
        {
            item = *it;
            qDebug() << item->text(0);
            //获取组的所有子节点
            int count = item->childCount();
            QTreeWidgetItem* child = NULL;
            for (int i = 0; i < count; ++i)
            {
                child = item->child(i);
                if (child == NULL)
                    continue;
                iSelCount++;
                phoneInfo = child->data(0, Qt::UserRole).value<S_PHONE_INFO>();   
                phoneInfo.bChecked = checked;
                //重新显示listWidget
                if (m_isIconMode)
                {
                    ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageIconMode);
                    strList << phoneInfo.strInstanceNo;
                    widget = new PhoneItemWidget(phoneInfo, this);                    
                    connect(widget, &PhoneItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
                    connect(widget, &PhoneItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
                    phoneItem = new QListWidgetItem(ui->listWidget);
                    phoneItem->setSizeHint(QSize(GlobalData::iPhoneItemWidth, GlobalData::iPhoneItemHeight));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
                    phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                    ui->listWidget->addItem(phoneItem);
                    ui->listWidget->setItemWidget(phoneItem, widget);
                }
                else
                {
                    ui->stackedWidgetPhoneItem->setCurrentWidget(ui->pageListMode);
                    //listWidget2
                    //获取对应level name
                    iterFind = m_mapLevelList.find(phoneInfo.iLevel);
                    if (iterFind != m_mapLevelList.end())
                    {
                        phoneInfo.strLevelName = iterFind->strLevelName;
                    }
                    widget2 = new PhoneListModeItemWidget(phoneInfo, this);
                    connect(widget2, &PhoneListModeItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
                    connect(widget2, &PhoneListModeItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
                    phoneItem = new QListWidgetItem(ui->listWidget2);
                    phoneItem->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
                    phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
                    ui->listWidget2->addItem(phoneItem);
                    ui->listWidget2->setItemWidget(phoneItem, widget2);
                }               
            }
                           
            ++it;
        }
        if (iSelCount <= 0)
        {
            m_TaskTimer->stop();
            ui->stackedWidgetPhoneItem->setCurrentWidget(m_isIconMode?ui->pageIconNoData: ui->pageListNoData);
            return;
        }

    }
    else
    {
        ui->stackedWidgetPhoneItem->setCurrentWidget(m_isIconMode ? ui->pageIconNoData : ui->pageListNoData);
    }

    int iCount = 0;
    if (m_isIconMode)
        iCount = ui->listWidget->count();
    else
        iCount = ui->listWidget2->count();

    ui->checkBoxAllSelect->setText(QString("全选(%1/%2)").arg(iSelCount).arg(iCount));
    ui->checkBoxAllSelect->setChecked((iSelCount == iCount&&iCount !=0) ? true : false);

    //生成截图
    m_listInstanceNo = strList;
    if (strList.size() > 0)
    {
        m_TaskTimer->start(TIMER_INTERVAL);
        this->m_toolObject->HttpPostInstanceScreenshotRefresh(strList);
    }
    else
    {
        if (m_TaskTimer->isActive())
        {
            m_TaskTimer->stop();
        }
    }
}

void MainWindow::AddIconModeListWidgetItem(S_PHONE_INFO phoneInfo)
{
    QListWidgetItem* phoneItem = NULL;
    PhoneItemWidget* widget = new PhoneItemWidget(phoneInfo, this);
    connect(widget, &PhoneItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
    connect(widget, &PhoneItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
    phoneItem = new QListWidgetItem(ui->listWidget);
    phoneItem->setSizeHint(QSize(GlobalData::iPhoneItemWidth, GlobalData::iPhoneItemHeight));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
    phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
    ui->listWidget->addItem(phoneItem);
    ui->listWidget->setItemWidget(phoneItem, widget);
}

void MainWindow::AddListModeListWidgetItem(S_PHONE_INFO phoneInfo)
{
    QListWidgetItem* phoneItem = NULL;
    PhoneListModeItemWidget* widget2 = NULL;

    //获取对应level name
    QMap<int, S_LEVEL_INFO>::iterator iterFind;
    iterFind = m_mapLevelList.find(phoneInfo.iLevel);
    if (iterFind != m_mapLevelList.end())
    {
        phoneInfo.strLevelName = iterFind->strLevelName;
    }
    widget2 = new PhoneListModeItemWidget(phoneInfo, this);
    connect(widget2, &PhoneListModeItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
    connect(widget2, &PhoneListModeItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
    phoneItem = new QListWidgetItem(ui->listWidget2);
    phoneItem->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
    phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
    ui->listWidget2->addItem(phoneItem);
    ui->listWidget2->setItemWidget(phoneItem, widget2);
}

void MainWindow::BianliTreeWidgetSelectItem(QTreeWidgetItem* currentItem)
{
    QTreeWidgetItemIterator iter(ui->treeWidget);
    QTreeWidgetItem* item = NULL;
    QTreeWidgetItem* child = NULL;
    S_PHONE_INFO phoneInfo;
    int iChildCount = 0;
    int i = 0;
    Qt::CheckState checkState;
    while (*iter)
    {
        qDebug() << (*iter)->text(0);
        iChildCount = (*iter)->childCount();
        
        item = *iter;
        //显示当前选中项的所有子项
        if (item->isSelected())
        {
            for (i = 0; i < iChildCount; ++i)
            {
                child = item->child(i);
                checkState = child->checkState(0);
                phoneInfo = child->data(0, Qt::UserRole).value<S_PHONE_INFO>();
                if (checkState == Qt::Checked)
                {
                    phoneInfo.bChecked = true;
                    m_iCheckCount++;
                }
                if (m_isIconMode)
                {
                    m_listInstanceNo << phoneInfo.strInstanceNo;
                    AddIconModeListWidgetItem(phoneInfo);
                }
                else
                {
                    AddListModeListWidgetItem(phoneInfo);
                }
            }
            iter++;
            continue;
        }
        qDebug() << item->text(0);
        //获取组的所有子节点
        for (i = 0; i < iChildCount; ++i)
        {
            child = item->child(i);
            checkState = child->checkState(0);
            if (checkState == Qt::Checked || child->isSelected())
            {            
                phoneInfo = child->data(0, Qt::UserRole).value<S_PHONE_INFO>();                
                if (checkState == Qt::Checked)
                {
                    phoneInfo.bChecked = true;
                    m_iCheckCount++;
                }
                if (m_isIconMode)
                {
                    m_listInstanceNo << phoneInfo.strInstanceNo;
                    AddIconModeListWidgetItem(phoneInfo);
                }
                else
                {
                    AddListModeListWidgetItem(phoneInfo);
                }
            }            
        }

        iter++;
    }

    int iCount = 0;
    if (m_isIconMode)
    {
        iCount = ui->listWidget->count();
        ui->stackedWidgetPhoneItem->setCurrentWidget(0 == iCount?ui->pageIconNoData: ui->pageIconMode);
    }        
    else
    {
        iCount = ui->listWidget2->count();
        ui->stackedWidgetPhoneItem->setCurrentWidget(0 == iCount ? ui->pageListNoData : ui->pageListMode);
    }
    ui->checkBoxAllSelect->setText(QString("全选(%1/%2)").arg(m_iCheckCount).arg(iCount));
    ui->checkBoxAllSelect->setChecked((m_iCheckCount == iCount&&iCount != 0) ? true : false);
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //点击复选框响应事件
    //qDebug()<<"点击复选框响应事件";
    if (item->parent() == NULL)
    {
        Qt::CheckState check = item->checkState(0);
        int count = item->childCount();
        QTreeWidgetItem* child = NULL;
        for (int i = 0; i < count; ++i)
        {
            child = item->child(i);
            if (child != NULL)
            {
                child->setCheckState(0, check);
            }
        }
    }

    on_treeWidget_itemPressed(item, column);
}

void MainWindow::do_stateChanged(int state)
{
    switch (state)
    {
    case Qt::Checked:
            m_iCheckCount++;
        break;
    case Qt::Unchecked:
        if(m_iCheckCount > 0)
            m_iCheckCount--;
        break;
    default:
        break;
    }
    int iCount = 0;
    if (m_isIconMode)
        iCount = ui->listWidget->count();
    else
        iCount = ui->listWidget2->count();
    
    ui->checkBoxAllSelect->setText(QString("全选(%1/%2)").arg(m_iCheckCount).arg(iCount));
    ui->checkBoxAllSelect->setChecked((m_iCheckCount == iCount&&iCount!=0) ? true : false);
}

void MainWindow::on_btnAddPhone_2_clicked()
{
    on_toolBtnBuy_clicked();
}

void MainWindow::do_hideIndividualCenterWidgetSignals()
{
    if (m_individualCenterWidget && m_individualCenterWidget->isVisible())
    {
        GlobalData::bHideIndividualCenterWidget = false;
        m_individualCenterWidget->hide();
    }
}


void MainWindow::on_toolBtnClearRenewList_clicked()
{
    ui->listWidgetRenewActiveCode->clear();
}


void MainWindow::on_toolBtnAddRenewActiveCode_clicked()
{
    if (ui->listWidgetRenewActiveCode->count() <= 0)
    {
        MessageTips* tips = new MessageTips("暂无可续费设备", this);
        tips->show();
        return;
    }

    //是否有勾选项
    bool bSelectItem = false;
    int iCount = ui->listWidgetRenewActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewActiveCode->item(i);
        if (item != NULL)
        {
            widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
            if (widget != NULL)
            {
                if(widget->getCheckBoxStatus())
                {
                    bSelectItem = true;
                    break;
                }
            }
        }
    }

    if(!bSelectItem)
    {
        MessageTips* tips = new MessageTips("请先勾选需续费的设备", this);
        tips->show();
        return;
    }

    AddActiveCodeDialog* dialog = new AddActiveCodeDialog();
    connect(dialog, &AddActiveCodeDialog::addActiveCodeSignals, this, [this](QStringList strActiveCodeList)
            {
                int iCount = strActiveCodeList.size();
                if (iCount > 0)
                {
                    ActiveCodeRenewItem* widget = NULL;
                    QListWidgetItem* item = NULL;
                    int iCodeIndex = 0;
                    int iListCount = ui->listWidgetRenewActiveCode->count();
                    int iSelCount = 0;
                    for(int i = 0 ; i < iListCount;i++)
                    {
                        item = ui->listWidgetRenewActiveCode->item(i);
                        if (item != NULL)
                        {
                            widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
                            if(widget != NULL && widget->getCheckBoxStatus())
                            {
                                iSelCount++;
                                if(iCodeIndex < iCount)
                                {
                                    widget->setRenewActiveCode(strActiveCodeList.at(iCodeIndex));
                                    iCodeIndex++;
                                }
                            }
                        }
                    }
                    if(iSelCount > iCount)
                    {
                        MessageTipsDialog* dialog = new MessageTipsDialog("添加的激活码少于续时设备的总数");
                        dialog->show();
                    }
                    else if(iSelCount < iCount)
                    {
                        MessageTipsDialog* dialog = new MessageTipsDialog("添加的激活码超过续时设备的总数,超出的部分不会添加到列表中.");
                        dialog->show();
                    }
                }
            });
    dialog->setModal(true);
    dialog->exec();
}


void MainWindow::on_btnOkRenewActiveCode_clicked()
{
    //激活码续费,选中才能续费
    int iCount = ui->listWidgetRenewActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    bool bChecked = false;
    S_PHONE_INFO phoneInfo;
    
    //激活码与设备匹配项
    QMap<int, S_ACTIVE_CODE_INFO> mapDeviceToActiveCode;
    S_ACTIVE_CODE_INFO activeCodeInfo;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewActiveCode->item(i);
        if (item == NULL)
            continue;
        widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
        if (widget == NULL)
            continue;
        bChecked = widget->getCheckBoxStatus();
        if (bChecked)
        {
            phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
            //过滤空的激活码
            if (!widget->getRenewActiveCode().isEmpty())
            {
                activeCodeInfo.iRelateId = phoneInfo.iId;
                activeCodeInfo.strRenewActiveCode = widget->getRenewActiveCode();
                mapDeviceToActiveCode.insert(phoneInfo.iId, activeCodeInfo);
            }
        }
    }

    if (mapDeviceToActiveCode.size() <= 0)
    {
        MessageTips* tips = new MessageTips("请先填写续费激活码", this);
        tips->show();
        return;
    }

    HttpPostActivateCode(mapDeviceToActiveCode);
}

void MainWindow::DeleteActiveItemByStatus(ENUM_ACTIVE_CODE_STATUS enType)
{
    int iCount = ui->listWidgetRenewActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    //删除成功项
    switch (enType)
    {
    case TYPE_SUCCESS:
    {
        for (int i = iCount - 1; i >= 0; i--)
        {
            item = ui->listWidgetRenewActiveCode->item(i);
            if (item == NULL)
                continue;
            widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
            if (widget != NULL && (widget->getStatus() == TYPE_SUCCESS))
            {
                ui->listWidgetRenewActiveCode->takeItem(i);
            }
        }
    }
    break;
    case TYPE_FAILED:
    {
        for (int i = iCount - 1; i >= 0; i--)
        {
            item = ui->listWidgetRenewActiveCode->item(i);
            if (item == NULL)
                continue;
            widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
            if (widget != NULL && (widget->getStatus() == TYPE_FAILED))
            {
                widget->clearStatus();
                //ui->listWidgetRenewActiveCode->takeItem(i);
            }
        }
    }
        break;
    default:
        break;
    } 
}
void MainWindow::on_toolBtnRemoveSuccessItem_clicked()
{
    //删除成功项
    DeleteActiveItemByStatus(TYPE_SUCCESS);
}


void MainWindow::on_toolBtnRemoveFailedItem_clicked()
{
    //更改失败项状态
    DeleteActiveItemByStatus(TYPE_FAILED);
}


void MainWindow::on_checkBoxActiveCodeRenew_clicked(bool checked)
{
    //选中状态
    int iCount = ui->listWidgetRenewActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewActiveCode->item(i);
        if (item != NULL)
        {
            widget = static_cast<ActiveCodeRenewItem*>(ui->listWidgetRenewActiveCode->itemWidget(item));
            if (widget != NULL)
            {
                widget->setCheckBoxStatus(checked);
            }
        }
    }
}

void MainWindow::on_toolBtnLevelVIP_clicked()
{
    //默认显示第一个
    CustomToolButton* toolBtn = qobject_cast<CustomToolButton*>(sender());
    if(toolBtn == NULL)
        return;
    int iLayoutCount = m_hBoxLevelTypeToolBtn->count();
    QWidget* widget = NULL;
    QToolButton* tBtn = NULL;
    for (int i = 0; i < iLayoutCount; i++)
    {
        widget = m_hBoxLevelTypeToolBtn->itemAt(i)->widget();
        if (widget== NULL)
        {
            continue;
        }
        tBtn = qobject_cast<QToolButton*>(widget);
        if (tBtn == toolBtn)
        {
            tBtn->setStyleSheet("QToolButton{background-color:#E6E9F2;color:#505465;border-top-left-radius:2px;border-top-right-radius:2px;padding-left:0px;font-size:14px;}");
        }
        else
        {
            tBtn->setStyleSheet("QToolButton {border:none;background-color:#F4F6FA;color:#A9ADB6;border-top-left-radius:2px;border-top-right-radius:2px;padding-left:0px;font-size:14px;}QToolButton:hover{background-color:#E6E9F2;color:#505465;border-top-left-radius:2px;border-top-right-radius:2px;padding-left:0px;font-size:14px;}");
        }
    }
    
    S_LEVEL_INFO levelInfo = toolBtn->data(Qt::UserRole).value<S_LEVEL_INFO>();
    m_ActiveRenewLevelType = levelInfo.iLevelId;

    ui->listWidgetRenewActiveCode->clear();
    //加载数据
    qDebug() << "select level id=" << levelInfo.iLevelId;
    //初始化续费列表listWidgetRenewList
    //加载数据并显示
    int iCount = m_mapPhoneInfo.size();
    if (iCount > 0)
    {
        //初始化续费列表
        QListWidgetItem* item = NULL;
        ActiveCodeRenewItem* widget = NULL;
        QMap<int, S_PHONE_INFO>::iterator iter = m_mapPhoneInfo.begin();
        for (; iter != m_mapPhoneInfo.end(); iter++)
        {
            if (iter->iLevel == levelInfo.iLevelId)
            {
                item = new QListWidgetItem(ui->listWidgetRenewActiveCode);
                item->setData(Qt::UserRole, QVariant::fromValue(*iter));
                item->setSizeHint(QSize(RENEW_ITEM_WIDTH, 30));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
                widget = new ActiveCodeRenewItem(*iter, this);
                widget->setCheckBoxStatus(ui->checkBoxActiveCodeRenew->isChecked());
                connect(this, &MainWindow::activeCodeStatusSignals, widget, &ActiveCodeRenewItem::do_activeCodeStatusSignals);
                connect(widget, &ActiveCodeRenewItem::deleteActiveItemSignals, this, &MainWindow::do_deleteActiveItemSignals);
                ui->listWidgetRenewActiveCode->addItem(item);
                ui->listWidgetRenewActiveCode->setItemWidget(item, widget);
            }
        }
    }
}

void MainWindow::do_deleteActiveItemSignals(S_PHONE_INFO phoneInfo)
{
    int iCount = ui->listWidgetRenewActiveCode->count();
    QListWidgetItem* item = NULL;
    ActiveCodeRenewItem* widget = NULL;
    S_PHONE_INFO pi;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetRenewActiveCode->item(i);
        if (item != NULL)
        {
            pi = item->data(Qt::UserRole).value<S_PHONE_INFO>();
            if (pi.iId = phoneInfo.iId)
            {
                ui->listWidgetRenewActiveCode->takeItem(i);
                break;
            }
        }
    }
}
void MainWindow::on_btnRefreshRenewList_clicked()
{
    if (m_ActiveRenewLevelType <= 0)
        return;
    ui->listWidgetRenewActiveCode->clear();
    //刷新续费列表
    qDebug() << "查询手机列表 m_ActiveRenewLevelType="<< m_ActiveRenewLevelType;
    HttpGetMyPhoneInstance(0, 1, 1000, m_ActiveRenewLevelType);
}

