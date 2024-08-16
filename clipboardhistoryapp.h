#ifndef CLIPBOARDHISTORYAPP_H
#define CLIPBOARDHISTORYAPP_H

#include <QApplication>
#include <QObject>
#include "clipboardhistory.h"
#include <QListWidget>
#include "global.h"

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

    //用于保存拷贝结果
    void addCopyStatus(S_RECENT_COPY_STATUS info);
    void clearCopyStatus();
signals:
    void addCopyStatusSignals(S_RECENT_COPY_STATUS info);
private:
    ClipboardHistory* clipboardHistory_;
    QListWidget* listWidget_;

    //用于存放拷贝结果
    QMap<QString, S_RECENT_COPY_STATUS> m_mapCopyStatus;
};
#endif // CLIPBOARDHISTORYAPP_H
