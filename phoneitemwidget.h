#ifndef PHONEITEMWIDGET_H
#define PHONEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class PhoneItemWidget;
}

class PhoneItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneItemWidget(QWidget *parent = nullptr);
    ~PhoneItemWidget();

private slots:
    void on_btnAddPhone_clicked();

private:
    Ui::PhoneItemWidget *ui;
};

#endif // PHONEITEMWIDGET_H
