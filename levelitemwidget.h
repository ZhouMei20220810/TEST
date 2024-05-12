#ifndef LEVELITEMWIDGET_H
#define LEVELITEMWIDGET_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class LevelItemWidget;
}

class LevelItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelItemWidget(QString strImage,QWidget *parent = nullptr);
    ~LevelItemWidget();

    void setLabelCheckStatus(bool bCheck);
private:
    Ui::LevelItemWidget *ui;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // LEVELITEMWIDGET_H
