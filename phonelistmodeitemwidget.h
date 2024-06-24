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
    void setCheckBoxStatus(bool bCheck);
    bool getCheckBoxStatus();
    void setPhoneName(QString strPhoneName);
signals:
    void ShowPhoneInstanceWidgetSignals(S_PHONE_INFO sPhoneInfo,bool bShowMenu);
    void stateChanged(int state);
private:
    Ui::PhoneListModeItemWidget *ui;
    S_PHONE_INFO m_sPhoneInfo;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // PHONELISTMODEITEMWIDGET_H
