#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

#include <QWidget>
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include <QTimer>
#include <QCheckBox>
#include <QLabel>
#include "phoneinstancewidget.h"

namespace Ui {
class PhoneItemWidget;
}

class PhoneItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneItemWidget(S_PHONE_INFO sTaskInfo,QWidget *parent = nullptr);
    ~PhoneItemWidget();
    void startRequest(QUrl url);
    void setRefreshTimer(bool bIsRefresh);
    void setCheckBoxStatus(bool bCheck);
    bool getCheckBoxStatus();
private slots:
    void httpFinished();//文件接收完成
    void httpReadyRead();//接受数据中
    void updateDataReadProgress(qint64, qint64);//进度条更新
    //一分钟响应一次
    //void do_timeoutRefreshPicture();
private:
    Ui::PhoneItemWidget *ui;

    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QFile* file;
    QUrl url;

    S_PHONE_INFO m_sTaskInfo;
    QByteArray byteArrayImageUrl;
    QString  m_strPicturePath;
    QString  m_strTemp;

    //设置定时器
    QTimer* m_refreshTimer;
    bool m_bIsRefresh;

    QLabel* m_LabelAccredit;
    QCheckBox* m_checkBox;

    PhoneInstanceWidget* m_PhoneInstanceWidget;
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // PHONEITEMWIDGET_H
