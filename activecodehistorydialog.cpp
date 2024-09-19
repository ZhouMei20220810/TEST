#include "activecodehistorydialog.h"
#include "ui_activecodehistorydialog.h"
#include "messagetipsdialog.h"
#include "activecodehistoryitem.h"
#include <QGraphicsDropShadowEffect>
#include "toolobject.h"

ActiveCodeHistoryDialog::ActiveCodeHistoryDialog(QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::ActiveCodeHistoryDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("激活码记录");

    setAttribute(Qt::WA_TranslucentBackground, true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    connect(ToolObject::getInstance(), &ToolObject::HttpGetAcitveCodeHistorySignals, this, &ActiveCodeHistoryDialog::ShowActiveCodeInfoList);
    ToolObject::getInstance()->HttpGetAcitveCodeHistory(1,1000);
}

ActiveCodeHistoryDialog::~ActiveCodeHistoryDialog()
{
    delete ui;
}

void ActiveCodeHistoryDialog::on_btnClose_clicked()
{
    this->close();
}

//显示数据
void ActiveCodeHistoryDialog::ShowActiveCodeInfoList(QMap<int, S_ACTIVE_CODE_DETAIL_INFO> mapActiveInfo)
{
    if (mapActiveInfo.size() <= 0)
    {
        MessageTipsDialog* tips = new MessageTipsDialog("没有激活记录!",this);
        tips->show();
        return;
    }

    QListWidgetItem* item = NULL;
    ActiveCodeHistoryItem* itemWidget;
    QMap<int, S_ACTIVE_CODE_DETAIL_INFO>::iterator iter = mapActiveInfo.begin();
    for (; iter != mapActiveInfo.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetActiveCodeHistory);
        ui->listWidgetActiveCodeHistory->addItem(item);

        itemWidget = new ActiveCodeHistoryItem(*iter,this);
        item->setSizeHint(QSize(ui->listWidgetActiveCodeHistory->width()-50, itemWidget->height()));
        ui->listWidgetActiveCodeHistory->setItemWidget(item, itemWidget);
    }
}
