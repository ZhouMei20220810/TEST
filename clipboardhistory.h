#ifndef CLIPBOARDHISTORY_H
#define CLIPBOARDHISTORY_H

#include <QObject>

class ClipboardHistory : public QObject
{
    Q_OBJECT
public:
    explicit ClipboardHistory(QApplication* app,QObject *parent = nullptr);
    ~ClipboardHistory();

    void onClipboardChanged();

    void addTextToHistory(const QString& text);
    QList<QString> getHistory() const;
    bool removeHistoryItem(QString strText);
    //���浽ע���
    void SaveCopyData();
signals:
    void dataChanged();
private:
    //��ע������
    void LoadCopyData();
private:
    QApplication* app_;
    QList<QString> history_;
};

#endif // CLIPBOARDHISTORY_H
