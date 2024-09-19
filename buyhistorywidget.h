#ifndef BUYHISTORYWIDGET_H
#define BUYHISTORYWIDGET_H

#include "global.h"

namespace Ui {
class BuyHistoryWidget;
}

class BuyHistoryWidget : public QMoveDialog
{
    Q_OBJECT

public:
    explicit BuyHistoryWidget(QWidget *parent = nullptr);
    ~BuyHistoryWidget();

private slots:
    void on_toolBtnClearBuyHistory_clicked();

    void on_btnClose_clicked();

    //显示数据
    void ShowOrderInfoList(QMap<int, S_ORDER_INFO> mapOrderInfo);
private:
    Ui::BuyHistoryWidget *ui;
};

#endif // BUYHISTORYWIDGET_H
