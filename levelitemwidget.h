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
    explicit LevelItemWidget(S_LEVEL_INFO levelInfo, QWidget* parent = nullptr);
    ~LevelItemWidget();

    void setLabelCheckStatus(bool bCheck);
    S_LEVEL_INFO getLevelInfo();

signals:
    void selectLevelTypeSignals(S_LEVEL_INFO levelInfo);
private:
    Ui::LevelItemWidget *ui;
    S_LEVEL_INFO m_levelInfo;

    QToolButton* m_toolBtn;
    // QObject interface
private slots:
    void on_toolButtonBG_clicked();
};

#endif // LEVELITEMWIDGET_H
