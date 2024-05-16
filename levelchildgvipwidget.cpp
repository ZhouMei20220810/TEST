#include "levelchildgvipwidget.h"
#include "ui_levelchildgvipwidget.h"

LevelChildGvipWidget::LevelChildGvipWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LevelChildGvipWidget)
{
    ui->setupUi(this);
}

LevelChildGvipWidget::~LevelChildGvipWidget()
{
    delete ui;
}
