#include "levelchildgvipwidget.h"
#include "ui_levelchildgvipwidget.h"

LevelChildGvipWidget::LevelChildGvipWidget(S_LEVEL_INFO levelInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelChildGvipWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);

    ui->labelVersion->setText(levelInfo.strLevelName);
    ui->labelFunction->setText(levelInfo.strLevelRemark);

}

LevelChildGvipWidget::~LevelChildGvipWidget()
{
    delete ui;
}
