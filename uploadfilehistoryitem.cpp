#include "uploadfilehistoryitem.h"
#include "ui_uploadfilehistoryitem.h"

UploadFileHistoryItem::UploadFileHistoryItem(S_UPLOADD_FILE_INFO uploadInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFileHistoryItem)
{
    ui->setupUi(this);

    QFontMetrics fontWidth(ui->labelFileName->font());
    QString strElideNote = fontWidth.elidedText(uploadInfo.strFileName, Qt::ElideRight, 190);
    ui->labelFileName->setText(strElideNote);
    QString style = "QLabel{border:none;background:transparent;}QToolTip {background-color:rgba(255,255,255,255); border:0px solid red; height:20}";
    ui->labelFileName->setStyleSheet(style);
    ui->labelFileName->setToolTip(uploadInfo.strFileName);
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
