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

private:
    Ui::VIPItemWidget *ui;
};

#endif // VIPITEMWIDGET_H
