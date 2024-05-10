#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#define     PAGE_WIDGET_X_POS       262
#define     PAGE_WIDGET_Y_POS       33

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlags(Qt::FramelessWindowHint);

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
    connect(m_smsLoginPage, &SMSLoginPage::LoginHttpResponseSignals, this, &LoginWindow::do_LoginHttpResponseSignals);

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

    connect(m_passwordLoginPage, &PasswordLoginPage::LoginHttpResponseSignals, this, &LoginWindow::do_LoginHttpResponseSignals);
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

void LoginWindow::do_LoginHttpResponseSignals(QByteArray response)
{
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << response;
        qWarning() << "Json parse error:" << parseError.errorString();
    }
    else
    {
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            int iCode = obj["code"].toInt();
            QString strMessage = obj["message"].toString();
            qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
            S_USER_LOGIN_INFO userInfo;
            if (HTTP_SUCCESS_CODE == iCode)
            {
                if (obj["data"].isObject())
                {
                    QJsonObject data = obj["data"].toObject();
                    userInfo.strToken = data["token"].toString();
                    userInfo.strMaxExpirationDate = data["maxExpirationDate"].toString();

                    QJsonObject userDetailVO = data["userDetailVO"].toObject();
                    userInfo.id = userDetailVO["id"].toInt();
                    userInfo.strName = userDetailVO["name"].toString();
                    userInfo.strAccount = userDetailVO["account"].toString();
                    userInfo.strMobile = userDetailVO["mobile"].toString();
                    userInfo.strPhotoUrl = userDetailVO["photoUrl"].toString();

                    qDebug() << "提示"<<"id="<< userInfo.id<<"name="<< userInfo.strName<<"account="<< userInfo.strAccount<<"mobile="<< userInfo.strMobile<<"MaxExpirationDate"<< userInfo.strMaxExpirationDate<<"token="<< userInfo.strToken;

                    //关闭
                    //this->close();

                    //去掉父窗口
                    m_mainWindow = new MainWindow();
                    m_mainWindow->setUserInfo(userInfo);
                    m_mainWindow->show();
                } 
            }
            else
            {
                QMessageBox::warning(this, tr("提示"), strMessage);
            }
        }
    }
}


