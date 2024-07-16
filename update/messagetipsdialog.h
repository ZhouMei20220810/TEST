#ifndef MESSAGETIPSDIALOG_H
#define MESSAGETIPSDIALOG_H

#include <QDialog>

namespace Ui {
class MessageTipsDialog;
}

class MessageTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageTipsDialog(QString strShow="默认显示",QWidget *parent = nullptr);
    ~MessageTipsDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_btnOK_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MessageTipsDialog *ui;
};

#endif // MESSAGETIPSDIALOG_H
