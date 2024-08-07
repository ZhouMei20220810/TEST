#include "messagetipsdialog.h"
#include "ui_messagetipsdialog.h"

MessageTipsDialog::MessageTipsDialog(QString strShow,QWidget *parent, MESSAGE_NOT_TIPS_TYPE type, QString strTitle)
    : QDialog(parent)
    , ui(new Ui::MessageTipsDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose,true);

    m_enType = type;
    if (!strTitle.isEmpty())
    {
        ui->labelTitle->setText(strTitle);
    }
    switch (type)
    {
    case MESSAGE_NOT_TIPS_CLOSE_MASTER_INSTANCE:
    case MESSAGE_NOT_TIPS_CLOSE_SYNC_OPER:
        ui->checkBoxNotTip->setVisible(true);
        break;
    default:
        ui->checkBoxNotTip->setVisible(false);
        break;
    }
    ui->labelText->setText(strShow);
}

MessageTipsDialog::~MessageTipsDialog()
{
    qDebug()<<"delete MessageTipsDialog";
    delete ui;
}

void MessageTipsDialog::on_pushButton_2_clicked()
{
    this->reject();
}

void MessageTipsDialog::on_btnOK_clicked()
{
    if(ui->checkBoxNotTip->isVisible())
    {
        Qt::CheckState bChecked = ui->checkBoxNotTip->checkState();
        switch (m_enType)
        {
        case MESSAGE_NOT_TIPS_CLOSE_MASTER_INSTANCE:
            GlobalData::bIsTipsCloseMasterInstance = bChecked;
            break;
        case MESSAGE_NOT_TIPS_CLOSE_SYNC_OPER:
            GlobalData::bIsTipsCloseSyncOper = bChecked;
            break;
        default:
            break;
        }        
    }
    this->accept();
}

