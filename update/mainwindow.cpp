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
#include "messagetipsdialog.h"

#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"
#define     KILL_PROCESS_NAME       "YiShunYun.exe"

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

    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    int bWriteLogFile = setting.value("WriteLogFile", true).toBool();
    if (bWriteLogFile)
    {
        qInstallMessageHandler(customMessageHandler);
    }

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    //自动重启
    ui->toolBtnUpdate->setVisible(false);

    //杀掉当前进程
    ui->progressBar->setValue(10);
    ProcessKiller killer;
    killer.killTheProcess(KILL_PROCESS_NAME);
    ui->progressBar->setValue(30);
    m_thread = new TInstallAppThread(this);
    connect(m_thread, &TInstallAppThread::showPrograssValueSignals, this, &MainWindow::do_showPrograssValueSignals);
    m_thread->start();
}

void MainWindow::do_showPrograssValueSignals(int value)
{
    if (value == 100)
        ui->toolBtnCancel->setEnabled(false);
    ui->progressBar->setValue(value);
}

MainWindow::~MainWindow()
{
    if (m_thread->isRunning())
    {
        m_thread->quit();
    }
    delete ui;
}

void MainWindow::on_toolBtnCancel_clicked()
{
    MessageTipsDialog* dialog = new MessageTipsDialog("程序正在安装,中断可能导致软件不可用,请确定是否退出");
    if(QDialog::Accepted == dialog->exec())
    {
        this->close();
    }
}

void MainWindow::on_btnClose_clicked()
{
    this->close();
}

