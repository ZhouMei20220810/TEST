#include "uploadfileitemwidget.h"
#include "ui_uploadfileitemwidget.h"
#include <QFileInfo>
#include <QToolButton>

UploadFileItemWidget::UploadFileItemWidget(QString strFilePath,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UploadFileItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_strFilePath = strFilePath;

    QFileInfo fileInfo(strFilePath);
    ui->labelFileName->setText(fileInfo.baseName());
    ui->labelFileName->setToolTip(fileInfo.baseName());

    QLabel* labelSuffix = new QLabel(ui->labelBg);
    labelSuffix->setStyleSheet("background-color:transparent;color:white");
    labelSuffix->setAlignment(Qt::AlignCenter);
    labelSuffix->setText(fileInfo.suffix());

    //ui->labelBg->hasSelectedText();
    QToolButton* btnDel = new QToolButton(this);
    connect(btnDel, &QToolButton::clicked, this, &UploadFileItemWidget::do_DeleteFileItem);
    btnDel->move(0, 0);
    btnDel->resize(16,16);
    btnDel->setStyleSheet("QToolButton{border:none;border-radius:999px;background:transparent;background-image:url(:/resource/upload/fileDel.png);}");
    //btnDel->setIcon(QIcon(":/resource/upload/fileDel.png"));
    btnDel->setIconSize(QSize(16,16));
    qDebug()<<"bundleName"<<fileInfo.bundleName() <<"baseName="<<fileInfo.baseName();
    qDebug()<<"suffix="<<fileInfo.suffix();

}

UploadFileItemWidget::~UploadFileItemWidget()
{
    delete ui;
}

void UploadFileItemWidget::do_DeleteFileItem(bool checked)
{
    emit deleteFileItemSignal(m_strFilePath);
}
