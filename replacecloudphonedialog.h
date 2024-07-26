#ifndef REPLACECLOUDPHONEDIALOG_H
#define REPLACECLOUDPHONEDIALOG_H

#include <QDialog>
#include "global.h"
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#define         REPLACE_LIST_ITEM_WIDTH             327
#define         REPLACE_LIST_ITEM_HEIGHT            20

namespace Ui {
class ReplaceCloudPhoneDialog;
}

class ReplaceListItem :public QWidget
{
    Q_OBJECT
public:
    explicit ReplaceListItem(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList, QWidget* parent)
        :QWidget(parent)
    {
        resize(REPLACE_LIST_ITEM_WIDTH, REPLACE_LIST_ITEM_HEIGHT);
        setupUI(phoneInfo, mapLevelList);
    }
    ~ReplaceListItem()
    {
        qDebug() << "delete ~ReplaceListItem()";
    }

    void setCheckBoxStatus(bool bCheck)
    {
        if (m_checkBox)
            m_checkBox->setChecked(bCheck);
    }
    bool getCheckBoxStatus()
    {
        return m_checkBox->isChecked();
    }
    
    void setReplacePhoneStatus(QString strRemark)
    {
        m_LabelStatus->setText(strRemark);
    }
signals:
    void replaceItemCheckBoxStatus(bool checked);
private:
    void setupUI(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList)
    {
        QVBoxLayout* vLayout = new QVBoxLayout(this);
        vLayout->setContentsMargins(0, 0, 0, 0);
        QHBoxLayout* hLayout = new QHBoxLayout(this);
        hLayout->setContentsMargins(0, 0, 0, 0);

        QString strStyleSheet = "QCheckBox{background:transparent;padding-left:0px;spacing:5px;color:#4A4A4A;font-size:12px;}QCheckBox::indicator{width:16px;height:16px;}QCheckBox::indicator:unchecked{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}";
        m_checkBox = new QCheckBox(this);
        connect(m_checkBox, &QCheckBox::clicked, this, &ReplaceListItem::replaceItemCheckBoxStatus);
        m_checkBox->setStyleSheet(strStyleSheet);
        QMap<int, S_LEVEL_INFO>::iterator iterFind = mapLevelList.find(phoneInfo.iLevel);
        QString strLevelName = "";
        if (iterFind != mapLevelList.end())
        {
            strLevelName = iterFind->strLevelName;
        }
        if (strLevelName.compare("BVIP", Qt::CaseInsensitive) == 0)
            m_checkBox->setIcon(QIcon(QString(":/main/resource/main/XVIP.png")));
        else
            m_checkBox->setIcon(QIcon(QString(":/main/resource/main/%1.png").arg(strLevelName)));
        if (phoneInfo.strName.isEmpty())
            m_checkBox->setText(phoneInfo.strInstanceNo);
        else
            m_checkBox->setText(phoneInfo.strName);
        hLayout->addWidget(m_checkBox);
        hLayout->addStretch();

        m_LabelStatus = new QLabel(this);
        strStyleSheet = "QLabel{background:transparent;color:#4A4A4A;font-size:12px;}";
        m_LabelStatus->setStyleSheet(strStyleSheet);
        m_LabelStatus->setText("222");
        m_LabelStatus->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        hLayout->addWidget(m_LabelStatus);

        //添加到垂直布局
        vLayout->addLayout(hLayout);
        this->setLayout(vLayout);
    };
private:
    QCheckBox* m_checkBox;
    QLabel* m_LabelStatus;
};

class ReplaceCloudPhoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceCloudPhoneDialog(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent = nullptr);
    ~ReplaceCloudPhoneDialog();

private slots:
    void on_btnClose_clicked();

    void on_btnOk_clicked();
    void do_replaceItemCheckBoxStatus(bool checked);
    void on_checkBoxAll_clicked(bool checked);

private:
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel);
    void HttpPostReplaceInstance(QMap<int, int> map);
    void ShowPhoneInfo(QMap<int, S_PHONE_INFO> mapPhoneInfo);
    void LoadReplaceInstanceStatus(QMap<int, S_REPLACE_INFO> map);
private:
    Ui::ReplaceCloudPhoneDialog *ui;
    QMap<int, S_PHONE_INFO> m_mapPhone;
    QMap<int, S_LEVEL_INFO> m_mapLevelList;
    int         m_iCurSelCount;
};

#endif // REPLACECLOUDPHONEDIALOG_H
