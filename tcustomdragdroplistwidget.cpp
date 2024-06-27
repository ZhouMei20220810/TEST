#include "tcustomdragdroplistwidget.h"
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

TCustomDragDropListWidget::TCustomDragDropListWidget(QWidget *parent):QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropOverwriteMode(false); // 不覆盖拖放目标
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
    setMouseTracking(true);

    /*for (int i = 0; i < PHONE_INSTANCE_TOOL_COUNT; i++)
    {
        m_arrayList[i] = i + 1;
    } */   
    QListWidgetItem* item = NULL;
    int iType = 0;
    QString strIcon;
    if (!GlobalData::strToolButtonList.isEmpty())
    {
        QStringList strList = GlobalData::strToolButtonList.split(',');
        for (int i = 0; i < strList.size(); i++)
        {
            item = new QListWidgetItem(this);
            iType = strList.at(i).toInt();
            item->setData(Qt::UserRole, iType);
            item->setSizeHint(QSize(ICON_WIDTH, ICON_HEIGHT));
            //item->setText(QString("%1").arg(iType));
            //btn = new QToolButton(this);
            //label = new QLabel(this);
            strIcon = QString::asprintf(":/resource/setting/%d.png", iType);
            //btn->setIcon(QIcon(strIcon));
            //btn->setIconSize(size);
            //label->setPixmap(QPixmap(strIcon));
            //label->resize(size);
            //ui->listWidget->addItem(item);
            item->setIcon(QIcon(strIcon));
            m_arrayList[i] = iType;
            this->insertItem(i, item);
            //ui->listWidget->setItemWidget(item, label);
        }
    }
    else
    {
        ResetArray();
    }
}
QString TCustomDragDropListWidget::SaveToolsOrder()
{
    QListWidgetItem* item = NULL;
    int iCount = count();
    int iType = 0;
    QString strToolList = "";
    for (int i = 0; i < iCount; i++)
    {
        item = this->item(i);
        iType = item->data(Qt::UserRole).toInt();
        if (strToolList.isEmpty())
        {
            strToolList = QString::asprintf("%d", iType);
        }
        else
        {
            strToolList += QString::asprintf(",%d", iType);
        }
    }
    return strToolList;
}

void TCustomDragDropListWidget::ResetArray()
{
    QListWidgetItem* item = NULL;
    QString strIcon;
    clear();
    for (int i = 0; i < PHONE_INSTANCE_TOOL_COUNT; i++)
    {
        m_arrayList[i] = i + 1;

        item = new QListWidgetItem(this);
        item->setData(Qt::UserRole, i+1);
        item->setSizeHint(QSize(ICON_WIDTH, ICON_HEIGHT));
        strIcon = QString::asprintf(":/resource/setting/%d.png", i+1);
        item->setIcon(QIcon(strIcon));
        this->insertItem(i, item);
    }
}

void TCustomDragDropListWidget::insertIntoArray(int arr[], int& size, int value, int position)
{
    // 确保插入位置合法
    if (position < 0 || position > size) {
        qDebug()<< "插入位置不合法！";
        return;
    }

    // 数组已满，无法插入新元素
    if (size == sizeof(arr) / sizeof(arr[0])) {
        qDebug() << "数组已满，无法插入新元素！";
        return;
    }

    // 将数组中position之后的所有元素向右移动一位
    for (int i = size; i > position; --i) 
    {
        arr[i] = arr[i - 1];
    }

    // 在指定位置插入新值
    arr[position] = value;
    ++size; // 增大数组的有效大小
}

void TCustomDragDropListWidget::dropEvent(QDropEvent* event)
{
    if (!event->source() || event->source() != this || !event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        return QListWidget::dropEvent(event);
    }

    // 计算放置位置
    QModelIndex targetIndex;
    bool foundGap = false;
    for (int i = 0; i < count(); ++i) {
        QRect itemRect = visualRect(model()->index(i, 0));
        itemRect.adjust(-15, -15, 0, 0); // 考虑边框边距调整矩形
        if (itemRect.contains(event->pos())) {
            targetIndex = model()->index(i, 0);
            break;
        } else if (i > 0) {
            QRect prevRect = visualRect(model()->index(i - 1, 0));
            prevRect.adjust(-15, -15, 0, 0); // 同样调整上一个项目的矩形
            if (event->pos().y() > prevRect.bottom() && event->pos().y() < itemRect.top()) {
                targetIndex = model()->index(i, 0);
                foundGap = true;
                break;
            }
        }
    }

    if (!foundGap) {
        if (event->pos().y() < visualRect(model()->index(0, 0)).top()) {
            targetIndex = model()->index(0, 0);
        } else {
        // 鼠标在所有项之下
        if(!targetIndex.isValid())
            targetIndex = model()->index(count(), 0);
        }
    }

    // 移动项
    QListWidgetItem* item = currentItem();
    if (item) {
        int srcRow = row(item);
        int destRow = targetIndex.row();
        if (destRow < 0)
        {
            destRow = count() - 1;
        }
        //过滤同一个选项移动
        if (destRow != srcRow)
        {
            int iData = item->data(Qt::UserRole).toInt();
            //不是交换位置，而是插入一个数
            int srcType = m_arrayList[srcRow];
            int j = PHONE_INSTANCE_TOOL_COUNT - 1;
            if (destRow < srcRow)
            {
                for (int i = srcRow; i > destRow; --i)
                {
                    m_arrayList[i] = m_arrayList[i - 1];
                }
                m_arrayList[destRow] = srcType;
            }
            else
            {
                destRow--;
                srcType = m_arrayList[srcRow];
                for (int i = srcRow; i < destRow; i++)
                {
                    m_arrayList[i] = m_arrayList[i+1];
                }
                m_arrayList[destRow] = srcType;
            }
        }
        this->clear();
        QString strIcon;
        for (int i = 0; i < PHONE_INSTANCE_TOOL_COUNT; i++)
        {
            item = new QListWidgetItem(this);
            item->setData(Qt::UserRole, m_arrayList[i]);
            item->setSizeHint(QSize(ICON_WIDTH, ICON_HEIGHT));
            strIcon = QString::asprintf(":/resource/setting/%d.png", m_arrayList[i]);
            item->setIcon(QIcon(strIcon));
            this->insertItem(i, item);
        }
                
    }

    event->acceptProposedAction();
}
