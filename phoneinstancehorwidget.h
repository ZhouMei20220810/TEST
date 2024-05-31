#ifndef PHONEINSTANCEHORWIDGET_H
#define PHONEINSTANCEHORWIDGET_H

#include <QWidget>
#include "global.h"
#include "toolobject.h"
#include "SWPlayer.h"
#include "SWDataSourceListener.h"
#include "videoviewwidget.h"

namespace Ui {
class PhoneInstanceHorWidget;
}

class PhoneInstanceHorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneInstanceHorWidget(QWidget *parent = nullptr);
    ~PhoneInstanceHorWidget();

private slots:
    void on_toolBtnMore_clicked();

private:
    Ui::PhoneInstanceHorWidget *ui;
};

#endif // PHONEINSTANCEHORWIDGET_H
