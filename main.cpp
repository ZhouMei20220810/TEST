#include "loginwindow.h"

#include <QApplication>
#include <QDir>
#include "SWRuntime.h"
#include "common/log.h"
#include "global.h"
#include <QSettings>
#include <QMenu>
#include <QAction>
QSystemTrayIcon* g_trayIcon = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    QString strDir = GlobalData::strFileTempDir;
    QDir dir(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }
    strDir = GlobalData::strPictureTempDir;
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }

    SWRuntime::getInstance()->init(ANDROID_LOG_DEFAULT, strDir.toStdString().c_str());

    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    GlobalData::enPictrueQuality = (ENUM_PICTURE_QUALITY)setting.value("PictureQuality", TYPE_QUALITY_HIGH_SPEED).toInt();
    GlobalData::bVerticalPhoneInstance = setting.value("VerticalScreen", true).toBool();
    GlobalData::bVerticalPhoneInstanceCenter = setting.value("PhoneInstanceCenter", true).toBool();
    GlobalData::pointPhoneInstance = setting.value("PhoneInstancePoint").toPoint();
    GlobalData::bCloseMainWindowExit = setting.value("CloseMainWindowExit", true).toBool();
    GlobalData::bShowSystemTrayIcon = setting.value("ShowSystemTrayIcon", false).toBool();

    
    if (GlobalData::bShowSystemTrayIcon)
    {
        g_trayIcon = new QSystemTrayIcon(QIcon(":/main/resource/main/aboutlogo.png"));
        g_trayIcon->setToolTip("全局系统托盘图标示例");
        g_trayIcon->show();        
    }    

    LoginWindow w;
    w.show();

    int iResult = a.exec();
    SWRuntime::getInstance()->deinit();
    return iResult;
}
