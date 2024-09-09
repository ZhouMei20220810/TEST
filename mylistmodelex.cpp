#include "mylistmodelex.h"

MyListModelEx::MyListModelEx(QObject *parent)
    : QAbstractListModel(parent)
{
    iItemIndex = 0;
}

MyListModelEx *MyListModelEx::getInstance(MainWindow* pMainWindows)
{
    static MyListModelEx* obj = NULL;
    if (obj == NULL)
    {
        obj = new MyListModelEx();
        obj->m_MainWindow = pMainWindows;
    }
    
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
    if (!index.isValid()|| index.row()<0 || index.row() >= items.count())
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
    /*case PhoneIdRole:
        return item->getPhoneId();
        break;*/
    case InstanceNoRole:
        return item->getPhoneInstanceNo();
        break;
    /*case ExpireTimeRole:
        return item->getExpireTime();
        break;*/
    case ItemIndexRole:
        return item->getItemIndex();
        break;
    case PhoneInfoRole:
        return QVariant::fromValue(item->getPhoneInfo());
        break;
    default:
        break;
    }
    // FIXME: Implement me!
    return QVariant();
}

//枚举-类型   QByteArray相当于字符串，值?
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
    //roles.insert(PhoneIdRole, "phoneId");
    //roles.insert(ExpireTimeRole, "expireTime");
    roles.insert(ItemIndexRole, "itemIndex");
    roles.insert(PhoneInfoRole, "phoneInfo");
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
    /*case PhoneIdRole:
        item->setPhoneId(value.toInt());
        emit dataChanged(index, index, { PhoneIdRole });
        return true;
        break;*/
    case InstanceNoRole:
        item->setPhoneInstanceNo(value.toString());
        emit dataChanged(index, index, { InstanceNoRole });
        return true;
        break;
    /*case ExpireTimeRole:
        item->setExpireTime(value.toString());
        emit dataChanged(index, index, { ExpireTimeRole });
        return true;
        break;*/
    case ItemIndexRole:
        item->setItemIndex(value.toInt());
        emit dataChanged(index, index, { ItemIndexRole });
        break;
    case PhoneInfoRole:
        item->setPhoneInfo(value.value<S_PHONE_INFO>());
        emit dataChanged(index, index, { PhoneInfoRole });
        break;
    default:
        break;
    }
    return false;
}

void MyListModelEx::addItem(MainWindow* mainWindow, S_PHONE_INFO info/*bool checked*/ )
{    
    beginInsertRows(QModelIndex(), items.size(), items.size());
    ListItem* listItem = new ListItem(info, mainWindow, this);
    items.append(listItem);
    items.last()->setItemIndex(iItemIndex++);
    if (info.strName.isEmpty())
    {
        items.last()->setPhoneName(info.strInstanceNo);
    }
    else
    {
        items.last()->setPhoneName(info.strName);
    }

    QString strFilePath = QString("%1%2.png").arg(GlobalData::strFileTempDir).arg(info.strInstanceNo);
    if (QFile::exists(strFilePath))
    {
        items.last()->setImagePath(QString("file:///%1%2.png").arg(GlobalData::strFileTempDir).arg(info.strInstanceNo));
    }
    else
    {
        items.last()->setImagePath("qrc:/main/resource/main/defaultSceenShot.png");
    }

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
    items.last()->setPhoneInfo(info);
    /*m_FileDownload = NULL;
    m_strPicturePath = GlobalData::strFileTempDir + info.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + info.strInstanceNo + "_bak.png";*/
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
    iItemIndex = 0;
    endRemoveRows();
}
//取消选择
int MyListModelEx::setCancelSelectCheckBox(bool bChecked)
{
    int iListCount = items.size();
    for (int i = 0; i < iListCount; i++) {
        ListItem* item = items.at(i);
        if (item != NULL)
        {
            item->setCheckBox(false);
            emit dataChanged(createIndex(i, 0), createIndex(i, 0), {CheckedRole});
        }
    }
    return 0;
}

//全选
int MyListModelEx::setAllCheckBox(bool newCheckBox)
{
    int iSelCount = 0; 
    int iListCount = items.size();
    for (int i = 0; i < iListCount; i++) {
        ListItem* item = items.at(i);
        if (item != NULL)
        {
            item->setCheckBox(newCheckBox);
            emit dataChanged(createIndex(i, 0), createIndex(i, 0), { CheckedRole });
        }
    }
    if (newCheckBox)
        iSelCount = items.count();
    return iSelCount;
}

//反选
int MyListModelEx::setFanXuanCheckBox()
{
    int iSelCount = 0;
    int iListCount = items.size();
    bool bChecked = false;
    for (int i = 0; i < iListCount; i++) {
        ListItem* item = items.at(i);
        if (item != NULL)
        {
            bChecked = !item->getCheckBox();
            item->setCheckBox(bChecked);
            if (bChecked)
                iSelCount++;
            emit dataChanged(createIndex(i, 0), createIndex(i, 0), { CheckedRole });
        }
    }
    return iSelCount;
}

//设置新的图片路径
void MyListModelEx::setNewImagePath(int itemIndex, QString strNewImagePath)
{
    int iListCount = items.size();
    if (itemIndex >= 0 && itemIndex < iListCount)
    {
        ListItem* item = items.at(itemIndex);
        if (item != NULL)
        {
            qDebug() << "itemIndex=" << itemIndex << " strNewImagePath=" << strNewImagePath;
            item->setImagePath(strNewImagePath);
            setData(createIndex(itemIndex, 0), strNewImagePath, ImagePathRole);
            //emit dataChanged(createIndex(itemIndex, 0), createIndex(itemIndex, 0), { ImagePathRole });
        }
    }

    if (this->m_MainWindow != NULL)
    {
        this->m_MainWindow->update();
    }
}

//连接到QML的信号
void MyListModelEx::onCheckBoxChanged(int index, bool bChecked)
{
    //可以记录CheckBox的值状态，选中写入存放同步操作的集合，取消选中集合删掉
    if (index >= 0 && index < items.size()) {
        ListItem* item = items[index];
        item->setCheckBox(bChecked);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), { CheckedRole });
        // 在这里处理checked状态的更改
        qDebug() << "Item at index" << index << " bChecked=" << item->getCheckBox();
    }
    if (this->m_MainWindow != NULL)
    {
        this->m_MainWindow->update();
    }
}
//通过下标获取ListItem
ListItem* MyListModelEx::getListItemByIndex(int index)
{
    ListItem* listItem = NULL;
    if (index >= 0 && index < items.size())
    {
        listItem = items[index];
    }
    return listItem;
}

//获取选中条数
int MyListModelEx::getListItemCheckedCount()
{
    int iSelCount = 0;
    int iListCount = items.size();
    ListItem* item = NULL;
    for (int i = 0; i < iListCount; i++) 
    {
        item = items.at(i);
        if (item == NULL)
            continue;
        if (item->getCheckBox())
            iSelCount++;
    }
    return iSelCount;
}

//鼠标点击事件
void MyListModelEx::mousePressEvent(const QVariantMap& event)
{
    int buttons = event.value("buttons").toInt();
    qreal x = event.value("x").toReal();
    qreal y = event.value("y").toReal();
    int index = event.value("index").toInt();

    bool bIsShowMenu = false;
    if (buttons & Qt::LeftButton) {
        bIsShowMenu = false;
        qDebug() << "Left mouse button pressed at" << x << y;
    }
    else if (buttons & Qt::RightButton) {
        bIsShowMenu = true;
        qDebug() << "Right mouse button pressed at" << x << y;
    }
    if (index >= 0 && index < items.size())
    {
        ListItem* item =items.at(index);
        if (item != NULL)
        {
            emit QmlSendMainWindowSignals(item->getPhoneInfo(), bIsShowMenu);
        }
        else
        {
            qDebug() << "item is null.index=" << index;
        }        
    }
    else
    {
        qDebug() << "超出数组下标index=" << index;
    }
}

void MyListModelEx::do_onContentXYChanged()
{
    qDebug() << "c++鼠标滚动do_onContentXYChanged";
    if (this->m_MainWindow != NULL)
    {
        this->m_MainWindow->update();
    }
}

void MyListModelEx::ShowInstanceSignalFromQMLFile(QString strPhoneName, QString strInstanceNo,bool bIsShowMenu, S_PHONE_INFO info)
{
    //后续可以考虑是否要根据id找全S_PHONE_INFO的信息
    qDebug() << "MyListModelEx ShowInstanceSignalFromQMLFile strPhoneName=" << strPhoneName << " VMNo=" << strInstanceNo << "iId=" << info.iId << "strExpireTime=" << info.strExpireTime << "bIsShowMenu=" << bIsShowMenu;
    //显示实例窗口,看看是否能间接调用MainWindow的窗口
    //on_ShowPhoneInstanceWidgetSignals(info, bIsShowMenu);
    
}
