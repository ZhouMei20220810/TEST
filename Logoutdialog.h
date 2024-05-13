#ifndef LOGOUTDIALOG_H
#define LOGOUTDIALOG_H

#include <QDialog>

namespace Ui {
class MyDialog;
}

class LogoutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogoutDialog(QWidget *parent = nullptr);
    ~LogoutDialog();

private slots:
    void on_toolButton3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();


private:
    Ui::MyDialog *ui;
};

#endif // LOGOUTDIALOG_H
