#ifndef RECENTCOPYSTATUSDIALOG_H
#define RECENTCOPYSTATUSDIALOG_H

#include "qmovedialog.h"
#include <QLabel>
#include "global.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class RecentCopyStatusDialog;
}

#define COPY_STATUS_ITEM_WIDTH          200
#define COPY_STATUS_ITEM_HEIGHT         30

class CopyStatusItem : public QWidget
{
    Q_OBJECT
public:
    CopyStatusItem(S_PHONE_INFO info, EN_COPY_STATUS enType, QWidget* parent = nullptr);
    ~CopyStatusItem();

    void setCopyStatus(EN_COPY_STATUS enType);
    EN_COPY_STATUS getCopyStatus();
private:
    QLabel* m_LabelInstance;
    QLabel*  m_LabelStatus;
    EN_COPY_STATUS m_enStatus;
};

class RecentCopyStatusDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit RecentCopyStatusDialog(QWidget *parent = nullptr);
    ~RecentCopyStatusDialog();
public slots:
    void do_addCopyStatusSignals(S_RECENT_COPY_STATUS info);
private slots:
    void on_btnClose_clicked();

    void on_btnOK_clicked();
private:
    //统计拷贝状态结果
    void calcCopyStatusResult();

private:
    Ui::RecentCopyStatusDialog *ui;
};

#endif // RECENTCOPYSTATUSDIALOG_H
