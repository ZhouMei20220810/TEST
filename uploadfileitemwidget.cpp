#include "uploadfileitemwidget.h"
#include "ui_uploadfileitemwidget.h"
#include <QFileInfo>

UploadFileItemWidget::UploadFileItemWidget(QString strFilePath,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFileItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QFileInfo fileInfo(strFilePath);
    ui->labelFileName->setText(fileInfo.fileName());
    ui->labelFileName->setToolTip(fileInfo.fileName());


}

UploadFileItemWidget::~UploadFileItemWidget()
{
    delete ui;
}
