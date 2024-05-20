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

    void on_btnGetCode_clicked();

    void on_btnOK_clicked();

private:
    void HttpPostResetPassword(QString strCode,QString strPassword);

    Ui::ForgetPWDialog *ui;
};

#endif // FORGETPWDIALOG_H
