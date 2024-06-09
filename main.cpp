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

/*void setAutoStart() {
    QString appPath = QCoreApplication::applicationFilePath();
    QString startupDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + "/../Startup");
    QString startupLink = startupDir + "/YourAppName.lnk";

    QProcess::startDetached("mkdir \"" + startupDir + "\"");
    QProcess::startDetached("cmd /c \"\"" + appPath + "\"\" > NUL 2> NUL && (echo 指向快捷方式的路径 && pause > NUL) || (echo 创建快捷方式 && copy /Y \"" + appPath + "\" \"" + startupLink + "\" && echo [InternetShortcut] > \"" + startupLink + "\" && echo URL=\\\"file:\\\\\\\"%1\\\"\\\" >> \"" + startupLink + "\" && echo IconIndex=0 >> \"" + startupLink + "\" && echo IconFile=\\\"file:\\\\\\\"%1\\\"\\\" >> \"" + startupLink + "\")");
}*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //setAutoStart();

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
    GlobalData::strToolButtonList = setting.value("ToolButtonList","").toString();
    GlobalData::bBootstrapAutoStart = setting.value("BootstrapAutoStart",false).toBool();    

    if (GlobalData::bShowSystemTrayIcon)
    {
        g_trayIcon = new QSystemTrayIcon(QIcon(":/main/resource/main/aboutlogo.png"));
        g_trayIcon->setToolTip("");
        g_trayIcon->show();        
    }    

    LoginWindow w;
    w.show();

    int iResult = a.exec();
    SWRuntime::getInstance()->deinit();
    return iResult;
}
