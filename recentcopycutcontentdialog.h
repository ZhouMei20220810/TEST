#ifndef RECENTCOPYCUTCONTENTDIALOG_H
#define RECENTCOPYCUTCONTENTDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QToolButton>
#include "global.h"
#include <QVBoxLayout>
#include <QButtonGroup>

namespace Ui {
class RecentCopyCutContentDialog;
}

#define RECENT_LIST_ITEM_WIDTH   100
#define RECENT_LIST_ITEM_HEIGHT  20
/*class RecentListItem :public QWidget
{
    Q_OBJECT
public:
    explicit RecentListItem(QString strContent, QWidget* parent)
        :QWidget(parent)
    {
        resize(RECENT_LIST_ITEM_WIDTH, RECENT_LIST_ITEM_HEIGHT);
        setupUI(strContent);
    }
    ~RecentListItem()
    {
        qDebug() << "delete ~RecentListItem()";
    }

signals:
    void selectItemSignals(bool checked);
private:
    void setupUI(QString strContent)
    {
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "QRadioButton::indicator::unchecked{border-image:url(:/main/resource/main/radioUncheck.png);}QRadioButton::indicator::checked{border-image:url(:/main/resource/main/radioCheck.png);}QRadioButton::indicator{width:16px;height:16px;}";        
        m_radioBtnContent = new QRadioButton(this);        
        connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
        m_radioBtnContent->setStyleSheet(strStyleSheet);
        m_radioBtnContent->setText(strContent);
        hLayout->addWidget(m_radioBtnContent);
        hLayout->addStretch();

        m_toolBtnDel = new QToolButton(this);
        strStyleSheet = "QToolButton{border:none;background:transparent;background-image: url(:/main/resource/main/deleteActiveItem.png);}";
        m_toolBtnDel->setStyleSheet(strStyleSheet);
        m_toolBtnDel->setToolButtonStyle(Qt::ToolButtonIconOnly);
        //m_toolBtnDel->setText("222");
        //m_toolBtnDel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        hLayout->addWidget(m_toolBtnDel);

        //添加到垂直布局
        vLayout->addLayout(hLayout);
        this->setLayout(vLayout);
    };
private:
    QRadioButton* m_radioBtnContent;
    QToolButton*  m_toolBtnDel;
};*/

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
private:
    void addItem(const QString& text);
private:
    Ui::RecentCopyCutContentDialog *ui;
    QButtonGroup*       m_buttonGroup;
    QString             m_strSelectText;
};

#endif // RECENTCOPYCUTCONTENTDIALOG_H
