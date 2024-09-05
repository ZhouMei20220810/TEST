#include "listitem.h"
#include "phoneinstancewidget.h"
#include "listmodel.h"

ListItem::ListItem(S_PHONE_INFO info, MainWindow* pMainWindow, QObject *parent)
    : QObject{parent}
{
    if (info.strName.isEmpty())
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

    setPhoneInfo(info);
    m_pMainWindow = pMainWindow;
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
