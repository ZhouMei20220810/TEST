#ifndef PHONEINSTANCEWIDGET_H
#define PHONEINSTANCEWIDGET_H

#include <QDialog>
#include "toolobject.h"
#include "SWPlayer.h"
#include "SWDataSourceListener.h"
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
    explicit PhoneInstanceWidget(S_PHONE_INFO sTaskInfo, bool bIsMasterOrNot =true, QDialog* parent = nullptr);
    ~PhoneInstanceWidget();
    // QWidget interface
    //�Ƿ����ػ�
    bool IsMasterOrNot()
    {
        return m_bIsMasterOrNot;//true�����أ�false������
    } 
    void setChildControl(bool bHasChildControl)
    {
        m_bHasChildControl = bHasChildControl;
    }
    bool hasChildControl()
    {
        return m_bHasChildControl;//true:�У�false�����Ƿ����ӿ�
    }
    S_PHONE_INFO getPhoneInfo()
    {
        return m_PhoneInfo;
    }
signals:
    void ReturnSignals();
    void HomeSignals();
    void ChangePageSignals();
    void TouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
    void dealTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);

    void RootSignals();
    void VolumeUpSignals();
    void VolumeDownSignals();
    void HorizontalSignals(bool bIsVertical);
    void SharkSignals();
    void GPSSignals();
    //ͬ���������豸
    void BatchDirectCopyToPhoneSignals(QString strTextList);
    void closePhoneInstanceWidgetSignals();
    //�ı������֪ͨvideoviewwidget
    void changeVerOrHorScreenSignals(bool bIsVertical);
    void closeNotMasterPhoneSignals();
public slots:
    void do_ReturnSignals();
    void do_HomeSignals();
    void do_ChangePageSignals();

    void do_rootSignals();
    void on_Screenshot_clicked(bool checked = false);
    void do_VolumeUpSignals();
    void do_VolumeDownSignals();
    void do_HorizontalSignals(bool bIsVertical);
    void do_SharkSignals();
    void do_GPSSignals();
    void do_closePhoneInstanceWidgetSignals();

    //����videoviewwidget�ź�
    void do_syncTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
    //���ο������ֻ�
    void do_BatchDirectCopyToPhoneSignals(QString strTextList);
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

    void on_toolBtnVolumnUp_clicked();
    void on_toolBtnVolumnDown_clicked();
    void on_toolBtnHorOrVer_clicked();
    void on_toolBtnClipboard_clicked();
    void on_toolBtnScreenshotDir_clicked();
    void on_toolBtnChangeKeyBoard_clicked();
    void on_toolBtnReboot_clicked();
    void on_toolBtnFactoryDataReset_clicked();
    void on_toolBtnRoot_clicked();
    void on_toolBtnShark_clicked();
    void on_toolBtnGPS_clicked();
    void on_toolBtnReturn_clicked();
    void on_toolBtnHome_clicked();
    void on_toolBtnChangePage_clicked();

    void onPositionUpdated(const QGeoPositionInfo& info);
    //ֱ�ӿ���
    void do_DirectCopyToPhoneSignals(QString strSelectText);      
private:
    void InitToolButtonList(int iToolIndex, QFrame* frame, QVBoxLayout* vBox);
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

    QVBoxLayout* m_vBox;
    QVBoxLayout* m_vBox2;

    QPoint      m_leftTopPoint;
    QSize       m_remmberSize;
    
    //�Ƿ����ػ�
    bool        m_bIsMasterOrNot;//true�����أ�false������
    bool        m_bHasChildControl;//true:�У�false�����Ƿ����ӿ�
    bool        m_bIsVertical;//true������false����
};

#endif // PHONEINSTANCEWIDGET_H
