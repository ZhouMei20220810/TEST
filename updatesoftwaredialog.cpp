#include "updatesoftwaredialog.h"
#include "ui_updatesoftwaredialog.h"
#include <QNetworkReply>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QDir>

UpdateSoftwareDialog::UpdateSoftwareDialog(S_VERSION_INFO versionInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateSoftwareDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    m_versionInfo = versionInfo;

    if (versionInfo.iIsFurcedUpdate)
        ui->toolBtnCancel->setVisible(false);
    else
        ui->toolBtnCancel->setVisible(true);
}

UpdateSoftwareDialog::~UpdateSoftwareDialog()
{
    delete ui;
}

void UpdateSoftwareDialog::on_toolBtnUpdate_clicked()
{
    //立即更新
    //1、下载文件
    //2、安装文件
    //3、重启应用
    startDownload(m_versionInfo.strDownloadUrl, ui->progressBar);
}


void UpdateSoftwareDialog::on_toolBtnCancel_clicked()
{
    //以后再说
    this->close();
}


void UpdateSoftwareDialog::on_btnClose_clicked()
{
    this->close();
}

void UpdateSoftwareDialog::startDownload(const QUrl& url, QProgressBar* progressBar)
{
    m_progressBar = progressBar;
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished,
        this, &UpdateSoftwareDialog::downloadFinished);

    QNetworkRequest request(url);
    QString strUrl(m_versionInfo.strDownloadUrl);
    QString strFileName = strUrl.right(strUrl.size() - strUrl.lastIndexOf('/') - 1);
    qDebug() << "url=" << strUrl << "strFileName=" << strFileName;
    m_outputFile = GlobalData::strPictureTempDir + strFileName;

    m_reply = m_networkManager->get(request);
    connect(m_reply, &QNetworkReply::downloadProgress, this, &UpdateSoftwareDialog::updateProgress);
    //connect(m_reply, &QNetworkReply::fini)
}

void UpdateSoftwareDialog::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        int progress = (bytesReceived * 100) / bytesTotal;
        m_progressBar->setValue(progress);
    }
}

void UpdateSoftwareDialog::downloadFinished()
{
    if (m_reply->error() == QNetworkReply::NoError) {
        // 成功下载，处理文件
        QFile file(m_outputFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(m_reply->readAll());
            file.close();
            qDebug() << "Downloaded successfully!" << m_outputFile;
            QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
            setting.setValue("UpdateMsiPath", QDir::toNativeSeparators(m_outputFile));
            setting.setValue("UpdateExe", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
            callUpdateApp();
        }
        else {
            qDebug() << "Failed to save the file!";
        }
    }
    else {
        qDebug() << "Download failed: " << m_reply->errorString();
    }

    m_reply->deleteLater();
    m_networkManager->deleteLater();
}

void UpdateSoftwareDialog::callUpdateApp()
{
    // msiexec命令格式用于静默安装
    /*QString command = "msiexec /i \"";
    command.append(msiFilePath); // 添加MSI文件的完整路径
    command.append("\" /qn"); // /qn 参数表示静默安装，无界面
    */
    QString command = QCoreApplication::applicationDirPath() + "/Update.exe";
    qDebug() << "command=" << command;
    // 使用QProcess执行命令
    QProcess process;
    process.start(command);
    process.waitForFinished(-1); // 等待进程结束，-1表示无限制等待时间

    // 检查进程退出代码以确定安装是否成功
    int exitCode = process.exitCode();
    if (exitCode == 0) {
        qDebug() << "MSI installed successfully.";
    }
    else {
        qDebug() << "MSI installation failed with exit code:" << exitCode;
    }
}