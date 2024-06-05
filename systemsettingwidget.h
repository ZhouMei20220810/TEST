#ifndef SYSTEMSETTINGWIDGET_H
#define SYSTEMSETTINGWIDGET_H

#include <QWidget>

namespace Ui {
class SystemSettingWidget;
}

class SystemSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSettingWidget(QWidget *parent = nullptr);
    ~SystemSettingWidget();

private slots:
    void on_toolBtnGeneralSetting_clicked();

    void on_toolBtnUISetting_clicked();

    void on_toolBtnAbout_clicked();

private:
    Ui::SystemSettingWidget *ui;
};

#endif // SYSTEMSETTINGWIDGET_H
