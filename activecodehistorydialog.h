#ifndef ACTIVECODEHISTORYDIALOG_H
#define ACTIVECODEHISTORYDIALOG_H

#include "global.h"

namespace Ui {
class ActiveCodeHistoryDialog;
}
//¼¤»îÂë¼ÇÂ¼
class ActiveCodeHistoryDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit ActiveCodeHistoryDialog(QWidget *parent = nullptr);
    ~ActiveCodeHistoryDialog();

private slots:
    void on_btnClose_clicked();
private:
    void HttpGetAcitveCodeHistory(int iPage, int iPageSize);
    void ShowActiveCodeInfoList();
private:
    Ui::ActiveCodeHistoryDialog *ui;
    QMap<int, S_ACTIVE_CODE_DETAIL_INFO> m_mapActiveInfo;
};

#endif // ACTIVECODEHISTORYDIALOG_H
