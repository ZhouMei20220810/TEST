#ifndef POLICYDIALOG_H
#define POLICYDIALOG_H

#include <QDialog>

namespace Ui {
class PolicyDialog;
}

class PolicyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolicyDialog(QString strTitle,QWidget *parent = nullptr);
    ~PolicyDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::PolicyDialog *ui;
};

#endif // POLICYDIALOG_H
