#include "phonelistmodeitemwidget.h"
#include "ui_phonelistmodeitemwidget.h"
#include <QDateTime>
#include <QMouseEvent>
#include <QSettings>

PhoneListModeItemWidget::PhoneListModeItemWidget(S_PHONE_INFO sPhoneInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneListModeItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    m_sPhoneInfo = sPhoneInfo;

    ui->checkBox->setText(sPhoneInfo.strName);
    ui->labelSystem->setText("Android");

    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime = QDateTime::fromString(sPhoneInfo.strExpireTime, "yyyy-MM-dd hh:mm:ss");
    qint64 mseconds = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();
    QString strTime;
    strTime = strTime.asprintf("%d天%d小时", mseconds / (1000 * 60 * 60 * 24), (mseconds / (1000 * 60 * 60)) % 24);
    ui->labelTime->setText(strTime);
    //ui->labelStatus->setText(sPhoneInfo.)
}

PhoneListModeItemWidget::~PhoneListModeItemWidget()
{
    delete ui;
}


void PhoneListModeItemWidget::mousePressEvent(QMouseEvent *event)
{

    switch (event->button())
    {
    case Qt::LeftButton:
    {
        emit ShowPhoneInstanceWidgetSignals(m_sPhoneInfo, false);
    }
    break;
    case Qt::RightButton:
        emit ShowPhoneInstanceWidgetSignals(m_sPhoneInfo, true);
    default:
        QWidget::mousePressEvent(event);;
    }
    
}
