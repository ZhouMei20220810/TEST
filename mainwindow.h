#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include <QMap>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "levelitemwidget.h"
#include <QMenu>
#include "toolobject.h"
#include "phoneinstancewidget.h"
#include "creategroupwidget.h"
#include "systemsettingwidget.h"
#include <QSystemTrayIcon>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    virtual void closeEvent(QCloseEvent *event) override;
private:
    //分组接口
    void HttpQueryAllGroup();//查询全部分组
    void HttpCreateGroup(QString strGroupName);//创建分组
    void HttpUpdateGroup(int iGroupId, QString strNewName);//修改分组
    void HttpDeleteGroup(int iGroupId);//删除分组

    //显示树内容
    void ShowGroupInfo();
    //实例列表
    void ShowPhoneInfo(int iGroupId, QMap<int, S_PHONE_INFO> m_mapPhoneInfo);
    //显示任务
    void ShowTaskInfo();

    //会员级别接口
    void HttpLevelList();
    //会员相关接口
    void HttpMemberLevelListData();

    //创建订单
    void HttpCreateOrder(int iChannel, int iMemberId, int iNum,int iPayType, QString strRelateId);
    //关闭订单
    void HttpCloseOrder(QString strOutTradeNo);
    //激活码接口
    void HttpPostActivateCode(QString strCode, int iRelateId);

    //注销
    void HttpLogout();
    
    //手机实例相关接口
    //实例重命名
    void HttpPostInstanceRename(int iId, QString strName);
    //设置实例分组
    void HttpPostInstanceSetGroup(int iGroupId, QStringList strList);
    //获取serverToken
    //void HttpGetServerToken();
    //获取我的手机实例
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel = 0);
    //获取我的实例级别
    void HttpGetMyInstanceLevel(int iPhoneId);
    //生成QrCode
    void gernerateQRCode(const QString& text, QPixmap& qrPixmap, int scale);

    //云手机
    void InitCloudPhoneTab();
    void InitPhoneList();
    void InitGroupMenu();
    void InitPhoneMenu();
    void InitBatchOperatorMenu();
    void InitComboBox();
    //激活码
    void InitActiveCodeTab();
    void InitActiveCodeRenewList();
    //购买
    void InitBuyTab();
    void InitLevelList();
    void InitVipList();
    void InitVipRenewList();
    

    //初始化vip列表
    void loadVipType(S_LEVEL_INFO levelInfo);

    void startDownload(QString strUrl);
private slots:
    //工具栏
    void on_toolBtnCloudPhone_clicked();
    void on_toolBtnActiveCode_clicked();
    void on_toolBtnBuy_clicked();    

    //菜单栏
    void on_btnPhotoUrl_clicked();
    void on_btnCustomerService_clicked();
    void on_btnSetting_clicked();
    void on_btnMin_clicked();
    void on_btnMax_clicked();
    void on_btnClose_clicked();    

    void on_btnCreateGroup_clicked();
    void on_btnGroupRefresh_clicked();

    void on_btnActiveCode_clicked();

    void on_toolBtnAdd_clicked();

    void on_toolButtonRenew_clicked();

    void on_toolBtnBuyPhone_clicked();

    void on_toolBtnRenewPhone_clicked();

    void on_btnBeginPay_clicked();

    //level item 
    void do_selectLevelTypeSignals(S_LEVEL_INFO levelInfo);
    //vip item
    void do_selectVIPTypeSignals(S_LEVEL_DATA_INFO levelInfo);

    void on_btnDecrese_clicked();
    void on_btnAdd_clicked();
    void on_lineEditBuyNumber_textChanged(const QString &arg1);

	//显示/隐藏分组
    void on_btnContraction_clicked();
    void on_btnExpansion_clicked();
	//展开/收起功能
    void on_toolBtnDropFunction_clicked();
    void on_toolBtnExpansionFunction_clicked();

	//切换横屏/竖屏
    void on_toolBtnChangeHorScreen_clicked();
    void on_toolBtnChangeVerScreen_clicked();

	//支付方式
    void on_toolBtnPayZhifubao_clicked();
    void on_toolBtnPayWechat_clicked();

    void on_btnReturn_clicked();
    void on_btnFinish_clicked();

    void on_toolBtnPhoto_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    //组菜单
    void do_DeleteGroupAction(bool bChecked);
    void do_EditGroupNameAction(bool bChecked);
    void do_createGroupSignals(ENUM_CREATE_OR_UPDATA type, QString strGroupName, int id);
    //手机菜单
    void do_ActionBeginControl(bool bChecked);
    void do_ActionCopyCloudId(bool bChecked);
    void do_ActionRename(bool bChecked);
    void do_ActionRestartCloudPhone(bool bChecked);
    void do_ActionNewPhone(bool bChecked);
    void do_ActionFactoryDataReset(bool bChecked);
    void do_ActionUploadFile(bool bChecked);
    void do_ActionMoveGroup(bool bChecked);
    void do_ActionRenewCloudPhone(bool bChecked);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    //一分钟响应一次
    void do_timeoutRefreshPicture();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_checkBoxRenew_clicked(bool checked);

    void on_btnCancelSelect_clicked();

    void on_toolBtnListMode_clicked();
	void on_toolBtnPreviewMode_clicked();

    void on_checkBoxAllSelect_clicked(bool checked);
    void on_checkBoxFanSelect_clicked(bool checked);

    void on_checkBoxRenewHeader_clicked(bool checked);

    void on_toolBtnBatchOperation_clicked(bool checked);

    QStringList getCheckedPhoneInstance();
    void do_ActionBatchReboot(bool bChecked);
    void do_ActionBatchUploadFile(bool bChecked);
    void do_ActionBatchFactoryReset(bool bChecked);

    void on_toolBtnSetting_clicked();

    void on_toolBtnCustomer_clicked();

    void on_comboBoxView_currentIndexChanged(int index);

    void handleTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    //同步操作
    void on_radioButtonSyncOperation_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;
    //会员数据列表
    S_LEVEL_DATA_INFO m_curLevelDataInfo;
	//手机列表数据
    QMap<int, S_PHONE_INFO> m_mapPhoneInfo;
    QPoint m_dragPosition;
    // QWidget interface

    LevelItemWidget* levelItem;
    //int会员等级
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
    //int会员等级id,会员列表
    QMap<int,QMap<int,S_LEVEL_DATA_INFO>> m_mapLevel;
    //任务列表
    QMap<QString, S_TASK_INFO> m_mapTask;
    QTimer* m_TaskTimer;//定时任务
    QStringList m_listInstanceNo;

    QTimer* m_Timer;

    //组列表
    QMenu* m_menu;

    QMenu* m_PhoneMenu;
    QMenu* m_SubPhoneMenu;
    
    QMenu* m_BatchOperMenu;
    QMenu* m_BatchOperSubMenu;

    QAction* pActionBeginControl;
    QAction* pActionCopyCloudId;
    QAction* pActionRename;
    QAction* pActionRestartCloudPhone;
    QAction* pActionNewPhone;
    QAction* pActionFactoryDataReset;
    QAction* pActionUploadFile;
    //QAction* pActionMoveGroup;
    QAction* pActionRenewCloudPhone;

    QAction* pActionBatchReboot;
    QAction* pActionBatchUploadFile;
    QAction* pActionBatchFactoryReset;

    QTreeWidgetItem* m_pCurItem;
    ToolObject* m_toolObject;

    PhoneInstanceWidget* m_PhoneInstanceWidget;
    CreateGroupWidget* m_createGroupWidget;
    SystemSettingWidget* m_systemSettingWidget;

    QPoint  m_LastPos;
    bool m_bMoving= false;
};

#endif // MAINWINDOW_H
