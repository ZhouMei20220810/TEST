#ifndef TRANSFERPHONEHISTORYDIALOG_H
#define TRANSFERPHONEHISTORYDIALOG_H

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "global.h"

#define         REPLACE_LIST_ITEM_WIDTH             337
#define         REPLACE_LIST_ITEM_HEIGHT            20

namespace Ui {
class TransferPhoneHistoryDialog;
}

class TransferHistoryListItem :public QWidget
{
    Q_OBJECT
public:
    explicit TransferHistoryListItem(S_TRANSFER_INFO transferInfo, QWidget* parent)
        :QWidget(parent)
    {
        resize(REPLACE_LIST_ITEM_WIDTH, REPLACE_LIST_ITEM_HEIGHT);
        setupUI(transferInfo);
    }
    ~TransferHistoryListItem()
    {
        qDebug() << "delete ~TransferHistoryListItem()";
    }
signals:
    void replaceItemCheckBoxStatus(bool checked);
private:
    void setupUI(S_TRANSFER_INFO transferInfo);
private:
    QLabel* m_LabelTransferType;
    QLabel* m_LabelTransferAccount;
    QLabel* m_LabelTransferTime;
    QLabel* m_LabelTransferCount;
    QLabel* m_LabelTransferStatus;
};

class TransferPhoneHistoryDialog : public QMoveDialog
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
    void ShowTransferInfoList(QMap<int, S_TRANSFER_INFO> map);
private:
    Ui::TransferPhoneHistoryDialog *ui;
};

#endif // TRANSFERPHONEHISTORYDIALOG_H
