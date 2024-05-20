#ifndef LEVELENHANCEMENTWIDGET_H
#define LEVELENHANCEMENTWIDGET_H

#include <QWidget>
#include "global.h"
#include <QLabel>
#include "levelchildgvipwidget.h"

namespace Ui {
class LevelEnhancementWidget;
}

class LevelEnhancementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelEnhancementWidget(S_LEVEL_INFO levelInfo,QWidget *parent = nullptr);
    ~LevelEnhancementWidget();
    void setLabelCheckStatus(bool bCheck);

signals:
    void selectLevelTypeSignals(S_LEVEL_INFO levelInfo);
private slots:
    void on_toolButtonBG_clicked();

private:
    Ui::LevelEnhancementWidget *ui;
    S_LEVEL_INFO m_levelInfo;
    QLabel* m_labelCheck;

    LevelChildGvipWidget* m_childGvipWidget;
};

#endif // LEVELENHANCEMENTWIDGET_H
