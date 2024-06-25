#include "qmousetreewidget.h"

QMouseTreeWidget::QMouseTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    this->setMouseTracking(true);
}


void QMouseTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->cursor() != Qt::ArrowCursor)
        this->setCursor(Qt::ArrowCursor);
}
