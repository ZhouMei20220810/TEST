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

    m_PhoneInstanceWidget = NULL;

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
    if(event->button() == Qt::LeftButton)
    {        
        if (NULL == m_PhoneInstanceWidget)
        {
            m_PhoneInstanceWidget = new PhoneInstanceWidget(m_sPhoneInfo);
        }
        if (!GlobalData::bVerticalPhoneInstanceCenter)
        {
            m_PhoneInstanceWidget->move(GlobalData::pointPhoneInstance);
        }

        m_PhoneInstanceWidget->setModal(true);
        m_PhoneInstanceWidget->show();
        m_PhoneInstanceWidget = NULL;
    }
    QWidget::mousePressEvent(event);
}
