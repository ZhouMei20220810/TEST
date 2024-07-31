#ifndef TUPLOADFILETHREAD_H
#define TUPLOADFILETHREAD_H

#include <QObject>
#include <QThread>
#include <QProgressBar>

class TUploadFileThread : public QThread
{
    Q_OBJECT
public:
    explicit TUploadFileThread(QObject *parent = nullptr);
    void setUploadFileParam(const QString& filePath, QStringList strPhoneList, int iIsAutoInstall,QProgressBar* progressBar);

protected:
    void run() override;

private:
    QString m_filePath;
    QStringList m_strPhoneList;
    int        m_iIsAutoInstall;
    QProgressBar* m_progressBar;

    std::string m_UploadId;
};

#endif // TUPLOADFILETHREAD_H
