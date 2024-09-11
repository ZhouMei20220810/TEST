#include "listitem.h"
#include "phoneinstancewidget.h"
#include "listmodel.h"
#include "mylistmodelex.h"

ListItem::ListItem(S_PHONE_INFO info, QObject *parent)
    : QObject{parent}
{
    m_strPicturePath = GlobalData::strFileTempDir + info.strInstanceNo + ".png";
    m_strTemp = GlobalData::strFileTempDir +info.strInstanceNo+"/" + info.strInstanceNo + "_bak.png";

    QString strDir = GlobalData::strFileTempDir;
    QDir dir(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }
    strDir = strDir+"/"+info.strInstanceNo;
    deleteDirectoryRecursively(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "create instanceNo dir failed:" << strDir;
    }
}

void ListItem::deleteDirectoryRecursively(const QString& path) {
    QDir dir(path);

    if (!dir.exists()) {
        qDebug() << "Directory does not exist:" << path;
        return;
    }

    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach(const QFileInfo & fileInfo, entries) {
        if (fileInfo.isDir()) {
            deleteDirectoryRecursively(fileInfo.absoluteFilePath());
        }
        else {
            if (QFile::exists(m_strPicturePath))
            {
                if (!QFile::remove(m_strPicturePath))
                {
                    qDebug() << "remove fail:" << m_strPicturePath;
                }
            }
            if (!QFile::rename(fileInfo.absoluteFilePath(), m_strPicturePath))
            {
                qDebug() << "rename fail: " << m_strPicturePath;
            }
            /*if (!QFile::remove()) {
                qDebug() << "Failed to remove file:" << fileInfo.absoluteFilePath();
            }
            else {
                qDebug() << "Removed file:" << fileInfo.absoluteFilePath();
            }*/
        }
    }

    // 删除空目录
    if (!dir.rmdir(path)) {
        qDebug() << "Failed to remove directory:" << path;
    }
    else {
        qDebug() << "Removed directory:" << path;
    }
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
    emit ImagePathChanged(itemIndex, ImagePath);
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

    if (QFile::exists(m_strPicturePath))
    {
        if (!QFile::remove(m_strPicturePath))
        {
            qDebug() << "remove fail:" << m_strPicturePath;
        }
    }
    if (QFile::exists(m_strTemp))
    {
        if (!QFile::rename(m_strTemp, m_strPicturePath))
        {
            qDebug() << "rename fail: " << m_strPicturePath;
        }
    }
    
    QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
    qDebug() << "url=" << url << "strFileName=" << strFileName;
    m_strTemp = GlobalData::strFileTempDir+ phoneInstanceNo + "/" + strFileName;
    
}

/*void ListItem::downloadUrl(QString url)
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
                        qDebug() << "time:" << QDateTime::currentDateTime().fromString("yyyy-MM-dd HH:mm:ss") <<"修改之前ImagePath="<< m_strPicturePath;
                        setImagePath(QString("file:///%1").arg(m_strTemp));
                        qDebug() << "time:" << QDateTime::currentDateTime().fromString("yyyy-MM-dd HH:mm:ss") << "修改之后ImagePath=" << m_strTemp;
                    }
                    else
                    {
                        qDebug() << "time:" << QDateTime::currentDateTime().fromString("yyyy-MM-dd HH:mm:ss") << "修改之前ImagePath=" << m_strTemp;
                        setImagePath(QString("file:///%1").arg(m_strPicturePath));
                        qDebug() << "time:" << QDateTime::currentDateTime().fromString("yyyy-MM-dd HH:mm:ss") << "修改之后ImagePath=" << m_strPicturePath;
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
                }
            });
        m_FileDownload->setUrlOutputFile(url, m_strTemp);
        m_FileDownload->start();
    }
}*/
