#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include <QMap>

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
    //http 分组接口
    void QueryAllGroup();//查询全部分组
    void CreateGroup(QString strGroupName);//创建分组
    void UpdateGroup(int iGroupId, QString strNewName);//修改分组
    void DeleteGroup();//删除分组

    //显示树内容
    void ShowGroupInfo();
private slots:
    //注销
    void on_btnClose_clicked();

    void on_btnCreateNewGroup_clicked();

private:
    Ui::MainWindow *ui;
    S_USER_LOGIN_INFO m_userInfo;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;
};

#endif // MAINWINDOW_H
