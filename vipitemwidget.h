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
    explicit VIPItemWidget(VIP_TYPE enVipType,QWidget *parent = nullptr);
    ~VIPItemWidget();

    void setLabelCheckStatus(bool bCheck);
signals:
    void selectVIPTypeSignals(VIP_TYPE enVipType);
private slots:
    void on_toolButton_clicked();

private:
    Ui::VIPItemWidget *ui;
    VIP_TYPE m_enVipType;
};

#endif // VIPITEMWIDGET_H
