#ifndef UPLOADFILEDIALOG_H
#define UPLOADFILEDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include "global.h"

namespace Ui {
class UploadFileDialog;
}

class UploadFileDialog : public QDialog
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
    //http上传文件历史
    void HttpGetUploadFileHistory(int iPage, int iPageSize);
    //显示数据
    void LoadUploadFileHistory(QMap<int, S_UPLOADD_FILE_INFO> map);

    void InitWidget(QListWidget* listWidget);

    void SelectFile();
    void uploadFile();

    void GetOSSInfo();

    Ui::UploadFileDialog *ui;
    QStringList m_strPhoneList;
    QLabel* m_LabelPoint;

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;
};

#endif // UPLOADFILEDIALOG_H
