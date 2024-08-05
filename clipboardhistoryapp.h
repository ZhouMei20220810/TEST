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
    QList<QString> getClipboardHistoryList();
private:
    ClipboardHistory* clipboardHistory_;
    QListWidget* listWidget_;
};
#endif // CLIPBOARDHISTORYAPP_H
