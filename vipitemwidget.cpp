#include "vipitemwidget.h"
#include "ui_vipitemwidget.h"
#include "global.h"

VIPItemWidget::VIPItemWidget(VIP_TYPE enVipType,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VIPItemWidget)
{
    ui->setupUi(this);
    this->resize(ITEM_WIDGET_VIP_WIDTH,ITEM_WIDGET_VIP_HEIGHT);

    ui->label->setText(QString("%1").arg(enVipType));
}

VIPItemWidget::~VIPItemWidget()
{
    delete ui;
}
