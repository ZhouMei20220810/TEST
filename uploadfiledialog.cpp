#include "uploadfiledialog.h"
#include "ui_uploadfiledialog.h"

UploadFileDialog::UploadFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UploadFileDialog)
{
    ui->setupUi(this);
}

UploadFileDialog::~UploadFileDialog()
{
    delete ui;
}
