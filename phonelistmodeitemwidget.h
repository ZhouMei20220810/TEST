#ifndef PHONELISTMODEITEMWIDGET_H
#define PHONELISTMODEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class PhoneListModeItemWidget;
}

class PhoneListModeItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneListModeItemWidget(QWidget *parent = nullptr);
    ~PhoneListModeItemWidget();

private:
    Ui::PhoneListModeItemWidget *ui;
};

#endif // PHONELISTMODEITEMWIDGET_H
