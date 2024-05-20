#ifndef VIPITEMWIDGET_H
#define VIPITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class VIPItemWidget;
}

class VIPItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VIPItemWidget(S_LEVEL_DATA_INFO levelInfo,QWidget *parent = nullptr);
    ~VIPItemWidget();

    void setLabelCheckStatus(bool bCheck);
signals:
    void selectVIPTypeSignals(S_LEVEL_DATA_INFO levelInfo);
private slots:
    void on_toolButton_clicked();

private:
    Ui::VIPItemWidget *ui;

    S_LEVEL_DATA_INFO m_levelInfo;
};

#endif // VIPITEMWIDGET_H
