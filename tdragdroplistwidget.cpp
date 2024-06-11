#include "tdragdroplistwidget.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QMimeData>

TDragDropListWidget::TDragDropListWidget(QWidget* parent) : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDragDropOverwriteMode(false); // 不覆盖拖放目标
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
}

void TDragDropListWidget::dropEvent(QDropEvent* event)
{
    if (event->source() == this && event->possibleActions().testFlag(Qt::MoveAction)) {
        const QMimeData* mimeData = event->mimeData();
        if (mimeData->hasFormat("application/x-qabstractitemmodeldatalist")) {
            QPoint pos = event->position().toPoint();
            QModelIndex index = indexAt(pos);
            int targetRow = -1; // 初始化为无效值

            int minDist = 15;
            // 寻找最佳的插入位置
            for (int row = 0; row < model()->rowCount(); ++row) {
                QRect itemRect = visualRect(model()->index(row, 0));
                QRect tempRect = QRect(itemRect.left(), itemRect.top(), itemRect.right(), itemRect.bottom());
                QPoint center = itemRect.center();
                int dist = qAbs(center.y() - pos.y()); // 计算y轴方向上的距离作为简化示例
                if (itemRect.contains(pos)) {
                    // 如果鼠标直接位于item上，则直接使用该item的位置
                    targetRow = row;
                    break;
                }
                else if (targetRow == -1 || dist < minDist) {
                    // 找到离鼠标最近的item（此处简化处理，仅比较y轴方向）
                    minDist = dist;
                    targetRow = row;
                }
            }

            if (targetRow == -1) {
                // 没有找到有效的插入位置，则放置到最后
                targetRow = model()->rowCount(QModelIndex());
            }

            // 解析拖动数据以获取源索引
            QList<QPersistentModelIndex> sourceIndexes;
            QByteArray encodedData = mimeData->data("application/x-qabstractitemmodeldatalist");
            QDataStream stream(&encodedData, QIODevice::ReadOnly);
            while (!stream.atEnd()) {
                int r, c;
                QMap<int, QVariant> roles;
                stream >> r >> c >> roles;
                sourceIndexes.append(QPersistentModelIndex(model()->index(r, c)));
            }

            // 移动数据
            for (const QPersistentModelIndex& srcIndex : qAsConst(sourceIndexes)) {
                int fromRow = srcIndex.row();
                if (fromRow >= 0 && fromRow != targetRow) {
                    QListWidgetItem* item = takeItem(fromRow);
                    insertItem(targetRow, item);
                    // 更新targetRow以避免重复插入到同一位置
                    if (fromRow < targetRow) targetRow++;
                }
            }

            // 更新当前行，这里假设只拖动了一个item
            if (!sourceIndexes.isEmpty()) {
                setCurrentRow(targetRow);
            }

            event->acceptProposedAction();
        }
    }
    else {
        QListWidget::dropEvent(event);
    }
}