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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setUserInfo(S_USER_LOGIN_INFO userInfo);

private:
    //分组接口
    void QueryAllGroup();//查询全部分组
    void CreateGroup(QString strGroupName);//创建分组
    void UpdateGroup(int iGroupId, QString strNewName);//修改分组
    void DeleteGroup(int iGroupId);//删除分组

    //显示树内容
    void ShowGroupInfo();

    //会员级别接口
    void HttpMemberLevelList();
    //会员相关接口
    void HttpMemberLevelListData();

    //订单接口
    // 我的支付订单
    void HttpGetMyOrder(int iPage, int iPageSize);
    //创建订单
    void HttpCreateOrder(int iChannel, int iMemberId, int iNum,int iPayType, QString strRelateId);
    //关闭订单
    void HttpCloseOrder(QString strOutTradeNo);
    //删除
    void HttpDeleteOrder(int iOrderId);
    //清空
    void HttpEmptyOrder();

    //手机实例相关接口
    //获取serverToken
    //void HttpGetServerToken();
    //获取我的手机实例
    void GetMyPhoneInstance();


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
    void loadVipType(LEVEL_TYPE enType);

    void on_btnCreateGroup_clicked();
    void on_btnGroupRefresh_clicked();

    void on_btnActiveCode_clicked();

    void on_toolBtnAdd_clicked();

    void on_toolButtonRenew_clicked();

    void on_toolBtnBuyPhone_clicked();

    void on_toolBtnRenewPhone_clicked();

    void on_btnBeginPay_clicked();

    //level item 
    void do_selectLevelTypeSignals(LEVEL_TYPE enType);
    //vip item
    void do_selectVIPTypeSignals(S_VIP_ITEM_INFO sVipInfo);

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

private:
    Ui::MainWindow *ui;
    S_USER_LOGIN_INFO m_userInfo;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;

    S_VIP_ITEM_INFO m_curVIPInfo;

    QPoint m_dragPosition;
    // QWidget interface

    LevelNormalWidget* normal;
    LevelEnhancementWidget* enhancement;
    LevelPremierWidget* premier;

    //会员列表
    QMap<LEVEL_TYPE,QMap<int,S_LEVEL_DATA_INFO>> m_mapLevel;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // MAINWINDOW_H
