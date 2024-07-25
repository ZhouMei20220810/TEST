#ifndef TRANSFERPHONEDIALOG_H
#define TRANSFERPHONEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class TransferPhoneDialog;
}

class TransferPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferPhoneDialog(QMap<int, S_PHONE_INFO> mapPhoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent = nullptr);
    ~TransferPhoneDialog();

private slots:
    void on_btnClose_clicked();
private:
    void LoadWidgetData(QMap<int, S_PHONE_INFO> mapPhoneInfo);
private:
    Ui::TransferPhoneDialog *ui;
    //int会员等级
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
};

#endif // TRANSFERPHONEDIALOG_H
