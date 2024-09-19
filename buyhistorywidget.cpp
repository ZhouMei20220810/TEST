#include "buyhistorywidget.h"
#include "ui_buyhistorywidget.h"
#include "messagetipsdialog.h"
#include "buyhistoryitemwidget.h"
#include "toolobject.h"
#include <QGraphicsDropShadowEffect>

BuyHistoryWidget::BuyHistoryWidget(QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::BuyHistoryWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("购买记录");

    setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    //获取我的订单
    connect(ToolObject::getInstance(), &ToolObject::ShowOrderInfoListSignals, this, &BuyHistoryWidget::ShowOrderInfoList);
    ToolObject::getInstance()->HttpGetMyOrder(1, 1000);
}

BuyHistoryWidget::~BuyHistoryWidget()
{
    delete ui;
}

void BuyHistoryWidget::on_toolBtnClearBuyHistory_clicked()
{
    //清空购买记录
    ToolObject::getInstance()->HttpEmptyOrder();
}

//显示数据
void BuyHistoryWidget::ShowOrderInfoList(QMap<int, S_ORDER_INFO> mapOrderInfo)
{
    if (mapOrderInfo.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("没有购买记录!");
        tips->show();
        return;
    }

    QListWidgetItem* item;
    BuyHistoryItemWidget* itemWidget;
    QMap<int, S_ORDER_INFO>::iterator iter = mapOrderInfo.begin();
    for (; iter != mapOrderInfo.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetBuyHistory);
        ui->listWidgetBuyHistory->addItem(item);

        itemWidget = new BuyHistoryItemWidget(iter.value(),this);
        item->setSizeHint(QSize(ui->listWidgetBuyHistory->width()-50, itemWidget->height()));
        ui->listWidgetBuyHistory->setItemWidget(item, itemWidget);        
    }
}

void BuyHistoryWidget::on_btnClose_clicked()
{
    this->close();
}

