#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "mainwindow.h"
#include <QGraphicsDropShadowEffect>

#define     PAGE_WIDGET_X_POS       324
#define     PAGE_WIDGET_Y_POS       32

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    ui->centralwidget->setGraphicsEffect(shadow);

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
            this->hide();
        });
    connect(m_smsLoginPage, &SMSLoginPage::logoutSignals, this, [=]() {
        this->show();
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
            this->hide();
        });
    connect(m_passwordLoginPage, &PasswordLoginPage::logoutSignals, this, [=]() {
        this->show();
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
        connect(m_registerPage, &RegisterPage::logoutSignals, this, [=]() {
            this->show();
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

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_LastPos = event->globalPosition().toPoint()-this->pos();
        m_bMoving = true;
    }
    return QWidget::mousePressEvent(event);
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPosition = event->globalPosition().toPoint();
    if(m_bMoving && (event->buttons()&Qt::LeftButton)
        && (globalPosition-m_LastPos-pos()).manhattanLength() > QApplication::startDragDistance()) //控制移动的距离，多少距离执行拖拽
    {
        move(globalPosition-m_LastPos);
        m_LastPos = globalPosition-pos();
    }
    return QWidget::mouseMoveEvent(event);
}


void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMoving = false;
}
