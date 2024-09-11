#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include "global.h"

// 下载图片的线程任务
class ImageDownloader : public QObject
{
    Q_OBJECT
public:
    explicit ImageDownloader(QNetworkAccessManager* manager, QObject* parent = nullptr);

    void downloadImages(QMap<QString, S_TASK_INFO> mapTask);

private:    
    QNetworkAccessManager* m_manager;
};
#endif // IMAGEDOWNLOADER_H
