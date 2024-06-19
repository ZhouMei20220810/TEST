#include "addactivecodedialog.h"
#include "ui_addactivecodedialog.h"

AddActiveCodeDialog::AddActiveCodeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddActiveCodeDialog)
{
    ui->setupUi(this);
}

AddActiveCodeDialog::~AddActiveCodeDialog()
{
    delete ui;
}
