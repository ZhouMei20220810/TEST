#include "tinstallappthread.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QApplication>
#include <QTextStream>
#include <winerror.h>
#include <QString>
#include <QByteArray>
#include <Windows.h>
#include <ShlObj.h>
#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"
#define     PROGRASS_INTERVAL       15

TInstallAppThread::TInstallAppThread(QObject* parent):QThread(parent)
{
}

QString getUserProgramsMenuPath() 
{
    wchar_t path[256] = {0};
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROGRAMS, NULL, 0, path))) 
    {
        return QString::fromWCharArray(path);
    }
    return QString();
}

int TInstallAppThread::installMsiSilently(const QString& msiFilePath, const QString& strExeFolder,const QString& strProductCode,int iPrograssValue)
{
    //卸载原来的软件
    //V1.0.10 ProductCode必须更新
    /*QString uninstallCommand = QString("msiexec /x %1 /qb").arg(strProductCode);
    QString strBatFile = QDir::tempPath() + "/update.bat";
    QFile file(strBatFile);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::System);
        out << uninstallCommand;
        qDebug() << "write command=" << uninstallCommand;
        file.close();
        showPrograssValueSignals(50);
    }
    else
    {
        qDebug() << "open failed." << strBatFile;
    }

    QProcess uninstallProcess;
    uninstallProcess.start(strBatFile);
    //process.start(command);
    if (!uninstallProcess.waitForFinished(-1)) // 等待进程结束，-1表示无限制等待时间
    {
        qDebug() << "uninstall command=" << uninstallCommand << "return false. error=" << uninstallProcess.readAllStandardError();
        return -1;
    }
    else
    {
        showPrograssValueSignals(60);
        qDebug() <<"uninstall command="<< uninstallCommand << " return true";
    }*/

    QString strBatFile = QDir::tempPath() + "/update.bat";
    QFile file(strBatFile);
    // msiexec命令格式用于静默安装
    QString command = "msiexec.exe /i \"";
    command.append(msiFilePath); // 添加MSI文件的完整路径
    //command.append("\""); // /qn 参数表示非静默安装，有界面
    command.append("\" /qn"); // /qn 参数表示静默安装，无界面
    command.append(" /norestart"); // /qn 确保不会发生重启
    command.append(QString(" TARGETDIR=\"%1\"").arg(strExeFolder));    
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::System);
        out << command;
        qDebug() << "write command=" << command;
        file.close();
        iPrograssValue += PROGRASS_INTERVAL;
        showPrograssValueSignals(iPrograssValue);
    }
    else
    {
        qDebug() << "open failed." << strBatFile;
    }
    iPrograssValue += PROGRASS_INTERVAL;
    showPrograssValueSignals(iPrograssValue);
    // 使用QProcess执行命令
    QProcess process;
    process.start(strBatFile);
    //process.start(command);
    if (!process.waitForFinished(-1)) // 等待进程结束，-1表示无限制等待时间
    {
        qDebug() << "waitForFinished return false. error="<<process.readAllStandardError();
        return -1;
    }
    else
    {
        iPrograssValue += PROGRASS_INTERVAL;
        showPrograssValueSignals(iPrograssValue);
        qDebug() << "waitForFinished return true";
    }
    QString strOutput = process.readAllStandardOutput();
    qDebug() << "output:" << strOutput;
    iPrograssValue += PROGRASS_INTERVAL;
    showPrograssValueSignals(iPrograssValue);
    // 检查进程退出代码以确定安装是否成功
    int exitCode = process.exitCode();
    switch (exitCode)
    {
    case ERROR_SUCCESS:
        qDebug() << "MSI installed successfully.exit code:"<< exitCode;
        break;
    case ERROR_SUCCESS_REBOOT_REQUIRED:
        qDebug() << "MSI installed successfully. neet restart.exit code=" << exitCode;
        break;
    default:
        qDebug() << "MSI installation failed with exit code:" << exitCode;
        break;
    }
    iPrograssValue += PROGRASS_INTERVAL;
    showPrograssValueSignals(iPrograssValue);
    return exitCode;
}


void TInstallAppThread::run()
{
    int iPrograssValue = 10;
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    QString strMsi = setting.value("UpdateMsiPath", "").toString();
    QString strExe = setting.value("UpdateExe", "").toString();
    QString strProductCode = setting.value("ProductCode", "").toString();
    bool bIsUninstallHighVersion = setting.value("IsUninstallHighVersion", false).toBool();
    int iLastIndex = strExe.lastIndexOf('\\');
    QString strExeFolder = strExe.left(iLastIndex);
    if (bIsUninstallHighVersion)
    {
        //获取快捷建目录  User's Programs Menu
        QString programsMenuPath = getUserProgramsMenuPath();
        qDebug() << "User's Programs Menu Path:" << programsMenuPath;
        showPrograssValueSignals(iPrograssValue);

        QString appUninstallShortCut = programsMenuPath + "\\卸载易舜云手机.lnk";
        iPrograssValue += PROGRASS_INTERVAL;
        showPrograssValueSignals(iPrograssValue);
        //卸载
        QString wShortcutPath = QString::fromWCharArray(appUninstallShortCut.toStdWString().c_str());

        // 使用ShellExecute打开快捷方式
        HINSTANCE result = ShellExecute(NULL, L"open", (LPCWSTR)wShortcutPath.utf16(), NULL, NULL, SW_SHOWNORMAL);

        if ((DWORD)result <= 32) {
            // 处理错误
            qDebug() << "Error launching shortcut. result=" << result;
        }
        iPrograssValue += PROGRASS_INTERVAL;
        if (iPrograssValue >= 100)
            iPrograssValue = 100 - 15;
        showPrograssValueSignals(iPrograssValue);
    }
    
    //安装msi
    qDebug() << "folder = " << strExeFolder;
    if (strMsi.isEmpty())
    {
        qDebug() << "not found file." << strMsi;
        return;
    }
    iPrograssValue += PROGRASS_INTERVAL;
    if (iPrograssValue >= 100)
        iPrograssValue = 100 - 10;
    showPrograssValueSignals(iPrograssValue);
    int exitCode = installMsiSilently(strMsi, strExeFolder,strProductCode, iPrograssValue);
    if (exitCode != 0)
    {
        qDebug() << "installMsiSilently failed.";
        return; 
    }
    
    iPrograssValue += PROGRASS_INTERVAL;
    if (iPrograssValue >= 100)
        iPrograssValue = 100-5;
    showPrograssValueSignals(iPrograssValue);
    qDebug() << "strMsi=" << strMsi;

    //自动重启
    //立即重启
    if (!strExe.isEmpty())
    {
        QFile file(strExe);
        if (file.exists())
        {
            // 使用QProcess执行命令
            QProcess* process = new QProcess;
            //异步启动
            //bool bSuccess = process.startDetached(strExe);
            //bool bStarted = process.waitForStarted(2000);
            process->start(strExe);
            bool bStart =  process->waitForStarted(5000);
            if (bStart)
            {
                showPrograssValueSignals(100);
                qDebug() << "restart app successfully." << strExe << "bStart=" << bStart;
            }
            else
                qDebug() << "restart app failed." << strExe << "bStart=" << bStart;
            //process.start(strExe);
            //process.waitForFinished(-1); // 等待进程结束，-1表示无限制等待时间
            //QApplication::exit();
            emit hideWindowSignals();
        }
        else
        {
            qDebug() << "文件不存在." << strExe;
        }
    }
}
