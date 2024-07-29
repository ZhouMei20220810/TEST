#ifndef MESSAGECENTERDIALOG_H
#define MESSAGECENTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class MessageCenterDialog;
}

class MessageCenterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageCenterDialog(QWidget *parent = nullptr);
    ~MessageCenterDialog();

private slots:
    //公告
    void on_btnAnnouncement_clicked();
    //活动
    void on_btnActivity_clicked();
    void on_btnClose_clicked();
private:
    void HttpGetNoticeListInfo(bool bIsAuth, int iPage, int iPageSize);
private:
    Ui::MessageCenterDialog *ui;
    //id
    QMap<int, S_NOTICE_INFO> m_mapNotice;
};

#endif // MESSAGECENTERDIALOG_H
