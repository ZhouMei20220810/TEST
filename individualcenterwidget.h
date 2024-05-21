#ifndef INDIVIDUALCENTERWIDGET_H
#define INDIVIDUALCENTERWIDGET_H

#include <QWidget>

namespace Ui {
class IndividualCenterWidget;
}

class IndividualCenterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndividualCenterWidget(QWidget *parent = nullptr);
    ~IndividualCenterWidget();

private slots:
    void on_toolBtnIndividualCenter_clicked();

    void on_toolBtnLogout_clicked();

    void on_toolBtnBuyHistory_clicked();

    void on_toolBtnActiveHistory_clicked();

private:
    //注销
    void HttpLogout();

    Ui::IndividualCenterWidget *ui;
};

#endif // INDIVIDUALCENTERWIDGET_H
