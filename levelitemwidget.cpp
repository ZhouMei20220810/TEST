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
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    ui->labelCheck->resize(30,30);

    m_levelType = enType;
    ui->labelCheck->move(this->width()-ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->toolButtonBG);

    QString strStyleSheet = QString("QToolButton{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->toolButtonBG->setStyleSheet(strStyleSheet);
    /*QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->labelBg->setStyleSheet(strStyleSheet);*/
}

LevelItemWidget::~LevelItemWidget()
{
    qDebug()<<"delete LevelItemWidget";
    delete ui;
}

void LevelItemWidget::setLabelCheckStatus(bool bCheck)
{
    ui->labelCheck->setVisible(bCheck);
    qDebug()<<"LevelItemWidget labelCheck status="<<bCheck;
}

void LevelItemWidget::on_toolButtonBG_clicked()
{
    ui->labelCheck->setVisible(true);
    //ui->toolButtonBG->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border - radius:3px;background-image:url(:/main/main/level_enhancenment.png);}");
    emit showVIPTypeSignals(m_levelType);
}

