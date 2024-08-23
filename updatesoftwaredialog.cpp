#include "updatesoftwaredialog.h"
#include "ui_updatesoftwaredialog.h"
#include <QNetworkReply>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QDir>
#include <QGraphicsDropShadowEffect>

UpdateSoftwareDialog::UpdateSoftwareDialog(S_VERSION_INFO versionInfo, QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::UpdateSoftwareDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("检查更新");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    m_versionInfo = versionInfo;


    ui->labelUpdateVersion->setText(versionInfo.strVersion);
    ui->labelUpdateVersion_2->setText(versionInfo.strVersion);
    ui->labelUpdateVersion_3->setText(versionInfo.strVersion);

    ui->textEditUpdateContent->setText(versionInfo.strUpdateContents);
    //设置为只读
    ui->textEditUpdateContent->setReadOnly(true);
    //禁止文本选中
    ui->textEditUpdateContent->setTextInteractionFlags(Qt::NoTextInteraction);
    //禁止鼠标光标变为文本选择光标
    ui->textEditUpdateContent->viewport()->setCursor(Qt::ArrowCursor);
    ui->stackedWidget->setCurrentWidget(ui->pageCheckUpdate);
}

UpdateSoftwareDialog::~UpdateSoftwareDialog()
{
    delete ui;
}

void UpdateSoftwareDialog::on_toolBtnUpdate_clicked()
{
    //跳转到更新页面
    ui->stackedWidget->setCurrentWidget(ui->pageUpdating);
    //立即更新
    //1、下载文件
    //2、安装文件
    //3、重启应用
    ui->toolBtnUpdate->setEnabled(false);
    this->setWindowTitle("更新中");
    ui->labelTitle->setText("更新中");
    
    ui->labelProgressValue->setText("已更新 1%");
    startDownload(m_versionInfo.strDownloadUrl, ui->progressBar);
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
}

void UpdateSoftwareDialog::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        //float progress = (bytesReceived * 1.0) / bytesTotal*100;
		float progress = (bytesReceived * 100) / bytesTotal;
        m_progressBar->setValue(progress);
        //qDebug() << "progress = " << progress;
        ui->labelProgressValue->setText(QString("已更新 %1%").arg(progress));
    }
}

void UpdateSoftwareDialog::downloadFinished()
{    
    QNetworkReply::NetworkError err =  m_reply->error();
    if (err == QNetworkReply::NoError) {
        // 成功下载，处理文件
        QFile file(m_outputFile);
        if (file.exists())
        {
            if (!file.remove())
            {
                qDebug() << "delete msi file failed." << m_outputFile;
            }
        }
        if (file.open(QIODevice::WriteOnly)) {
            file.write(m_reply->readAll());
            file.close();
            qDebug() << "Downloaded successfully!" << m_outputFile;
            QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
            setting.setValue("UpdateMsiPath", QDir::toNativeSeparators(m_outputFile));
            setting.setValue("UpdateExe", QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
            setting.setValue("UpdateVersion", m_versionInfo.strVersion);
            callUpdateApp();
        }
        else {
            qDebug() << "Failed to save the file!"<< m_outputFile;
            ui->labelTitle->setText("更新失败");
            setWindowTitle("更新失败");
            ui->stackedWidget->setCurrentWidget(ui->pageUpdateError);
        }
    }
    else {
        qDebug() << "Download failed: " << m_reply->errorString();
        ui->labelTitle->setText("更新失败");
        setWindowTitle("更新失败");
        ui->stackedWidget->setCurrentWidget(ui->pageUpdateError);
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
    QString strUpdateExe = QCoreApplication::applicationDirPath() + "/update.exe";
    QString strUpdateExe2 = QCoreApplication::applicationDirPath() + "/updateYSY.exe";
    QFile fileUpdate(strUpdateExe2);
    if (fileUpdate.exists())
    {
        fileUpdate.remove();
        fileUpdate.close();
    }
    QFile file(strUpdateExe);
    if (file.exists())
    {
        if (!file.rename(strUpdateExe2))
        {
            qDebug() << "file rename failed" << strUpdateExe2;
        }
    }
    else
    {
        qDebug() << "update app exe not exists.";
        return;
    }
    qDebug() << "Run update exe " << strUpdateExe2;
    // 使用QProcess执行命令
    QProcess process;
    process.start(strUpdateExe2);
    process.waitForFinished(5000); // 等待进程结束，-1表示无限制等待时间

    // 检查进程退出代码以确定安装是否成功
    int exitCode = process.exitCode();
    if (exitCode == 0) {
        qDebug() << "Run update exe successfully.";
    }
    else {
        qDebug() << "Run update exe failed with exit code:" << exitCode;
    }
}


void UpdateSoftwareDialog::on_toolBtnReupdate_clicked()
{
    //重新更新
    on_toolBtnUpdate_clicked();
}

