#ifndef PHONEINSTANCEWIDGET_H
#define PHONEINSTANCEWIDGET_H

#include <QWidget>
#include "global.h"
#include "toolobject.h"
#include "SWPlayer.h"
#include "SWDataSourceListener.h"
#include "videoviewwidget.h"

namespace Ui {
    class PhoneInstanceWidget;
}

class PhoneInstanceWidget : public QWidget,public SWDataSourceListener
{
    Q_OBJECT

public:
    explicit PhoneInstanceWidget(S_PHONE_INFO sTaskInfo,QWidget* parent = nullptr);
    ~PhoneInstanceWidget();
    // QWidget interface
protected:
    bool onPlayStart();
    void onPlayStop(bool bQuit);

    // SWDataSourceListener implementation
    void onReconnecting(int NthTime) override;
    void onConnected() override;
    void onDisconnected(int errcode) override;
    void onDisconnected(int errcode, const char* errmsg) override;
    void onPlayInfo(const char* info) override;
private:
    void setToolBtnVisible(bool bVisible);
private slots:
    void on_toolBtnPhoneInstance_clicked();
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
    

    void on_toolBtnReturn_clicked();

    void on_toolBtnHome_clicked();

    void on_toolBtnChangePage_clicked();

    void on_toolBtnMore_clicked();

private:
    Ui::PhoneInstanceWidget* ui;

    S_PHONE_INFO m_PhoneInfo;
    QStringList m_strPhoneList;
    ToolObject* m_toolObject;

    //VideoViewWidget* m_Display;
    SWPlayer* m_Player;
    Mutex m_Mutex;
};

#endif // PHONEINSTANCEWIDGET_H
