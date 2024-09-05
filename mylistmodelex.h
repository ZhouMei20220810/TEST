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
    enum MyRoleName
    {
        Name = Qt::DisplayRole + 1,
        InstanceNo,
        ImagePath,
        Checked,
        IsShowAuthImg,
        PhoneId,
        AuthorStatus,
        ExpireTime
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

    QList<ListItem*> itemList()
    {
        return items;
    }
public slots:
    void ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu, S_PHONE_INFO info);
    void do_ItemClickSignals(int index, const QVariant& data);
    //void do_itemClicked(int index);
    Q_INVOKABLE void do_notifyRefreshWindow();
    //QML中点击后,通知C++处理,添加Q_INVOKABLE QML文件即可访问
    Q_INVOKABLE void itemCheckBoxClicked(int index);
signals:
    
    //复选框勾选通知界面刷新
    void notifyMainWindowRefreshWindow();
private:
    //QList<QString> m_data;
    int iItemIndex;
    QList<ListItem*> items;
    enum Roles {
        /*CheckedRole = Qt::UserRole + 1 */
        NameRole = Qt::DisplayRole + 1,
        InstanceNoRole,
        ImagePathRole,
        CheckedRole,
        IsShowAuthImgRole,
        PhoneIdRole,
        AuthorStatusRole,
        ExpireTimeRole,
        ItemIndexRole,
        PhoneInfoRole
    };

    MainWindow* m_MainWindow;
};

#endif // MYLISTMODELEX_H
