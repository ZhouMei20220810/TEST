#ifndef TDRAGDROPLISTWIDGET_H
#define TDRAGDROPLISTWIDGET_H

#include <QListWidget>
#include <QObject>

class TDragDropListWidget : public QListWidget
{
    Q_OBJECT
public:
    TDragDropListWidget(QWidget* parent = nullptr);

protected:
    void dropEvent(QDropEvent *event);
};

#endif // TDRAGDROPLISTWIDGET_H
