#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include "global.h"
#include <QMouseEvent>

namespace Ui {
class RegisterPage;
}

class RegisterPage : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();
signals:
    void showPageType(ENUM_LOGIN_PAGE_TYPE type);
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
    // QWidget interface
/*protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;*/
};

#endif // REGISTERPAGE_H
