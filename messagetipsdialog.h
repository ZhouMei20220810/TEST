#ifndef MESSAGETIPSDIALOG_H
#define MESSAGETIPSDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class MessageTipsDialog;
}

class MessageTipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageTipsDialog(QString strShow="默认显示",QWidget *parent = nullptr, MESSAGE_NOT_TIPS_TYPE type=MESSAGE_NOT_TIPS_NORMAL,QString strTitle="");
    ~MessageTipsDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_btnOK_clicked();

private:
    Ui::MessageTipsDialog *ui;

    MESSAGE_NOT_TIPS_TYPE m_enType;
};

#endif // MESSAGETIPSDIALOG_H
