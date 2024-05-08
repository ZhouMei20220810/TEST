#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QObject>

class HttpRequest : public QObject {
    Q_OBJECT
public:
    HttpRequest(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &HttpRequest::onFinished);
    }

    ~HttpRequest() {
        delete manager;
    }

public slots:
    void get(const QUrl &url) {
        QNetworkRequest request(url);
        QNetworkReply *reply = manager->get(request);
        connect(reply, &QNetworkReply::finished, this, &HttpRequest::onFinished);
    }

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};
#endif // HTTPREQUEST_H
