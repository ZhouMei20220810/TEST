#include "vipitemwidget.h"
#include "ui_vipitemwidget.h"
#include "global.h"

VIPItemWidget::VIPItemWidget(S_LEVEL_DATA_INFO levelInfo, QString strLevelName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VIPItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_VIP_WIDTH,ITEM_WIDGET_VIP_HEIGHT);
    m_levelInfo = levelInfo;
    m_strLevelName = strLevelName;
    m_toolBtn = new QToolButton(this);
    QString strIcon = QString(":/main/resource/buy/%1_bg_normal.png").arg(strLevelName);
    //m_toolBtn->setIcon(QIcon(":/main/resource/main/gvip_bg_normal.png"));
    m_toolBtn->setIcon(QIcon(strIcon));
    m_toolBtn->setIconSize(QSize(ITEM_WIDGET_VIP_WIDTH-2, ITEM_WIDGET_VIP_HEIGHT));
    connect(m_toolBtn, &QToolButton::clicked, this, &VIPItemWidget::on_toolButton_clicked);

    ui->frame->setParent(m_toolBtn);
    ui->labelDayPrice->setParent(m_toolBtn);
    ui->labelTotalTime->setParent(m_toolBtn);

    //ui->toolButton->setIcon(QIcon(":/main/resource/main/gvip_bg_normal.png"));
    //ui->toolButton->setIconSize(QSize(ITEM_WIDGET_VIP_WIDTH, ITEM_WIDGET_VIP_HEIGHT));
    /*ui->labelCheck->move(this->width() - ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->toolButton);
    ui->labelCheck->setStyleSheet("border:none;background:transparent;");*/
    //ui->labelCheck->setText(QString("%1").arg(enVipType));

    ui->labelTotalTime->setText(QString("%1").arg(levelInfo.strMemberName));
    float fPrice = 0;
    QString strTmp;
    if (levelInfo.fActivityPrice>-0.000001 && m_levelInfo.fActivityPrice <0.000001)
    {
        fPrice = levelInfo.fPrice;
    }
    else
    {
        fPrice = levelInfo.fActivityPrice;
    }
    strTmp = strTmp.asprintf("%.2f", fPrice);
    QStringList strValueList = strTmp.split('.');
    if (strValueList.size() > 1)
    {
        ui->labelTotalPrice0->setText(strValueList.at(0));
        ui->labelTotalPrice->setText(QString(".%1").arg(strValueList.at(1)));
    }
    

    strTmp = strTmp.asprintf("%.2f元/天", fPrice/levelInfo.iUseDay);
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
    QString strIcon;
    if (bCheck)
    {
        strIcon= QString(":/main/resource/buy/%1_bg_select.png").arg(m_strLevelName);
    }
    else
    {
        strIcon = QString(":/main/resource/buy/%1_bg_normal.png").arg(m_strLevelName);        
    }
    m_toolBtn->setIcon(QIcon(strIcon));
    //ui->labelCheck->setVisible(bCheck);
    //ui->toolButton->setStyleSheet("QToolButton{background-color:#FFE6DDE5;border:4px;border-color:red;}");
    //qDebug() << "VIPItemWidget labelCheck status<<" << bCheck;
}

void VIPItemWidget::on_toolButton_clicked()
{
    //ui->labelCheck->setVisible(true);
    //m_toolBtn->setIcon(QIcon(":/main/resource/main/gvip_bg_select.png"));    
    QString strIcon= QString(":/main/resource/buy/%1_bg_select.png").arg(m_strLevelName);
    m_toolBtn->setIcon(QIcon(strIcon));
    qDebug() << "select vip type=" << m_levelInfo.iMemberId;
    emit selectVIPTypeSignals(m_levelInfo);
}

