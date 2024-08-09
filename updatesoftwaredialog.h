#ifndef UPDATESOFTWAREDIALOG_H
#define UPDATESOFTWAREDIALOG_H

#include "global.h"
#include <QProgressBar>
#include <QNetworkAccessManager> 

namespace Ui {
class UpdateSoftwareDialog;
}

class UpdateSoftwareDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit UpdateSoftwareDialog(S_VERSION_INFO versionInfo, QWidget *parent = nullptr);
    ~UpdateSoftwareDialog();
    void startDownload(const QUrl& url, QProgressBar* progressBar);
private slots:
    void on_toolBtnUpdate_clicked();

    void on_toolBtnCancel_clicked();

    void on_btnClose_clicked();
    
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);

    void downloadFinished();
private:
    void callUpdateApp();
private:
    Ui::UpdateSoftwareDialog *ui;
    QProgressBar* m_progressBar;
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_reply;
    S_VERSION_INFO m_versionInfo;
    QString         m_outputFile;
};

#endif // UPDATESOFTWAREDIALOG_H
