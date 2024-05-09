#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "passwordloginpage.h"
#include "smsloginpage.h"
#include "registerpage.h"
#include "mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    void showPage(ENUM_LOGIN_PAGE_TYPE type);
private slots:
    void on_pushButton_clicked();

    void do_LoginHttpResponseSignals(QByteArray response);
private:
    Ui::LoginWindow *ui;

    PasswordLoginPage* m_passwordLoginPage;
    SMSLoginPage* m_smsLoginPage;
    RegisterPage* m_registerPage;
    MainWindow* m_mainWindow;
};
#endif // LOGINWINDOW_H
