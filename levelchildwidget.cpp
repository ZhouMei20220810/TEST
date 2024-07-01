#include "levelchildwidget.h"
#include "ui_levelchildwidget.h"
#include "global.h"

levelChildWidget::levelChildWidget(S_LEVEL_INFO levelInfo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::levelChildWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);

    ui->labelVersion->setText(levelInfo.strLevelName);
    ui->labelFunction->setText(levelInfo.strLevelRemark);
}

levelChildWidget::~levelChildWidget()
{
    delete ui;
}
