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
}


void TCustomDragDropListWidget::mousePressEvent(QMouseEvent *event)
{
	QListWidget::mousePressEvent(event);
}

void TCustomDragDropListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QPoint globalPos = event->globalPos();  
    QPoint widgetPos = event->pos();               
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        QRect itemRect = visualRect(index);

        QPoint relativePos = event->pos() - itemRect.topLeft();

        qDebug() << "release:" << globalPos;
        qDebug() << "release:" << widgetPos;
        qDebug() << "release :" << relativePos;
    }

    QListWidget::mouseReleaseEvent(event);
}

void TCustomDragDropListWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPos = event->globalPos();
    QPoint widgetPos = event->pos();     
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        QRect itemRect = visualRect(index);
        QPoint relativePos = event->pos() - itemRect.topLeft();

        qDebug() << "mouse move(globalPos):" << globalPos;
        qDebug() << "mouse move(widgetPos):" << widgetPos;
        qDebug() << "mouse move(relativePos):" << relativePos;
    }

    QListWidget::mouseMoveEvent(event);
}

void TCustomDragDropListWidget::dropEvent(QDropEvent* event)
{
    // 获取鼠标所在项的索引
    QModelIndex index = indexAt(event->pos());
    if (!event->source() || event->source() != this || !event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
        return QListWidget::dropEvent(event);
    }

    // 计算放置位置 
    QModelIndex targetIndex;
    bool foundGap = false;
    for (int i = 0; i < count(); ++i) {
        QRect itemRect = visualRect(model()->index(i, 0));
        //加入边框边距
        itemRect.setX(itemRect.x() - 15);
        itemRect.setY(itemRect.y() - 15);
        if (itemRect.contains(event->pos())) {
            // 鼠标直接位于某项上
            targetIndex = model()->index(i, 0);
            break;
        }
        else if (i > 0) {
            QRect prevRect = visualRect(model()->index(i - 1, 0));
            if (event->pos().y() > prevRect.bottom() && event->pos().y() < itemRect.top()) {
                // 鼠标位于两个项之间
                targetIndex = model()->index(i, 0); // 插入到下一个项之前
                foundGap = true;
                break;
            }
        }
    }

    if (!foundGap && event->pos().y() < visualRect(model()->index(0, 0)).top()) {
        // 鼠标在所有项之上
        targetIndex = model()->index(0, 0);
    }
    else if (!foundGap) {
        // 鼠标在所有项之下
        if(!targetIndex.isValid())
            targetIndex = model()->index(count(), 0);
    }

    // 移动项
    QListWidgetItem* item = currentItem();
    if (item) 
    {
        int originalRow = row(item);
        int srcRow = targetIndex.row();
        qDebug() << "originalRow=" << originalRow << "srcRow=" << srcRow;
        // 为了安全地移动项，先处理插入再移除
        if (originalRow != srcRow) 
        {
            takeItem(originalRow); // 现在可以安全移除原始位置的项
            insertItem(targetIndex.row(), item);
            setCurrentItem(item);
        }
    }

    event->acceptProposedAction();
}
