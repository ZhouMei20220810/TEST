#ifndef BUYHISTORYITEMWIDGET_H
#define BUYHISTORYITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class BuyHistoryItemWidget;
}

class BuyHistoryItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyHistoryItemWidget(S_ORDER_INFO orderInfo,QWidget *parent = nullptr);
    ~BuyHistoryItemWidget();

private slots:
    void on_btnClear_clicked();

private:
    //É¾³ý
    void HttpDeleteOrder(int iOrderId);

    S_ORDER_INFO m_orderInfo;
    Ui::BuyHistoryItemWidget *ui;
};

#endif // BUYHISTORYITEMWIDGET_H
