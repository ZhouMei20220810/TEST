#include "levelchildkvipwidget.h"
#include "ui_levelchildkvipwidget.h"

LevelChildKvipWidget::LevelChildKvipWidget(S_LEVEL_INFO levelInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelChildKvipWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);

    ui->labelVersion->setText(levelInfo.strLevelName);
    ui->labelFunction->setText(levelInfo.strLevelRemark);
}

LevelChildKvipWidget::~LevelChildKvipWidget()
{
    delete ui;
}
