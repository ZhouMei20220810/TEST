#ifndef CLIPBOARDHISTORYAPP_H
#define CLIPBOARDHISTORYAPP_H

#include <QApplication>
#include <QObject>
#include "clipboardhistory.h"
#include <QListWidget>

class ClipboardHistoryApp : public QApplication
{
    Q_OBJECT
public:
    ClipboardHistoryApp(int& argc, char** argv);

    ~ClipboardHistoryApp();

    //void updateListWidget();
    //获取列表
    QList<QString> getClipboardHistoryList();
    //清空列表
    void clearClipboardHistoryList();
    void SaveCopyData();
    bool removeHistoryItem(QString strText);
private:
    ClipboardHistory* clipboardHistory_;
    QListWidget* listWidget_;
};
#endif // CLIPBOARDHISTORYAPP_H
