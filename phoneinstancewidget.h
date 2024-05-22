#ifndef PHONEINSTANCEWIDGET_H
#define PHONEINSTANCEWIDGET_H

#include <QWidget>

namespace Ui {
    class PhoneInstanceWidget;
}

class PhoneInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PhoneInstanceWidget(QWidget* parent = nullptr);
    ~PhoneInstanceWidget();

private slots:
    void on_toolBtnPictureQuality_clicked();

    void on_toolBtnTopLevel_clicked();

    void on_toolBtnMin_clicked();

    void on_toolBtnMax_clicked();

    void on_toolBtnClose_clicked();

    void on_toolBtnShow_clicked();

    void on_toolBtnHide_clicked();

    void on_toolBtnSignal_clicked();

    void on_toolBtnVerOrHor_clicked();

    void on_toolBtnScreenshot_clicked();

    void on_toolBtnAddVolume_clicked();

    void on_toolBtnSubVolume_clicked();

    void on_toolBtnClipboard_clicked();

    void on_toolBtnRestart_clicked();

    void on_toolBtnFactoryDataReset_clicked();

private:
    Ui::PhoneInstanceWidget* ui;
};

#endif // PHONEINSTANCEWIDGET_H
