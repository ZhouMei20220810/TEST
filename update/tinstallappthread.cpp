#include "tinstallappthread.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QApplication>
#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"

TInstallAppThread::TInstallAppThread(QObject* parent):QThread(parent)
{
}

int installMsiSilently(const QString& msiFilePath, const QString& strExeFolder)
{
    // msiexec命令格式用于静默安装
    QString command = "msiexec /i \"";
    command.append(msiFilePath); // 添加MSI文件的完整路径
    //command.append("\""); // /qn 参数表示非静默安装，有界面
    command.append("\" /qn"); // /qn 参数表示静默安装，无界面
    command.append(" /norestart"); // /qn 确保不会发生重启
    command.append(QString(" TARGETDIR=\"%1\"").arg(strExeFolder));
    qDebug() << "command=" << command;
    QString strBatFile = QDir::tempPath() + "/update.bat";
    QFile file(strBatFile);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out << command;
        file.close();
    }

    // 使用QProcess执行命令
    QProcess process;
    process.start(strBatFile);
    process.waitForFinished(-1); // 等待进程结束，-1表示无限制等待时间

    // 检查进程退出代码以确定安装是否成功
    int exitCode = process.exitCode();
    if (exitCode == 0) {
        qDebug() << "MSI installed successfully.";
        return exitCode;
    }
    else {
        qDebug() << "MSI installation failed with exit code:" << exitCode;
        return exitCode;
    }
}


void TInstallAppThread::run()
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    QString strMsi = setting.value("UpdateMsiPath", "").toString();
    QString strExe = setting.value("UpdateExe", "").toString();
    int iLastIndex = strExe.lastIndexOf('\\');
    QString strExeFolder = strExe.left(iLastIndex);
    showPrograssValueSignals(50);
    //安装msi
    int exitCode = installMsiSilently(strMsi, strExeFolder);
    showPrograssValueSignals(80);

    qDebug() << "folder = " << strExeFolder;
    if (strMsi.isEmpty())
    {
        qDebug() << "not found file." << strMsi;
        return;
    }
    showPrograssValueSignals(100);
    qDebug() << "strMsi=" << strMsi;

    //自动重启
    //立即重启
    if (!strExe.isEmpty())
    {
        QFile file(strExe);
        if (file.exists())
        {
            // 使用QProcess执行命令
            QProcess process;
            process.startDetached(strExe);
            //process.start(strExe);
            //process.waitForFinished(-1); // 等待进程结束，-1表示无限制等待时间
            QApplication::exit();
            //emit installFinishSignals();
            qDebug() << "restart app successfully." << strExe;
            // 检查进程退出代码以确定安装是否成功
            //int exitCode = process.exitCode();
            /*if (exitCode == 0)
            {
                qDebug() << "restart app successfully." << strExe;
            }
            else {
                qDebug() << "restart app failed with exit code:" << exitCode << strExe;
            }*/
        }
        else
        {
            qDebug() << "文件不存在." << strExe;
        }
    }
}
