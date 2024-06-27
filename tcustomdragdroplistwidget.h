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
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void dropEvent(QDropEvent* event) override;
};

#endif // TCUSTOMDRAGDROPLISTWIDGET_H
