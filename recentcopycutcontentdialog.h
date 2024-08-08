#ifndef RECENTCOPYCUTCONTENTDIALOG_H
#define RECENTCOPYCUTCONTENTDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QToolButton>
#include "global.h"
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QListWidgetItem>

namespace Ui {
class RecentCopyCutContentDialog;
}

#define RECENT_LIST_ITEM_WIDTH   100
#define RECENT_LIST_ITEM_HEIGHT  20

// �Զ�����б���С����
class RecentListItem : public QWidget
{
    Q_OBJECT
public:
    RecentListItem(const QString& text,QButtonGroup* buttonGroup,int iBtnID, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}";
        radioBtnContent = new QRadioButton(this);
        //��������Ϊ�ǻ��⣬ʹ��QButtonGroup������
        radioBtnContent->setAutoExclusive(false);
        buttonGroup->addButton(radioBtnContent, iBtnID);
        //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
        radioBtnContent->setStyleSheet(strStyleSheet);
        QFontMetrics fontWidth(radioBtnContent->font());
        QString strElideNote = fontWidth.elidedText(text, Qt::ElideRight, 244);
        radioBtnContent->setText(strElideNote);
        radioBtnContent->setToolTip(text);
        

        hLayout->addWidget(radioBtnContent);
        hLayout->addStretch();

        toolBtnDel = new QToolButton(this);
        strStyleSheet = "QToolButton{border:none;background:transparent;background-image: url(:/main/resource/main/deleteActiveItem.png);}";
        toolBtnDel->setStyleSheet(strStyleSheet);
        toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
        connect(toolBtnDel, &QToolButton::clicked, this, &RecentListItem::do_Clicked);
        //m_toolBtnDel->setText("222");
        //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hLayout->addWidget(toolBtnDel);

        //��ӵ���ֱ����
        vLayout->addLayout(hLayout);
        this->setLayout(vLayout);
        /*QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        // ����һ��ˮƽ����������QRadioButton��QToolButton
        QHBoxLayout* buttonLayout = new QHBoxLayout;
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        buttonLayout->setSpacing(10);

        // ����QRadioButton
        radioButton = new QRadioButton(text);
        buttonLayout->addWidget(radioButton);

        // ����QToolButton����ɾ��
        deleteButton = new QToolButton;
        deleteButton->setText("X");
        deleteButton->setStyleSheet("QToolButton { background-color: #FF6B6B; color: white; border-radius: 5px; }");
        connect(deleteButton, &QToolButton::clicked, this, &RecentListItem::deleteClicked);
        buttonLayout->addWidget(deleteButton);

        layout->addLayout(buttonLayout);

        setLayout(layout);*/
    }
private slots:
    //void do_toolBtnDelClick(bool bChecked);
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

class RecentCopyCutContentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentCopyCutContentDialog(QWidget *parent = nullptr);
    ~RecentCopyCutContentDialog();

    void LoadHistoryList();
signals:
    void DirectCopyToPhoneSignals(QString strSelectText);
private slots:
    void on_btnCopyToPhone_clicked();
    void on_btnDirectCopy_clicked();
    void on_btnCopyByOrder_clicked();
    void on_btnClose_clicked();

    void do_idClicked(int id);
    void onClipboardChanged();

    //ɾ����ť
    //void do_toolBtnDelClick(bool bChecked);
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
