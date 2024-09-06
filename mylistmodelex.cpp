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

void MyListModelEx::do_ImagePathChanged(int index,QString strImagePath)
{
    qDebug() << "ListItem 切换图片后,发送信号do_ImagePathChanged";
    if (index >= 0 && index < items.size()) {
        ListItem* item = items[index];
        item->setImagePath(strImagePath);
        emit dataChanged(createIndex(index, 0), createIndex(index, 0), { ImagePathRole });
        // 在这里处理checked状态的更改
        qDebug() << "Item at index" << index << "changed to" << item->getCheckBox();
    }
}

void MyListModelEx::addItem(MainWindow* mainWindow, S_PHONE_INFO info/*bool checked*/ )
{    
    beginInsertRows(QModelIndex(), items.size(), items.size());
    ListItem* listItem = new ListItem(info, mainWindow, this);
    connect(listItem, &ListItem::ImagePathChanged, this, &MyListModelEx::do_ImagePathChanged);
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
    items.last()->setPhoneInfo(info);
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
    iItemIndex = 0;
    endRemoveRows();
}
//取消选择
void MyListModelEx::setCancelSelectCheckBox(bool bChecked)
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
}

//全选
void MyListModelEx::setAllCheckBox(bool newCheckBox)
{
    int iListCount = items.size();
    for (int i = 0; i < iListCount; i++) {
        ListItem* item = items.at(i);
        if (item != NULL)
        {
            item->setCheckBox(newCheckBox);
            emit dataChanged(createIndex(i, 0), createIndex(i, 0), { CheckedRole });
        }
    }
    /*QList<ListItem*> list;
    for (const QVariant& variant : items) {
        ListItem* item = variant.value<ListItem*>();
        item->setCheckBox(newCheckBox);
        list.append(item);
    }*/
}

//反选
void MyListModelEx::setFanXuanCheckBox()
{
    int iListCount = items.size();
    for (int i = 0; i < iListCount; i++) {
        ListItem* item = items.at(i);
        if (item != NULL)
        {
            item->setCheckBox(!item->getCheckBox());
            emit dataChanged(createIndex(i, 0), createIndex(i, 0), { CheckedRole });
        }
    }
    /*QList<ListItem*> list;
    for (const QVariant& variant : items) {
        ListItem* item = variant.value<ListItem*>();
        item->setCheckBox(!item->getCheckBox());
        list.append(item);
    }*/
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
        qDebug() << "Item at index" << index << "changed to" << item->getCheckBox();
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
//鼠标点击事件
void MyListModelEx::mousePressEvent(const QVariantMap& event)
{
    int buttons = event.value("buttons").toInt();
    qreal x = event.value("x").toReal();
    qreal y = event.value("y").toReal();
    int index = event.value("index").toInt();

    if (buttons & Qt::LeftButton) {
        qDebug() << "Left mouse button pressed at" << x << y;
    }
    else if (buttons & Qt::RightButton) {
        qDebug() << "Right mouse button pressed at" << x << y;
    }
}

void MyListModelEx::ShowInstanceSignalFromQMLFile(QString strPhoneName, QString strInstanceNo,bool bIsShowMenu, S_PHONE_INFO info)
{
    //后续可以考虑是否要根据id找全S_PHONE_INFO的信息
    qDebug() << "MyListModelEx ShowInstanceSignalFromQMLFile strPhoneName=" << strPhoneName << " VMNo=" << strInstanceNo << "iId=" << info.iId << "strExpireTime=" << info.strExpireTime << "bIsShowMenu=" << bIsShowMenu;
    //显示实例窗口
    on_ShowPhoneInstanceWidgetSignals(info, bIsShowMenu);
}

void MyListModelEx::do_ItemClickSignals(int index, const QVariant& data)
{
    qDebug() << "MyListModelEx Clicked item at index:" << index;
    qDebug() << "Item data:" << data;
}

//显示实例
void MyListModelEx::on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu)
{
    PhoneInstanceWidget* phoneWidget = new PhoneInstanceWidget(sPhoneInfo);
    phoneWidget->show();

    //暂时屏蔽右键菜单
    /*m_CurSelMenuPhoneInfo = sPhoneInfo;
    if (bShowMenu)
    {
        pActionCopyCloudId->setText("复制云号[" + m_CurSelMenuPhoneInfo.strInstanceNo + "]");
        m_PhoneMenu->exec(QCursor::pos());
        return;
    }

    //非同步模式
    if (!GlobalData::bIsSyncOperation)
    {
        PhoneInstanceWidget* phoneWidget = NULL;
        phoneWidget = m_mapWindows.value(sPhoneInfo.strInstanceNo, nullptr);
        if (phoneWidget)
        {
            phoneWidget->raise();//将窗口置顶
            phoneWidget->activateWindow();//激活窗口
        }
        else
        {
            on_ShowPhoneInstanceNotMaster(sPhoneInfo);
        }
        return;
    }

    //同步操作模式
    if (NULL != m_MainPhoneInstanceWidget)
    {
        if (m_MainPhoneInstanceWidget->isWindow() && m_MainPhoneInstanceWidget->getPhoneInfo().iId == m_CurSelMenuPhoneInfo.iId)
        {
            qDebug() << "已经是主控不能成为非主控 id=" << m_CurSelMenuPhoneInfo.iId;
            MessageTips* tips = new MessageTips("已经是主控不能成为非主控");
            tips->show();
            return;
        }
        if (m_MainPhoneInstanceWidget->hasChildControl())
        {
            //已经有子控
            //关闭之前，打开
            PhoneInstanceWidget* phoneWidget = NULL;
            phoneWidget = m_mapSyncWindows.value(sPhoneInfo.strInstanceNo, nullptr);
            if (phoneWidget)
            {
                phoneWidget->raise();//将窗口置顶
                phoneWidget->activateWindow();//激活窗口
            }
            else
            {
                MessageTipsDialog* dialog = new MessageTipsDialog("同步模式下仅支持打开一台非主控云手机,将为您自动切换非主控设备.");
                if (QDialog::Accepted != dialog->exec())
                    return;

                //删除非主设备
                PhoneInstanceWidget* phoneWidget = NULL;
                auto it = m_mapSyncWindows.begin();
                if (it != m_mapSyncWindows.end())
                {
                    phoneWidget = it.value();
                    //phoneWidget->hide();
                    //不能关闭窗口，关闭的话不能同步了
                    //phoneWidget->close();
                    //添加到列表
                    if (phoneWidget != NULL)
                    {
                        AddSyncPhoneInstanceWidget(phoneWidget->getPhoneInfo(), phoneWidget);
                        //delete phoneWidget;
                        //phoneWidget = NULL;
                    }
                    m_mapSyncWindows.remove(it.key());
                }
                on_ShowPhoneInstanceNotMaster(sPhoneInfo);
            }
        }
        else
        {
            //子控设备,同步列表移除后，后面不会再同步
            //QMap<int, S_PHONE_INFO>::iterator iterFind = GlobalData::mapSyncPhoneList.find(sPhoneInfo.iId);
            //if (iterFind != GlobalData::mapSyncPhoneList.end())
            //{
            //    GlobalData::mapSyncPhoneList.remove(sPhoneInfo.iId);
            //}
            m_MainPhoneInstanceWidget->setChildControl(true);
            on_ShowPhoneInstanceNotMaster(sPhoneInfo);
        }
        return;
    }

    //重新获取一次列表，列表数据
    getCheckedPhoneInstance();
    GlobalData::mapSyncPhoneList.insert(sPhoneInfo.iId, sPhoneInfo);
    if (GlobalData::mapSyncPhoneList.size() <= 0)
        return;

    QMap<int, S_PHONE_INFO>::iterator iter = GlobalData::mapSyncPhoneList.begin();
    if (m_SyncOperListWidget != NULL)
    {
        //释放资源
        ClearSyncPhoneInstanceWidgetList();
    }
    else
    {
        m_SyncOperListWidget = new QListWidget();
        m_SyncOperListWidget->setViewMode(QListView::IconMode);
        //设置QListWidget中单元项的图片大小
        //ui->imageList->setIconSize(QSize(100,100));
        //设置QListWidget中单元项的间距
        m_SyncOperListWidget->setSpacing(ITEM_WIDGET_SPACING);
        //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
        m_SyncOperListWidget->setResizeMode(QListWidget::Adjust);
        //设置不能移动
        m_SyncOperListWidget->setMovement(QListWidget::Static);
        //设置单选
        m_SyncOperListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    QListWidgetItem* item = NULL;
    int height = PHONE_INSTANCE_VERTICAL_HEIGHT;
    int iwidth = calculateWidth(height);
    PhoneInstanceWidget* phoneWidget = NULL;
    for (; iter != GlobalData::mapSyncPhoneList.end(); iter++)
    {
        if (!GlobalData::bIsSyncOperation && iter->iId != sPhoneInfo.iId)
            continue;
        qDebug() << "同步实例id=" << iter.value().iId;
        //HttpGetInstanceSession(iter.value().iId);
        phoneWidget = new PhoneInstanceWidget(*iter);
        phoneWidget->setIsMasterOrNot(false);
        connect(phoneWidget, &PhoneInstanceWidget::TouchEventSignals, this, &MainWindow::paifaTouchEventSignals);
        connect(this, &MainWindow::paifaTouchEventSignals, phoneWidget, &PhoneInstanceWidget::dealTouchEventSignals);

        connect(phoneWidget, &PhoneInstanceWidget::ReturnSignals, this, &MainWindow::returnSignals);
        connect(this, &MainWindow::returnSignals, phoneWidget, &PhoneInstanceWidget::do_ReturnSignals);

        connect(phoneWidget, &PhoneInstanceWidget::HomeSignals, this, &MainWindow::homeSignals);
        connect(this, &MainWindow::homeSignals, phoneWidget, &PhoneInstanceWidget::do_HomeSignals);

        connect(phoneWidget, &PhoneInstanceWidget::ChangePageSignals, this, &MainWindow::changePageSignals);
        connect(this, &MainWindow::changePageSignals, phoneWidget, &PhoneInstanceWidget::do_ChangePageSignals);

        connect(phoneWidget, &PhoneInstanceWidget::RootSignals, this, &MainWindow::RootSignals);
        connect(this, &MainWindow::RootSignals, phoneWidget, &PhoneInstanceWidget::do_rootSignals);

        connect(phoneWidget, &PhoneInstanceWidget::VolumeUpSignals, this, &MainWindow::VolumeUpSignals);
        connect(this, &MainWindow::VolumeUpSignals, phoneWidget, &PhoneInstanceWidget::do_VolumeUpSignals);
        connect(phoneWidget, &PhoneInstanceWidget::VolumeDownSignals, this, &MainWindow::VolumeDownSignals);
        connect(this, &MainWindow::VolumeDownSignals, phoneWidget, &PhoneInstanceWidget::do_VolumeDownSignals);

        connect(phoneWidget, &PhoneInstanceWidget::HorizontalSignals, this, &MainWindow::HorizontalSignals);
        connect(this, &MainWindow::HorizontalSignals, phoneWidget, &PhoneInstanceWidget::do_HorizontalSignals);
        connect(phoneWidget, &PhoneInstanceWidget::SharkSignals, this, &MainWindow::SharkSignals);
        connect(this, &MainWindow::SharkSignals, phoneWidget, &PhoneInstanceWidget::do_SharkSignals);
        connect(phoneWidget, &PhoneInstanceWidget::GPSSignals, this, &MainWindow::GPSSignals);
        connect(this, &MainWindow::GPSSignals, phoneWidget, &PhoneInstanceWidget::do_GPSSignals);

        connect(phoneWidget, &PhoneInstanceWidget::BatchDirectCopyToPhoneSignals, this, &MainWindow::BatchDirectCopyToPhoneSignals);
        connect(this, &MainWindow::BatchDirectCopyToPhoneSignals, phoneWidget, &PhoneInstanceWidget::do_BatchDirectCopyToPhoneSignals);
        connect(phoneWidget, &PhoneInstanceWidget::DirectCopyToPhoneSignals, this, &MainWindow::DirectCopyToPhoneSignals);
        connect(this, &MainWindow::DirectCopyToPhoneSignals, phoneWidget, &PhoneInstanceWidget::do_DirectCopyToPhoneSignals);

        connect(phoneWidget, &PhoneInstanceWidget::closePhoneInstanceWidgetSignals, [this](PhoneInstanceWidget* widget) {
            if (widget == m_MainPhoneInstanceWidget)
            {
                delete m_MainPhoneInstanceWidget;
                m_MainPhoneInstanceWidget = NULL;
            }
            emit closePhoneInstanceWidgetSignals();
            });
        connect(this, &MainWindow::closePhoneInstanceWidgetSignals, phoneWidget, &PhoneInstanceWidget::do_closePhoneInstanceWidgetSignals);

        if (iter->iId != sPhoneInfo.iId)
        {
            item = new QListWidgetItem(m_SyncOperListWidget);
            item->setSizeHint(QSize(iwidth, height));
            item->setData(Qt::UserRole, QVariant::fromValue(*iter));
            m_SyncOperListWidget->setItemWidget(item, phoneWidget);
        }
        else
        {
            m_MainPhoneInstanceWidget = phoneWidget;
            //设置为主控
            m_MainPhoneInstanceWidget->setIsMasterOrNot(true);
            m_MainPhoneInstanceWidget->setModal(false);
            m_MainPhoneInstanceWidget->show();
        }
    }
    //m_SyncOperListWidget->show();
    //m_SyncOperListWidget->move(0, 0);
    //m_SyncOperListWidget->resize(2000,2000);
    //m_SyncOperListWidget->setVisible(false);
    */
}