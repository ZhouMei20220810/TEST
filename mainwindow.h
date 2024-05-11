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
    //����ӿ�
    void QueryAllGroup();//��ѯȫ������
    void CreateGroup(QString strGroupName);//��������
    void UpdateGroup(int iGroupId, QString strNewName);//�޸ķ���
    void DeleteGroup(int iGroupId);//ɾ������

    //��ʾ������
    void ShowGroupInfo();

    //�����ӿ�
    // �ҵ�֧������
    void HttpGetMyOrder(int iPage, int iPageSize);
    //��������
    void HttpCreateOrder(int iChannel, int iMemberId, int iNum, QString strRelateId);
    //�رն���
    void HttpCloseOrder(QString strOutTradeNo);
    //ɾ��
    void HttpDeleteOrder(int iOrderId);
    //���
    void HttpEmptyOrder();

    //�ֻ�ʵ����ؽӿ�
    //��ȡserverToken
    //void HttpGetServerToken();
    //��ȡ�ҵ��ֻ�ʵ��
    void GetMyPhoneInstance();


private slots:
    //ע��
    void on_btnClose_clicked();

    void on_btnCreateNewGroup_clicked();

    void on_treeWidget_itemPressed(QTreeWidgetItem *item, int column);

    void on_btnRefresh_clicked();

    void on_btnCreateOrder_clicked();

    void on_btnMyOrder_clicked();

    void on_btnCloseOrder_clicked();

    void on_btnDeleteOrder_clicked();

    void on_btnPayOrder_clicked();

    void on_btnMax_clicked();

private:
    Ui::MainWindow *ui;
    S_USER_LOGIN_INFO m_userInfo;
    QMap<int, S_GROUP_INFO> m_mapGroupInfo;
};

#endif // MAINWINDOW_H
