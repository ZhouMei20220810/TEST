#include "clipboardhistoryapp.h"

ClipboardHistoryApp::ClipboardHistoryApp(int& argc, char** argv) : QApplication(argc, argv)
{
    // 创建一个用于显示历史记录的列表窗口
    //listWidget_ = new QListWidget;
    //listWidget_->show();

    // 创建一个用于维护历史记录的对象
    clipboardHistory_ = new ClipboardHistory(this); //new ClipboardHistory(this);

    // 更新列表窗口
    //updateListWidget();
}

ClipboardHistoryApp::~ClipboardHistoryApp()
{
    delete clipboardHistory_;
}

QList<QString> ClipboardHistoryApp::getClipboardHistoryList()
{
    return clipboardHistory_->getHistory();
}

bool ClipboardHistoryApp::removeHistoryItem(QString strText)
{
    return clipboardHistory_->removeHistoryItem(strText);
}

void ClipboardHistoryApp::SaveCopyData()
{
    clipboardHistory_->SaveCopyData();
}