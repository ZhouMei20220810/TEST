#ifndef PHONEINSTANCEWIDGET_H
#define PHONEINSTANCEWIDGET_H

#include <QDialog>
#include "global.h"
#include "toolobject.h"
#include "SWPlayer.h"
#include "SWDataSourceListener.h"
#include "videoviewwidget.h"
#include <QGeoPositionInfo>

namespace Ui {
    class PhoneInstanceWidget;
}

class PhoneInstanceWidget : public QDialog,public SWDataSourceListener
{
    Q_OBJECT

public:
    explicit PhoneInstanceWidget(S_PHONE_INFO sTaskInfo, QDialog* parent = nullptr);
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

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    void InitToolButton(QToolButton* toolBtn);
    void setToolBtnVisible(bool bVisible);

    void HttpGetInstanceSession(int id);
private slots:
    void on_toolBtnPhoneInstance_clicked();
    void on_toolBtnPictureQuality_clicked();
    void on_toolBtnTopLevel_clicked();
    void on_toolBtnMin_clicked();
    void on_toolBtnMax_clicked();
    void on_toolBtnClose_clicked();
    void on_toolBtnShow_clicked();
    void on_toolBtnHide_clicked();

    /*void on_toolBtnSignal_clicked();
    void on_toolBtnVerOrHor_clicked();
    void on_toolBtnScreenshot_clicked();
    void on_toolBtnAddVolume_clicked();
    void on_toolBtnSubVolume_clicked();
    void on_toolBtnClipboard_clicked();
    void on_toolBtnInput_clicked();
    void on_toolBtnScreenshotsDir_clicked();
    void on_toolBtnShark_clicked();
    void on_toolBtnRoot_clicked();
    void on_toolBtnKeyboard_clicked();
    void on_toolBtnADB_clicked();*/
    void on_toolBtnMore_clicked();

    void on_toolBtnReturn_clicked();
    void on_toolBtnHome_clicked();
    void on_toolBtnChangePage_clicked();

    /*void on_toolBtnScreenshot_2_clicked();
    void on_toolBtnScreenshotsDir_2_clicked();
    void on_toolBtnClipboard_2_clicked();
    void on_toolBtnShark_2_clicked();
    void on_toolBtnRoot_2_clicked();
    void on_toolBtnKeyboard_2_clicked();
    void on_toolBtnADB_2_clicked();*/

    void on_toolButton_1_clicked();
    void on_toolButton_2_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_7_clicked();
    void on_toolButton_8_clicked();
    void on_toolButton_9_clicked();
    void on_toolButton_10_clicked();
    void on_toolButton_11_clicked();
    void on_toolButton_12_clicked();

    void onPositionUpdated(const QGeoPositionInfo& info);
private:
    Ui::PhoneInstanceWidget* ui;

    S_PHONE_INFO m_PhoneInfo;
    QStringList m_strPhoneList;
    ToolObject* m_toolObject;


    QString m_strSessionId;
    QString m_strControlIp;
    float   m_fControlPort;
    QString m_strControlCode;
    QString m_strDomain;
    QString m_strControlTactics;
    QString m_strTraceServer;
    double m_dControlPort;

    //VideoViewWidget* m_Display;
    SWPlayer* m_Player;
    Mutex m_Mutex;

    QPoint  m_LastPos;
    bool m_bMoving= false;

    int     m_width;
    int     m_height;

    CPadInfo m_padInfo;
};

#endif // PHONEINSTANCEWIDGET_H
