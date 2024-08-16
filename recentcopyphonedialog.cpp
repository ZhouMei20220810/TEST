#include "recentcopyphonedialog.h"
#include "ui_recentcopyphonedialog.h"
#include <QGraphicsDropShadowEffect>
#include "global.h"

CopyPhoneItem::CopyPhoneItem(int iOrderIndex, QString strPhoneName, QWidget* parent)
    : QWidget(parent)
{
    resize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QString strStyleSheet = "background:transparent;color:#596380;font-size:12px;font-weight:bold;";
    m_LabelOrder = new QLabel(this);
    m_LabelOrder->setStyleSheet(strStyleSheet);
    m_LabelOrder->setText(QString("%1").arg(iOrderIndex));
    m_LabelOrder->setFixedWidth(118);
    hLayout->addWidget(m_LabelOrder);

    m_LabelPhoneName = new QLabel(this);
    m_LabelPhoneName->setText(strPhoneName);
    m_LabelPhoneName->setStyleSheet(strStyleSheet);
    hLayout->addWidget(m_LabelPhoneName);

    //添加到垂直布局
    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);
}

RecentCopyPhoneDialog::RecentCopyPhoneDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecentCopyPhoneDialog)
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

    InitListWidget();
}

RecentCopyPhoneDialog::~RecentCopyPhoneDialog()
{
    delete ui;
}

void RecentCopyPhoneDialog::on_btnOK_clicked()
{
    //确定拷贝
    this->accept();
}

void RecentCopyPhoneDialog::on_btnClose_clicked()
{
    this->reject();
}

void RecentCopyPhoneDialog::InitListWidget()
{
    QListWidgetItem* item = NULL;
    CopyPhoneItem* widget = NULL;
    int iOrderIndex = 0;
    QMap<int, S_PHONE_INFO>::iterator iter = GlobalData::mapSyncPhoneList.begin();
    for (; iter != GlobalData::mapSyncPhoneList.end(); iter++)
    {
        iOrderIndex++;
        item = new QListWidgetItem(ui->listWidget);
        widget = new CopyPhoneItem(iOrderIndex, iter->strName, this);
        item->setSizeHint(QSize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT));
        ui->listWidget->setItemWidget(item, widget);
        //qDebug() << "同步操作,添加需要同步设备" << iter->strInstanceNo;
        //m_strPhoneList << iter->strInstanceNo;
    }
}