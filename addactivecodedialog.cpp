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

    QStringList strActiveCodeList;
    QTextDocument* doc = ui->editActiveCode->document();
    QString strActiveCode="";
    for(int i = 0;i< doc->blockCount(); i++)
    {
        strActiveCode = doc->findBlockByNumber(i).text();
        if(!strActiveCode.isEmpty())
        {
            strActiveCodeList<< strActiveCode;
        }

    }
    emit addActiveCodeSignals(strActiveCodeList);
}


void AddActiveCodeDialog::on_editActiveCode_textChanged()
{
    int lines = 0;//ui->editActiveCode->document()->lineCount();
    QTextDocument* doc = ui->editActiveCode->document();
    QString strActiveCode="";
    for(int i = 0;i< doc->blockCount(); i++)
    {
        strActiveCode = doc->findBlockByNumber(i).text();
        if(!strActiveCode.isEmpty())
        {
            lines++;
        }

    }
    ui->labelActiveCount->setText(QString("(总数：%1)").arg(lines));
}

