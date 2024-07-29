#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QEventLoop>
#include <QTimer>

class FileDownloader : public QThread
{
    Q_OBJECT
public:
    explicit FileDownloader(QObject *parent = nullptr);
    void setUrl(const QString &url, const QString &outputFile);
    void setUrlOutputFile(const QString& url, const QString& outputFile);

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(bool success, QString errorMessage);

protected:
    void run() override;

private:
    QString m_url;
    QString m_outputFile;
};

#endif // FILEDOWNLOADER_H
