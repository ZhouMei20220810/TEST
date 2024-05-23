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
    void httpFinished();//�ļ��������
    void httpReadyRead();//����������
    void updateDataReadProgress(qint64, qint64);//����������
    //һ������Ӧһ��
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

    //���ö�ʱ��
    QTimer* m_refreshTimer;
    bool m_bIsRefresh;
};

#endif // PHONEITEMWIDGET_H
