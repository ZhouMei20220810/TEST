#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "mainwindow.h"

#define     PAGE_WIDGET_X_POS       324
#define     PAGE_WIDGET_Y_POS       32

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);

    m_passwordLoginPage = new PasswordLoginPage(this);
    m_smsLoginPage = new SMSLoginPage(this);
    //loginWindow的子窗口
    //m_registerPage = new RegisterPage(this);    

    m_passwordLoginPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);
    m_smsLoginPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);
    //m_registerPage->move(PAGE_WIDGET_X_POS,PAGE_WIDGET_Y_POS);

    //showPage(TYPE_SMSLOGIN_PAGE);
    showPage(TYPE_PASSWORDLOGIN_PAGE);

    connect(m_smsLoginPage, &SMSLoginPage::showPageType, this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
        showPage(type);
    });
    connect(m_smsLoginPage, &SMSLoginPage::closeWindowSignals, this, [=]()
        {
            this->close();
        });

    /*connect(m_registerPage, &RegisterPage::showPageType, this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
                this->show();
                showPage(type);
            });*/
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
    connect(m_passwordLoginPage, &PasswordLoginPage::closeWindowSignals, this, [=]()
        {
            this->close();
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
    //m_registerPage->hide();

    switch (type) {
    case TYPE_PASSWORDLOGIN_PAGE:
        m_passwordLoginPage->show();
        break;
    case TYPE_SMSLOGIN_PAGE:
        m_smsLoginPage->show();
        break;
    case TYPE_REGISTER_PAGE:
    {
        //关闭登录窗口
        this->hide();
        //独立窗口
        m_registerPage = new RegisterPage();
        connect(m_registerPage, &RegisterPage::showPageType, this, [=](ENUM_LOGIN_PAGE_TYPE type)
            {
                this->show();
                showPage(type);
            });
        m_registerPage->show();
    }        
        break;
    default:
        break;
    }
}

void LoginWindow::on_pushButton_clicked()
{
    this->close();
}
/*
void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos()-frameGeometry().topLeft();
        raise();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}*/
