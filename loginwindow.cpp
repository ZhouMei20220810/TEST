#include "loginwindow.h"
#include "ui_loginwindow.h"


#define     PAGE_WIDGET_X_POS       262
#define     PAGE_WIDGET_Y_POS       33

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);

    m_passwordLoginPage = new PasswordLoginPage(this);
    m_smsLoginPage = new SMSLoginPage(this);
    m_registerPage = new RegisterPage(this);

    m_passwordLoginPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);
    m_smsLoginPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);
    m_registerPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);

    showPage(TYPE_SMSLOGIN_PAGE);

    connect(m_smsLoginPage, &SMSLoginPage::showPageType, this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
        showPage(type);
    });
    connect(m_registerPage, &RegisterPage::showPageType, this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
                showPage(type);
            });
    connect(m_passwordLoginPage,&PasswordLoginPage::showPageType,this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
        showPage(type);        
    });
    connect(m_passwordLoginPage,&PasswordLoginPage::showLoginWindow,this,[=](ENUM_LOGIN_STATUS enStatus)
            {
            switch (enStatus)
            {
            case TYPE_WINDOWS_HIDE:
                this->hide();
                break;
            case TYPE_WINDOWS_SHOW:
                this->show();
                break;
            case TYPE_WINDOWS_CLOSE:
                this->close();
                break;
            default:
                break;
            }
            });
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::showPage(ENUM_LOGIN_PAGE_TYPE type)
{
    m_passwordLoginPage->hide();
    m_smsLoginPage->hide();
    m_registerPage->hide();

    switch (type) {
    case TYPE_PASSWORDLOGIN_PAGE:
        m_passwordLoginPage->show();
        break;
    case TYPE_SMSLOGIN_PAGE:
        m_smsLoginPage->show();
        break;
    case TYPE_REGISTER_PAGE:
        m_registerPage->show();
        break;
    default:
        break;
    }
}

void LoginWindow::on_pushButton_clicked()
{
    this->close();
}


void LoginWindow::on_btnForgetPW_clicked()
{

}


void LoginWindow::on_btnLogin_clicked()
{

}


void LoginWindow::on_btnRegister_clicked()
{

}

