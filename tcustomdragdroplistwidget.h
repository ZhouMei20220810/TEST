#ifndef TCUSTOMDRAGDROPLISTWIDGET_H
#define TCUSTOMDRAGDROPLISTWIDGET_H

#include <QListWidget>
#include <QObject>

class TCustomDragDropListWidget : public QListWidget
{
    Q_OBJECT
public:
    TCustomDragDropListWidget(QWidget *parent = nullptr);

    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent* event) override;
};

#endif // TCUSTOMDRAGDROPLISTWIDGET_H
