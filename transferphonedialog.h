#ifndef TRANSFERPHONEDIALOG_H
#define TRANSFERPHONEDIALOG_H

#include <QDialog>

namespace Ui {
class TransferPhoneDialog;
}

class TransferPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferPhoneDialog(QWidget *parent = nullptr);
    ~TransferPhoneDialog();

private slots:
    void on_btnClose_clicked();

private:
    Ui::TransferPhoneDialog *ui;
};

#endif // TRANSFERPHONEDIALOG_H
