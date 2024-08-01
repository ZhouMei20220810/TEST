#ifndef ONECLICKNEWMACHINEDIALOG_H
#define ONECLICKNEWMACHINEDIALOG_H

#include <QDialog>

namespace Ui {
class OneClickNewMachineDialog;
}

class OneClickNewMachineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OneClickNewMachineDialog(QWidget *parent = nullptr);
    ~OneClickNewMachineDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::OneClickNewMachineDialog *ui;
};

#endif // ONECLICKNEWMACHINEDIALOG_H
