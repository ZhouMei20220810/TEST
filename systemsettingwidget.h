#ifndef SYSTEMSETTINGWIDGET_H
#define SYSTEMSETTINGWIDGET_H

#include <QWidget>
#include <QButtonGroup>
#include "global.h"
namespace Ui {
class SystemSettingWidget;
}

class SystemSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SystemSettingWidget(QWidget *parent = nullptr);
    ~SystemSettingWidget();

private:
    void InitListWidget();

    void getLoggedInUserInfo();
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

    void on_radioButton_10_clicked(bool checked);
    void on_radioButton_11_clicked(bool checked);
    void on_radioButton_12_clicked(bool checked);
    void on_radioButton_13_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_radioButton_clicked(bool checked);
    void on_radioButton_2_clicked(bool checked);
private:
    Ui::SystemSettingWidget *ui;

    QButtonGroup* m_qualityGroup;
    ENUM_PICTURE_QUALITY  m_enQuality;
    bool            m_bVerticalScreen;
    bool            m_bPhoneInstanceCenter;
    bool            m_bCloseMainWindowExit;
    bool            m_bShowTrayIcon;
};

#endif // SYSTEMSETTINGWIDGET_H
