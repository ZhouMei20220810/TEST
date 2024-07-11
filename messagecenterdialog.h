#ifndef MESSAGECENTERDIALOG_H
#define MESSAGECENTERDIALOG_H

#include <QDialog>

namespace Ui {
class MessageCenterDialog;
}

class MessageCenterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageCenterDialog(QWidget *parent = nullptr);
    ~MessageCenterDialog();

private:
    Ui::MessageCenterDialog *ui;
};

#endif // MESSAGECENTERDIALOG_H
