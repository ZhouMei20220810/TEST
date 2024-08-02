#ifndef RECENTCOPYCUTCONTENTDIALOG_H
#define RECENTCOPYCUTCONTENTDIALOG_H

#include <QDialog>

namespace Ui {
class RecentCopyCutContentDialog;
}

class RecentCopyCutContentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentCopyCutContentDialog(QWidget *parent = nullptr);
    ~RecentCopyCutContentDialog();

private slots:
    void on_btnCopyToPhone_clicked();

    void on_btnDirectCopy_clicked();

    void on_btnCopyByOrder_clicked();

    void on_btnClose_clicked();

private:
    Ui::RecentCopyCutContentDialog *ui;
};

#endif // RECENTCOPYCUTCONTENTDIALOG_H
