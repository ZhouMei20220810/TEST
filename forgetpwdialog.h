#ifndef FORGETPWDIALOG_H
#define FORGETPWDIALOG_H

#include <QDialog>

namespace Ui {
class ForgetPWDialog;
}

class ForgetPWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetPWDialog(QWidget *parent = nullptr);
    ~ForgetPWDialog();

private slots:
    void on_btnReturn_clicked();

    void on_btnClose_clicked();

private:
    Ui::ForgetPWDialog *ui;
};

#endif // FORGETPWDIALOG_H
