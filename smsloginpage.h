#ifndef SMSLOGINPAGE_H
#define SMSLOGINPAGE_H

#include <QWidget>
#include "global.h"
#include "mainwindow.h"
namespace Ui {
class SMSLoginPage;
}

class SMSLoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit SMSLoginPage(QWidget *parent = nullptr);
    ~SMSLoginPage();
signals:
    void showPageType(ENUM_LOGIN_PAGE_TYPE type);
private slots:
    void on_btnPasswordLogin_clicked();

    void on_btnGetSMSCode_clicked();

    void on_btnSMSLogin_clicked();

private:
    Ui::SMSLoginPage *ui;
    MainWindow* m_mainWindow;
};

#endif // SMSLOGINPAGE_H
