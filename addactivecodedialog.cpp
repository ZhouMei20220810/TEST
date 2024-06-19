#include "addactivecodedialog.h"
#include "ui_addactivecodedialog.h"
#include <QTextDocument>
#include <QTextBlock>

AddActiveCodeDialog::AddActiveCodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddActiveCodeDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
}

AddActiveCodeDialog::~AddActiveCodeDialog()
{
    delete ui;
}

void AddActiveCodeDialog::on_btnClose_clicked()
{
    this->reject();
}


void AddActiveCodeDialog::on_btnOk_clicked()
{
    this->accept();

    QString str=ui->editActiveCode->toPlainText();
    qDebug()<<"str= "<< str;
    QStringList strActiveCodeList;
    QTextDocument* doc = ui->editActiveCode->document();
    for(int i = 0 ; i < doc->blockCount(); i++)
    {
        strActiveCodeList<<doc->findBlockByNumber(i).text();
    }
    emit addActiveCodeSignals(strActiveCodeList);
}

