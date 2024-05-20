#include "levelenhancementwidget.h"
#include "ui_levelenhancementwidget.h"

LevelEnhancementWidget::LevelEnhancementWidget(S_LEVEL_INFO levelInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelEnhancementWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    m_labelCheck = new QLabel(this);
    m_labelCheck->setPixmap(QPixmap(":/main/resource/main/levelCheck.png"));
    m_labelCheck->resize(ITEM_LEVEL_LABELCHECK_WIDTH, ITEM_LEVEL_LABELCHECK_HEIGHT);
    m_levelInfo = levelInfo;
    m_labelCheck->move(this->width()-m_labelCheck->width(), 0);
    m_labelCheck->setVisible(false);
    m_labelCheck->setParent(ui->toolButtonBG);
    m_labelCheck->setStyleSheet("border:none;background:transparent;");

    //QString strStyleSheet = QString("QToolButton{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->toolButtonBG->setStyleSheet("QToolButton{background-image: url(:/main/resource/main/level_enhancenment.png);border: none;}");
    /*QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(strImage);//QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->labelBg->setStyleSheet(strStyleSheet);*/
    m_childGvipWidget = new LevelChildGvipWidget(levelInfo,ui->toolButtonBG);
    m_childGvipWidget->show();
}

LevelEnhancementWidget::~LevelEnhancementWidget()
{
    delete ui;
}
void LevelEnhancementWidget::setLabelCheckStatus(bool bCheck)
{
    m_labelCheck->setVisible(bCheck);
    //ui->labelCheck->setVisible(bCheck);
    qDebug()<<"LevelEnhancementWidget labelCheck status="<<bCheck;
}
void LevelEnhancementWidget::on_toolButtonBG_clicked()
{
    m_labelCheck->setVisible(true);
    //ui->labelCheck->setVisible(true);
    //ui->toolButtonBG->setStyleSheet("QToolButton {border:none;color: rgb(204, 204, 204);border - radius:3px;background-image:url(:/main/main/level_enhancenment.png);}");
    emit selectLevelTypeSignals(m_levelInfo);
}

