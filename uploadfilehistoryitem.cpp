#include "uploadfilehistoryitem.h"
#include "ui_uploadfilehistoryitem.h"

UploadFileHistoryItem::UploadFileHistoryItem(S_UPLOADD_FILE_INFO uploadInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFileHistoryItem)
{
    ui->setupUi(this);

    ui->labelFileName->setText(uploadInfo.strFileName);
    ui->labelCreateTime->setText(uploadInfo.strCreateTime);
    switch (uploadInfo.iStatus)
    {
    case 1:
        ui->labelUploadStatus->setText("成功");
        break;
    case 2:
        ui->labelUploadStatus->setText("失败");
        break;
    default:
        break;
    }
}

UploadFileHistoryItem::~UploadFileHistoryItem()
{
    delete ui;
}
