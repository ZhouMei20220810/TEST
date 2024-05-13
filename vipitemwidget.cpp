#include "vipitemwidget.h"
#include "ui_vipitemwidget.h"
#include "global.h"

VIPItemWidget::VIPItemWidget(VIP_TYPE enVipType,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VIPItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_VIP_WIDTH,ITEM_WIDGET_VIP_HEIGHT);
    m_enVipType = enVipType;

    ui->labelCheck->move(this->width() - ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->toolButton);

    ui->labelCheck->setText(QString("%1").arg(enVipType));
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
    qDebug() << "select vip type=" << m_enVipType;
    emit selectVIPTypeSignals(m_enVipType);
}

