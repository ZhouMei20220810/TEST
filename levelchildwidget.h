#ifndef LEVELCHILDWIDGET_H
#define LEVELCHILDWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class levelChildWidget;
}

class levelChildWidget : public QWidget
{
    Q_OBJECT

public:
    explicit levelChildWidget(S_LEVEL_INFO levelInfo,QWidget *parent = nullptr);
    ~levelChildWidget();

private:
    Ui::levelChildWidget *ui;
};

#endif // LEVELCHILDWIDGET_H
