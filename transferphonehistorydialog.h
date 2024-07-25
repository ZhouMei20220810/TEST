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
    // 我的支付订单
    void HttpGetTransferHistory(int iPage, int iPageSize);
    //显示数据
    void ShowTransferInfoList();
private:
    Ui::TransferPhoneHistoryDialog *ui;
};

#endif // TRANSFERPHONEHISTORYDIALOG_H
