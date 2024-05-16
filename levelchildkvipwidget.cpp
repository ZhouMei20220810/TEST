#include "levelchildkvipwidget.h"
#include "ui_levelchildkvipwidget.h"

LevelChildKvipWidget::LevelChildKvipWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelChildKvipWidget)
{
    ui->setupUi(this);
}

LevelChildKvipWidget::~LevelChildKvipWidget()
{
    delete ui;
}
