#include "trichtextdisplay.h"
#include <QTextEdit>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QRegularExpression>
#include <QByteArray>
#include <QBuffer>
#include "global.h"

TRichTextDisplay::TRichTextDisplay(QWidget* parent):QTextEdit(parent)
{
    //设置为只读
    setReadOnly(true);
    //禁止文本选中
    setTextInteractionFlags(Qt::NoTextInteraction);
    //禁止鼠标光标变为文本选择光标
    viewport()->setCursor(Qt::ArrowCursor);
}

void TRichTextDisplay::RefreshUIData(QString strRemark)
{
    // 富文本内容
    /*QString htmlContent = "<html><body>"
        "<p><img src=\"https://officia-file.oss-cn-beijing.aliyuncs.com/renew-list.png?Expires=1722852466&amp;OSSAccessKeyId=LTAI5t9dS32WzeVB148iWsZV&amp;Signature=0pUyF1JMRos2fEi%2BLhttFDPxoZo%3D\"></p>"
        "<p>今天是2024年8月5日</p>"
        "<p>天气🌤</p>"
        "</body></html>";*/
    QString htmlContent = QString("<html><body>%1</body></html>").arg(strRemark);

    // 使用正则表达式提取图像的URL
    QRegularExpression re("<img[^>]*src\\s*=\\s*\"([^\"]*)\"");
    QRegularExpressionMatchIterator matches = re.globalMatch(htmlContent);

    // 创建QNetworkAccessManager实例
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QMap<QString, QNetworkReply*> imageRequests;
    // 连接网络请求完成的信号
    connect(networkManager, &QNetworkAccessManager::finished, this, &TRichTextDisplay::onImageDownloaded);
    if (matches.hasNext()) 
    {
        QRegularExpressionMatch match = matches.next();
        // 图像URL
        QString imageUrl = match.captured(1);

        QNetworkRequest request;
        QUrl url(imageUrl);
        qDebug() << "url:" << imageUrl;
        request.setUrl(url);

        QNetworkReply* reply =  networkManager->get(request);

        //保存请求与URL的对应关系
        imageRequests.insert(imageUrl, reply);
    }
    else {
        qDebug() << "No image URL found.";
    }

    // 设置富文本内容（初始状态）
    setHtml(htmlContent);
    m_imageRequests = imageRequests;
}

void TRichTextDisplay::onImageDownloaded(QNetworkReply* reply)
{
    // 检查回复的状态码
    if (reply->error() == QNetworkReply::NoError) {
        // 读取图像数据
        QByteArray imageData = reply->readAll();

        // 创建QPixmap对象
        QPixmap pixmap;
        pixmap.loadFromData(imageData);

        // 将QPixmap转换为QByteArray
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        pixmap.save(&buffer, "PNG");

        // 将QByteArray转换为Base64编码的字符串
        QByteArray base64ImageData = byteArray.toBase64();

        //获取图像URL
        QUrl url = reply->url();
        QString imageUrl = url.toString();
        // 更新富文本内容
        updateHtmlContent(imageUrl, base64ImageData);
        /*QString htmlContent = "<html><body>"
                              "<p><img src=\"data:image/png;base64," + base64ImageData.toPercentEncoding() + "\"></p>"
                                                                      "<p>今天是2024年8月5日</p>"
                                                                      "<p>天气🌤</p>"
                                                                      "</body></html>";

        // 设置富文本内容
        setHtml(htmlContent);*/
    }
    else {
        qDebug() << "Failed to download image: " << reply->errorString().toStdString();
    }

    // 清理资源
    reply->deleteLater();
}

void TRichTextDisplay::updateHtmlContent(const QString& imageUrl, const QByteArray& base64ImageData)
{
    // 更新富文本内容
    QString htmlContent = toHtml();

    // 替换图像URL为Base64编码的字符串
    QString base64ImageSrc = "data:image/png;base64," + base64ImageData.toPercentEncoding();
    htmlContent.replace(imageUrl, base64ImageSrc);

    // 设置新的富文本内容
    setHtml(htmlContent);
}