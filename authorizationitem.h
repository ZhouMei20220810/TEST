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
signals:
    void notifyMainWindowRefreshGroupListSignals();
private slots:
    void on_toolBtnOper_clicked();

private:
    Ui::authorizationItem *ui;
    S_AUTHOR_INFO           m_authInfo;
};

#endif // AUTHORIZATIONITEM_H
