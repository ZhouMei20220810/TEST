#ifndef LEVELCHILDKVIPWIDGET_H
#define LEVELCHILDKVIPWIDGET_H

#include <QWidget>

namespace Ui {
class LevelChildKvipWidget;
}

class LevelChildKvipWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelChildKvipWidget(QWidget *parent = nullptr);
    ~LevelChildKvipWidget();

private:
    Ui::LevelChildKvipWidget *ui;
};

#endif // LEVELCHILDKVIPWIDGET_H
