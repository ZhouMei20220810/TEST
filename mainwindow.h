#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include <QMap>
#include <QTreeWidgetItem>

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

    //订单接口
    // 我的支付订单
    void HttpGetMyOrder(int iPage, int iPageSize);
    //创建订单
    void HttpCreateOrder(int iChannel, int iMemberId, int iNum, QString strRelateId);

    //手机实例相关接口
    //获取serverToken
    //void HttpGetServerToken();
    //获取我的手机实例
    void GetMyPhoneInstance();


private slots:
    //注销
    void on_btnClose_clicked();

    void on_btnCreateNewGroup_clicked();

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void on_btnRefresh_clicked();

    void on_btnCreateOrder_clicked();

    void on_btnMyOrder_clicked();

private:
    Ui::MainWindow *ui;
    S_USER_LOGIN_INFO m_userInfo;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;
};

#endif // MAINWINDOW_H
