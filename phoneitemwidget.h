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
    void setCheckBoxStatus(bool bCheck);
    bool getCheckBoxStatus();
    void setPhoneName(QString strPhoneName);

    virtual bool eventFilter(QObject* watched, QEvent* event) override;
signals:
    void ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo, bool bShowMenu);
    void stateChanged(int state);
private slots:
    void httpFinished();//文件接收完成
    void httpReadyRead();//接受数据中
    void updateDataReadProgress(qint64, qint64);//进度条更新
private:
    void showLabelImage(QString strImagePath);
    Ui::PhoneItemWidget *ui;

    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;
    QFile* m_File;
    QUrl url;

    S_PHONE_INFO m_sPhoneInfo;
    QByteArray byteArrayImageUrl;
    QString  m_strPicturePath;
    QString  m_strTemp;

    //设置定时器
    QTimer* m_refreshTimer;

    QLabel* m_LabelAccredit;
    QCheckBox* m_checkBox;
};

#endif // PHONEITEMWIDGET_H
