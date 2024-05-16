#include "levelchildwidget.h"
#include "ui_levelchildwidget.h"
#include "global.h"

levelChildWidget::levelChildWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::levelChildWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    this->resize(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);

    /*ui->listFuncWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listFuncWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listFuncWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listFuncWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listFuncWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QToolButton* toolBtn;
    QListWidgetItem* item;
    for(int i = 0; i < 6;i++)
    {
        item = new QListWidgetItem(ui->listFuncWidget);
        item->setSizeHint(QSize(60,60));
        toolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        toolBtn->setIcon(QIcon(":/func/resource/function/1.png"));
        toolBtn->setText("云端管理");
        ui->listFuncWidget->addItem(item);
        ui->listFuncWidget->setItemWidget(item,toolBtn);

    }*/

}

levelChildWidget::~levelChildWidget()
{
    delete ui;
}
