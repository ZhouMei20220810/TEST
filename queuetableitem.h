#ifndef QUEUETABLEITEM_H
#define QUEUETABLEITEM_H

#include <QWidget>
#include <QTimer>
#include "global.h"

namespace Ui {
class QueueTableItem;
}

class QueueTableItem : public QWidget
{
    Q_OBJECT

public:
    explicit QueueTableItem(QStringList strPhoneList,QString strFilePath,QWidget *parent = nullptr);
    ~QueueTableItem();

    void RefreshPrograssBarValue(int64_t transfered, int64_t total);
    
signals:
    void deleteQueueFileItemSignal(QString strFilePath);
private slots:
    void on_toolBtnCancel_clicked();
    void on_toolBtnDelete_clicked();
    void on_toolBtnReupload_clicked();
private:
    //�ϴε�OSS
    //bool uploadFile(const QString& filePath, QStringList strPhoneList, int iIsAutoInstall = 1);
    //ȡ���ļ��ϴ�
    bool cancelUploadFile(const QString& filePath, QStringList strPhoneList);
    //ɾ���ļ�
    bool DeleteFile(const QString& filePath, QStringList strPhoneList);
    //1���ļ�md5У��
    void HttpPostFileCheckMd5(QString strFileMd5);
    //2������ļ�md5ֵ
    void HttpPostFileUploadBaidu(S_UPLOADD_FILE_INFO uploadInfo);
    void startThreadUploadFile();
    void downloadFinishRefreshUI();
private:
    Ui::QueueTableItem *ui;
    QString m_strFilePath;
    QStringList m_strPhoneList;
    std::string m_UploadId;
    QTimer* m_Timer;
    bool    m_IsAbort;

    qint64 m_i64FileSize;
};

#endif // QUEUETABLEITEM_H
