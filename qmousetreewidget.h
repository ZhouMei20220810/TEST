#ifndef QMOUSETREEWIDGET_H
#define QMOUSETREEWIDGET_H

#include <QObject>
#include <QTreeWidget>

class QMouseTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QMouseTreeWidget(QWidget* parent=nullptr);
    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // QMOUSETREEWIDGET_H
