#ifndef RECENTCOPYSTATUSDIALOG_H
#define RECENTCOPYSTATUSDIALOG_H

#include <QDialog>
#include <QLabel>
#include "global.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace Ui {
class RecentCopyStatusDialog;
}

#define COPY_STATUS_ITEM_WIDTH          200
#define COPY_STATUS_ITEM_HEIGHT         30

class CopyStatusItem : public QWidget
{
    Q_OBJECT
public:
    CopyStatusItem(S_PHONE_INFO info, int iRet, QWidget* parent = nullptr)
        : QWidget(parent)
    {
        resize(COPY_STATUS_ITEM_WIDTH, COPY_STATUS_ITEM_HEIGHT);
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "color:#596380;font-size:12px;font-weight:bold;";
        m_LabelInstance = new QLabel(this);
        //这里设置为非互斥，使用QButtonGroup来控制
        //connect(m_radioBtnContent, &QRadioButton::clicked, this, &RecentListItem::selectItemSignals);
        m_LabelInstance->setStyleSheet(strStyleSheet);
        //radioBtnContent->setFixedSize(QSize(RECENT_LIST_ITEM_WIDTH - 50, RECENT_LIST_ITEM_HEIGHT));
        QFontMetrics fontWidth(m_LabelInstance->font());
        QString strElideNote = fontWidth.elidedText(info.strName, Qt::ElideRight, COPY_STATUS_ITEM_WIDTH - 50);
        m_LabelInstance->setText(strElideNote);
        m_LabelInstance->setToolTip(info.strName);


        hLayout->addWidget(m_LabelInstance);
        hLayout->addStretch();

        m_LabelStatus = new QLabel(this);
        //成功
        if (iRet == 0)
        {
            strStyleSheet = "color:#3071F2;font-size:12px;font-weight:bold;";
            m_LabelStatus->setText("粘贴成功");
        }
        else
        {
            strStyleSheet = "color:#FF4D4D;font-size:12px;font-weight:bold;";
            //m_LabelStatus->setText("失败");
        }
        m_LabelStatus->setStyleSheet(strStyleSheet);
        hLayout->addWidget(m_LabelStatus);

        //添加到垂直布局
        vLayout->addLayout(hLayout);
        this->setLayout(vLayout);
    };

private:
    QLabel* m_LabelInstance;
    QLabel*  m_LabelStatus;
};

class RecentCopyStatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecentCopyStatusDialog(QWidget *parent = nullptr);
    ~RecentCopyStatusDialog();
public slots:
    void do_CopyStatusSignals(S_PHONE_INFO info, int iRet);
private slots:
    void on_btnClose_clicked();

    void on_btnOK_clicked();

private:
    Ui::RecentCopyStatusDialog *ui;
};

#endif // RECENTCOPYSTATUSDIALOG_H
