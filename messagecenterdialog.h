#ifndef MESSAGECENTERDIALOG_H
#define MESSAGECENTERDIALOG_H

#include "global.h"
#include <QListWidgetItem>
#include <QLabel>
#include <QToolButton>

namespace Ui {
class MessageCenterDialog;
}
#define  NOTICE_ITEM_WIDTH      136
#define  NOTICE_ITEM_HEIGHT     32
class NoticeItem :public QWidget
{
    Q_OBJECT
public:
    explicit NoticeItem(S_NOTICE_INFO info, QWidget* parent)
        :QWidget(parent)
    {
        resize(NOTICE_ITEM_WIDTH, NOTICE_ITEM_HEIGHT);
        setupUI(info);
    }
    ~NoticeItem()
    {
        qDebug() << "delete ~NoticeItem()";
    }
signals:
    void itemSelectSignals(S_NOTICE_INFO info);
private slots:
    void do_NoticeItem_clicked(bool checked);
private:
    void setupUI(S_NOTICE_INFO info);
    //设置已读
    void HttpPostSetNoticeRead(int iCreateBy, int iId, NOTICE_TYPE enType);
private:
    QToolButton* m_button;
    S_NOTICE_INFO m_info;
};


class MessageCenterDialog : public QMoveDialog
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
    //获取通知列表
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
