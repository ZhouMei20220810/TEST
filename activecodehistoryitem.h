#ifndef ACTIVECODEHISTORYITEM_H
#define ACTIVECODEHISTORYITEM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class ActiveCodeHistoryItem;
}

class ActiveCodeHistoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveCodeHistoryItem(S_ACTIVE_CODE_DETAIL_INFO detailInfo,QWidget *parent = nullptr);
    ~ActiveCodeHistoryItem();

private:
    Ui::ActiveCodeHistoryItem *ui;
};

#endif // ACTIVECODEHISTORYITEM_H
