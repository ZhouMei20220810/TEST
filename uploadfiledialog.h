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

private:
    Ui::UploadFileDialog *ui;
};

#endif // UPLOADFILEDIALOG_H
