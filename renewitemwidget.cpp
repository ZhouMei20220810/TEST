#include "renewitemwidget.h"
#include "ui_renewitemwidget.h"

renewItemWidget::renewItemWidget(S_PHONE_INFO phoneInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::renewItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose,true);
    if(phoneInfo.strName.isEmpty())
        ui->checkBox->setText(phoneInfo.strInstanceNo);
    else
        ui->checkBox->setText(phoneInfo.strName);
    ui->labelTime->setText(phoneInfo.strExpireTime);
}

renewItemWidget::~renewItemWidget()
{
    delete ui;
}

void renewItemWidget::setCheckBoxStatus(bool bCheck)
{
    ui->checkBox->setChecked(bCheck);
}

bool renewItemWidget::getCheckBoxStatus()
{
    return ui->checkBox->isChecked();
}