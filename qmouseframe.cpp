#include "qmouseframe.h"
#include "global.h"

QMouseFrame::QMouseFrame(QWidget* parent):QFrame(parent)
{
    this->setMouseTracking(true);
}


void QMouseFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(this->cursor() != Qt::ArrowCursor)
        this->setCursor(Qt::ArrowCursor);
    if (GlobalData::bHideIndividualCenterWidget)
    {
        emit hideIndividualCenterWidgetSignals();
    }
}
