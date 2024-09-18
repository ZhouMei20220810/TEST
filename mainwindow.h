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
#include "phoneinstancewidget.h"
#include "creategroupwidget.h"
#include "systemsettingwidget.h"
#include "individualcenterwidget.h"
#include <QSystemTrayIcon>
#include <QToolButton>
#include <QQuickWidget>
#include "imagedownloader.h"

namespace Ui {
class MainWindow;
}
enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void logoutSignals();

    void returnSignals();
    void homeSignals();
    void changePageSignals();
    void paifaTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
    void RootSignals();
    void VolumeUpSignals();
    void VolumeDownSignals();
    void HorizontalSignals(bool bIsVertical);
    void SharkSignals();
    void GPSSignals();
    //同步到其他设备
    void BatchDirectCopyToPhoneSignals(QString strTextList);

    void closePhoneInstanceWidgetSignals();    
    //同步模式剪切板分发
    void DirectCopyToPhoneSignals(QString strSelectText);
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

    virtual void closeEvent(QCloseEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;
	virtual void resizeEvent(QResizeEvent *event) override;
private:
    //分组接口
    void HttpQueryAllGroup();//查询全部分组  
    //显示树内容
    void ShowGroupInfo();
    //实例列表
    void ShowPhoneInfo(int iGroupId, QMap<int, S_PHONE_INFO> mapPhoneInfo);
    //显示任务
    void ShowTaskInfo();
    //激活码续费列表
    void ShowActiveCodeItemInfo(int iLevelId, QMap<int, S_PHONE_INFO> mapPhoneInfo);

    //会员相关接口
    void HttpMemberLevelListData();

    
    //手机实例相关接口    
    //获取serverToken
    //void HttpGetServerToken();
    //获取我的手机实例
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel = 0);

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
    
    void calcNeedPayMoney();

    void startDownload(QString strUrl);

    void AddListModeListWidgetItem(S_PHONE_INFO phoneInfo);
    //获取所有选中的项
    void BianliTreeWidgetSelectItem();

    void DeleteActiveItemByStatus(ENUM_ACTIVE_CODE_STATUS enType);
    QMap<int, S_PHONE_INFO> getCurrentAllSelectItem(EN_RIGHT_CLICK_TYPE enType);
private slots:
    //工具栏
    void on_toolBtnCloudPhone_clicked();
    void on_toolBtnActiveCode_clicked();
    void on_toolBtnBuy_clicked();    

    //菜单栏
    void on_btnPhotoUrl_clicked();
    void on_toolBtnSetting_clicked();
    void on_toolBtnCustomer_clicked();
    void on_btnMin_clicked();
    void on_btnMax_clicked();
    void on_btnClose_clicked();    

    void on_btnCreateGroup_clicked();
    void on_btnGroupRefresh_clicked();
    void on_checkBoxGroup_clicked(bool checked);
    void checkAllChildren(QTreeWidgetItem* parent,bool checked);

    void on_btnActiveCode_clicked();
    void on_btnAddActiveCode_clicked();
    void on_btnRenewActiveCode_clicked();
	void on_toolBtnClearList_clicked();
    void on_toolBtnAddActiveCode_clicked();

    void on_toolBtnBuyPhone_clicked();

    void on_toolBtnRenewPhone_clicked();

    void on_btnBeginPay_clicked();

    //vip item
    void do_selectVIPTypeSignals(S_LEVEL_DATA_INFO levelInfo);
    //refresh vip Item
    void do_refreshMemberListSignals(int iLevelId,QMap<int, S_LEVEL_DATA_INFO> mapData);

    void on_toolBtnSub_clicked();
    void on_toolBtnAdd_clicked();
    void on_lineEditBuyNumber_textChanged(const QString &arg1);

	//显示/隐藏分组
    void do_ContractionOrExpansion(bool bChecked);
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

    //组菜单
    void do_DeleteGroupAction(bool bChecked);
    void do_EditGroupNameAction(bool bChecked);
    void do_createGroupSignals(ENUM_CREATE_OR_UPDATA type, QString strGroupName, int id);
    //手机菜单
    void do_ActionBeginControl(bool bChecked);
    void do_ActionCopyCloudId(bool bChecked);
    void do_ActionRename(bool bChecked);
    void do_ActionRestartCloudPhone(bool bChecked);
    void do_ActonOneClickNewMachine(bool bChecked);
    void do_ActionFactoryDataReset(bool bChecked);
    void do_ActionUploadFile(bool bChecked);
    void do_ActionMoveGroup(bool bChecked);
    void do_ActionBatchMoveGroup(bool bChecked);
    void do_ActionRenewCloudPhone(bool bChecked);
    void do_ActionAuthorization(bool bChecked);
    void do_ActionReplaceCloudPhone(bool bChecked);
    void do_ActionTransferCloudPhone(bool bChecked);
    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    //一分钟响应一次
    void do_timeoutRefreshPicture();

    void on_checkBoxRenew_clicked(bool checked);

    void on_btnCancelSelect_clicked();

    void on_toolBtnListMode_clicked();
	void on_toolBtnPreviewMode_clicked();

    void on_checkBoxAllSelect_clicked(bool checked);
    void on_checkBoxFanSelect_clicked(bool checked);
    void on_toolBtnBatchOperation_clicked(bool checked);

    QStringList getCheckedPhoneInstance(bool IsPhoneId = false);
    void do_ActionBatchReboot(bool bChecked);
    void do_ActionBatchUploadFile(bool bChecked);
    void do_ActionBatchOneClickNewMachine(bool bChecked);
    void do_ActionBatchFactoryReset(bool bChecked);
    void do_ActionBatchAuth(bool bChecked);

    void on_comboBoxView_currentIndexChanged(int index);

    void handleTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    //同步操作
    void on_radioButtonSyncOperation_clicked(bool checked);
    //显示实例
    void on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo,bool bShowMenu);
    //显示非主控设备
    void on_ShowPhoneInstanceNotMaster(S_PHONE_INFO sPhoneInfo);
    void on_btnAddPhone_clicked();
    //统计
    void do_checkBoxClick(bool bCheck);

    void on_btnVipServerPolicy_clicked();

    void on_btnRefreshQrCode_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_btnAddPhone_2_clicked();

    void do_hideIndividualCenterWidgetSignals();

    void on_toolBtnClearRenewList_clicked();
    void on_toolBtnAddRenewActiveCode_clicked();
    void on_btnOkRenewActiveCode_clicked();
    void on_toolBtnRemoveSuccessItem_clicked();
    void on_toolBtnRemoveFailedItem_clicked();
    void on_checkBoxActiveCodeRenew_clicked(bool checked);
    void on_toolBtnLevelVIP_clicked();

    void do_deleteActiveItemSignals(S_PHONE_INFO phoneInfo);
    void on_btnRefreshRenewList_clicked();

    //转移之后，刷新所有列表数据
    void do_TransferSuccessRefreshInstanceListSignals();    

    void on_toolBtnMessageCenter_clicked();
    //重命名响应
    void do_HttpPostInstanceRenameSignals(int iId,QString strName);
    //刷新树形列表
    void do_HttpGroupRefreshSignals();
    //创建订单响应
    void do_HttpCreateOrderSignals(QString strQrCode);
    //会员级别响应
    void do_HttpLevelListSignals(QMap<int, S_LEVEL_INFO> mapLevelList);
private:
    //转移后刷新列表
    void RefreshTransferPhoneList();
    //清空同步列表
    void ClearSyncPhoneInstanceWidgetList();
    //同步列表添加widget
    void AddSyncPhoneInstanceWidget(S_PHONE_INFO info,PhoneInstanceWidget* phoneWidget=NULL);
    //同步列表删除widget
    void DeleteSyncPhoneInstanceWidget(QString strInstanceNo);

    //注册下载图片线程
    void registerDownloadImageThread();
    //注销下载图片线程
    void UnregisterDownloadImageThread();

    //通过QML加载数据
    void loadPreviewModeListByQML();
    //关闭窗口关闭所有手机示例窗口
    void closeMainWindowcloseAllPhoneInstanceWidget();

private:
    Ui::MainWindow *ui;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;
    //会员数据列表
    S_LEVEL_DATA_INFO   m_curLevelDataInfo;
    int                 m_iBuyNum;
    QString             m_strPayRelateId;
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
    QTimer* m_PayTimer;
    int     m_iPayCount = 0;

    //组列表
    QMenu* m_menu;

    QMenu* m_PhoneMenu;
    QMenu* m_SubPhoneMenu;
    S_PHONE_INFO m_CurSelMenuPhoneInfo;
    
    QMenu* m_BatchOperMenu;
    QMenu* m_BatchOperSubMenu;

    QAction* pActionBeginControl;
    QAction* pActionCopyCloudId;
    QAction* pActionRename;
    QAction* pActonOneClickNewMachine;
    QAction* pActionRestartCloudPhone;
    QAction* pActionFactoryDataReset;
    QAction* pActionUploadFile;
    //QAction* pActionMoveGroup;
    QAction* pActionRenewCloudPhone;
    QAction* pActionAuthorization;
    QAction* pActionReplaceCloudPhone;
    QAction* pActionTransferCloudPhone;

    QAction* pActionBatchReboot;
    QAction* pActionBatchUploadFile;
    QAction* pActionBatchOneClickNewMachine;
    QAction* pActionBatchFactoryReset;
    QAction* pActionBatchAuth;//批量授权

    QTreeWidgetItem* m_pCurItem;

    PhoneInstanceWidget* m_MainPhoneInstanceWidget;
    CreateGroupWidget* m_createGroupWidget;
    SystemSettingWidget* m_systemSettingWidget;
    IndividualCenterWidget* m_individualCenterWidget;

    QPoint  m_LastPos;
    bool m_bMoving= false;

    //同步操作
    QListWidget* m_SyncOperListWidget;

    QToolButton* m_toolBtnContraction;
    QToolButton* m_toolBtnExpansion;
    bool         m_IsContraction;

    //拉伸
    //QPoint clickPos;
    bool isLeftPressDown = false;
    QPoint dragPosition;
    Direction dir = NONE;
    void CalculateBorderIndex(QMouseEvent* ev);
    bool m_bCanResize = false;
    bool m_bResizeIng = false;
    QSize  m_oldSize;
    QPoint m_globalPoint;
    //bool m_IsHide = false;

    bool m_isIconMode;
    int  m_iCheckCount;

    QHBoxLayout* m_hBoxLevelTypeToolBtn; //激活码续费level布局
    int          m_ActiveRenewLevelType;

    //Phone instanceNo - 手机实例窗口
    QMap<QString, PhoneInstanceWidget*> m_mapWindows;//非同步模式，所有打开的窗口
    //Phone instanceNo - 同步手机实例窗口
    QMap<QString, PhoneInstanceWidget*> m_mapSyncWindows;//记录同步模式，非主控设置

    //QML接口
    QQuickWidget* m_quickWidget;

    //当前树上，所有选中的item
    QMap<int, S_PHONE_INFO> m_mapCurTreeItemSelect;

    QNetworkAccessManager m_manager;
    ImageDownloader* m_downloader;
    QThread* m_thread;
};

#endif // MAINWINDOW_H
