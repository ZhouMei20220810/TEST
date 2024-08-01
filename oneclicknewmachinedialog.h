#ifndef ONECLICKNEWMACHINEDIALOG_H
#define ONECLICKNEWMACHINEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class OneClickNewMachineDialog;
}

class OneClickNewMachineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OneClickNewMachineDialog(QWidget *parent = nullptr);
    ~OneClickNewMachineDialog();
private:
    void HttpGetModelList(QString strBrand, int iPage, int iPageSize);
    //根据品牌过滤
    void LoadBrandList(QMap<QString, S_BRAND_INFO> map);

    void LoadModelList(QMap<QString, S_BRAND_INFO> map);
private slots:
    void on_btnClose_clicked();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_comboBoxBrand_currentIndexChanged(int index);

private:
    Ui::OneClickNewMachineDialog *ui;
};

#endif // ONECLICKNEWMACHINEDIALOG_H
