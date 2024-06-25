#ifndef QMOUSEFRAME_H
#define QMOUSEFRAME_H

#include <QFrame>
#include <QObject>

class QMouseFrame : public QFrame
{
    Q_OBJECT
public:
    QMouseFrame(QWidget* parent=nullptr);

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QMOUSEFRAME_H
