#ifndef LEVELCHILDGVIPWIDGET_H
#define LEVELCHILDGVIPWIDGET_H

#include <QWidget>
#include "global.h"
namespace Ui {
class LevelChildGvipWidget;
}

class LevelChildGvipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelChildGvipWidget(S_LEVEL_INFO levelInfo,QWidget *parent = nullptr);
    ~LevelChildGvipWidget();

private:
    Ui::LevelChildGvipWidget *ui;
};

#endif // LEVELCHILDGVIPWIDGET_H
