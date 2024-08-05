#include "loginwindow.h"

#include <QApplication>
#include <QDir>
#include "SWRuntime.h"
#include "common/log.h"
#include "global.h"
#include <QSettings>
#include <QMenu>
#include <QAction>
#include "toolobject.h"
#include <QProcess>
#include "clipboardhistoryapp.h"

QSystemTrayIcon* g_trayIcon = NULL;

class ProcessKiller {
    //Q_OBJECT
public:
    explicit ProcessKiller()
    {

    }

public:

    void killTheProcess(const QString& processName)
    {
        QProcess process;
        process.start("tasklist", QStringList() << "/NH" << "/FI" << "IMAGENAME eq " + processName);
        process.waitForFinished();

        QByteArray output = process.readAllStandardOutput();
        QString strOutput = QString::fromLocal8Bit(output);

        // 假设每行都是 '进程名   PID 标题 ...' 的格式
        QStringList lines = strOutput.trimmed().split('\n');
        foreach(const QString & line, lines) {
            if (line.isEmpty()) {
                continue;
            }
            // 按空格分割字符串
            //QStringList parts = line.split(' ', QString::SkipEmptyParts);
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                bool ok;
                quintptr pid = parts[1].toULongLong(&ok); // 转换为无符号长整型
                if (!ok) {
                    qDebug() << "Failed to parse the PID from tasklist output.";
                    continue;
                }
                qDebug() << "Process ID for " << processName << " is:" << pid;

                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, EXIT_FAILURE); // 结束进程
                    CloseHandle(hProcess);
                }
                else {
                    qDebug() << "Failed to open the process for termination.";
                }
            }
        }

    }

};

int main(int argc, char *argv[])
{
    //QApplication app(argc, argv);
    ClipboardHistoryApp app(argc, argv);
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    GlobalData::enPictrueQuality = (ENUM_PICTURE_QUALITY)setting.value("PictureQuality", TYPE_QUALITY_HIGH_DEFINITION).toInt();
    GlobalData::bVerticalPhoneInstance = setting.value("VerticalScreen", true).toBool();
    GlobalData::bVerticalPhoneInstanceCenter = setting.value("PhoneInstanceCenter", true).toBool();
    GlobalData::pointPhoneInstance = setting.value("PhoneInstancePoint").toPoint();
    GlobalData::bCloseMainWindowExit = setting.value("CloseMainWindowExit", true).toBool();
    GlobalData::bShowSystemTrayIcon = setting.value("ShowSystemTrayIcon", false).toBool();
    GlobalData::strToolButtonList = setting.value("ToolButtonList", "1,2,3,4,5,6,7,8,9,10,11,12").toString();
    GlobalData::bBootstrapAutoStart = setting.value("BootstrapAutoStart", false).toBool();
    GlobalData::bIsTopWindow = setting.value("MainWindowTopWindow", false).toBool();
    GlobalData::bIsPageMode = setting.value("PageMode", false).toBool();
    int bWriteLogFile = setting.value("WriteLogFile", false).toBool();
    if (bWriteLogFile)
    {
        qInstallMessageHandler(customMessageHandler);
    }
    //关闭进程
    ProcessKiller killer;
    killer.killTheProcess(KILL_PROCESS_NAME);
    
	//软件更新检测
    ToolObject toolObj;
    toolObj.HttpPostCheckAppVersion();
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

    GlobalData::strPhoneInstanceScreenshotDir = QCoreApplication::applicationDirPath() + "/screenshots";
    strDir = GlobalData::strPhoneInstanceScreenshotDir;
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }

    

    if (GlobalData::bShowSystemTrayIcon)
    {
        g_trayIcon = new QSystemTrayIcon(QIcon(":/main/resource/main/aboutlogo.png"));
        g_trayIcon->setToolTip("");
        g_trayIcon->show();        
    }    

    LoginWindow w;
    w.show();

    int iResult = app.exec();
    SWRuntime::getInstance()->deinit();
    return iResult;
}
