#include "levelitemwidget.h"
#include "ui_levelitemwidget.h"
#include <QPalette>
#include "global.h"

LevelItemWidget::LevelItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelItemWidget)
{
    ui->setupUi(this);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    ui->labelCheck->move(this->width()-ui->labelCheck->width(), 0);
    ui->labelCheck->setVisible(false);
    ui->labelCheck->setParent(ui->labelBg);

    QString strStyleSheet = QString("QLabel{background-image: url(%1);border: none;}").arg(":/main/resource/main/level.png");
    ui->labelBg->setStyleSheet(strStyleSheet);

    ui->labelBg->installEventFilter(this);
}

LevelItemWidget::~LevelItemWidget()
{
    delete ui;
}

bool LevelItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->labelBg)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            ui->labelCheck->setVisible(true);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QWidget::eventFilter(watched, event);
    }
}
