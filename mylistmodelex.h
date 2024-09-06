#ifndef MYLISTMODELEX_H
#define MYLISTMODELEX_H
#include <QAbstractListModel>
#include "listitem.h"
#include "mainwindow.h"
//是不是直接可以改成S_PHONE_INFO
class MyListModelEx : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        /*CheckedRole = Qt::UserRole + 1 */
        NameRole = Qt::DisplayRole + 1,
        InstanceNoRole,
        ImagePathRole,
        CheckedRole,
        IsShowAuthImgRole,
        //PhoneIdRole,
        AuthorStatusRole,
        //ExpireTimeRole,
        ItemIndexRole,
        PhoneInfoRole
    };

    explicit MyListModelEx(QObject *parent = nullptr);

    static MyListModelEx* getInstance(MainWindow *pMainWindows);
    // Header:
    //ListModel基本用不到
    /*QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;*/

    // Basic functionality:
    //最简单的model需要实现下面三个函数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    
    Q_INVOKABLE bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    void addItem(MainWindow* mainWindow,S_PHONE_INFO info);
    //移除所有项
    void removeAllItem();
    //取消选择
    void setCancelSelectCheckBox(bool bChecked);
    //全选
    void setAllCheckBox(bool newCheckBox);
    //反选
    void setFanXuanCheckBox();

    QList<ListItem*> itemList()
    {
        return items;
    }
    //通过下标获取ListItem
    ListItem* getListItemByIndex(int index);

    //鼠标点击事件
    Q_INVOKABLE void mousePressEvent(const QVariantMap& event);
public slots:
    void ShowInstanceSignalFromQMLFile(QString strPhoneName, QString strInstanceNo, bool bIsShowMenu, S_PHONE_INFO info);
    void do_ItemClickSignals(int index, const QVariant& data);
    //void do_itemClicked(int index);
    //QML中点击后,通知C++处理,添加Q_INVOKABLE QML文件即可访问
    //连接到QML的信号
    Q_INVOKABLE void onCheckBoxChanged(int index,bool bChecked);
private slots:
    void do_ImagePathChanged(int index, QString strImagePath);
signals:
    //qml点击事件通过QmlSendMainWindowSignals信号通知到MainWindow
    void QmlSendMainWindowSignals(S_PHONE_INFO info, bool bShowMenu);
    //复选框勾选通知界面刷新
    void notifyMainWindowRefreshWindow();
private:
    //显示实例,将显示实例部分从MainWindow中提取出来
    void on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu);
private:
    int iItemIndex;
    QList<ListItem*> items;   

    MainWindow* m_MainWindow;
};

#endif // MYLISTMODELEX_H
