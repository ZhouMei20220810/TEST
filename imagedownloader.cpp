#include "imagedownloader.h"
#include "mylistmodelex.h"

ImageDownloader::ImageDownloader(QNetworkAccessManager* manager, QObject* parent)
    : QObject(parent), m_manager(manager)
{
}

void ImageDownloader::downloadImages(QMap<QString, S_TASK_INFO> mapTask)
{
    QMap<QString, S_TASK_INFO>::iterator iterFind;
    for (auto& item : MyListModelEx::getInstance()->items)
    {
        iterFind = mapTask.find(item->getPhoneInstanceNo());
        if (iterFind == mapTask.end())
        {
            continue;
        }
        //if (!item.image.isNull())
        //    continue;
        if (iterFind->strUrl.isEmpty())
        {
            continue;
        }

        QString strUrl = iterFind->strUrl;
        QNetworkRequest request(strUrl);
        QNetworkReply* reply = m_manager->get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply, item, strUrl]() {
            if (reply->error() == QNetworkReply::NoError) {
                /*QByteArray imageData = reply->readAll();
                QPixmap pixmap;
                pixmap.loadFromData(imageData);   */

                //判断之前的文件是否存在
                QString strPicturePath = GlobalData::strFileTempDir + item->getPhoneInstanceNo() + ".png";
                if (QFile::exists(strPicturePath))
                {
                    if (!QFile::remove(strPicturePath))
                    {
                        qDebug() << "remove fail:" << item->getImagePath();
                    }
                }

                QString strTempImagePath = item->getImagePath();
                if (QFile::exists(strTempImagePath))
                {
                    if (!QFile::rename(strTempImagePath, strPicturePath))
                    {
                        qDebug() << "rename fail: " << strPicturePath;
                    }
                }

                QString strFileName = strUrl.right(strUrl.size() - strUrl.lastIndexOf('/') - 1);
                qDebug() << "url=" << strUrl << "strFileName=" << strFileName;
                strTempImagePath = GlobalData::strFileTempDir + item->getPhoneInstanceNo() + "/" + strFileName;

                //QMetaObject::invokeMethod(m_model, "setImage", Qt::QueuedConnection,
                //    Q_ARG(QUrl, item.url),
                //    Q_ARG(QPixmap, pixmap));
                QFile   file(strTempImagePath);
                if (file.open(QIODevice::WriteOnly))
                {
                    file.write(reply->readAll());
                    file.close();
                }
                else
                {
                    qDebug() << "downloadImages 下载完毕,但写入文件失败.";
                }
                MyListModelEx::getInstance()->setNewImagePath(item->getItemIndex(), strTempImagePath);
                //QMetaObject::invokeMethod(m_model, "setNewImagePath", Qt::QueuedConnection,
                //    Q_ARG(int, item->getItemIndex()),
                //    Q_ARG(QString, strTempImagePath));
            }
            else {
                qDebug() << "Download failed:" << reply->errorString();
            }

            reply->deleteLater();
            });
    }
}