#ifndef QUEUETABLEITEM_H
#define QUEUETABLEITEM_H

#include <QWidget>

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

    bool uploadFile(const QString& filePath, QStringList strPhoneList);
    bool cancelUploadFile(const QString& filePath, QStringList strPhoneList);
    bool uploadFileCallback(QString filePath, QStringList strPhoneList,int iAutoInstall = 0);
};

#endif // QUEUETABLEITEM_H
