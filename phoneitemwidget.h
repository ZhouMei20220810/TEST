#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

#include <QWidget>
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include <QTimer>

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
    void setRefreshTimer(bool bIsRefresh);

private slots:
    void httpFinished();//文件接收完成
    void httpReadyRead();//接受数据中
    void updateDataReadProgress(qint64, qint64);//进度条更新
    //一分钟响应一次
    void do_timeoutRefreshPicture();
private:
    Ui::PhoneItemWidget *ui;

    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QFile* file;
    QUrl url;

    S_TASK_INFO m_sTaskInfo;
    QByteArray byteArrayImageUrl;
    QString  m_strPicturePath;

    //设置定时器
    QTimer* m_refreshTimer;
    bool m_bIsRefresh;
};

#endif // PHONEITEMWIDGET_H
