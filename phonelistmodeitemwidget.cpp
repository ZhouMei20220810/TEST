#include "phonelistmodeitemwidget.h"
#include "ui_phonelistmodeitemwidget.h"

PhoneListModeItemWidget::PhoneListModeItemWidget(S_PHONE_INFO sPhoneInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneListModeItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_sPhoneInfo = sPhoneInfo;

    ui->checkBox->setText(sPhoneInfo.strName);
    ui->labelSystem->setText(QString("%1").arg(sPhoneInfo.iType == 8 ? QString("°²×¿") : QString("°²×¿10")));
    ui->labelTime->setText(sPhoneInfo.strExpireTime);
    //ui->labelStatus->setText(sPhoneInfo.)
}

PhoneListModeItemWidget::~PhoneListModeItemWidget()
{
    delete ui;
}
