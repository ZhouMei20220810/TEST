#include "replacecloudphonedialog.h"
#include "ui_replacecloudphonedialog.h"
#include "messagetips.h"
#include "toolobject.h"

ReplaceCloudPhoneDialog::ReplaceCloudPhoneDialog(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList, QWidget *parent)
    : QMoveDialog(parent)
    , ui(new Ui::ReplaceCloudPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("更换云机");
    m_mapLevelList = mapLevelList;
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_iCurSelCount = 0;
    ui->checkBoxAll->setText(QString("%1/%2         名称").arg(0).arg(0));
    //按等级拉去数据
    ToolObject::getInstance()->HttpGetMyPhoneInstance(0, 1, 1000, phoneInfo.iLevel);
    connect(ToolObject::getInstance(), &ToolObject::HttpGetMyPhoneInstanceSignals, this, &ReplaceCloudPhoneDialog::do_HttpGetMyPhoneInstanceSignals);
    connect(ToolObject::getInstance(), &ToolObject::HttpPostReplaceInstanceSignals, this, &ReplaceCloudPhoneDialog::LoadReplaceInstanceStatus);
}

ReplaceCloudPhoneDialog::~ReplaceCloudPhoneDialog()
{
    delete ui;
}

void ReplaceCloudPhoneDialog::on_btnClose_clicked()
{
    this->close();
}


void ReplaceCloudPhoneDialog::on_btnOk_clicked()
{
    qDebug()<<"确定更换";
    //QVector<int> vector;
    //行-手机id
    QMap<int, int> map;    
    int iCount = ui->listWidget->count();
    if (iCount <= 0)
    {        
        MessageTips* tips = new MessageTips("数据列表为空", this);
        tips->show();
        return;
    }

    ReplaceListItem* widget = NULL;
    QListWidgetItem* item = NULL;
    S_PHONE_INFO phoneInfo;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        if (item != NULL)
        {
            widget = qobject_cast<ReplaceListItem*>(ui->listWidget->itemWidget(item));
            if (widget != NULL && widget->getCheckBoxStatus())
            {
                phoneInfo = item->data(Qt::UserRole).value<S_PHONE_INFO>();
                map.insert(i, phoneInfo.iId);
            }
        }
    }
    if (map.size() <= 0)
    {
        MessageTips* tips = new MessageTips("暂无勾选数据", this);
        tips->show();
        return;
    }
    ToolObject::getInstance()->HttpPostReplaceInstance(map);
}

void ReplaceCloudPhoneDialog::LoadReplaceInstanceStatus(QMap<int, S_REPLACE_INFO> map)
{
    if (map.size() <= 0)
        return;
    QMap<int, S_REPLACE_INFO>::iterator iter = map.begin();
    QListWidgetItem* item = NULL;
    ReplaceListItem* widget = NULL;
    QString strText;
    for (; iter != map.end(); iter++)
    {
        strText = "";
        item = ui->listWidget->item(iter.key());
        if (item != NULL)
        {
            widget = static_cast<ReplaceListItem*>(ui->listWidget->itemWidget(item));
            widget->setReplacePhoneStatus(iter->strRemark);
        }
    }
}

void ReplaceCloudPhoneDialog::ShowPhoneInfo(QMap<int, S_PHONE_INFO> mapPhoneInfo)
{
    ui->listWidget->clear();
    if (mapPhoneInfo.size() <= 0)
        return;

    QListWidgetItem* item = NULL;
    QCheckBox* checkBox = NULL;
    QString strLevelName;
    QMap<int, S_LEVEL_INFO>::iterator iterFind;
    QMap<int, S_PHONE_INFO>::iterator iter = mapPhoneInfo.begin();
    ReplaceListItem* widget = NULL;
    for (; iter != mapPhoneInfo.end(); iter++)
    {        
        widget = new ReplaceListItem(*iter, m_mapLevelList, this);
        connect(widget, &ReplaceListItem::replaceItemCheckBoxStatus, this, &ReplaceCloudPhoneDialog::do_replaceItemCheckBoxStatus);
        item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(REPLACE_LIST_ITEM_WIDTH, REPLACE_LIST_ITEM_HEIGHT));
        item->setData(Qt::UserRole, QVariant::fromValue(*iter));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void ReplaceCloudPhoneDialog::do_replaceItemCheckBoxStatus(bool checked)
{
    qDebug() << "ReplaceCloudPhoneDialog do_replaceItemCheckBoxStatus 改变状态=" << checked;
    if (checked)
    {
        m_iCurSelCount++;
    }
    else
    {
        m_iCurSelCount--;
    }
    int iCount = ui->listWidget->count();
    ui->checkBoxAll->setText(QString("%1/%2         名称").arg(m_iCurSelCount).arg(iCount));
    if (m_iCurSelCount == 0)
        ui->checkBoxAll->setCheckState(Qt::Unchecked);
    else
    {
        if (m_iCurSelCount == iCount && iCount != 0)
            ui->checkBoxAll->setCheckState(Qt::Checked);
        else
            ui->checkBoxAll->setCheckState(Qt::PartiallyChecked);
    }
    
}

void ReplaceCloudPhoneDialog::on_checkBoxAll_clicked(bool checked)
{
    int iCount = ui->listWidget->count();
    if (iCount <= 0)
    {
        return;
    }

    QListWidgetItem* item = NULL;
    ReplaceListItem* widget = NULL;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidget->item(i);
        if (item != NULL)
        {
            widget = static_cast<ReplaceListItem*>(ui->listWidget->itemWidget(item));
            if (widget != NULL)
            {
                widget->setCheckBoxStatus(checked);
            }
        }
    }
    if (checked)
    {
        m_iCurSelCount = iCount;
        ui->checkBoxAll->setText(QString("%1/%2         名称").arg(iCount).arg(iCount));
    }
    else
    {
        m_iCurSelCount = 0;
        ui->checkBoxAll->setText(QString("%1/%2         名称").arg(0).arg(iCount));
    }
}

void ReplaceCloudPhoneDialog::do_HttpGetMyPhoneInstanceSignals(int iLevel, int iGroupId, QMap<int, S_PHONE_INFO> map)
{
    qDebug() << "更换云机 等级" << iLevel;
    ShowPhoneInfo(map);
    ui->checkBoxAll->setText(QString("%1/%2         名称").arg(0).arg(map.size()));
}