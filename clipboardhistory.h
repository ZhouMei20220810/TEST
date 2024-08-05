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

signals:
    void dataChanged();

private:
    QApplication* app_;
    QList<QString> history_;
};

#endif // CLIPBOARDHISTORY_H
