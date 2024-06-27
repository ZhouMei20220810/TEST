#ifndef TCUSTOMDRAGDROPLISTWIDGET_H
#define TCUSTOMDRAGDROPLISTWIDGET_H

#include <QListWidget>
#include <QObject>
#include "global.h"

class TCustomDragDropListWidget : public QListWidget
{
    Q_OBJECT
public:
    TCustomDragDropListWidget(QWidget *parent = nullptr);

    void ResetArray();
    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent* event) override;
private:
    void insertIntoArray(int arr[], int& size, int value, int position);
    int m_arrayList[PHONE_INSTANCE_TOOL_COUNT];
};

#endif // TCUSTOMDRAGDROPLISTWIDGET_H
