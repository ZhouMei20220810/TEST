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
    explicit VIPItemWidget(S_VIP_ITEM_INFO sVipInfo,QWidget *parent = nullptr);
    ~VIPItemWidget();

    void setLabelCheckStatus(bool bCheck);
signals:
    void selectVIPTypeSignals(S_VIP_ITEM_INFO sVipInfo);
private slots:
    void on_toolButton_clicked();

private:
    Ui::VIPItemWidget *ui;

    S_VIP_ITEM_INFO m_sVipInfo;
};

#endif // VIPITEMWIDGET_H
