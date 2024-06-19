#ifndef ADDACTIVECODEDIALOG_H
#define ADDACTIVECODEDIALOG_H

#include <QDialog>

namespace Ui {
class AddActiveCodeDialog;
}

class AddActiveCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddActiveCodeDialog(QWidget *parent = nullptr);
    ~AddActiveCodeDialog();

private:
    Ui::AddActiveCodeDialog *ui;
};

#endif // ADDACTIVECODEDIALOG_H
