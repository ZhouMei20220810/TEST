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

    void uploadFile(const QString& filePath, QStringList strPhoneList);
};

#endif // QUEUETABLEITEM_H
