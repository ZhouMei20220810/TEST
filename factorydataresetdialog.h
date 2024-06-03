#ifndef FACTORYDATARESETDIALOG_H
#define FACTORYDATARESETDIALOG_H

#include <QDialog>

namespace Ui {
class FactoryDataResetDialog;
}

class FactoryDataResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FactoryDataResetDialog(QWidget *parent = nullptr);
    ~FactoryDataResetDialog();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

    void on_btnClose_clicked();

private:
    Ui::FactoryDataResetDialog *ui;
};

#endif // FACTORYDATARESETDIALOG_H
