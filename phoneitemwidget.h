#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class PhoneItemWidget;
}

class PhoneItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneItemWidget(S_TASK_INFO sTaskInfo,QWidget *parent = nullptr);
    explicit PhoneItemWidget(S_PHONE_INFO sPhoneInfo, QWidget* parent = nullptr);
    ~PhoneItemWidget();

private slots:
    void do_DownloadImageFinish();
private:
    Ui::PhoneItemWidget *ui;

    S_PHONE_INFO m_sPhoneInfo;
    S_TASK_INFO m_sTaskInfo;
    QByteArray byteArrayImageUrl;
    QString  m_strPicturePath;
};

#endif // PHONEITEMWIDGET_H
