#ifndef REPLACECLOUDPHONEDIALOG_H
#define REPLACECLOUDPHONEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class ReplaceCloudPhoneDialog;
}

class ReplaceCloudPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceCloudPhoneDialog(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent = nullptr);
    ~ReplaceCloudPhoneDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();
    void do_replaceItemCheckBoxStatus(bool checked);
    void on_checkBoxAll_clicked(bool checked);

private:
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel);
    void HttpPostReplaceInstance(QMap<int, int> map);
    void ShowPhoneInfo(QMap<int, S_PHONE_INFO> mapPhoneInfo);
    void LoadReplaceInstanceStatus(QMap<int, S_REPLACE_INFO> map);
private:
    Ui::ReplaceCloudPhoneDialog *ui;
    QMap<int, S_PHONE_INFO> m_mapPhone;
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
    int         m_iCurSelCount;
};

#endif // REPLACECLOUDPHONEDIALOG_H
