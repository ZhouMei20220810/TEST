#ifndef ADDAUTHORIZATIONDIALOG_H
#define ADDAUTHORIZATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AddAuthorizationDialog;
}

class AddAuthorizationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAuthorizationDialog(QWidget *parent = nullptr);
    ~AddAuthorizationDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();

private:
    Ui::AddAuthorizationDialog *ui;
};

#endif // ADDAUTHORIZATIONDIALOG_H
