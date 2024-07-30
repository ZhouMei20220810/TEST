#ifndef MESSAGECENTERDIALOG_H
#define MESSAGECENTERDIALOG_H

#include <QDialog>
#include "global.h"
#include <QListWidgetItem>
#include <QLabel>

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
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    void HttpGetNoticeListInfo(NOTICE_TYPE enType, int iPage, int iPageSize);
    void LoadNoticeInfoList(NOTICE_TYPE enType);
private:
    Ui::MessageCenterDialog *ui;
    //id
    QMap<int, S_NOTICE_INFO> m_mapNotice;
    QLabel* m_LabelActivityPoint;
    QLabel* m_LabelAnnouncementPoint;
};

#endif // MESSAGECENTERDIALOG_H
