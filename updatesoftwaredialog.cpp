#include "updatesoftwaredialog.h"
#include "ui_updatesoftwaredialog.h"
#include <QNetworkReply>
#include <QFile>

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
