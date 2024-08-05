#include "recentcopycutcontentdialog.h"
#include "ui_recentcopycutcontentdialog.h"
#include <QGraphicsDropShadowEffect>

RecentCopyCutContentDialog::RecentCopyCutContentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RecentCopyCutContentDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("最近复制/剪切过的文字");
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
    ui->listWidget->setSpacing(0);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &RecentCopyCutContentDialog::do_idClicked);

    //假数据
    QListWidgetItem* item = NULL;
    QWidget* widget = NULL;
    QRadioButton* m_radioBtnContent = NULL;
    QToolButton* m_toolBtnDel = NULL;
    QString strContent;
    for (int i = 0; i < 10; i++)
    {
        strContent = strContent.asprintf("测试%d", i);
        item = new QListWidgetItem(ui->listWidget);
        widget = new QWidget(this);
        widget->resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}";
        m_radioBtnContent = new QRadioButton(widget);
        //这里设置为非互斥，使用QButtonGroup来控制
        m_radioBtnContent->setAutoExclusive(false);
        m_buttonGroup->addButton(m_radioBtnContent);
        //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
        m_radioBtnContent->setStyleSheet(strStyleSheet);
        m_radioBtnContent->setText(strContent);
        hLayout->addWidget(m_radioBtnContent);
        hLayout->addStretch();

        m_toolBtnDel = new QToolButton(widget);
        strStyleSheet = "QToolButton{border:none;background:transparent;background-image: url(:/main/resource/main/deleteActiveItem.png);}";
        m_toolBtnDel->setStyleSheet(strStyleSheet);
        m_toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
        //m_toolBtnDel->setText("222");
        //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hLayout->addWidget(m_toolBtnDel);

        //添加到垂直布局
        vLayout->addLayout(hLayout);
        widget->setLayout(vLayout);



        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void RecentCopyCutContentDialog::do_idClicked(int id)
{
    QRadioButton* button =  qobject_cast<QRadioButton*>(m_buttonGroup->button(id));
    m_strSelectText = button->text();
    qDebug() << "Selected radio button:" << m_strSelectText;

}

RecentCopyCutContentDialog::~RecentCopyCutContentDialog()
{
    delete ui;
}

void RecentCopyCutContentDialog::on_btnCopyToPhone_clicked()
{
    //粘贴到云手机中
    emit DirectCopyToPhoneSignals(m_strSelectText);
    this->close();
}


void RecentCopyCutContentDialog::on_btnDirectCopy_clicked()
{
    //直接拷贝
}


void RecentCopyCutContentDialog::on_btnCopyByOrder_clicked()
{
    //按顺序依次拷贝
}


void RecentCopyCutContentDialog::on_btnClose_clicked()
{
    this->close();
}

