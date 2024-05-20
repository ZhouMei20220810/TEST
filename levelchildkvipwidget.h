#ifndef LEVELCHILDKVIPWIDGET_H
#define LEVELCHILDKVIPWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class LevelChildKvipWidget;
}

class LevelChildKvipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelChildKvipWidget(S_LEVEL_INFO levelInfo,QWidget *parent = nullptr);
    ~LevelChildKvipWidget();

private:
    Ui::LevelChildKvipWidget *ui;
};

#endif // LEVELCHILDKVIPWIDGET_H
