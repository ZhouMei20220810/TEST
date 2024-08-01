#ifndef PHONEINSTANCEWIDGET_H
#define PHONEINSTANCEWIDGET_H

#include <QDialog>
#include "global.h"
#include "toolobject.h"
#include "SWPlayer.h"
#include "SWDataSourceListener.h"
#include "videoviewwidget.h"
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QListWidget>
#include "global.h"
#include <QMap>
#include <QTimer>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVBoxLayout>

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
signals:
    void ReturnSignals();
    void HomeSignals();
    void ChangePageSignals();
    void TouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
    void dealTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);

    void RebootSignals();
    void FactoryDataResetSignals();
    void RootSignals();
    void ScreenshotsSignals();
    void VolumeUpSignals();
    void VolumeDownSignals();
    void HorizontalSignals();
    void SharkSignals();
    void GPSSignals();
    void closePhoneInstanceWidgetSignals();
public slots:
    void do_ReturnSignals();
    void do_HomeSignals();
    void do_ChangePageSignals();

    void do_RebootSignals();
    void do_FactoryDataResetSignals();
    void do_rootSignals();
    void on_Screenshot_clicked(bool checked = false);
    void do_ScreenshotsSignals();
    void do_VolumeUpSignals();
    void do_VolumeDownSignals();
    void do_HorizontalSignals();
    void do_SharkSignals();
    void do_GPSSignals();
    void do_closePhoneInstanceWidgetSignals();
protected:
    bool onPlayStart(S_PAD_INFO padInfo);
    void onPlayStop(bool bQuit);

    // SWDataSourceListener implementation
    void onReconnecting(int NthTime) override;
    void onConnected() override;
    void onDisconnected(int errcode) override;
    void onDisconnected(int errcode, const char* errmsg) override;
    void onPlayInfo(const char* info) override;
    void onSensorInput(int inputtype, int state) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
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

    void on_toolBtnMore_clicked();

    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void on_toolButton_6_clicked();
    void on_toolButton_12_clicked();

    void onPositionUpdated(const QGeoPositionInfo& info);
private:
    void InitToolButtonList();
    Ui::PhoneInstanceWidget* ui;

    S_PHONE_INFO m_PhoneInfo;
    QStringList m_strPhoneList;
    ToolObject* m_toolObject;

    SWPlayer* m_Player;
    Mutex m_Mutex;

    QPoint  m_LastPos;
    bool m_bMoving= false;

    int     m_width;
    int     m_height;

    QMap<int, S_PAD_INFO> m_mapPadInfo;
    QGeoPositionInfoSource* m_GeoSource;

    VideoViewWidget* m_OtherVideoViewWidget;

    QTimer* m_getScreenshotsTimer;
    QNetworkAccessManager* m_manager;
    QNetworkReply* m_reply;

    QToolButton* m_tBtnVolumnUp;
    QToolButton* m_tBtnVolumnDown;
    QToolButton* m_tBtnHorVerScreen;
    QToolButton* m_tBtnClipboard;
    QToolButton* m_tBtnScreenshots;
    QToolButton* m_tBtnScreenshotsFolder;
    QToolButton* m_tBtnRestart;
    QToolButton* m_tBtnResetFactoryData;
    QToolButton* m_tBtnRoot;
    QToolButton* m_tBtnShark;
    QToolButton* m_tBtnGPS;
    QToolButton* m_tBtnChangeKeyBoard;

    QToolButton* m_tBtnHide;
    QToolButton* m_tBtnMoreTool;
    QToolButton* m_tBtnRetrun;
    QToolButton* m_tBtnHome;
    QToolButton* m_tBtnChangeMenu;

    QVBoxLayout* vBox;
    QVBoxLayout* vBox2;

    QPoint      m_leftTopPoint;
    QSize       m_remmberSize;
};

#endif // PHONEINSTANCEWIDGET_H
