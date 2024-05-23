#include "uploadfileitemwidget.h"
#include "ui_uploadfileitemwidget.h"

UploadFileItemWidget::UploadFileItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFileItemWidget)
{
    ui->setupUi(this);
}

UploadFileItemWidget::~UploadFileItemWidget()
{
    delete ui;
}
