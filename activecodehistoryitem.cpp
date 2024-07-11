#include "activecodehistoryitem.h"
#include "ui_activecodehistoryitem.h"

ActiveCodeHistoryItem::ActiveCodeHistoryItem(S_ACTIVE_CODE_DETAIL_INFO detailInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ActiveCodeHistoryItem)
{
    ui->setupUi(this);

    ui->labelActiveCode->setText(detailInfo.strActiveCode);
    ui->labelInstanceNo->setText(detailInfo.strInstanceNo);
    ui->labelTime->setText(detailInfo.strInstanceExpireTime);
    if(detailInfo.iType == 1)
        ui->labelType->setText("购买");
    else if (detailInfo.iType == 2)
        ui->labelType->setText("续费");
}

ActiveCodeHistoryItem::~ActiveCodeHistoryItem()
{
    delete ui;
}
