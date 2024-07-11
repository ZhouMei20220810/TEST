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
private:
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel);
    void ShowPhoneInfo(QMap<int, S_PHONE_INFO> mapPhoneInfo);
private:
    Ui::ReplaceCloudPhoneDialog *ui;
    QMap<int, S_PHONE_INFO> m_mapPhoneInfo;
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
};

#endif // REPLACECLOUDPHONEDIALOG_H
