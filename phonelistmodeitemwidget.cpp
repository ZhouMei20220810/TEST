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
    if (sPhoneInfo.strName.isEmpty())
        ui->checkBox->setText(sPhoneInfo.strInstanceNo);
    else
        ui->checkBox->setText(sPhoneInfo.strName);
    ui->checkBox->setChecked(sPhoneInfo.bChecked);
    if(sPhoneInfo.bIsAuth)
    {
        //已授权
        ui->labelAuthImg->setPixmap(QPixmap(":/main/resource/main/Authorized.png"));
    }
    else if(sPhoneInfo.iType == EN_BE_AUTHORIZATION)
    {
        //被授权
        ui->labelAuthImg->setPixmap(QPixmap(":/main/resource/main/BeAuthorized.png"));
    }

    //ui->labelSystem->setText("Android");
    if(sPhoneInfo.strLevelName.compare("BVIP",Qt::CaseInsensitive) == 0)
        ui->toolBtnLevel->setIcon(QIcon(QString(":/main/resource/main/XVIP.png")));
    else
        ui->toolBtnLevel->setIcon(QIcon(QString(":/main/resource/main/%1.png").arg(sPhoneInfo.strLevelName)));
    ui->toolBtnLevel->setText(QString("%1").arg(sPhoneInfo.strLevelName));

    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime = QDateTime::fromString(sPhoneInfo.strExpireTime, "yyyy-MM-dd hh:mm:ss");
    qint64 mseconds = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();
    QString strTime;
    strTime = strTime.asprintf("%d天%d小时", mseconds / (1000 * 60 * 60 * 24), (mseconds / (1000 * 60 * 60)) % 24);
    ui->labelTime->setText(strTime);
   
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &PhoneListModeItemWidget::stateChanged);
}

PhoneListModeItemWidget::~PhoneListModeItemWidget()
{
    delete ui;
}
void PhoneListModeItemWidget::setCheckBoxStatus(bool bCheck)
{
    ui->checkBox->setChecked(bCheck);
}
bool PhoneListModeItemWidget::getCheckBoxStatus()
{
    return ui->checkBox->isChecked();
}
void PhoneListModeItemWidget::setPhoneName(QString strPhoneName)
{
    ui->checkBox->setText(strPhoneName);
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
        GlobalData::enRightClickType = EN_LIST_MODE_WIDGET;
        emit ShowPhoneInstanceWidgetSignals(m_sPhoneInfo, true);
    default:
        QWidget::mousePressEvent(event);;
    }
    
}
