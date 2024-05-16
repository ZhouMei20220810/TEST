#ifndef LEVELCHILDGVIPWIDGET_H
#define LEVELCHILDGVIPWIDGET_H

#include <QWidget>

namespace Ui {
class LevelChildGvipWidget;
}

class LevelChildGvipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelChildGvipWidget(QWidget *parent = nullptr);
    ~LevelChildGvipWidget();

private:
    Ui::LevelChildGvipWidget *ui;
};

#endif // LEVELCHILDGVIPWIDGET_H
