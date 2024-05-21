#ifndef BUYHISTORYWIDGET_H
#define BUYHISTORYWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class BuyHistoryWidget;
}

class BuyHistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyHistoryWidget(QWidget *parent = nullptr);
    ~BuyHistoryWidget();

private slots:
    void on_toolBtnClearBuyHistory_clicked();

    void on_btnClose_clicked();

private:
    //订单接口
    // 我的支付订单
    void HttpGetMyOrder(int iPage, int iPageSize);
    //删除
    void HttpDeleteOrder(int iOrderId);
    //清空
    void HttpEmptyOrder();

    //显示数据
    void ShowOrderInfoList();

    Ui::BuyHistoryWidget *ui;

    //int序号
    QMap<int, S_ORDER_INFO> m_mapOrderInfo;
};

#endif // BUYHISTORYWIDGET_H
