#include "tdragdroptreewidget.h"
#include <QDragEnterEvent>
#include <QMimeData>

TDragDropTreeWidget::TDragDropTreeWidget(QWidget* parent)
    :QTreeWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

bool TDragDropTreeWidget::isParentItem(const QModelIndex& index)
{
    if (!index.isValid())
        return false;

    // 获取当前项的父项索引，如果父项无效，说明当前项是顶级项，即父节点
    QModelIndex parentIndex = index.parent();
    return !parentIndex.isValid();
}

void TDragDropTreeWidget::dragEnterEvent(QDragEnterEvent* event)
{
    // 允许拖放操作继续，因为我们希望在符合条件的情况下允许内部拖放
    event->acceptProposedAction();
}

void TDragDropTreeWidget::dragMoveEvent(QDragMoveEvent* event)
{
    // 同样允许拖动操作继续
    event->acceptProposedAction();
}

void TDragDropTreeWidget::dropEvent(QDropEvent* event)
{
    if (isParentItem(indexAt(event->pos())))
    {
        QTreeWidget::dropEvent(event);
    }
    else
    {
        // 如果尝试放置在非父节点上，可以选择忽略或根据需要处理
        event->ignore();
    }
}
