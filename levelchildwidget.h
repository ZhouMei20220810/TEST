#ifndef LEVELCHILDWIDGET_H
#define LEVELCHILDWIDGET_H

#include <QWidget>

namespace Ui {
class levelChildWidget;
}

class levelChildWidget : public QWidget
{
    Q_OBJECT

public:
    explicit levelChildWidget(QWidget *parent = nullptr);
    ~levelChildWidget();

private:
    Ui::levelChildWidget *ui;
};

#endif // LEVELCHILDWIDGET_H
