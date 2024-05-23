#include "Logoutdialog.h"
#include "ui_Logoutdialog.h"
//#include "globaldata.h"

LogoutDialog::LogoutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint );
    //GlobalData::setToolButtonSize(ui->toolButton3);
    setWindowFlags(Qt::Tool);
    setWindowFlags(Qt::WindowDoesNotAcceptFocus);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint|Qt::WindowDoesNotAcceptFocus);
}

LogoutDialog::~LogoutDialog()
{
    delete ui;
}

void LogoutDialog::on_toolButton3_clicked()
{
    reject();
}

void LogoutDialog::on_pushButton_5_clicked()
{
    accept();
}

void LogoutDialog::on_pushButton_clicked()
{
    reject();
}
