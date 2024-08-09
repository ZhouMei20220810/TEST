#include "qmovedialog.h"
#include <QApplication>

QMoveDialog::QMoveDialog(QWidget* parent):QDialog(parent)
{

}

void QMoveDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_LastPos = event->globalPosition().toPoint()-this->pos();
        qDebug() << "globbal Position x="<<event->globalPosition().toPoint().x()
                 <<",y="<<event->globalPosition().toPoint().y()
                 <<"; pos x=" << this->pos().x()
                 <<",y=" << this->pos().y()
                 <<";LastPos x="<<m_LastPos.x()
                 <<" ;y="<<m_LastPos.y();
        m_bMoving = true;
    }
    return QWidget::mousePressEvent(event);
}

void QMoveDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMoving = false;
}

void QMoveDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPosition = event->globalPosition().toPoint();
    if(m_bMoving && (event->buttons()&Qt::LeftButton)
        && (globalPosition-m_LastPos-pos()).manhattanLength() > QApplication::startDragDistance()) //控制移动的距离，多少距离执行拖拽
    {
        move(globalPosition-m_LastPos);
        m_LastPos = globalPosition-pos();
        qDebug() << "mouseMoveEvent globbal Position x="<<globalPosition.x()
                 <<",y="<<globalPosition.y()
                 <<" pos x=" << this->pos().x()
                 <<",y=" << this->pos().y()
                 <<"LastPos x="<<m_LastPos.x()
                 <<" ;y="<<m_LastPos.y();
    }
    return QWidget::mouseMoveEvent(event);
}
