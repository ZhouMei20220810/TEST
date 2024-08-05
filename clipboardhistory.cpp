#include "clipboardhistory.h"
#include <QClipboard>
#include <QApplication>
#include <QMimeData>

ClipboardHistory::ClipboardHistory(QApplication* app,QObject *parent)
    : QObject{parent},app_(app)
{
    // 连接信号与槽
    connect(app_->clipboard(), &QClipboard::dataChanged, this, &ClipboardHistory::onClipboardChanged);
}

ClipboardHistory::~ClipboardHistory()
{
    // 清理资源
}

void ClipboardHistory::onClipboardChanged()
{
    const QMimeData* mimeData = app_->clipboard()->mimeData();
    if (mimeData && mimeData->hasText())
    {
        QString text = mimeData->text();
        addTextToHistory(text);
    }
}

void ClipboardHistory::addTextToHistory(const QString& text)
{
    if (history_.size() >= 10)
    {
        history_.removeFirst();
    }
    history_.append(text);
}

QList<QString> ClipboardHistory::getHistory() const
{
    return history_;
}
