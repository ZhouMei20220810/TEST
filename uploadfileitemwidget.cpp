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
    btnDel->resize(12,12);
    btnDel->setStyleSheet("QToolButton {border: none; border-radius: 999px; background: transparent;background-image: url(:/main/resource/main/default_photoUrl.png);}");
    btnDel->setIcon(QIcon(":/main/resource/main/photo_border.png"));
    btnDel->setIconSize(QSize(12,12));
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
