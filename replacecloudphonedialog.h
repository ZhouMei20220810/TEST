#ifndef REPLACECLOUDPHONEDIALOG_H
#define REPLACECLOUDPHONEDIALOG_H

#include <QDialog>

namespace Ui {
class ReplaceCloudPhoneDialog;
}

class ReplaceCloudPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceCloudPhoneDialog(QWidget *parent = nullptr);
    ~ReplaceCloudPhoneDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();

private:
    Ui::ReplaceCloudPhoneDialog *ui;
};

#endif // REPLACECLOUDPHONEDIALOG_H
