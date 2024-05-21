#include "creategroupwidget.h"
#include "ui_creategroupwidget.h"
#include "messagetipsdialog.h"

CreateGroupWidget::CreateGroupWidget(ENUM_CREATE_OR_UPDATA type, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateGroupWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);
    m_type = type;
    switch (type)
    {
    case TYPE_CREATE_GROUP:
        ui->labelTitle->setText("创建分组");
        break;
    case TYPE_UPDATE_GROUP:
        ui->labelTitle->setText("编辑分组名称");
        break;
    default:
        ui->labelTitle->setText("创建分组");
        break;
    }
    //最大长度限制
    ui->lineEditGroupName->setMaxLength(16);
}

CreateGroupWidget::~CreateGroupWidget()
{
    delete ui;
}

void CreateGroupWidget::on_btnOk_clicked()
{
    QString strGroupName = ui->lineEditGroupName->text();
    if(strGroupName.isEmpty())
    {
        MessageTipsDialog* tips = new MessageTipsDialog("名称不能为空!", this);
        tips->show();
        return;
    }

    emit createGroupSignals(m_type,strGroupName);

    this->close();
}

