#ifndef QMOUSEWIDGET_H
#define QMOUSEWIDGET_H

#include <QWidget>

class QMouseWidget:public QWidget
{
    Q_OBJECT
public:
    QMouseWidget(QWidget* parent=nullptr);
    // QWidget interface
signals:
    void hideIndividualCenterWidgetSignals();
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QMOUSEWIDGET_H
