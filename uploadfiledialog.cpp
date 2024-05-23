#include "uploadfiledialog.h"
#include "ui_uploadfiledialog.h"
#include <QFileDialog>

UploadFileDialog::UploadFileDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UploadFileDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //透明背景
    //setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->toolBtnUpload->setDisabled(true);
}

UploadFileDialog::~UploadFileDialog()
{
    delete ui;
}

void UploadFileDialog::on_btnClose_clicked()
{
    this->close();
}


void UploadFileDialog::on_toolBtnAddUploadFile_clicked()
{
    if(ui->listWidgetChooseFile->count() > 0)
        ui->stackedWidget->setCurrentWidget(ui->pageChooseFile);
    else
        ui->stackedWidget->setCurrentWidget(ui->page);
}


void UploadFileDialog::on_toolBtnUploadQueue_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageQueue);
}


void UploadFileDialog::on_toolBtnUploadHistory_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->pageHistory);
}


void UploadFileDialog::on_toolBtnSelectFile_2_clicked()
{
    SelectFile();
}


void UploadFileDialog::on_toolBtnUpload_2_clicked()
{
    uploadFile();
}


void UploadFileDialog::on_toolBtnSelectFile_clicked()
{
    SelectFile();
}


void UploadFileDialog::on_toolBtnUpload_clicked()
{
    uploadFile();
}

void UploadFileDialog::SelectFile()
{
    QString strCurPath = QDir::currentPath();
    QStringList strFileList = QFileDialog::getOpenFileNames(this,"选择文件",strCurPath,"请选择上传文件(*.*)");
    int iFileSize = strFileList.size();
    if(iFileSize > 0)
    {
        for(int i = 0; i < iFileSize; i++)
        {
            qDebug()<<"第"<<i<<"个："<<strFileList.at(i);
        }
        ui->stackedWidget->setCurrentWidget(ui->pageChooseFile);
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}

void UploadFileDialog::uploadFile()
{

}
