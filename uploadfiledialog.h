#ifndef UPLOADFILEDIALOG_H
#define UPLOADFILEDIALOG_H

#include <QDialog>

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

private:
    Ui::UploadFileDialog *ui;
};

#endif // UPLOADFILEDIALOG_H
