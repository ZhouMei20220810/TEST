#include "buyhistoryitemwidget.h"
#include "ui_buyhistoryitemwidget.h"

BuyHistoryItemWidget::BuyHistoryItemWidget(S_ORDER_INFO orderInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BuyHistoryItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->labelOrderNo->setText(orderInfo.strOutTradeNo);
    ui->labelMeal->setText(orderInfo.strOrderTitle);
    //实际支付
    ui->labelAmount->setText(QString::asprintf("%.2f",orderInfo.fActualAmount));
    ui->labelCreateTime->setText(orderInfo.strCreateTime);
    ui->labelCloudId->setText(QString::asprintf("%d",orderInfo.iId));
    QString strStatus;//0:未支付 1:已支付 2:已完成
    switch (orderInfo.iStatus) {
    case 0:
        strStatus="未支付";
        break;
    case 1:
        strStatus="已支付";
        break;
    case 2:
        strStatus="已完成";
        break;
    default:
        break;
    }
    ui->labelStatus->setText(strStatus);
    ui->labelBuyNum->setText(QString("%1").arg(orderInfo.iBuyNum));
}

BuyHistoryItemWidget::~BuyHistoryItemWidget()
{
    delete ui;
}
