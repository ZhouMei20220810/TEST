#ifndef PHONELISTMODEITEMWIDGET_H
#define PHONELISTMODEITEMWIDGET_H

#include <QWidget>
#include "global.h"
#include "phoneinstancewidget.h"

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

    PhoneInstanceWidget* m_PhoneInstanceWidget;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // PHONELISTMODEITEMWIDGET_H
