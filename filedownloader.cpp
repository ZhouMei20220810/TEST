#include "filedownloader.h"
#include "global.h"

FileDownloader::FileDownloader(QObject *parent)
    : QThread{parent},m_url(""), m_outputFile("")
{

}

void FileDownloader::setUrl(const QString &url, const QString &outputFile)
{
    m_url = url;
    QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
    qDebug() << "url=" << url << "strFileName=" << strFileName;
    m_outputFile = GlobalData::strPictureTempDir + strFileName;
}

void FileDownloader::run()
{
    // 如果URL或输出文件为空，则发出错误信号并返回
    if (m_url.isEmpty() || m_outputFile.isEmpty()) {
        emit downloadFinished(false, "url或保存路径有误");
        return;
    }

    QUrl                    qurl(m_url);
    QNetworkRequest         xrqt(qurl);
    QNetworkAccessManager   manager;
    QNetworkReply           *reply = NULL;
    QTimer                  timeoutTimer;
    QEventLoop              loop;

    // 如果网络请求失败，则发出错误信号并返回
    reply = manager.get(xrqt);
    if (!reply) {
        emit downloadFinished(false, "连接服务器失败.");
        return;
    }

    // 创建超时定时器
    timeoutTimer.setSingleShot(true);
    connect(&timeoutTimer, &QTimer::timeout, [&](){
        // 如果超时，则中止下载
        reply->abort();
    });

    // 连接下载进度信号
    connect(reply, &QNetworkReply::downloadProgress, [&](qint64 bytesReceived, qint64 bytesTotal){
        emit downloadProgress(bytesReceived, bytesTotal);
        // 更新超时计时器
        timeoutTimer.start(10000); // 重启超时定时器
    });

    // 连接下载完成信号
    connect(reply, &QNetworkReply::finished, [&](){
        // 停止超时计时器
        timeoutTimer.stop();

        // 根据网络响应的状态进行处理
        if (reply->error() == QNetworkReply::NoError)
        {
            QFile   file(m_outputFile);
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(reply->readAll());
                file.close();
                emit downloadFinished(true, "下载成功.");
            }
            else
            {
                emit downloadFinished(false, "下载完毕,但写入文件失败.");
            }
        }
        else
        {
            emit downloadFinished(false, reply->errorString());
        }

        // 删除网络回复对象
        reply->deleteLater();
    });

    // 启动超时定时器
    timeoutTimer.start(10000); // 设置初始超时时间为10秒

    // 开启事件循环
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 如果超时计时器仍在运行，则说明下载超时
    if (timeoutTimer.isActive())
    {
        timeoutTimer.stop(); // 停止超时计时器
        emit downloadFinished(false, "下载超时.");
    }
}
