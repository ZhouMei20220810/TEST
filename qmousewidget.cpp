#include "qmousewidget.h"
#include "global.h"

QMouseWidget::QMouseWidget(QWidget* parent) : QWidget(parent)
{
    this->setMouseTracking(true);
}


void QMouseWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->cursor() != Qt::ArrowCursor)
        this->setCursor(Qt::ArrowCursor);
    if (GlobalData::bHideIndividualCenterWidget)
    {
        emit hideIndividualCenterWidgetSignals();
    }
}
