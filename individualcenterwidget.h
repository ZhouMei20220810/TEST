#ifndef INDIVIDUALCENTERWIDGET_H
#define INDIVIDUALCENTERWIDGET_H

#include <QDialog>

namespace Ui {
class IndividualCenterWidget;
}

class IndividualCenterWidget : public QDialog
{
    Q_OBJECT

public:
    explicit IndividualCenterWidget(QWidget *parent = nullptr);
    ~IndividualCenterWidget();
signals:
    void logoutSignals();
private slots:
    void on_toolBtnIndividualCenter_clicked();

    void on_toolBtnLogout_clicked();

    void on_toolBtnBuyHistory_clicked();

    void on_toolBtnAuthorizationManage_clicked();

private:
    //注销
    void HttpLogout();

    Ui::IndividualCenterWidget *ui;
};

#endif // INDIVIDUALCENTERWIDGET_H
