#ifndef RECENTCOPYCUTCONTENTDIALOG_H
#define RECENTCOPYCUTCONTENTDIALOG_H

#include <QRadioButton>
#include <QToolButton>
#include "global.h"
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QListWidgetItem>

namespace Ui {
class RecentCopyCutContentDialog;
}

#define RECENT_LIST_ITEM_WIDTH   300
#define RECENT_LIST_ITEM_HEIGHT  20

// 自定义的列表项小部件
class RecentListItem : public QWidget
{
    Q_OBJECT
public:
    RecentListItem(S_RECENT_COPY_DATA info, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}QRadioButton{background:transparent;}";
        radioBtnContent = new QRadioButton(this);
        //这里设置为非互斥，使用QButtonGroup来控制
        radioBtnContent->setAutoExclusive(false);
        info.pButtonGroup->addButton(radioBtnContent, info.iBtnGroupId);
        //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
        radioBtnContent->setStyleSheet(strStyleSheet);
        radioBtnContent->setFixedSize(QSize(RECENT_LIST_ITEM_WIDTH - 50, RECENT_LIST_ITEM_HEIGHT));
        QFontMetrics fontWidth(radioBtnContent->font());
        QString strElideNote = fontWidth.elidedText(info.strContent, Qt::ElideRight, RECENT_LIST_ITEM_WIDTH - 50);
        radioBtnContent->setText(strElideNote);
        radioBtnContent->setToolTip(info.strContent);


        hLayout->addWidget(radioBtnContent);
        hLayout->addStretch();

        toolBtnDel = new QToolButton(this);
        strStyleSheet = "QToolButton{border:none;background:transparent;}";
        toolBtnDel->setStyleSheet(strStyleSheet);
        toolBtnDel->resize(QSize(16, 16));
        toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolBtnDel->setIcon(QIcon(":/main/resource/main/copyDel.png"));
        toolBtnDel->setIconSize(QSize(16, 16));
        connect(toolBtnDel, &QToolButton::clicked, this, &RecentListItem::do_Clicked);
        //m_toolBtnDel->setText("222");
        //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hLayout->addWidget(toolBtnDel);

        //添加到垂直布局
        vLayout->addLayout(hLayout);
        this->setLayout(vLayout);
    }
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

class RecentCopyCutContentDialog : public QMoveDialog
{
    Q_OBJECT

public:
    explicit RecentCopyCutContentDialog(QWidget *parent = nullptr);
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

private:
    void deleteItem(QListWidgetItem* item);
    void addItem(const QString& text);
private:
    Ui::RecentCopyCutContentDialog *ui;
    QButtonGroup*       m_buttonGroup;
    QString             m_strSelectText;
    int                 m_iBtnID;
};

#endif // RECENTCOPYCUTCONTENTDIALOG_H
