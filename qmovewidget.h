#ifndef QMOVEWIDGET_H
#define QMOVEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>

class QMoveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QMoveWidget(QWidget *parent = nullptr);

signals:

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
private:
    QPoint  m_LastPos;
    bool m_bMoving= false;
};

#endif // QMOVEWIDGET_H
