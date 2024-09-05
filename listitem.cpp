#include "listitem.h"
#include "phoneinstancewidget.h"
#include "listmodel.h"

ListItem::ListItem(/*S_PHONE_INFO info, */ QObject * parent)
    : QObject{parent}
{
    /*if (info.strName.isEmpty())
    {
        setPhoneName(info.strInstanceNo);
    }
    else
    {
        setPhoneName(info.strName);
    }

    setImagePath(QString("file:///%1%2.png").arg(GlobalData::strFileTempDir).arg(info.strInstanceNo));

    setCheckBox(info.bChecked);
    setBShowAuthorImg(true);
    if (info.bIsAuth)
    {
        setAuthorStatus(1);
        //从外部传入图片路径不能显示
        //AuthorImgPath = "qrc:/main/resource/main/Authorized.png";//QString("qrc:/main/resource/main/Authorized.png");
    }
    else if (info.iAuthType == EN_BE_AUTHORIZATION)
    {
        setAuthorStatus(2);
        //从外部传入图片路径不能显示
        //AuthorImgPath = "qrc:/main/resource/main/BeAuthorized.png";
    }
    else
        setBShowAuthorImg(false);

    m_FileDownload = NULL;
    m_strPicturePath = GlobalData::strFileTempDir + info.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir + info.strInstanceNo + "_bak.png";

    setPhoneInfo(info);*/
}

int ListItem::getPhoneId() const
{
    return phoneId;
}

void ListItem::setPhoneId(int newPhoneId)
{
    if (phoneId == newPhoneId)
        return;
    phoneId = newPhoneId;
    emit phoneIdChanged();
}

QString ListItem::getPhoneName() const
{
    return phoneName;
}

void ListItem::setPhoneName(const QString &newPhoneName)
{
    if (phoneName == newPhoneName)
        return;
    phoneName = newPhoneName;
    emit phoneNameChanged();
}

QString ListItem::getPhoneInstanceNo() const
{
    return phoneInstanceNo;
}

void ListItem::setPhoneInstanceNo(const QString &newPhoneInstanceNo)
{
    if (phoneInstanceNo == newPhoneInstanceNo)
        return;
    phoneInstanceNo = newPhoneInstanceNo;
    emit phoneInstanceNoChanged();
}

bool ListItem::getCheckBox() const
{
    return checkBox;
}

void ListItem::setCheckBox(bool newCheckBox)
{
    if (checkBox == newCheckBox)
        return;
    checkBox = newCheckBox;
    emit checkBoxChanged();
}

QString ListItem::getImagePath() const
{
    return ImagePath;
}

void ListItem::setImagePath(const QString &newImagePath)
{
    if (ImagePath == newImagePath)
        return;
    ImagePath = newImagePath;
    emit ImagePathChanged();
}

S_PHONE_INFO ListItem::getPhoneInfo() const
{
    return PhoneInfo;
}

void ListItem::setPhoneInfo(const S_PHONE_INFO &newPhoneInfo)
{
    if (&PhoneInfo == &newPhoneInfo)
        return;
    PhoneInfo = newPhoneInfo;
    emit PhoneInfoChanged();
}

int ListItem::getAuthorStatus() const
{
    return authorStatus;
}

void ListItem::setAuthorStatus(int newAuthorStatus)
{
    if (authorStatus == newAuthorStatus)
        return;
    authorStatus = newAuthorStatus;
    emit authorStatusChanged();
}

bool ListItem::getBShowAuthorImg() const
{
    return bShowAuthorImg;
}

void ListItem::setBShowAuthorImg(bool newBShowAuthorImg)
{
    if (bShowAuthorImg == newBShowAuthorImg)
        return;
    bShowAuthorImg = newBShowAuthorImg;
    emit bShowAuthorImgChanged();
}

void ListItem::ShowInstanceSignalFromQMLFile(int iId, QString strPhoneName, QString strInstanceNo, QString strExpireTime, bool bIsShowMenu)
{
    //PhoneInfo为空
    S_PHONE_INFO info;
    info.iId = iId;
    info.strInstanceNo = strInstanceNo;
    info.strExpireTime = strExpireTime;
    //后续可以考虑是否要根据id找全S_PHONE_INFO的信息
    qDebug() << "ShowInstanceSignalFromQMLFile strPhoneName="<<strPhoneName<< " VMNo=" << strInstanceNo <<"iId="<<iId<<"strExpireTime="<< strExpireTime<<"bIsShowMenu="<< bIsShowMenu;
    //emit ShowPhoneInstanceWidgetSignals(info, bIsShowMenu);
    on_ShowPhoneInstanceWidgetSignals(info, bIsShowMenu);
}

QString ListItem::getExpireTime() const
{
    return ExpireTime;
}

void ListItem::setExpireTime(const QString &newExpireTime)
{
    if (ExpireTime == newExpireTime)
        return;
    ExpireTime = newExpireTime;
    emit ExpireTimeChanged();
}

int ListItem::getItemIndex() const
{
    return itemIndex;
}

void ListItem::setItemIndex(int newItemIndex)
{
    if (itemIndex == newItemIndex)
        return;
    itemIndex = newItemIndex;
    emit itemIndexChanged();
}

//显示实例
void ListItem::on_ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu)
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
void ListItem::downloadUrl(QString url)
{
    if (NULL == m_FileDownload)
        m_FileDownload = new FileDownloader(this);
    if (m_FileDownload != NULL)
    {
        connect(m_FileDownload, &FileDownloader::downloadFinished, this, [this]()
            {
                QPixmap pixmap(m_strTemp);
                if (!pixmap.isNull())
                {                    
                    if (ImagePath.contains(m_strPicturePath))
                    {
                        setImagePath(QString("file:///%1").arg(m_strTemp));
                    }
                    else
                    {
                        setImagePath(QString("file:///%1").arg(m_strPicturePath));
                    }
                    if (QFile::exists(m_strPicturePath))
                    {
                        if (!QFile::remove(m_strPicturePath))
                        {
                            qDebug() << "remove fail:" << m_strPicturePath;
                        }
                    }
                    if (!QFile::rename(m_strTemp, m_strPicturePath))
                    {
                        qDebug() << "rename fail: " << m_strPicturePath;
                    }
                                        
                }
            });
        m_FileDownload->setUrlOutputFile(url, m_strTemp);
        m_FileDownload->start();
    }
}
