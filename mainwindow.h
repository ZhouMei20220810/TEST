#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include <QMap>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QMouseEvent>
#include "levelnormalwidget.h"
#include "levelenhancementwidget.h"
#include "levelpremierwidget.h"
#include <QMenu>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //分组接口
    void HttpQueryAllGroup();//查询全部分组
    void HttpCreateGroup(QString strGroupName);//创建分组
    void HttpUpdateGroup(int iGroupId, QString strNewName);//修改分组
    void HttpDeleteGroup(int iGroupId);//删除分组

    //显示树内容
    void ShowGroupInfo();

    //会员级别接口
    void HttpMemberLevelList();
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
    //获取serverToken
    //void HttpGetServerToken();
    //获取我的手机实例
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel = 0);
    //获取我的实例级别
    void HttpGetMyInstanceLevel();
    //生成QrCode
    void gernerateQRCode(const QString& text, QPixmap& qrPixmap, int scale);

    //云手机
    void InitCloudPhoneTab();
    //激活码
    void InitActiveCodeTab();
    //购买
    void InitBuyTab();
    //初始化列列表
    void InitPhoneList();
    void InitLevelList();
    void InitVipList();
    void InitVipRenewList();

    //初始化vip列表
    void loadVipType(S_LEVEL_INFO levelInfo);
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

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    //组菜单
    void do_DeleteGroupAction(bool bChecked);
    void do_EditGroupNameAction(bool bChecked);
    void do_createGroupSignals(ENUM_CREATE_OR_UPDATA type, QString strGroupName);
private:
    Ui::MainWindow *ui;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;

    S_LEVEL_DATA_INFO m_curLevelDataInfo;

    QPoint m_dragPosition;
    // QWidget interface

    LevelNormalWidget* normal;
    LevelEnhancementWidget* enhancement;
    LevelPremierWidget* premier;

    //会员列表
    QMap<LEVEL_TYPE,QMap<int,S_LEVEL_DATA_INFO>> m_mapLevel;

    //组列表
    QMenu* m_menu;
    QMenu* m_PhoneMenu;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
