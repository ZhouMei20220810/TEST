#ifndef TDRAGDROPTREEWIDGET_H
#define TDRAGDROPTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>

class TDragDropTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    TDragDropTreeWidget(QWidget* parent = nullptr);

    // QWidget interface
protected:
    //virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
private:
    bool isParentItem(const QModelIndex& index);
};

#endif // TDRAGDROPTREEWIDGET_H
