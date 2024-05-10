#include "updategroupwidget.h"
#include "ui_updategroupwidget.h"
#include <QMessageBox>

UpdateGroupWidget::UpdateGroupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UpdateGroupWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    //最大长度限制
    ui->lineEditGroupName->setMaxLength(16);
}

UpdateGroupWidget::~UpdateGroupWidget()
{
    delete ui;
}

void UpdateGroupWidget::on_btnOk_clicked()
{
    QString strGroupName = ui->lineEditGroupName->text();
    if(strGroupName.isEmpty())
    {
        QMessageBox::warning(this, "错误提示", "名称不能为空");
        return;
    }

    emit updateGroupSignals(strGroupName);

    this->close();
}

