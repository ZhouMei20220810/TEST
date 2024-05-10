#include "creategroupwidget.h"
#include "ui_creategroupwidget.h"
#include <QMessageBox>

CreateGroupWidget::CreateGroupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CreateGroupWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
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
        QMessageBox::warning(this, "错误提示", "名称不能为空");
        return;
    }

    emit createGroupSignals(strGroupName);

    this->close();
}

