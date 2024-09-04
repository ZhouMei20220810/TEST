#ifndef MYLISTMODELEX_H
#define MYLISTMODELEX_H

#include <QAbstractListModel>

class MyListModelEx : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MyRoleName
    {
        Name = Qt::DisplayRole+1,
        Value,
        InstanceNo,
        ImagePath
    };
    explicit MyListModelEx(QObject *parent = nullptr);

    static MyListModelEx* getInstance();
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

public slots:
    void ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu);
    void do_ItemClickSignals(int index, const QVariant& data);
    Q_INVOKABLE void do_notifyRefreshWindow();
private:
    QList<QString> m_data;
    //QHash<int,QByteArray> m_roles;
};

#endif // MYLISTMODELEX_H
