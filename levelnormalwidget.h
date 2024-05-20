#ifndef LEVELNORMALWIDGET_H
#define LEVELNORMALWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>
#include "levelchildwidget.h"

namespace Ui {
class LevelNormalWidget;
}

class LevelNormalWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelNormalWidget(S_LEVEL_INFO levelInfo/*LEVEL_TYPE enType*/, QWidget* parent = nullptr);
    ~LevelNormalWidget();
    void setLabelCheckStatus(bool bCheck);

signals:
    void selectLevelTypeSignals(LEVEL_TYPE enType);
private slots:
    void on_toolButtonBG_clicked();

private:
    Ui::LevelNormalWidget *ui;
    LEVEL_TYPE m_levelType;
    QLabel* m_labelCheck;

    levelChildWidget* m_childWidget;
};

#endif // LEVELNORMALWIDGET_H
