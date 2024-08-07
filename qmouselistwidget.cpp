#include "qmouselistwidget.h"
#include <QMouseEvent>
#include "global.h"

QMouseListWidget::QMouseListWidget(QWidget* parent) : QListWidget(parent)
{
    this->setMouseTracking(true);
}


void QMouseListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->cursor() != Qt::ArrowCursor)
        this->setCursor(Qt::ArrowCursor);  
    if (GlobalData::bHideIndividualCenterWidget)
    {
        emit hideIndividualCenterWidgetSignals();
    }
}