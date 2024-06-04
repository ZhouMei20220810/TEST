#ifndef UPLOADFILEITEMWIDGET_H
#define UPLOADFILEITEMWIDGET_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class UploadFileItemWidget;
}

class UploadFileItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UploadFileItemWidget(QString strFilePath,QWidget *parent = nullptr);
    ~UploadFileItemWidget();

signals:
    void deleteFileItemSignal(QString strFilePath);
private slots:
    void do_DeleteFileItem(bool checked);
private:
    Ui::UploadFileItemWidget *ui;
    QString m_strFilePath;


    QLabel* m_labelSuffix;
    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;
};

#endif // UPLOADFILEITEMWIDGET_H
