#ifndef PHONELISTMODEITEMWIDGET_H
#define PHONELISTMODEITEMWIDGET_H

#include <QWidget>
#include "global.h"

namespace Ui {
class PhoneListModeItemWidget;
}

class PhoneListModeItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneListModeItemWidget(S_PHONE_INFO sPhoneInfo,QWidget *parent = nullptr);
    ~PhoneListModeItemWidget();

private:
    Ui::PhoneListModeItemWidget *ui;
    S_PHONE_INFO m_sPhoneInfo;
};

#endif // PHONELISTMODEITEMWIDGET_H
