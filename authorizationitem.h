#ifndef AUTHORIZATIONITEM_H
#define AUTHORIZATIONITEM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class authorizationItem;
}

class authorizationItem : public QWidget
{
    Q_OBJECT

public:
    explicit authorizationItem(S_AUTHOR_INFO authInfo,QWidget *parent = nullptr);
    ~authorizationItem();

private slots:
    void on_toolBtnOper_clicked();

private:
    Ui::authorizationItem *ui;
};

#endif // AUTHORIZATIONITEM_H
