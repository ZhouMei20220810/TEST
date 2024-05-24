#ifndef RENEWITEMWIDGET_H
#define RENEWITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class renewItemWidget;
}

class renewItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit renewItemWidget(S_PHONE_INFO phoneInfo,QWidget *parent = nullptr);
    ~renewItemWidget();

    void setCheckBoxStatus(bool bCheck);
    bool getCheckBoxStatus();
private:
    Ui::renewItemWidget *ui;
};

#endif // RENEWITEMWIDGET_H
