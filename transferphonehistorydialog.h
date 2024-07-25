#ifndef TRANSFERPHONEHISTORYDIALOG_H
#define TRANSFERPHONEHISTORYDIALOG_H

#include <QDialog>

namespace Ui {
class TransferPhoneHistoryDialog;
}

class TransferPhoneHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferPhoneHistoryDialog(QWidget *parent = nullptr);
    ~TransferPhoneHistoryDialog();

private slots:
    void on_btnClose_clicked();
private:
    // �ҵ�֧������
    void HttpGetTransferHistory(int iPage, int iPageSize);
    //��ʾ����
    void ShowTransferInfoList();
private:
    Ui::TransferPhoneHistoryDialog *ui;
};

#endif // TRANSFERPHONEHISTORYDIALOG_H
