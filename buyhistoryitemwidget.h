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

private:
    Ui::BuyHistoryItemWidget *ui;
};

#endif // BUYHISTORYITEMWIDGET_H
