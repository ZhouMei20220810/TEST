#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

#include <QWidget>
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>

namespace Ui {
class PhoneItemWidget;
}

class PhoneItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneItemWidget(S_TASK_INFO sTaskInfo,QWidget *parent = nullptr);
    ~PhoneItemWidget();
    void startRequest(QUrl url);

private slots:
    void httpFinished();//文件接收完成
    void httpReadyRead();//接受数据中
    void updateDataReadProgress(qint64, qint64);//进度条更新
    
    void do_DownloadImageFinish();
private:
    Ui::PhoneItemWidget *ui;

    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QFile* file;
    QUrl url;

    S_PHONE_INFO m_sPhoneInfo;
    S_TASK_INFO m_sTaskInfo;
    QByteArray byteArrayImageUrl;
    QString  m_strPicturePath;
};

#endif // PHONEITEMWIDGET_H
