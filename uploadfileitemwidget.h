#ifndef UPLOADFILEITEMWIDGET_H
#define UPLOADFILEITEMWIDGET_H

#include <QWidget>

namespace Ui {
class UploadFileItemWidget;
}

class UploadFileItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UploadFileItemWidget(QString strFilePath,QWidget *parent = nullptr);
    ~UploadFileItemWidget();

private:
    Ui::UploadFileItemWidget *ui;
};

#endif // UPLOADFILEITEMWIDGET_H
