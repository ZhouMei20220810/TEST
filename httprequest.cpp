#include "httprequest.h"


void HttpRequest::onFinished(QNetworkReply *reply) {
    if (reply->error()) {
        // 处理错误
        qDebug() << "Error: " << reply->errorString();
    } else {
        // 处理响应数据
        QString response = reply->readAll();
        qDebug() << "Response: " << response;
    }
    reply->deleteLater();
}
