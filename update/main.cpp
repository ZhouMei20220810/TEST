#include "mainwindow.h"

#include <QApplication>
#include <QProcess>
#include <QSettings>
#include <QMessageBox>
#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"

int installMsiSilently(const QString& msiFilePath)
{
    // msiexec命令格式用于静默安装
    QString command = "msiexec /i \"";
    command.append(msiFilePath); // 添加MSI文件的完整路径
    //command.append("\" /qn"); // /qn 参数表示静默安装，无界面

    // 使用QProcess执行命令
    QProcess process;
    process.start(command);
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
    if(strMsi.isEmpty())
    {
        QMessageBox::warning(NULL,"错误提示","当前没有可更新程序");
        return -1;
    }
    qDebug()<<"strMsi="<<strMsi;

    int exitCode = installMsiSilently(strMsi);
    //MainWindow w;
    //w.show();
    if(!strExe.isEmpty())
    {

    }
    return exitCode; //a.exec();
}
