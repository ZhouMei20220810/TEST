#include "addauthorizationdialog.h"
#include "ui_addauthorizationdialog.h"
#include <QDateTime>
#include "messagetipsdialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "policydialog.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"
#include <QClipboard>
#include "activecodeitem.h"
#include <QListWidgetItem>
AddAuthorizationDialog::AddAuthorizationDialog(S_PHONE_INFO phoneInfo, QWidget* parent)
    : QMoveDialog(parent)
    , ui(new Ui::AddAuthorizationDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("授权管理");

    //只读+不能选择
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    m_map.insert(phoneInfo.iId, phoneInfo);

    //百度暂不支持PC端选择是否只读或可控,屏蔽界面选择
    ui->frame_9->setVisible(false);

    m_bIsAccountAuth = false;
    m_bIsReadOnly = true;
    m_phoneInfo = phoneInfo;
    m_toolObject = new ToolObject(this);
    QRegularExpression regExp("[0-9]*");
    QValidator* validator = new QRegularExpressionValidator(regExp, this);
    ui->lineEditDay->setValidator(validator);
    ui->lineEditPhoneNum->setValidator(validator);

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->radioButtonAccount);
    m_btnGroup->addButton(ui->radioButtonAuthorCode);

    QFontMetrics fontWidth2(ui->labelPhoneInstance->font());
    QString strElideNote2 = fontWidth2.elidedText(phoneInfo.strInstanceNo, Qt::ElideRight, 456);
    ui->labelPhoneInstance->setText(strElideNote2);
    ui->labelPhoneInstance->setToolTip(phoneInfo.strInstanceNo);
    ui->labelPhoneName->setText(strElideNote2);
    ui->labelPhoneName->setToolTip(phoneInfo.strInstanceNo);

    if (!phoneInfo.strName.isEmpty())
    {
        QFontMetrics fontWidth(ui->labelPhoneName->font());
        QString strElideNote = fontWidth.elidedText(phoneInfo.strName, Qt::ElideRight, 456);
        ui->labelPhoneName->setText(strElideNote);
        ui->labelPhoneName->setToolTip(phoneInfo.strName);
    }

    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime = QDateTime::fromString(phoneInfo.strExpireTime, "yyyy-MM-dd hh:mm:ss");
    qint64 mseconds = 0;
    mseconds = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();
    m_iDay = mseconds / (1000 * 60 * 60 * 24);
    QString strTime = strTime.asprintf("%d天%d小时", m_iDay, (mseconds / (1000 * 60 * 60)) % 24);
    qDebug() << "strTime=" << strTime;
    ui->labelDay->setText(strTime);
    ui->stackedWidget->setCurrentWidget(ui->pageAddAuth);
}

AddAuthorizationDialog::AddAuthorizationDialog(QMap<int, S_PHONE_INFO> map/*S_PHONE_INFO phoneInfo*/, QWidget* parent)
    : QMoveDialog(parent)
    , ui(new Ui::AddAuthorizationDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    //只读+不能选择
    ui->plainTextEdit->setReadOnly(true);
    ui->plainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);

    //百度暂不支持PC端选择是否只读或可控,屏蔽界面选择
    ui->frame_9->setVisible(false);

    m_bIsAccountAuth = false;
    m_bIsReadOnly = true;
    m_map = map;
    m_toolObject = new ToolObject(this);
    QRegularExpression regExp("[0-9]*");
    QValidator* validator = new QRegularExpressionValidator(regExp, this);
    ui->lineEditDay->setValidator(validator);
    ui->lineEditPhoneNum->setValidator(validator);

    m_btnGroup = new QButtonGroup(this);
    m_btnGroup->addButton(ui->radioButtonAccount);
    m_btnGroup->addButton(ui->radioButtonAuthorCode);
    QMap<int, S_PHONE_INFO>::iterator iter = map.begin();
    QString strPhoneNameList="";
    QString strPhoneInstanceList = "";
    QString strPhoneTime;
    QDateTime curDateTime = QDateTime::currentDateTime();
    QDateTime expireTime;
    qint64 mseconds = 0;
    qint64 timeTmp = 0;
    for (; iter != map.end(); iter++)
    {
        //过滤已授权和被授权的设备
        if (iter->bIsAuth || iter->iAuthType == EN_BE_AUTHORIZATION)
        {
            qDebug() << "已授权或者被授权 name=" << iter->strName << "No=" << iter->strInstanceNo;
            continue;
        }

        if (!strPhoneNameList.isEmpty())
        {
            strPhoneNameList += ",";
        }
        if (!strPhoneInstanceList.isEmpty())
        {
            strPhoneInstanceList += ",";
        }
        if (iter->strName.isEmpty())
            strPhoneNameList += iter->strInstanceNo;
        else
            strPhoneNameList += iter->strName;

        strPhoneInstanceList += iter->strInstanceNo;

        //计算云手机最小的天数
        expireTime = QDateTime::fromString(iter->strExpireTime, "yyyy-MM-dd hh:mm:ss");
        timeTmp = expireTime.toMSecsSinceEpoch() - curDateTime.toMSecsSinceEpoch();
        if (mseconds == 0)
            mseconds = timeTmp;
        else if (timeTmp < mseconds)
        {
            mseconds = timeTmp;
        }
            
    }

    if (strPhoneNameList.isEmpty())
        ui->labelPhoneName->setText("暂无可授权设备");
    else
    {
        QFontMetrics fontWidth(ui->labelPhoneName->font());
        QString strElideNote = fontWidth.elidedText(strPhoneNameList, Qt::ElideRight, 456);
        ui->labelPhoneName->setText(strElideNote);
        ui->labelPhoneName->setToolTip(strPhoneNameList);
    }

    if (strPhoneInstanceList.isEmpty())
        ui->labelPhoneInstance->setText("暂无可授权设备");
    else
    {
        QFontMetrics fontWidth2(ui->labelPhoneInstance->font());
        QString strElideNote2 = fontWidth2.elidedText(strPhoneInstanceList, Qt::ElideRight, 456);
        ui->labelPhoneInstance->setText(strElideNote2);
        ui->labelPhoneInstance->setToolTip(strPhoneInstanceList);
    }
    
    m_iDay = mseconds / (1000 * 60 * 60 * 24);
    QString strTime = strTime.asprintf("%d天%d小时", m_iDay, (mseconds / (1000 * 60 * 60)) % 24);
    qDebug() << "strTime=" << strTime;
    ui->labelDay->setText(strTime);
    //批量需过滤已授权或者被授权设备
    ui->stackedWidget->setCurrentWidget(ui->pageAddAuth);
}

AddAuthorizationDialog::~AddAuthorizationDialog()
{
    delete ui;
}

void AddAuthorizationDialog::InitWidget(S_AUTHOR_INFO authInfo)
{
    //授权码为空，则账号授权
    if (authInfo.strAuthCode.isEmpty())
        InitAccountPage(authInfo); 
    else
        InitAuthCodePage(authInfo);
    switch (authInfo.iType)
    {
    case EN_AUTHORIZATION:
        ui->btnCancelAuthCode->setEnabled(true);
        ui->btnCancelAuthAccount->setEnabled(true);
        break;
    case EN_BE_AUTHORIZATION:
        ui->btnCancelAuthCode->setEnabled(false);
        ui->btnCancelAuthAccount->setEnabled(false);
        break;
    default:
        break;
    }
}

void AddAuthorizationDialog::on_btnClose_clicked()
{
    this->close();
}


void AddAuthorizationDialog::on_btnOk_clicked()
{
    //确定授权
    //发送请求给服务器
    if (!ui->checkBoxPolicy->isChecked())
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("请先阅读授权协议", this);
        dialog->show();
        return;
    }

    QString strPhone = ui->lineEditPhoneNum->text();
    int iAuthDay = ui->lineEditDay->text().toInt();
    qDebug()<<"instance="<<m_phoneInfo.strInstanceNo<<"strPhone="<<strPhone<<"iDay="<<iAuthDay<<"readonly="<<m_bIsReadOnly<<"accountAuth="<<m_bIsAccountAuth;
    if(m_bIsAccountAuth && strPhone.isEmpty())
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("指定账号授权，账号不能为空", this);
        dialog->show();
        return;
    }

    int iDay = ui->lineEditDay->text().toInt();
    if (m_iDay < iDay)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权天数不能大于云手机剩余天数", this);
        dialog->show();
        return;
    }
    if (iDay > 999)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权最大值999天", this);
        dialog->show();
        return;
    }
    if (iDay < 1)
    {
        MessageTipsDialog* dialog = new MessageTipsDialog("授权最小值1天", this);
        dialog->show();
        return;
    }

    QDateTime ExpireTime = QDateTime::currentDateTime().addDays(iDay);
    //根据单选弹框
    if(ui->radioButtonAccount->isChecked())
    {
        HttpPostBatchAuthAccountByPhone(m_bIsReadOnly, m_map, ExpireTime.toMSecsSinceEpoch(), strPhone);
    }
    else
    {
        //生成授权码
        HttpPostBatchGeneratorAuthCode(m_bIsReadOnly, m_map, ExpireTime.toMSecsSinceEpoch());
    }
    emit notifyMainWindowRefreshGroupListSignals();
}
void AddAuthorizationDialog::HttpPostBatchGeneratorAuthCode(bool bIsReadOnly, QMap<int, S_PHONE_INFO> map, qint64 iExpireTime)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_GENERATOR_AUTH_CODE;

    QString strGrantControl = "CONTROL";
    if (bIsReadOnly)
        strGrantControl = "WATCH";
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("grantControl", strGrantControl);
    obj.insert("expireTime", iExpireTime);
    QJsonArray listArray;
    QMap<int, S_PHONE_INFO>::iterator iter = map.begin();
    for (;iter != map.end();iter++)
    {
        listArray.append(iter->iId);
    }
    //doc.setObject(listArray);
    //jsonObj["ids"] = listArray;
    obj.insert("userInstanceIds", listArray);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

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
                QString strData = obj["data"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << strData << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    m_iInstanceId = m_phoneInfo.iId;
                    if (m_phoneInfo.strName.isEmpty())
                        ui->labelName->setText(m_phoneInfo.strInstanceNo);
                    else
                        ui->labelName->setText(m_phoneInfo.strName);
                    if (ui->radioButtonReadOnly->isChecked())
                    {
                        ui->labelQuanxian->setText("仅观看");
                    }
                    else
                    {
                        ui->labelQuanxian->setText("可操控");
                    }

                    /*if (m_phoneInfo.bUsed)
                    {
                        ui->labelUseStatus->setText("已使用");
                    }
                    else
                    {
                        ui->labelUseStatus->setText("未使用");
                    }*/
                    QDateTime currentTime = QDateTime::currentDateTime();
                    QDateTime newDateTime = currentTime.addDays(ui->lineEditDay->text().toInt());
                    ui->labelUseDay->setText(newDateTime.toString("yyyy-MM-dd hh:mm:ss"));

                    ui->label->setText("授权信息");
                    setWindowTitle("授权信息");
                    ui->toolBtnAuthCode->setText(strData);
                    ui->stackedWidget->setCurrentWidget(ui->pageCode);
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
void AddAuthorizationDialog::HttpPostBatchAuthAccountByPhone(bool bIsReadOnly, QMap<int, S_PHONE_INFO> map, qint64 iExpireTime, QString strPhoneNum)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_AUTH_ACCOUNT_BY_PHONE;

    QString strGrantControl = "CONTROL";
    if (bIsReadOnly)
        strGrantControl = "WATCH";
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("grantControl", strGrantControl);
    obj.insert("expireTime", iExpireTime);
    QJsonArray listArray;
    QMap<int, S_PHONE_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        listArray.append(iter->iId);
    }
    //doc.setObject(listArray);
    //jsonObj["ids"] = listArray;
    obj.insert("userInstanceIds", listArray);
    obj.insert("mobile", strPhoneNum);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << "postData=" << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << response;

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
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    QJsonArray dataArray = obj["data"].toArray();
                    int iSize = dataArray.size();
                    S_AUTH_RESULT_INFO resultInfo;
                    QJsonObject data;
                    QMap<int, S_AUTH_RESULT_INFO> map;
                    for (int i = 0; i < iSize; i++)
                    {
                        data = dataArray[i].toObject();
                        resultInfo.strPhoneName = data["code"].toString();
                        resultInfo.bIsSuccess = data["isSuccess"].toBool();
                        resultInfo.strRemark = data["msg"].toString();
                        map.insert(i, resultInfo);
                    }

                    if (map.size() > 1)
                    {
                        LoadResultInfo(map);
                        ui->stackedWidget->setCurrentWidget(ui->pageBatchResult);
                    }
                    else
                    {
                        //刚生成授权码直接从界面获取值显示，避免UI等待
                        m_iInstanceId = m_phoneInfo.iId;
                        if (m_phoneInfo.strName.isEmpty())
                            ui->labelName_2->setText(m_phoneInfo.strInstanceNo);
                        else
                            ui->labelName_2->setText(m_phoneInfo.strName);
                        if (ui->radioButtonReadOnly->isChecked())
                        {
                            ui->labelQuanxian_2->setText("仅观看");
                        }
                        else
                        {
                            ui->labelQuanxian_2->setText("可操控");
                        }

                        //if (m_phoneInfo.bUsed)
                        //{
                        //    ui->labelUseStatus_2->setText("已使用");
                        //}
                        //else
                        //{
                        ui->labelUseStatus_2->setText("未使用");
                        //}

                        QDateTime currentTime = QDateTime::currentDateTime();
                        QDateTime newDateTime = currentTime.addDays(ui->lineEditDay->text().toInt());
                        ui->labelUseDay_2->setText(newDateTime.toString("yyyy-MM-dd hh:mm:ss"));

                        ui->label->setText("授权信息");
                        setWindowTitle("授权信息");
                        ui->stackedWidget->setCurrentWidget(ui->pageAccount);
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
void AddAuthorizationDialog::LoadResultInfo(QMap<int, S_AUTH_RESULT_INFO> map)
{
    if (map.size() <= 0)
        return;
    ui->listWidgetBatchAuthResult->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetBatchAuthResult->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetBatchAuthResult->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetBatchAuthResult->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidgetBatchAuthResult->setSelectionMode(QAbstractItemView::SingleSelection);

    QMap<int, S_AUTH_RESULT_INFO>::iterator iter = map.begin();
    ActiveCodeItem* widget= NULL;
    QListWidgetItem* item = NULL;
    for (; iter != map.end(); iter++)
    {
        widget = new ActiveCodeItem(iter->strPhoneName, iter->strRemark, this);        
        item = new QListWidgetItem(ui->listWidgetBatchAuthResult);
        item->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, 28));
        //phoneItem->setData(Qt::UserRole, QVariant::fromValue(phoneInfo));
        ui->listWidgetBatchAuthResult->addItem(item);
        ui->listWidgetBatchAuthResult->setItemWidget(item, widget);
    }
}
void AddAuthorizationDialog::InitAuthCodePage(S_AUTHOR_INFO authInfo)
{
    m_iInstanceId = authInfo.iInstanceId;
    if (authInfo.strInstanceName.isEmpty())
        ui->labelName->setText(authInfo.strInstanceNo);
    else
        ui->labelName->setText(authInfo.strInstanceName);
    if (authInfo.strGrantControl.compare("WATCH", Qt::CaseInsensitive) == 0)
    {
        ui->labelQuanxian->setText("仅浏览");
    }
    else
    {
        ui->labelQuanxian->setText("控制云手机");
    }
    ui->labelUseStatus->setText(getAuthStatusString(authInfo.iStatus));
    ui->labelUseDay->setText(authInfo.strExpireTime);

    ui->label->setText("授权信息");
    setWindowTitle("授权信息");
    ui->toolBtnAuthCode->setText(authInfo.strAuthCode);
    ui->stackedWidget->setCurrentWidget(ui->pageCode);
}
void AddAuthorizationDialog::InitAccountPage(S_AUTHOR_INFO authInfo)
{
    m_iInstanceId = authInfo.iInstanceId;
    if (authInfo.strInstanceName.isEmpty())
        ui->labelName_2->setText(authInfo.strInstanceNo);
    else
        ui->labelName_2->setText(authInfo.strInstanceName);
    ui->labelUseStatus_2->setText(getAuthStatusString(authInfo.iStatus));
    //ui->labelQuanxian_2->setText();
    ui->labelUseDay_2->setText(authInfo.strExpireTime);
    if (authInfo.strGrantControl.compare("WATCH", Qt::CaseInsensitive) == 0)
    {
        ui->labelQuanxian_2->setText("仅浏览");
    }
    else
    {
        ui->labelQuanxian_2->setText("控制云手机");
    }    
    ui->label->setText("授权信息");
    setWindowTitle("授权信息");
    ui->stackedWidget->setCurrentWidget(ui->pageAccount);
}

void AddAuthorizationDialog::on_radioButtonAccount_clicked(bool checked)
{
    //账号授权
    if(checked)
    {
        m_bIsAccountAuth= true;
    }
}


void AddAuthorizationDialog::on_radioButtonAuthorCode_clicked(bool checked)
{
    //授权码授权
    if(checked)
    {
        m_bIsAccountAuth = false;
    }
}


void AddAuthorizationDialog::on_radioButtonReadOnly_clicked(bool checked)
{
    //仅浏览
    if(checked)
    {
        m_bIsReadOnly = true;
    }
}


void AddAuthorizationDialog::on_radioButtonControl_clicked(bool checked)
{
    //控制
    if(checked)
    {
        m_bIsReadOnly = false;
    }
}


void AddAuthorizationDialog::on_btnAuthorPolicy_clicked()
{
    //易舜授权协议
    PolicyDialog* policy = new PolicyDialog("易舜授权协议", "https://www.ysyos.com/deal/Authorization.html", this);
    //policy->show();
    policy->exec();
}


void AddAuthorizationDialog::on_btnCancel_3_clicked()
{
    this->close();
}

void AddAuthorizationDialog::on_btnCancel_2_clicked()
{
    this->close();
}

void AddAuthorizationDialog::on_toolBtnAuthCode_clicked()
{
    //复制授权码到剪贴板
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->toolBtnAuthCode->text());

    MessageTips* tips = new MessageTips("已复制授权号至剪贴板", this);
    tips->show();
}


void AddAuthorizationDialog::on_btnCancelAuthCode_clicked()
{    
    //取消授权，向服务器发送请求
    qDebug() << "取消授权id=" << m_iInstanceId;
    m_toolObject->HttpPostCancelAuth(m_iInstanceId);  
    emit notifyMainWindowRefreshGroupListSignals();
    connect(m_toolObject, &ToolObject::closeAuthDialogOrGroupRefreshSignals, this, [=]() {
        this->close();
        });
}


void AddAuthorizationDialog::on_btnCancelAuthAccount_clicked()
{
    on_btnCancelAuthCode_clicked();
}

