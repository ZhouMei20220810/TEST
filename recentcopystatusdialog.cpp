#include "recentcopystatusdialog.h"
#include "ui_recentcopystatusdialog.h"
#include <QGraphicsDropShadowEffect>

CopyStatusItem::CopyStatusItem(S_PHONE_INFO info, EN_COPY_STATUS enType, QWidget* parent)
    : QWidget(parent)
{
    resize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QString strStyleSheet = "background:transparent;color:#596380;font-size:12px;font-weight:bold;";
    m_LabelInstance = new QLabel(this);
    //这里设置为非互斥，使用QButtonGroup来控制
    //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
    m_LabelInstance->setStyleSheet(strStyleSheet);
    //radioBtnContent->setFixedSize(QSize(RECENT_LIST_ITEM_WIDTH - 50, RECENT_LIST_ITEM_HEIGHT));
    QFontMetrics fontWidth(m_LabelInstance->font());
    QString strElideNote = fontWidth.elidedText(info.strName, Qt::ElideRight, COPY_STATUS_ITEM_WIDTH - 50);
    m_LabelInstance->setText(strElideNote);
    m_LabelInstance->setToolTip(info.strName);


    hLayout->addWidget(m_LabelInstance);
    hLayout->addStretch();

    m_LabelStatus = new QLabel(this);
    //成功
    switch (enType)
    {
    case EN_COPY_STATUS_SUCCESS:    //成功
    {
        strStyleSheet = "background:transparent;color:#3071F2;font-size:12px;font-weight:bold;";
        m_LabelStatus->setText(QString("粘贴成功").toUtf8());
    }
    break;
    case EN_COPY_STATUS_FAILD:       //错误
    {
        strStyleSheet = "background:transparent;color:#FF4D4D;font-size:12px;font-weight:bold;";
        m_LabelStatus->setText(QString("失败").toUtf8());
    }
    break;
    case EN_COPY_STATUS_NO_CONTENT:   //无内容
    {
        strStyleSheet = "background:transparent;color:#596380;font-size:12px;font-weight:bold;";
        m_LabelStatus->setText(QString("无内容").toUtf8());
    }
    break;
    default:
        break;
    }

    m_LabelStatus->setStyleSheet(strStyleSheet);
    hLayout->addWidget(m_LabelStatus);

    //添加到垂直布局
    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);
}

CopyStatusItem::~CopyStatusItem()
{
    qDebug() << "delete ~CopyStatusItem";
    delete this;
}

void CopyStatusItem::setCopyStatus(EN_COPY_STATUS enType)
{
    m_enStatus = enType;
}

EN_COPY_STATUS CopyStatusItem::getCopyStatus()
{
    return m_enStatus;
}

RecentCopyStatusDialog::RecentCopyStatusDialog(QWidget *parent)
    : QMoveDialog(parent)
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

void RecentCopyStatusDialog::do_CopyStatusSignals(S_PHONE_INFO info, int iRet,QString strContent)
{
    qDebug() << "新增一条记录" << info.strInstanceNo << "iRet=" << iRet;
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    
    EN_COPY_STATUS enType;
    if (!strContent.isEmpty())
    {
        if (iRet == 0)
            enType = EN_COPY_STATUS_SUCCESS;
        else
            enType = EN_COPY_STATUS_FAILD;
    }
    else
    {
        enType = EN_COPY_STATUS_NO_CONTENT;
    }
    item->setData(Qt::UserRole, enType);
    CopyStatusItem* widget = new CopyStatusItem(info, enType, this);
    item->setSizeHint(QSize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT));
    ui->listWidget->setItemWidget(item, widget);

    //显示下方统计成功、失败、无内容
    calcCopyStatusResult();
}

//统计拷贝状态结果
void RecentCopyStatusDialog::calcCopyStatusResult()
{
    int iListCount = ui->listWidget->count();
    int iSuccess = 0;
    int iFailed = 0;
    int iNoContent = 0;
    QListWidgetItem* item = NULL;
    EN_COPY_STATUS enType;
    for (int row = 0; row < iListCount; row++)
    {
        item = ui->listWidget->item(row);
        if (item != NULL)
        {
            enType = (EN_COPY_STATUS)item->data(Qt::UserRole).toInt();
            switch (enType)
            {
            case EN_COPY_STATUS_SUCCESS:    //成功
            {
                iSuccess++;
            }
            break;
            case EN_COPY_STATUS_FAILD:       //错误
            {
                iFailed++;
            }
            break;
            case EN_COPY_STATUS_NO_CONTENT:   //无内容
            {
                iNoContent++;
            }
            break;
            default:
                break;
            }
        }
    }
    ui->labelSuccess->setText(QString("成功：%1台").arg(iSuccess));
    ui->labelFailed->setText(QString("失败：%1台").arg(iFailed));
    ui->labelNoContent->setText(QString("无内容：%1台").arg(iNoContent));
}
