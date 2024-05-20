#include "vipitemwidget.h"
#include "ui_vipitemwidget.h"
#include "global.h"

VIPItemWidget::VIPItemWidget(S_VIP_ITEM_INFO sVipInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VIPItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_VIP_WIDTH,ITEM_WIDGET_VIP_HEIGHT);
    m_sVipInfo = sVipInfo;
    ui->labelCheck->move(this->width() - ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->toolButton);
    ui->labelCheck->setStyleSheet("border:none;background:transparent;");
    //ui->labelCheck->setText(QString("%1").arg(enVipType));

    ui->labelTotalTime->setText(QString("%1").arg(sVipInfo.strVipText));
    QString strTmp;
    strTmp = strTmp.asprintf("%.2f",sVipInfo.fTotalPrice);
    QStringList strValueList = strTmp.split('.');
    if (strValueList.size() > 1)
    {
        ui->labelTotalPrice0->setText(strValueList.at(0));
        ui->labelTotalPrice->setText(QString(".%1").arg(strValueList.at(1)));
    }
    

    strTmp = strTmp.asprintf("%.2f元/天",sVipInfo.fTotalPrice/sVipInfo.iDayCount);
    ui->labelDayPrice->setText(strTmp);
    //QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    //ui->labelBg->setStyleSheet(strStyleSheet)
}

VIPItemWidget::~VIPItemWidget()
{
    qDebug()<<"delete VIPItemWidget";
    delete ui;
}

void VIPItemWidget::setLabelCheckStatus(bool bCheck)
{
    ui->labelCheck->setVisible(bCheck);
    ui->toolButton->setStyleSheet("QToolButton{background-color:#FFE6DDE5;border:4px;border-color:red;}");
    qDebug() << "VIPItemWidget labelCheck status<<" << bCheck;
}

void VIPItemWidget::on_toolButton_clicked()
{
    ui->labelCheck->setVisible(true);

    ui->toolButton->setStyleSheet("QToolButton{background-color:#FFE0D3D3;border:4px;border-color:#FF000000;}");
    qDebug() << "select vip type=" << m_sVipInfo.vipType;
    emit selectVIPTypeSignals(m_sVipInfo);
}

