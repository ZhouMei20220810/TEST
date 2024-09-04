#include "mylistmodelex.h"

MyListModelEx::MyListModelEx(QObject *parent)
    : QAbstractListModel(parent)
{
    //初始化数据
    //m_data.append(MyData(true, false,1,0,"Vm1","Vmmm","imagePath","2024"));
    //m_data.append(MyData(true, false, 1, 0, "Vm2", "Vmmm", "imagePath", "2024"));
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
    //return m_data.count();
    return items.count();
}

//ListElement
QVariant MyListModelEx::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= items.count())
        return QVariant();

    const ListItem* item = items.at(index.row());
    if (item == NULL)
    {
        qDebug() << "item is null";
        return QVariant();
    }
    /*if (role == CheckedRole)
    {
        return item->getCheckBox();
    }*/
    switch (role)
    {
    case NameRole:
        return item->getPhoneName();
        break;
    case ImagePathRole:
        return item->getImagePath();
        break;
    case AuthorStatusRole:
        return item->getAuthorStatus();
        break;
    case CheckedRole:
        return item->getCheckBox();
        break;
    case IsShowAuthImgRole:
        return item->getBShowAuthorImg();
        break;
    case PhoneIdRole:
        return item->getPhoneId();
        break;
    case InstanceNoRole:
        return item->getPhoneInstanceNo();
        break;
    case ExpireTimeRole:
        return item->getExpireTime();
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
    /*static const QHash<int, QByteArray> roles = {
            {CheckedRole, "checked"}
    };
    return roles;*/
    static QHash<int, QByteArray> roles;
    //映射
    roles.insert(NameRole,"phoneName");//字符串是QML端用
    roles.insert(ImagePathRole,"imagePath");//枚举是C++端来判断
    roles.insert(CheckedRole, "bChecked"); //是否勾选
    roles.insert(IsShowAuthImgRole, "isShowAuthImg");
    roles.insert(AuthorStatusRole, "authorStatus");
    roles.insert(InstanceNoRole, "phoneInstanceNo");
    roles.insert(PhoneIdRole, "phoneId");
    roles.insert(ExpireTimeRole, "expireTime");
    return roles;
}

bool MyListModelEx::setData(const QModelIndex& index, const QVariant& value, int role/* = Qt::EditRole*/)
{
    if (!index.isValid() || index.row() >= items.count())
        return false;

    ListItem* item = items[index.row()];
    switch (role)
    {
    case NameRole:
        item->setPhoneName(value.toString());
        emit dataChanged(index, index, { NameRole });
        return true;
        break;
    case ImagePathRole:
        item->setImagePath(value.toString());
        emit dataChanged(index, index, { ImagePathRole });
        return true;
        break;
    case AuthorStatusRole:
        item->setAuthorStatus(value.toInt());
        emit dataChanged(index, index, { AuthorStatusRole });
        return true;
        break;
    case CheckedRole:
        item->setCheckBox(value.toBool());
        emit dataChanged(index, index, { CheckedRole });
        return true;
        break;
    case IsShowAuthImgRole:
        item->setBShowAuthorImg(value.toBool());
        emit dataChanged(index, index, { IsShowAuthImgRole });
        return true;
        break;
    case PhoneIdRole:
        item->setPhoneId(value.toInt());
        emit dataChanged(index, index, { PhoneIdRole });
        return true;
        break;
    case InstanceNoRole:
        item->setPhoneInstanceNo(value.toString());
        emit dataChanged(index, index, { InstanceNoRole });
        return true;
        break;
    case ExpireTimeRole:
        item->setExpireTime(value.toString());
        emit dataChanged(index, index, { ExpireTimeRole });
        return true;
        break;
    default:
        break;
    }
    return false;
}

void MyListModelEx::addItem(S_PHONE_INFO info/*bool checked*/ )
{
    beginInsertRows(QModelIndex(), items.size(), items.size());
    items.append(new ListItem(this));
    if (info.strName.isEmpty())
    {
        items.last()->setPhoneName(info.strInstanceNo);
    }
    else
    {
        items.last()->setPhoneName(info.strName);
    }

    items.last()->setImagePath(QString("file:///%1%2.png").arg(GlobalData::strFileTempDir).arg(info.strInstanceNo));

    items.last()->setCheckBox(info.bChecked);
    items.last()->setBShowAuthorImg(true);
    if (info.bIsAuth)
    {
        items.last()->setAuthorStatus(1);
        //从外部传入图片路径不能显示
        //AuthorImgPath = "qrc:/main/resource/main/Authorized.png";//QString("qrc:/main/resource/main/Authorized.png");
    }
    else if (info.iAuthType == EN_BE_AUTHORIZATION)
    {
        items.last()->setAuthorStatus(2);
        //从外部传入图片路径不能显示
        //AuthorImgPath = "qrc:/main/resource/main/BeAuthorized.png";
    }
    else
    {
        items.last()->setBShowAuthorImg(false);
    }
    
    items.last()->setPhoneInstanceNo(info.strInstanceNo);
    items.last()->setExpireTime(info.strExpireTime);
    items.last()->setPhoneId(info.iId);
    /*m_FileDownload = NULL;
    m_strPicturePath = GlobalData::strFileTempDir + info.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + info.strInstanceNo + "_bak.png";

    setPhoneInfo(info);*/
    endInsertRows();
}

//移除所有项
void MyListModelEx::removeAllItem()
{
    if (items.isEmpty())
        return;
    //记录当前的行数
    int rows = items.count();
    //开始移除操作
    beginRemoveRows(QModelIndex(), 0, rows - 1);
    //清空存储项的向量
    qDeleteAll(items);
    items.clear();
    endRemoveRows();
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

