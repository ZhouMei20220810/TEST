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
    
signals:
    void addActiveCodeSignals(QStringList strActiveCodeList);
private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();

    void on_editActiveCode_textChanged();

private:
    Ui::AddActiveCodeDialog *ui;    
};

#endif // ADDACTIVECODEDIALOG_H
