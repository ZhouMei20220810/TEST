#include "recentcopystatusdialog.h"
#include "ui_recentcopystatusdialog.h"
#include <QGraphicsDropShadowEffect>

RecentCopyStatusDialog::RecentCopyStatusDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecentCopyStatusDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("登录");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    //ui->listWidgetActiveCode->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    ui->listWidget->setSpacing(5);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

RecentCopyStatusDialog::~RecentCopyStatusDialog()
{
    delete ui;
}

void RecentCopyStatusDialog::on_btnClose_clicked()
{
    this->close();
}


void RecentCopyStatusDialog::on_btnOK_clicked()
{
    //确定
    this->close();
}

void RecentCopyStatusDialog::do_CopyStatusSignals(S_PHONE_INFO info, int iRet)
{
    qDebug() << "新增一条记录" << info.strInstanceNo << "iRet=" << iRet;
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    CopyStatusItem* widget = new CopyStatusItem(info, iRet, this);
    item->setSizeHint(QSize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT));
    ui->listWidget->setItemWidget(item, widget);
}