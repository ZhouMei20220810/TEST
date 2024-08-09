#include "clipboardhistory.h"
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QSettings>

ClipboardHistory::ClipboardHistory(QApplication* app,QObject *parent)
    : QObject{parent},app_(app)
{
    // 连接信号与槽
    connect(app_->clipboard(), &QClipboard::dataChanged, this, &ClipboardHistory::onClipboardChanged);
    //注册表相关操作-加载
    LoadCopyData();
    
}

ClipboardHistory::~ClipboardHistory()
{
    // 清理资源
}

//注册表相关操作-加载
void ClipboardHistory::LoadCopyData()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\YSY\\YSY STUDIO", QSettings::NativeFormat);
    // 读取数据
    QVariantList data = settings.value("RecentCopyData").toList();
    // 显示数据
    for (const auto& item : data)
    {
        addTextToHistory(item.toString());
    }
}

//注册表相关操作-保存
void ClipboardHistory::SaveCopyData()
{
    QVariantList copyData;
    for (const QString& strContent : history_)
    {
        copyData.append(strContent);
    }
    
    QSettings settings("HKEY_CURRENT_USER\\Software\\YSY\\YSY STUDIO", QSettings::NativeFormat);

    // 保存数据
    settings.setValue("RecentCopyData", copyData);
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
    qDebug() << "addTextToHistory 添加" << text;
    history_.append(text);
}

QList<QString> ClipboardHistory::getHistory() const
{
    return history_;
}

void ClipboardHistory::clearHistory()
{
    history_.clear();
}

bool ClipboardHistory::removeHistoryItem(QString strText)
{
    if (history_.size() <= 0)
        return false;

    //删除全局列表中的Item
    bool find = history_.removeOne(strText);
    if (!find)
    {
        qDebug() << "not found. strText=" << strText;
        return false;
    }
    return true;
}