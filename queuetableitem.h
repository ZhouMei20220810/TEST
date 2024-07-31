#ifndef QUEUETABLEITEM_H
#define QUEUETABLEITEM_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class QueueTableItem;
}

class QueueTableItem : public QWidget
{
    Q_OBJECT

public:
    explicit QueueTableItem(QStringList strPhoneList,QString strFilePath,QWidget *parent = nullptr);
    ~QueueTableItem();
signals:
    void deleteQueueFileItemSignal(QString strFilePath);
private slots:
    void on_toolBtnCancel_clicked();
    void on_toolBtnDelete_clicked();
    void on_toolBtnReupload_clicked();

private:
    Ui::QueueTableItem *ui;
    QString m_strFilePath;
    QStringList m_strPhoneList;
    std::string m_UploadId;
    QTimer* m_Timer;
    bool    m_IsAbort;

    //�ϴε�OSS
    //bool uploadFile(const QString& filePath, QStringList strPhoneList, int iIsAutoInstall = 1);
    //ȡ���ļ��ϴ�
    bool cancelUploadFile(const QString& filePath, QStringList strPhoneList);
    //ɾ���ļ�
    bool DeleteFile(const QString& filePath, QStringList strPhoneList);
};

#endif // QUEUETABLEITEM_H
