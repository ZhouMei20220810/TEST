#ifndef LEVELITEMWIDGET_H
#define LEVELITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include "global.h"
#include "levelchildwidget.h"
namespace Ui {
class LevelItemWidget;
}

class LevelItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LevelItemWidget(S_LEVEL_INFO levelInfo, QString strImage, QWidget* parent = nullptr);
    ~LevelItemWidget();

    void setLabelCheckStatus(bool bCheck);

signals:
    void selectLevelTypeSignals(LEVEL_TYPE enType);
private:
    Ui::LevelItemWidget *ui;
    LEVEL_TYPE m_levelType;
    QLabel* m_labelCheck;
    levelChildWidget* m_childWidget;
    // QObject interface
private slots:
    void on_toolButtonBG_clicked();
};

#endif // LEVELITEMWIDGET_H
