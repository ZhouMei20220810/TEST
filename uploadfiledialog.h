#ifndef UPLOADFILEDIALOG_H
#define UPLOADFILEDIALOG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class UploadFileDialog;
}

class UploadFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UploadFileDialog(QWidget *parent = nullptr);
    ~UploadFileDialog();

private slots:
    void on_btnClose_clicked();

    void on_toolBtnAddUploadFile_clicked();
    void on_toolBtnUploadQueue_clicked();
    void on_toolBtnUploadHistory_clicked();

    void on_toolBtnSelectFile_2_clicked();
    void on_toolBtnUpload_2_clicked();
    void on_toolBtnSelectFile_clicked();
    void on_toolBtnUpload_clicked();

private:
    void InitWidget(QListWidget* listWidget);

    void SelectFile();
    void uploadFile();

    Ui::UploadFileDialog *ui;
};

#endif // UPLOADFILEDIALOG_H
