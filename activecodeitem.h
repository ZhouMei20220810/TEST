#ifndef ACTIVECODEITEM_H
#define ACTIVECODEITEM_H

#include <QWidget>

namespace Ui {
class ActiveCodeItem;
}

class ActiveCodeItem : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveCodeItem(QString strActiveCode,QWidget *parent = nullptr);
    ~ActiveCodeItem();

private:
    Ui::ActiveCodeItem *ui;
};

#endif // ACTIVECODEITEM_H
