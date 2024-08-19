#include "recentcopycutcontentdialog.h"
#include "ui_recentcopycutcontentdialog.h"
#include <QGraphicsDropShadowEffect>
#include "clipboardhistoryapp.h"
#include <QClipboard>
#include <QMimeData>
#include <QTextDocument>
#include <QTextBlock>
#include "recentcopystatusdialog.h"
#include "recentcopyphonedialog.h"

RecentListItem::RecentListItem(S_RECENT_COPY_DATA info, QWidget* parent)
    : QWidget(parent)
{
    resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}QRadioButton{background:transparent;}";
    radioBtnContent = new QRadioButton(this);
    //这里设置为非互斥，使用QButtonGroup来控制
    radioBtnContent->setAutoExclusive(false);
    info.pButtonGroup->addButton(radioBtnContent, info.iBtnGroupId);
    //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
    radioBtnContent->setStyleSheet(strStyleSheet);
    radioBtnContent->setFixedSize(QSize(RECENT_LIST_ITEM_WIDTH - 50, RECENT_LIST_ITEM_HEIGHT));
    QFontMetrics fontWidth(radioBtnContent->font());
    QString strElideNote = fontWidth.elidedText(info.strContent, Qt::ElideRight, RECENT_LIST_ITEM_WIDTH - 50);
    radioBtnContent->setText(strElideNote);
    radioBtnContent->setToolTip(info.strContent);


    hLayout->addWidget(radioBtnContent);
    hLayout->addStretch();

    toolBtnDel = new QToolButton(this);
    strStyleSheet = "QToolButton{border:none;background:transparent;}";
    toolBtnDel->setStyleSheet(strStyleSheet);
    toolBtnDel->resize(QSize(16, 16));
    toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBtnDel->setIcon(QIcon(":/main/resource/main/copyDel.png"));
    toolBtnDel->setIconSize(QSize(16, 16));
    connect(toolBtnDel, &QToolButton::clicked, this, &RecentListItem::do_Clicked);
    //m_toolBtnDel->setText("222");
    //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hLayout->addWidget(toolBtnDel);

    //添加到垂直布局
    vLayout->addLayout(hLayout);
    this->setLayout(vLayout);
}

RecentListEditableItem::RecentListEditableItem(int iRowIndex, QString strText, QWidget* parent)
    : QWidget(parent)
{
    resize(RECENT_LIST_EDITABLE_ITEM_WIDTH, RECENT_LIST_EDITABLE_ITEM_HEIGHT);
    m_strText = strText;

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);

    m_textEdit = new TPlainTextEdit(this);
    connect(m_textEdit, &TPlainTextEdit::returnPressed, this, &RecentListEditableItem::do_returnPressed);
    QString strStyleSheet = "QPlainTextEdit{border:none;background:transparent;color:#4A4A4A;font-size:12px;border-bottom: 1px solid #E6E9F2;}";
    
    m_textEdit->resize(QSize(RECENT_LIST_EDITABLE_ITEM_WIDTH - 70, 40));
    m_textEdit->setPlaceholderText("请输入需要粘贴的内容");
    if (!strText.isEmpty())
    {
        m_textEdit->setPlainText(QString("%1、%2").arg(iRowIndex+1).arg(strText));
        //m_textEdit->setText(QString("%1、%2").arg(iRowIndex).arg(strText));
    }
    else
    {
        //没有内容
        if (iRowIndex == 0 && strText.isEmpty())
        {
            strStyleSheet = "QPlainTextEdit{border:none;background:transparent;color:#4A4A4A;font-size:12px;border-bottom:0px solid #E6E9F2;}";
        }
    }
    m_textEdit->setStyleSheet(strStyleSheet);
    hLayout->addWidget(m_textEdit);
    this->setLayout(hLayout);

    //监听拷贝事件
    connect(m_textEdit, &QPlainTextEdit::copyAvailable, this, &RecentListEditableItem::do_copyAvailable);
}

void RecentListEditableItem::do_returnPressed()
{
    QString text = m_textEdit->toPlainText().trimmed();
    if (!text.isEmpty())
    {
        /*QStringList items = text.split(',', Qt::SkipEmptyParts);
        for (const QString &item : items) {
            QListWidgetItem *newItem = new QListWidgetItem(item.trimmed(), listWidget);
            newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
        }*/
        emit enterTextSignals(text);
        m_textEdit->clear(); // 清除输入框中的文本
    }
}

void RecentListEditableItem::do_copyAvailable(bool available)
{
    if (available) 
    {
        // 当有内容可以复制时，监听剪贴板的变化
        auto clipboard = QApplication::clipboard();
        connect(clipboard, &QClipboard::dataChanged, this, &RecentListEditableItem::onClipboardDataChanged);
    }
}

void RecentListEditableItem::onClipboardDataChanged()
{
    auto clipboard = QApplication::clipboard();
    if (clipboard->mimeData()->hasText()) {
        const QString& clipboardText = clipboard->text();
        qDebug() << "Clipboard Text:" << clipboardText;

        // 检查文本中是否包含换行符
        bool containsNewline = clipboardText.contains('\n');
        qDebug() << "Contains newline:" << containsNewline;

        // 在这里可以处理拷贝的文本，例如插入到其他地方
    }
}

RecentCopyCutContentDialog::RecentCopyCutContentDialog(QStringList strPhoneList,QWidget *parent)
    : QMoveDialog(parent)
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

    ui->listWidgetEditable->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    //ui->listWidgetActiveCode->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    ui->listWidgetEditable->setSpacing(5);//去掉上下左右空格
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetEditable->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetEditable->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetEditable->setSelectionMode(QAbstractItemView::SingleSelection);    

    m_buttonGroup = new QButtonGroup(this);
    connect(m_buttonGroup, &QButtonGroup::idClicked, this, &RecentCopyCutContentDialog::do_idClicked);

    m_iBtnID = 0;
    // 连接信号与槽
    connect(qApp->clipboard(), &QClipboard::dataChanged, this, &RecentCopyCutContentDialog::onClipboardChanged);
    LoadHistoryList();
    ui->labelPhoneCount->setText(QString("设备数量： %1").arg(strPhoneList.size()));
    ui->labelTextCount->setText(QString("当前文字数量： %1").arg(0));

    m_UITimer = new QTimer(this);
    m_UITimer->start(2);
    connect(m_UITimer, &QTimer::timeout, this, [this]() {
        //初始化数据
        m_UITimer->stop();
        do_enterTextSignals("");
        });
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
    RECENT_COPY_DATA info;
    info.strContent = strContent.isEmpty() ? "New Option" : strContent;
    info.pButtonGroup = m_buttonGroup;
    info.iBtnGroupId = m_iBtnID;
    RecentListItem* widget = new RecentListItem(info, this);
    item->setSizeHint(QSize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT));
    item->setData(Qt::UserRole, QVariant::fromValue(info));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);
    m_iBtnID++;
    connect(widget, &RecentListItem::deleteClicked, this, [this, widget]() 
        {
        QListWidgetItem* item = NULL;
        RECENT_COPY_DATA info;
        for (int i = 0; i < ui->listWidget->count(); i++)
        {
            item = ui->listWidget->item(i);
            if (widget == ui->listWidget->itemWidget(item))
            {
                info = item->data(Qt::UserRole).value<RECENT_COPY_DATA>();
                qobject_cast<ClipboardHistoryApp*>(qApp)->removeHistoryItem(info.strContent);
                m_buttonGroup->removeButton(m_buttonGroup->button(info.iBtnGroupId));                
                ui->listWidget->takeItem(i);
                break;
            }
        }
        delete item;
        item = NULL;
        });
}

void RecentCopyCutContentDialog::do_idClicked(int id)
{
    QRadioButton* button = qobject_cast<QRadioButton*>(m_buttonGroup->button(id));
    m_strSelectText = button->text();
    qDebug() << "Selected radio button:" << m_strSelectText;
}

RecentCopyCutContentDialog::~RecentCopyCutContentDialog()
{
    delete ui;
}
//是否弹窗提示复制状态
void RecentCopyCutContentDialog::showCopyStatusDialog()
{
    //同步状态需要弹窗提示成功状态
    if (GlobalData::bIsSyncOperation)
    { 
        RecentCopyStatusDialog* dialog = new RecentCopyStatusDialog();
        connect(qobject_cast<ClipboardHistoryApp*>(qApp), &ClipboardHistoryApp::addCopyStatusSignals, dialog, &RecentCopyStatusDialog::do_addCopyStatusSignals);
        dialog->setModal(true);
        dialog->show();
        this->close();
    }
    else
    {
        this->close();
    }
}

void RecentCopyCutContentDialog::on_btnCopyToPhone_clicked()
{
    //增加提示框
    if (GlobalData::bIsSyncOperation)
    {
        RecentCopyPhoneDialog dialog;
        if (QDialog::Accepted != dialog.exec())
        {
            return;
        }
    }     
    //同步状态需要弹窗提示成功状态
    showCopyStatusDialog();
    //粘贴到云手机中
    //先清空之前的记录
    qobject_cast<ClipboardHistoryApp*>(qApp)->clearCopyStatus();
    emit DirectCopyToPhoneSignals(m_strSelectText);
}


void RecentCopyCutContentDialog::on_btnDirectCopy_clicked()
{
    //增加提示框
    if (GlobalData::bIsSyncOperation)
    {
        RecentCopyPhoneDialog dialog;
        if (QDialog::Accepted != dialog.exec())
        {
            return;
        }
    }
    //同步状态需要弹窗提示成功状态
    showCopyStatusDialog();
    //直接拷贝
    QString strText = getEditableListWidgetText();//ui->plainTextEdit->toPlainText();
    //先清空之前的记录
    qobject_cast<ClipboardHistoryApp*>(qApp)->clearCopyStatus();
    emit DirectCopyToPhoneSignals(strText);    
}

QString RecentCopyCutContentDialog::getEditableListWidgetText()
{
    QString strText = "";
    int iCount = ui->listWidgetEditable->count();
    QListWidgetItem* item = NULL;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetEditable->item(i);
        if (item != NULL)
        {
            if (!strText.isEmpty())
                strText += '\n';
            strText += item->data(Qt::UserRole).toString();
        }
    }
    return strText;
}

void RecentCopyCutContentDialog::on_btnCopyByOrder_clicked()
{
    //增加提示框
    if (GlobalData::bIsSyncOperation)
    {
        RecentCopyPhoneDialog dialog;
        if (QDialog::Accepted != dialog.exec())
        {
            return;
        }
    }
    //同步状态需要弹窗提示成功状态
    showCopyStatusDialog();
    //按顺序依次拷贝
    QString strText = getEditableListWidgetText();//ui->plainTextEdit->toPlainText();
    GlobalData::iSyncPhoneIndex = 0;
    //先清空之前的记录
    qobject_cast<ClipboardHistoryApp*>(qApp)->clearCopyStatus();
    emit BatchDirectCopyToPhoneSignals(strText);    
}


void RecentCopyCutContentDialog::on_btnClose_clicked()
{
    this->close();
}


void RecentCopyCutContentDialog::on_toolBtnClear_clicked()
{
    //清空
    ui->listWidget->clear();
    //清空缓存数据
    QList<QString> history;
    ClipboardHistoryApp* app = qobject_cast<ClipboardHistoryApp*>(qApp);
    if (app != NULL)
        app->clearClipboardHistoryList();
}


void RecentCopyCutContentDialog::do_enterTextSignals(QString strEnterText)
{
    //ui->listWidgetEditable->clear();
    //不清空之前的内容
    //删除最后一行
    if (ui->listWidgetEditable->count() > 0)
    {
        ui->listWidgetEditable->takeItem(ui->listWidgetEditable->count() - 1);
    }

    QStringList items = strEnterText.split('\n', Qt::SkipEmptyParts);
    QListWidgetItem* item = NULL;
    RecentListEditableItem* widget = NULL;
    int i = ui->listWidgetEditable->count();
    for (const QString& strItem : items) 
    {
        item = new QListWidgetItem(ui->listWidgetEditable);
        widget = new RecentListEditableItem(i, strItem, this);        
        connect(widget, &RecentListEditableItem::enterTextSignals, this, &RecentCopyCutContentDialog::do_enterTextSignals);
        item->setSizeHint(QSize(RECENT_LIST_EDITABLE_ITEM_WIDTH, RECENT_LIST_EDITABLE_ITEM_HEIGHT));
        item->setData(Qt::UserRole, strItem);
        ui->listWidgetEditable->insertItem(i, item);
        ui->listWidgetEditable->setItemWidget(item, widget);
        i++;
    }
    ui->labelTextCount->setText(QString("当前文字数量： %1").arg(ui->listWidgetEditable->count()));

    //最后一行添加空白行，支持拷贝
    item = new QListWidgetItem(ui->listWidgetEditable);
    widget = new RecentListEditableItem(i, "", this);
    connect(widget, &RecentListEditableItem::enterTextSignals, this, &RecentCopyCutContentDialog::do_enterTextSignals);
    item->setSizeHint(ui->listWidgetEditable->size());
    item->setData(Qt::UserRole, "");
    ui->listWidgetEditable->insertItem(i, item);
    ui->listWidgetEditable->setItemWidget(item, widget);
}
