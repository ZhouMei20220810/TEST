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
   
    m_iBtnID = 0;
    // 连接信号与槽
    connect(qApp->clipboard(), &QClipboard::dataChanged, this, &RecentCopyCutContentDialog::onClipboardChanged);
    LoadHistoryList();
}
void RecentCopyCutContentDialog::onClipboardChanged()
{
    LoadHistoryList();
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

void RecentCopyCutContentDialog::deleteItem(QListWidgetItem* item)
{
    if (item) 
    {
        // 从QButtonGroup中移除对应的QRadioButton
        m_buttonGroup->removeButton(m_buttonGroup->button(item->data(Qt::UserRole).toInt()));

        ui->listWidget->takeItem(ui->listWidget->row(item));
        delete item;
    }
}

void RecentCopyCutContentDialog::addItem(const QString& strContent)
{
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    RecentListItem* widget = new RecentListItem(strContent.isEmpty()?"New Option":strContent, m_buttonGroup, m_iBtnID, this);
    item->setData(Qt::UserRole, m_iBtnID);
    item->setData(Qt::DisplayRole, strContent);
    m_iBtnID++;
    connect(widget, &RecentListItem::deleteClicked, this, [this, widget]() {
        QListWidgetItem* item = NULL;
        for (int i = 0; i < ui->listWidget->count(); i++)
        {
            item = ui->listWidget->item(i);
            if (widget == ui->listWidget->itemWidget(item))
            {
                qobject_cast<ClipboardHistoryApp*>(qApp)->removeHistoryItem(item->data(Qt::DisplayRole).toString());
                m_buttonGroup->removeButton(m_buttonGroup->button(item->data(Qt::UserRole).toInt()));                
                ui->listWidget->takeItem(i);
                break;
            }
        }
        delete item;
        });
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);
}

//删除按钮
/*void RecentCopyCutContentDialog::do_toolBtnDelClick(bool bChecked)
{

}*/

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

