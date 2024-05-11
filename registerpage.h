#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include "global.h"

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

private:
    Ui::RegisterPage *ui;
};

#endif // REGISTERPAGE_H
