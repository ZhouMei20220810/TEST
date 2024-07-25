#ifndef TRANSFERTIPSDIALOG_H
#define TRANSFERTIPSDIALOG_H

#include <QDialog>

namespace Ui {
class TransferTipsDialog;
}

class TransferTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferTipsDialog(int iPhoneCount, QString strPhoneOrAccount,QWidget *parent = nullptr);
    ~TransferTipsDialog();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

    void on_btnClose_clicked();

private:
    Ui::TransferTipsDialog *ui;
};

#endif // TRANSFERTIPSDIALOG_H
