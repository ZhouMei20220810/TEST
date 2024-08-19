#ifndef RECENTCOPYCUTCONTENTDIALOG_H
#define RECENTCOPYCUTCONTENTDIALOG_H

#include <QRadioButton>
#include <QToolButton>
#include "global.h"
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QListWidgetItem>
#include <QFrame>
#include "tplaintextedit.h"
#include "recentcopystatusdialog.h"
#include <QTimer>

namespace Ui {
class RecentCopyCutContentDialog;
}

#define RECENT_LIST_ITEM_WIDTH   300
#define RECENT_LIST_ITEM_HEIGHT  20
#define RECENT_LIST_EDITABLE_ITEM_WIDTH   300
#define RECENT_LIST_EDITABLE_ITEM_HEIGHT  50
// 自定义的列表项小部件
class RecentListItem : public QWidget
{
    Q_OBJECT
public:
    RecentListItem(S_RECENT_COPY_DATA info, QWidget* parent = nullptr);
private slots:
    void do_Clicked(bool bChecked)
    {
        emit deleteClicked(this);
    }
signals:
    void deleteClicked(RecentListItem*);

private:
    QRadioButton* radioBtnContent;
    QToolButton* toolBtnDel;
};

class RecentListEditableItem : public QWidget
{
    Q_OBJECT
public:
    RecentListEditableItem(int iRowIndex, QString strText, QWidget* parent = nullptr);

    QString getText()
    {
        return m_strText;
    }
private slots:
    void do_returnPressed();
    void do_copyAvailable(bool available);
    void onClipboardDataChanged();
signals:
    void enterTextSignals(QString strEnterText);
private:
    QString m_strText;
    //QFrame* m_Frame;
    TPlainTextEdit* m_textEdit;
};


class RecentCopyCutContentDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit RecentCopyCutContentDialog(QStringList strPhoneList,QWidget *parent = nullptr);
    ~RecentCopyCutContentDialog();

    void LoadHistoryList();
signals:
    void DirectCopyToPhoneSignals(QString strSelectText);
    void BatchDirectCopyToPhoneSignals(QString strTextList);
private slots:
    void on_btnCopyToPhone_clicked();
    void on_btnDirectCopy_clicked();
    void on_btnCopyByOrder_clicked();
    void on_btnClose_clicked();

    void do_idClicked(int id);
    void onClipboardChanged();

    void on_toolBtnClear_clicked();

    //处理拷贝到文本框的文字
    void do_enterTextSignals(QString strEnterText);
    //是否弹窗提示复制状态
    void showCopyStatusDialog();
    QString getEditableListWidgetText();
private:
    void deleteItem(QListWidgetItem* item);
    void addItem(const QString& text);
private:
    Ui::RecentCopyCutContentDialog *ui;
    QButtonGroup*       m_buttonGroup;
    QString             m_strSelectText;
    int                 m_iBtnID;
    QTimer* m_UITimer;
};

#endif // RECENTCOPYCUTCONTENTDIALOG_H
