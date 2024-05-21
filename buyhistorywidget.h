#ifndef BUYHISTORYWIDGET_H
#define BUYHISTORYWIDGET_H

#include <QWidget>

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

private:
    //订单接口
    // 我的支付订单
    void HttpGetMyOrder(int iPage, int iPageSize);
    //删除
    void HttpDeleteOrder(int iOrderId);
    //清空
    void HttpEmptyOrder();

    Ui::BuyHistoryWidget *ui;
};

#endif // BUYHISTORYWIDGET_H
