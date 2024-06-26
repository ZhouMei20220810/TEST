#ifndef QMOUSELISTWIDGET_H
#define QMOUSELISTWIDGET_H

#include <QListWidget>
#include <QObject>

class QMouseListWidget : public QListWidget
{
    Q_OBJECT
public:
    QMouseListWidget(QWidget* parent=nullptr);
signals:
    void hideIndividualCenterWidgetSignals();
    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QMOUSELISTWIDGET_H
