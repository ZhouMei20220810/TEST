#ifndef PHONEITEMNODATAWIDGET_H
#define PHONEITEMNODATAWIDGET_H

#include <QWidget>

namespace Ui {
class PhoneItemNoDataWidget;
}

class PhoneItemNoDataWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneItemNoDataWidget(QWidget *parent = nullptr);
    ~PhoneItemNoDataWidget();

signals:
    void changeStackedWidgetSignal();
private slots:
    void on_btnAddPhone_clicked();

private:
    Ui::PhoneItemNoDataWidget *ui;
};

#endif // PHONEITEMNODATAWIDGET_H
