#include "uploadfiledialog.h"
#include "ui_uploadfiledialog.h"
#include <QFileDialog>
#include "uploadfileitemwidget.h"
#include "global.h"

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

    InitWidget(ui->listWidgetChooseFile);
}

UploadFileDialog::~UploadFileDialog()
{
    delete ui;
}

void UploadFileDialog::InitWidget(QListWidget* listWidget)
{
    listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    listWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    listWidget->setMovement(QListWidget::Static);
    //设置单选
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
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
        QListWidgetItem* item = NULL;
        UploadFileItemWidget* fileItem = NULL;
        for(int i = 0; i < iFileSize; i++)
        {
            qDebug()<<"第"<<i<<"个："<<strFileList.at(i);
            item = new QListWidgetItem(ui->listWidgetChooseFile);

            fileItem = new UploadFileItemWidget(strFileList.at(i),this);
            item->setSizeHint(QSize(fileItem->size()));
            ui->listWidgetChooseFile->addItem(item);
            ui->listWidgetChooseFile->setItemWidget(item, fileItem);
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
