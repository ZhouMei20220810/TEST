#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include "qmovewidget.h"
#include "global.h"
#include <QMouseEvent>

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QMoveWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QMoveWidget *parent = nullptr);
    ~RegisterPage();
signals:
    void showPageType(ENUM_LOGIN_PAGE_TYPE type);
    void logoutSignals();
private slots:
    void on_btnRegister_clicked();

    void on_btnReturn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btnUserPolicy_clicked();

    void on_btnPrivacyPolicy_clicked();

    void on_btnDisclaimer_clicked();

private:
    Ui::RegisterPage *ui;
    QPoint m_dragPosition;
};

#endif // REGISTERPAGE_H
