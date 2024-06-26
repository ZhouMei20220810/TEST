#include "qmousetreewidget.h"
#include <QMouseEvent>
#include "global.h"

QMouseTreeWidget::QMouseTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    this->setMouseTracking(true);
}


void QMouseTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->cursor() != Qt::ArrowCursor)
        this->setCursor(Qt::ArrowCursor);
    if (GlobalData::bHideIndividualCenterWidget)
    {
        emit hideIndividualCenterWidgetSignals();
    }
}
