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

    void on_pushButton_clicked();

    void on_btnCancel_clicked();
    void on_btnSave_clicked();

    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();

private:
    Ui::SystemSettingWidget *ui;
};

#endif // SYSTEMSETTINGWIDGET_H
