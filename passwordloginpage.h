#ifndef PASSWORDLOGINPAGE_H
#define PASSWORDLOGINPAGE_H

#include <QWidget>
#include "global.h"
#include "forgetpwdialog.h"

namespace Ui {
class PasswordLoginPage;
}

class PasswordLoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordLoginPage(QWidget *parent = nullptr);
    ~PasswordLoginPage();
signals:
    void showPageType(ENUM_LOGIN_PAGE_TYPE type);
    void showLoginWindow(ENUM_LOGIN_STATUS enStatus);
    void closeWindowSignals();
private slots:
    //void on_btnReturn_clicked();

    void on_btnRegister_clicked();

    void on_btnSMSLogin_clicked();

    void on_btnLogin_clicked();

    void on_btnForgetPW_clicked();

    void on_checkBoxAutoLogin_clicked(bool checked);

private:
    Ui::PasswordLoginPage *ui;
    ForgetPWDialog* m_forgetPWDialog;
};

#endif // PASSWORDLOGINPAGE_H
