#include "authorizationitem.h"
#include "ui_authorizationitem.h"

authorizationItem::authorizationItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::authorizationItem)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
}

authorizationItem::~authorizationItem()
{
    delete ui;
}

void authorizationItem::on_toolBtnOper_clicked()
{
    //管理

}

