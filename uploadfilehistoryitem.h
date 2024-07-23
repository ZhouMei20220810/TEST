#ifndef UPLOADFILEHISTORYITEM_H
#define UPLOADFILEHISTORYITEM_H

#include <QWidget>
#include "global.h"

namespace Ui {
class UploadFileHistoryItem;
}

class UploadFileHistoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit UploadFileHistoryItem(S_UPLOADD_FILE_INFO uploadInfo,QWidget *parent = nullptr);
    ~UploadFileHistoryItem();

private:
    Ui::UploadFileHistoryItem *ui;
};

#endif // UPLOADFILEHISTORYITEM_H
