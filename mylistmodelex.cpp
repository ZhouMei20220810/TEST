#include "mylistmodelex.h"

MyListModelEx::MyListModelEx(QObject *parent)
    : QAbstractListModel(parent)
{
    //初始化数据
    m_data.append(MyData(true, false,1,0,"Vm1","Vmmm","imagePath","2024"));
    m_data.append(MyData(true, false, 1, 0, "Vm2", "Vmmm", "imagePath", "2024"));
}

MyListModelEx *MyListModelEx::getInstance()
{
    static MyListModelEx* obj = new MyListModelEx();
    return obj;
}

/*QVariant MyListModelEx::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}*/

//元素个数或者说是model的大小/长度/size
int MyListModelEx::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return m_data.count();
}

//ListElement
QVariant MyListModelEx::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case MyRoleName::Name:
        return m_data[index.row()].m_strPhoneName;
        break;
    case MyRoleName::ImagePath:
        return m_data[index.row()].m_strImagePath;
        break;
    default:
        break;
    }
    // FIXME: Implement me!
    return QVariant();
}

//枚举-类型   QByteArray相当于字符串，值？
QHash<int, QByteArray> MyListModelEx::roleNames() const
{
    QHash<int, QByteArray> roles;
    //映射
    roles.insert(MyRoleName::Name,"name");//字符串是QML端用
    roles.insert(MyRoleName::ImagePath,"imagePath");//枚举是C++端来判断
    return roles;
}

void MyListModelEx::ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu)
{
    //后续可以考虑是否要根据id找全S_PHONE_INFO的信息
    qDebug() << "MyListModelEx ShowInstanceSignalFromQMLFile strPhoneName=" << strPhoneName << " VMNo=" << strInstanceNo << "iId=" << iId << "strExpireTime=" << strExpireTime << "bIsShowMenu=" << bIsShowMenu;

}

void MyListModelEx::do_ItemClickSignals(int index, const QVariant& data)
{
    qDebug() << "MyListModelEx Clicked item at index:" << index;
    qDebug() << "Item data:" << data;
}

void MyListModelEx::do_notifyRefreshWindow()
{
    qDebug() << "slot MyListModelEx do_notifyRefreshWindow";
    //emit notifyMainWindowRefreshWindow();
}

