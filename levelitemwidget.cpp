#include "levelitemwidget.h"
#include "ui_levelitemwidget.h"
#include "global.h"

LevelItemWidget::LevelItemWidget(S_LEVEL_INFO levelInfo, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LevelItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);

    m_levelInfo = levelInfo;

    QString strStyleSheet = QString("QToolButton{background-image: url(:/main/resource/buy/%1_level_bg_normal.png);border: none;}").arg(levelInfo.strLevelName);
    ui->toolButtonBG->setStyleSheet(strStyleSheet);

    m_childWidget = new levelChildWidget(levelInfo,ui->toolButtonBG);
    m_childWidget->show();
}

LevelItemWidget::~LevelItemWidget()
{
    qDebug()<<"delete LevelItemWidget";
    delete ui;
}

S_LEVEL_INFO LevelItemWidget::getLevelInfo()
{
    return m_levelInfo;
}

void LevelItemWidget::setLabelCheckStatus(bool bCheck)
{
    QString strStyleSheet;
    if (bCheck)
    {
        strStyleSheet = QString("QToolButton{background-image: url(:/main/resource/buy/%1_level_bg_select.png);border: none;}").arg(m_levelInfo.strLevelName);
    }
    else
    {
        strStyleSheet = QString("QToolButton{background-image: url(:/main/resource/buy/%1_level_bg_normal.png);border: none;}").arg(m_levelInfo.strLevelName);
    }
    ui->toolButtonBG->setStyleSheet(strStyleSheet);

    qDebug()<<"LevelItemWidget labelCheck status="<<bCheck;
}

void LevelItemWidget::on_toolButtonBG_clicked()
{
    QString strStyleSheet = QString("QToolButton{background-image: url(:/main/resource/buy/%1_level_bg_select.png);border: none;}").arg(m_levelInfo.strLevelName);
    ui->toolButtonBG->setStyleSheet(strStyleSheet);
    
    emit selectLevelTypeSignals(m_levelInfo);
}

