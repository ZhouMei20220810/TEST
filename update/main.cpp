#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString logText;
    switch (type) {
    case QtDebugMsg:
        logText = QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
        // 类似的处理其他类型...
    case QtCriticalMsg:
    case QtFatalMsg:
    case QtWarningMsg:
    default:
        // 处理其他日志类型
        break;
    }

    // 这里可以写入文件、发送网络请求、显示在界面上等
    QDate date = QDate::currentDate();
    QString strDate = QString("/YSY_Log_%1.txt").arg(date.toString("yyyy-MM-dd"));
    QFile file(QDir::tempPath() + strDate);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&file);
        out << logText;
        file.close();
    }
}

int installMsiSilently(const QString& msiFilePath,const QString& strExeFolder)
{
    
    // msiexec命令格式用于静默安装
    QString command = "msiexec /i \"";
    command.append(msiFilePath); // 添加MSI文件的完整路径
    //command.append("\""); // /qn 参数表示非静默安装，有界面
    command.append("\" /qn"); // /qn 参数表示静默安装，无界面
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    QString strMsi = setting.value("UpdateMsiPath", "").toString();
    QString strExe = setting.value("UpdateExe","").toString();
    int iLastIndex = strExe.lastIndexOf('\\');
    QString strExeFolder = strExe.left(iLastIndex);
    qDebug() << "folder = " << strExeFolder;
    int bWriteLogFile = setting.value("WriteLogFile", true).toBool();
    if (bWriteLogFile)
    {
        qInstallMessageHandler(customMessageHandler);
    }
    if(strMsi.isEmpty())
    {
        QMessageBox::warning(NULL,"错误提示","当前没有可更新程序");
        return -1;
    }
    qDebug()<<"strMsi="<<strMsi;

    int exitCode = installMsiSilently(strMsi,strExeFolder);
    //MainWindow w;
    //w.show();

    if (!strExe.isEmpty())
    {
        QFile file(strExe);
        if (file.exists())
        {
            // 使用QProcess执行命令
            QProcess process;
            process.start(strExe);
            process.waitForFinished(-1); // 等待进程结束，-1表示无限制等待时间

            // 检查进程退出代码以确定安装是否成功
            int exitCode = process.exitCode();
            if (exitCode == 0) {
                qDebug() << "MSI installed successfully."<< strExe;
            }
            else {
                qDebug() << "MSI installation failed with exit code:" << exitCode<<strExe;
            }
        }
        else
        {
            qDebug() << "文件不存在." << strExe;
        }
    }
    return exitCode; //a.exec();
}
