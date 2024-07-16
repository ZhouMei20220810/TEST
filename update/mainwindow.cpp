#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QProcess>
#include <QApplication>
#include <Windows.h>
#include <QString>
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolBtnCancel_clicked()
{
    this->close();
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

void MainWindow::on_toolBtnUpdate_clicked()
{
    //杀掉当前进程
    ui->progressBar->setValue(10);
    ProcessKiller killer;
    killer.killTheProcess("YiShunYun.exe");
    ui->progressBar->setValue(30);

    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    QString strMsi = setting.value("UpdateMsiPath", "").toString();
    QString strExe = setting.value("UpdateExe","").toString();
    int iLastIndex = strExe.lastIndexOf('\\');
    QString strExeFolder = strExe.left(iLastIndex);
    //安装msi
    int exitCode = installMsiSilently(strMsi,strExeFolder);
    ui->progressBar->setValue(50);
    int bWriteLogFile = setting.value("WriteLogFile", true).toBool();
    if (bWriteLogFile)
    {
        qInstallMessageHandler(customMessageHandler);
    }
    qDebug() << "folder = " << strExeFolder;
    if(strMsi.isEmpty())
    {
        QMessageBox::warning(NULL,"错误提示","当前没有可更新程序");
        return;
    }
    ui->progressBar->setValue(80);
    qDebug()<<"strMsi="<<strMsi;
    //启动软件
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
    ui->progressBar->setValue(100);
}


void MainWindow::on_btnClose_clicked()
{
    this->close();
}

