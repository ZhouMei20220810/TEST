#include "recentcopycutcontentdialog.h"
#include "ui_recentcopycutcontentdialog.h"
#include <QGraphicsDropShadowEffect>
#include "clipboardhistoryapp.h"
#include <QClipboard>
#include <QMimeData>

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
    ui->listWidget->setSpacing(5);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &RecentCopyCutContentDialog::do_idClicked);
   
    // 连接信号与槽
    connect(qApp->clipboard(), &QClipboard::dataChanged, this, &RecentCopyCutContentDialog::onClipboardChanged);
    LoadHistoryList();
}
void RecentCopyCutContentDialog::onClipboardChanged()
{
    LoadHistoryList();
    /*const QMimeData* mimeData = qApp->clipboard()->mimeData();
    if (mimeData && mimeData->hasText())
    {
        QString text = mimeData->text();
        addItem(text);
    }*/
}
void RecentCopyCutContentDialog::LoadHistoryList()
{
    ui->listWidget->clear();

    QList<QString> history;
    ClipboardHistoryApp* app = qobject_cast<ClipboardHistoryApp*>(qApp);
    if(app != NULL)
        history = app->getClipboardHistoryList();    
    for (const QString& strContent : history)
    {
        addItem(strContent);
    }
}

void RecentCopyCutContentDialog::addItem(const QString& strContent)
{
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    QWidget* widget = new QWidget(this);
    widget->resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}";
    QRadioButton* radioBtnContent = new QRadioButton(widget);
    //这里设置为非互斥，使用QButtonGroup来控制
    radioBtnContent->setAutoExclusive(false);
    m_buttonGroup->addButton(radioBtnContent);
    //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
    radioBtnContent->setStyleSheet(strStyleSheet);
    QFontMetrics fontWidth(radioBtnContent->font());
    QString strElideNote = fontWidth.elidedText(strContent, Qt::ElideRight, 244);
    radioBtnContent->setText(strElideNote);
    radioBtnContent->setToolTip(strContent);

    hLayout->addWidget(radioBtnContent);
    hLayout->addStretch();

    QToolButton* toolBtnDel = new QToolButton(widget);
    strStyleSheet = "QToolButton{border:none;background:transparent;background-image: url(:/main/resource/main/deleteActiveItem.png);}";
    toolBtnDel->setStyleSheet(strStyleSheet);
    toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
    connect(toolBtnDel, &QToolButton::clicked, this, &RecentCopyCutContentDialog::do_toolBtnDelClick);
    //m_toolBtnDel->setText("222");
    //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hLayout->addWidget(toolBtnDel);

    //添加到垂直布局
    vLayout->addLayout(hLayout);
    widget->setLayout(vLayout);

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);
}

//删除按钮
void RecentCopyCutContentDialog::do_toolBtnDelClick(bool bChecked)
{

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

