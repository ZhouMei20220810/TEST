#include "levelitemwidget.h"
#include "ui_levelitemwidget.h"
#include <QPalette>
#include "global.h"
#include "vipitemwidget.h"
#include <QListWidgetItem>

LevelItemWidget::LevelItemWidget(LEVEL_TYPE enType, QString strImage,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelItemWidget)
{
    ui->setupUi(this);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    ui->labelCheck->resize(30,30);

    m_levelType = enType;
    ui->labelCheck->move(this->width()-ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->toolButtonBG);

    QString strStyleSheet = QString("QToolButton{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->toolButtonBG->setStyleSheet(strStyleSheet);
    /*QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->labelBg->setStyleSheet(strStyleSheet);

    ui->labelBg->installEventFilter(this);*/
}

LevelItemWidget::~LevelItemWidget()
{
    delete ui;
}

bool LevelItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    /*if (watched == ui->labelBg)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            ui->labelCheck->setVisible(true);
            qDebug()<<"显示labelCheck";
            //return true;
        }
        else
        {
            return false;
        }
    }
    else*/
    {
        return QWidget::eventFilter(watched, event);
    }
}

void LevelItemWidget::setLabelCheckStatus(bool bCheck)
{
    ui->labelCheck->setVisible(false);
    qDebug()<<"labelCheck status<<"<<bCheck;
}

void LevelItemWidget::on_toolButtonBG_clicked()
{
    ui->labelCheck->setVisible(true);
    //ui->toolButtonBG->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border - radius:3px;background-image:url(:/main/main/level_enhancenment.png);}");
    emit showVIPTypeSignals(m_levelType);
    //加载套餐列表
    //ui->listWidgetVIP
    /*LEVEL_TYPE enType;
    int iVIPType = 0;
    QListWidgetItem* vipItem = NULL;
    VIPItemWidget* vipWidget = NULL;
    for(iVIPType = 0; iVIPType = ITEM_WIDGET_VIP_COUNT;iVIPType++)
    {
        vipItem = new QListWidgetItem(ui->listWidgetVIP);
        vipItem->setSizeHint(QSize(ITEM_WIDGET_VIP_WIDTH, ITEM_WIDGET_VIP_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        vipItem->setData(Qt::UserRole, iVIPType);
        ui->listWidgetVIP->addItem(vipItem);
        vipWidget = new VIPItemWidget((VIP_TYPE)iVIPType,this);
        ui->listWidgetVIP->setItemWidget(vipItem, vipWidget);
    }*/



    //续费的列表
    //ui->listWidgetRenewList
}

