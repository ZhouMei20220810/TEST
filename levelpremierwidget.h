#ifndef LEVELPREMIERWIDGET_H
#define LEVELPREMIERWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>
#include "levelchildkvipwidget.h"
namespace Ui {
class LevelPremierWidget;
}

class LevelPremierWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelPremierWidget(S_LEVEL_INFO levelInfo,QWidget *parent = nullptr);
    ~LevelPremierWidget();
    void setLabelCheckStatus(bool bCheck);

signals:
    void selectLevelTypeSignals(LEVEL_TYPE enType);
private slots:
    void on_toolButtonBG_clicked();

private:
    Ui::LevelPremierWidget *ui;
    LEVEL_TYPE m_levelType;
    QLabel* m_labelCheck;

    LevelChildKvipWidget* m_childKvipWidget;
};

#endif // LEVELPREMIERWIDGET_H