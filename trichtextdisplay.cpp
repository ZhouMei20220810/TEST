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
    setReadOnly(true);
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
    QRegularExpressionMatch match = re.match(htmlContent);

    if (match.hasMatch()) {
        // 图像URL
        QString imageUrl = match.captured(1);

        // 创建QNetworkAccessManager实例
        QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);

        // 连接网络请求完成的信号
        connect(networkManager, &QNetworkAccessManager::finished, this, &TRichTextDisplay::onImageDownloaded);

        // 发起网络请求
        //QNetworkRequest request(QUrl(imageUrl));
        //QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        //创建请求对象
        QNetworkRequest request;
        QUrl url(imageUrl);
        qDebug() << "url:" << imageUrl;
        QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
        request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
        request.setUrl(url);

        networkManager->get(request);
    }
    else {
        qDebug() << "No image URL found.";
    }

    // 设置富文本内容（初始状态）
    setHtml(htmlContent);
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

        // 更新富文本内容
        QString htmlContent = "<html><body>"
                              "<p><img src=\"data:image/png;base64," + base64ImageData.toPercentEncoding() + "\"></p>"
                                                                      "<p>今天是2024年8月5日</p>"
                                                                      "<p>天气🌤</p>"
                                                                      "</body></html>";

        // 设置富文本内容
        setHtml(htmlContent);
    }
    else {
        qDebug() << "Failed to download image: " << reply->errorString().toStdString();
    }

    // 清理资源
    reply->deleteLater();
}
