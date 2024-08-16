#ifndef RECENTCOPYPHONEDIALOG_H
#define RECENTCOPYPHONEDIALOG_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class RecentCopyPhoneDialog;
}
#define COPY_STATUS_ITEM_WIDTH          200
#define COPY_STATUS_ITEM_HEIGHT         30
class CopyPhoneItem : public QWidget
{
    Q_OBJECT
public:
    CopyPhoneItem(int iOrderIndex, QString strPhoneName, QWidget* parent = nullptr);

private:
    QLabel* m_LabelOrder;
    QLabel* m_LabelPhoneName;
};

class RecentCopyPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentCopyPhoneDialog(QWidget *parent = nullptr);
    ~RecentCopyPhoneDialog();

private slots:
    void on_btnOK_clicked();
    void on_btnClose_clicked();
private:
    void InitListWidget();
private:
    Ui::RecentCopyPhoneDialog *ui;
};

#endif // RECENTCOPYPHONEDIALOG_H
