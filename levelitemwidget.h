#ifndef LEVELITEMWIDGET_H
#define LEVELITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include "global.h"

namespace Ui {
class LevelItemWidget;
}

class LevelItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelItemWidget(LEVEL_TYPE enType, QString strImage, QWidget* parent = nullptr);
    ~LevelItemWidget();

    void setLabelCheckStatus(bool bCheck);

signals:
    void showVIPTypeSignals(LEVEL_TYPE enType);
private:
    Ui::LevelItemWidget *ui;
    LEVEL_TYPE m_levelType;
    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private slots:
    void on_toolButtonBG_clicked();
};

#endif // LEVELITEMWIDGET_H
