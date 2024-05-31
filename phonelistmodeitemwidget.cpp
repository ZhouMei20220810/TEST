#include "phonelistmodeitemwidget.h"
#include "ui_phonelistmodeitemwidget.h"

PhoneListModeItemWidget::PhoneListModeItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneListModeItemWidget)
{
    ui->setupUi(this);
}

PhoneListModeItemWidget::~PhoneListModeItemWidget()
{
    delete ui;
}
