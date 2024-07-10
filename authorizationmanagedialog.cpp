#include "authorizationmanagedialog.h"
#include "ui_authorizationmanagedialog.h"
#include "policydialog.h"
#include "messagetips.h"
#include <QRandomGenerator>
#include <QPainter>
#include "authorizationitem.h"
#include "global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#define  PICTURE_CODE_WIDTH     90
#define  PICTURE_CODE_HEIGHT    28

// 生成随机字符串
QString AuthorizationManageDialog::generateRandomCode(int length/* = 4*/)
{
    const QString possibleChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString code;
    QRandomGenerator* generator = QRandomGenerator::global();
    for (int i = 0; i < length; ++i) {
        code.append(possibleChars.at(generator->generate() % possibleChars.size()));
    }
    return code;
}

// 生成验证码图像
QPixmap AuthorizationManageDialog::generateCaptchaImage(const QString& code)
{
    QPixmap captcha(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT);
    captcha.fill(QColor(215, 215, 222));

    QPainter painter(&captcha);
    QFont font("Arial", 12);
    painter.setFont(font);

    QColor textColor;
    int iVerOffset = 0;
    for (int i = 0; i < code.length(); ++i) {
        textColor.setHsv((i * 150) % 360, 255, 150); // 随机颜色
        painter.setPen(textColor);     
        //添加垂直随机偏移量
        iVerOffset = QRandomGenerator::global()->bounded(-5, 5);
        //绘制字符，同时应用水平和垂直偏移
        painter.drawText(10 + i * 17, 20+iVerOffset, code.at(i)); // 分散字符以避免被轻易识别
    }

    // 可以增加噪声线或点以提高安全性
    /*for (int i = 0; i < 50; ++i) {
        painter.drawLine(QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)),
            QPoint(QRandomGenerator::global()->bounded(0, PICTURE_CODE_WIDTH), QRandomGenerator::global()->bounded(0, PICTURE_CODE_HEIGHT)));
    }*/

    painter.end();
    return captcha;
}

void AuthorizationManageDialog::InitListWidget()
{
    ui->listWidgetAuthorized->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetAuthorized->setSpacing(5);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetAuthorized->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetAuthorized->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetAuthorized->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->listWidgetBeAuthorized->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetBeAuthorized->setSpacing(5);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetBeAuthorized->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetBeAuthorized->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetBeAuthorized->setSelectionMode(QAbstractItemView::SingleSelection);
}

AuthorizationManageDialog::AuthorizationManageDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AuthorizationManageDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->toolBtnRefresh->setVisible(false);

    ui->labelPictureCode->installEventFilter(this);

    InitListWidget();
    RefreshPictureCode();       
}

void AuthorizationManageDialog::RefreshPictureCode()
{
    // 生成验证码字符串
    m_strPictureCode = generateRandomCode();
    qDebug() << "Piccode = " << m_strPictureCode;
    // 生成验证码图像
    QPixmap captchaImg = generateCaptchaImage(m_strPictureCode);
    ui->labelPictureCode->setPixmap(captchaImg.scaled(QSize(PICTURE_CODE_WIDTH, PICTURE_CODE_HEIGHT), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

AuthorizationManageDialog::~AuthorizationManageDialog()
{
    delete ui;
}

void AuthorizationManageDialog::on_btnClose_clicked()
{
    this->close();
}


void AuthorizationManageDialog::on_toolBtnAddAuthorization_clicked()
{
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->toolBtnRefresh->setVisible(false);
    ui->stackedWidget->setCurrentWidget(ui->pageAddAuthorization);

}


void AuthorizationManageDialog::on_toolBtnAuthorized_clicked()
{
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->toolBtnRefresh->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageAuthorized);

    HttpGetAuthorizedListInfo(true,1,1000);
}


void AuthorizationManageDialog::on_toolBtnBeAuthorized_clicked()
{
    ui->toolBtnBeAuthorized->setStyleSheet("QToolButton {border:none;color:#FF505465;background-color:#FFFFFFFF;border-radius:0px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #FF505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAuthorized->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");
    ui->toolBtnAddAuthorization->setStyleSheet("QToolButton {border:none;color:#505465;border-radius:1px;padding-left:16px;}QToolButton:hover {background-color: #FFFFFFFF;color: #505465;border-radius:0px;padding-left:16px;}");

    ui->toolBtnRefresh->setVisible(true);
    ui->stackedWidget->setCurrentWidget(ui->pageBeAuthorized);

    HttpGetAuthorizedListInfo(false, 1, 1000);
}


void AuthorizationManageDialog::on_btnAuthorPolicy_clicked()
{
    //易舜授权协议
    PolicyDialog* policy = new PolicyDialog("易舜授权协议","https://www.ysyos.com/deal/Authorization.html",this);
    //policy->show();
    policy->exec();
}

void AuthorizationManageDialog::on_btnUserPolicy_clicked()
{
    //用户使用协议
    PolicyDialog* policy = new PolicyDialog("用户使用协议","https://www.ysyos.com/deal/UserPrivacy.html",this);
    //policy->show();
    policy->exec();
}


void AuthorizationManageDialog::on_btnOk_clicked()
{
    //确定
    QString strAuthCode = ui->lineEditAuthorCode->text();
    if (strAuthCode.isEmpty())
    {
        MessageTips* dialog = new MessageTips("授权码不能为空", this);
        dialog->show();
        return;
    }

    QString strInputCode = ui->lineEditPictureCode->text();
    if (strInputCode.compare(m_strPictureCode,Qt::CaseInsensitive) != 0)
    {
        MessageTips* dialog = new MessageTips("验证码不一致", this);
        dialog->show();
        return;
    }

    bool bCheck = ui->checkBoxPolicy->isChecked();
    if(!bCheck)
    {
        MessageTips* dialog = new MessageTips("请先阅读并勾选",this);
        dialog->show();
        return;
    }

    //添加授权操作
    HttpPostAddAuthCode(strAuthCode);
}
void AuthorizationManageDialog::HttpPostAddAuthCode(QString strAuthCode)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_ADD_AUTH_CODE;
    strUrl += QString("%1").arg(strAuthCode);
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isObject())
                    {
                        qDebug() << "添加授权码";
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

void AuthorizationManageDialog::on_btnCancel_clicked()
{
    //取消
    this->close();
}


void AuthorizationManageDialog::on_toolBtnRefresh_clicked()
{
    //刷新,重新拉取授权记录
    ui->stackedWidget->setCurrentWidget(ui->pageBeAuthorized);

    
    if (ui->stackedWidget->currentWidget() == ui->pageBeAuthorized)
        HttpGetAuthorizedListInfo(false, 1, 1000);
    else
        HttpGetAuthorizedListInfo(true, 1, 1000);
}



bool AuthorizationManageDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched== ui->labelPictureCode)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* e = static_cast<QMouseEvent*>(event);
            switch (e->button())
            {
            case Qt::LeftButton:
                RefreshPictureCode();
                return true;
                break;
            default:
                break;
            }
        }
        else
        {
            return false;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void AuthorizationManageDialog::HttpGetAuthorizedListInfo(bool bIsAuth, int iPage,int iPageSize)
{
    //已授权列表
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    if(bIsAuth)
        strUrl += HTTP_GET_AUTHORIZATION_LIST;
    else
        strUrl += HTTP_GET_BE_AUTHORIZATION_LIST;
    //level不传值,返回该 组下面所有的level
    strUrl += QString::asprintf("?page=%d&pageSize=%d", iPage, iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    //发出GET请求
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(response, &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            qWarning() << "Json parse error:" << parseError.errorString();
        }
        else
        {
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                int iCode = obj["code"].toInt();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iTotal = data["total"].toInt();
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();
                        QMap<int, S_AUTHOR_INFO> map;
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_AUTHOR_INFO authInfo;                            
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                if(bIsAuth)
                                    authInfo.iType = EN_AUTHORIZATION;
                                else
                                    authInfo.iType = EN_BE_AUTHORIZATION;
                                authInfo.strAuthCode = recordObj["authCode"].toString();
                                authInfo.iAuthUserId = recordObj["authUserId"].toInt();
                                authInfo.iCreateBy = recordObj["createBy"].toInt();
                                authInfo.strCreateTime = recordObj["createTime"].toString();
                                authInfo.iInstanceId = recordObj["userInstanceId"].toInt();
                                authInfo.strInstanceName = recordObj["instanceName"].toString();
                                authInfo.strInstanceNo = recordObj["instanceNo"].toString();
                                authInfo.iStatus = recordObj["status"].toInt();
                                authInfo.strExpireTime = recordObj["expireTime"].toString();
                                authInfo.strGrantControl = recordObj["grantControl"].toString();
                                map.insert(i, authInfo);
                            }                            
                        }
                        if (bIsAuth)
                            LoadAuthorizedList(map);
                        else
                            LoadBeAuthorizedList(map);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

void AuthorizationManageDialog::LoadAuthorizedList(QMap<int, S_AUTHOR_INFO> map)
{
    ui->listWidgetAuthorized->clear();
    //获取已授权记录,假数据
    QListWidgetItem* item = NULL;
    authorizationItem* itemWidget = NULL;
    QMap<int, S_AUTHOR_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetAuthorized);
        item->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
        //item->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
        itemWidget = new authorizationItem(*iter,this);
        ui->listWidgetAuthorized->addItem(item);
        ui->listWidgetAuthorized->setItemWidget(item, itemWidget);
    }
}

void AuthorizationManageDialog::LoadBeAuthorizedList(QMap<int, S_AUTHOR_INFO> map)
{
    ui->listWidgetBeAuthorized->clear();
    QMap<int, S_AUTHOR_INFO>::iterator iter = map.begin();
    authorizationItem* itemWidget = NULL;
    QListWidgetItem* item = NULL;
    for (; iter != map.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetBeAuthorized);
        item->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
        //item->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
        itemWidget = new authorizationItem(*iter,this);
        ui->listWidgetBeAuthorized->addItem(item);
        ui->listWidgetBeAuthorized->setItemWidget(item, itemWidget);
    }
}
