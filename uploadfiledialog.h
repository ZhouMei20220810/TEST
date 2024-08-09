#ifndef UPLOADFILEDIALOG_H
#define UPLOADFILEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include "global.h"

namespace Ui {
class UploadFileDialog;
}

class UploadFileDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit UploadFileDialog(QStringList strList,QWidget *parent = nullptr);
    ~UploadFileDialog();

private slots:
    void on_btnClose_clicked();

    void on_toolBtnAddUploadFile_clicked();
    void on_toolBtnUploadQueue_clicked();
    void on_toolBtnUploadHistory_clicked();

    void on_toolBtnSelectFile_clicked();
    void on_toolBtnUpload_clicked();

    void do_deleteFileItemSignal(QString strFilePath);
private:
    //http�ϴ��ļ���ʷ
    void HttpGetUploadFileHistory(int iPage, int iPageSize);
    //��ʾ����
    void LoadUploadFileHistory(QMap<int, S_UPLOADD_FILE_INFO> map);

    void InitWidget(QListWidget* listWidget);

    void GetOSSInfo();
    //�ж��ļ���С�Ƿ񳬹�2G
    bool isFileSizeOver2GB(const QString& filePath);

    Ui::UploadFileDialog *ui;
    QStringList m_strPhoneList;
    QLabel* m_LabelPoint;

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;
};

#endif // UPLOADFILEDIALOG_H
