#include "toolobject.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"
#include <QEventLoop>
#include "updatesoftwaredialog.h"
#include <QVersionNumber>
#include <QSettings>

ToolObject::ToolObject(QObject *parent)
    : QObject{parent}
{

}
ToolObject* ToolObject::getInstance()
{
    //单实例对象
    static ToolObject* toolObject = new ToolObject();
    return toolObject;
}

//获取实例截图
void ToolObject::HttpPostInstanceScreenshot(QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_INSTANCE_SCREENSHOT;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    //qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i));
    }
    //doc.setObject(listArray);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //qDebug() << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        //qDebug() << response;

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
                //qDebug() << "获取实例截图Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //更新界面图
                    QMap<QString, S_TASK_INFO> mapScreenshotTask;
                    mapScreenshotTask.clear();
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        QJsonObject dataObj;
                        S_TASK_INFO taskInfo;
                        int iDataSize = dataArray.size();
                        for (int i = 0; i < iDataSize; i++)
                        {
                            dataObj = dataArray[i].toObject();
                            taskInfo.fTaskId = dataObj["taskId"].toDouble();
                            taskInfo.fTaskStatus = dataObj["taskStatus"].toDouble();
                            taskInfo.strUrl = dataObj["url"].toString();
                            taskInfo.strPadCode = dataObj["padCode"].toString();
                            if (taskInfo.strUrl.isEmpty())
                            {
                                qDebug() << "任务返回空数据 No" << taskInfo.strPadCode << "下载图片地址:" << taskInfo.strUrl;
                            }
                            mapScreenshotTask.insert(taskInfo.strPadCode, taskInfo);
                        }
                    }
                    emit getScreenshortSignals(mapScreenshotTask);
                    
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//刷新实例截图
void ToolObject::HttpPostInstanceScreenshotRefresh(QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_INSTANCE_SCREENSHOT_REFRESH;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    //qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i));
    }
    //doc.setObject(listArray);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //qDebug() << postData;
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        //qDebug() << response;

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
                QString data = obj["data"].toString();
                //qDebug() << "刷新实例截图 Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    emit startTimerShowScreenshotSignals();
                    //获取截图
                    //QThread::sleep(6);//延迟6秒
                    //HttpPostInstanceScreenshot(strList);
                    //保存图片到本地
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}
//实例重启
void ToolObject::HttpPostInstanceReboot(QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_INSTANCE_REBOOT;
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
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i));
    }
    //doc.setObject(listArray);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
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
                QString data = obj["data"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    MessageTips* tips = new MessageTips("实例重启操作成功!");
                    tips->show();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}

//实例重置
void ToolObject::HttpPostInstanceReset(QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_INSTANCE_RESET;
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
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i));
    }
    //doc.setObject(listArray);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
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
                QString data = obj["data"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    MessageTips* tips = new MessageTips("恢复出厂设置操作成功!");
                    tips->show();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
    });
}


void ToolObject::HttpPostCancelAuth(int iPhoneId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_CANCEL_AUTH;
    strUrl += QString("%1").arg(iPhoneId);
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                
                MessageTips* tips = new MessageTips(strMessage);
                tips->show();
                emit closeAuthDialogOrGroupRefreshSignals();
            }
        }
        reply->deleteLater();
        });
}

void ToolObject::HttpPostAuthDetail(int iPhoneId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_AUTH_DETAIL;
    strUrl += QString("%1").arg(iPhoneId);
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                        QJsonObject data = obj["data"].toObject();
                        S_AUTHOR_INFO authInfo;
                        authInfo.strAuthCode = data["authCode"].toString();
                        authInfo.iAuthUserId = data["authUserId"].toInt();
                        authInfo.iCreateBy = data["createBy"].toInt();
                        authInfo.strCreateTime = data["createTime"].toString();
                        authInfo.iInstanceId = data["userInstanceId"].toInt();
                        authInfo.strInstanceName = data["instanceName"].toString();
                        authInfo.strInstanceNo = data["instanceNo"].toString();
                        authInfo.iStatus = data["status"].toInt();
                        authInfo.strExpireTime = data["expireTime"].toString();
                        authInfo.strGrantControl = data["grantControl"].toString();
                        emit ShowAuthDetailSignals(authInfo);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//版本检测
void ToolObject::HttpPostCheckAppVersion()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_CHECK_UPDATE_APP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    //QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    //request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("type", "versionCodePC");
    //obj.insert("pageSize", iPageSize);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    //发出GET请求
    QNetworkReply* reply = manager->post(request, postData);
    QEventLoop              loop;    
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
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        int iSize = dataArray.size();
                        S_VERSION_INFO versionInfo;
                        QJsonObject obj;
                        for (int i = 0; i < iSize; i++)
                        {
                            obj = dataArray[i].toObject();
                            versionInfo.strType = obj["type"].toString();
                            if (versionInfo.strType.compare("versionCodePC") != 0)
                            {
                                continue;
                            }
                            versionInfo.strVersion = obj["k"].toString(); //"1.0.11";//obj["k"].toString();
                            versionInfo.iIsFurcedUpdate = obj["v1"].toString();
                            versionInfo.strDownloadUrl = obj["v2"].toString();
                            versionInfo.strUpdateContents = obj["v3"].toString();
                            versionInfo.strV4 = obj["v4"].toString();
                            versionInfo.strV5 = obj["v5"].toString();
                            QVersionNumber serverVer = QVersionNumber::fromString(versionInfo.strVersion);
                            QVersionNumber appVer = QVersionNumber::fromString(CURRENT_APP_VERSION);
                            if (versionInfo.iIsFurcedUpdate.toInt() == 1)//强制更新
                            {
                                //不需要判断版本，直接更新
                                qDebug() << "强制更新";
                                if (appVer > serverVer)
                                {
                                    qDebug() << "need uninstall local app.";
                                    GlobalData::bIsUninstallHighVersion = true;                                    
                                }
                                else
                                {
                                    qDebug() << "don't need uninstall local app.";
                                    GlobalData::bIsUninstallHighVersion = false;
                                }
                                if (serverVer != appVer)
                                {
                                    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
                                    setting.setValue("IsUninstallHighVersion", GlobalData::bIsUninstallHighVersion);
                                    GlobalData::bNeedForcedUpdateApp = true;
                                    UpdateSoftwareDialog* dialog = new UpdateSoftwareDialog(versionInfo);
                                    dialog->exec();
                                }
                            }
                            else
                            {
                                //弹出提示框是否更新
                                if (appVer < serverVer)
                                {
                                    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
                                    setting.setValue("IsUninstallHighVersion", GlobalData::bIsUninstallHighVersion);
                                    qDebug() << "don't need uninstall local app.";
                                    GlobalData::bIsUninstallHighVersion = false;
                                    UpdateSoftwareDialog* dialog = new UpdateSoftwareDialog(versionInfo);
                                    dialog->exec();
                                }
                            }                            
                            qDebug() << "type=" << versionInfo.strType << "serverVer=" << versionInfo.strVersion<<"appVer="<< CURRENT_APP_VERSION << "v1（1：强制，0:不强制）=" << versionInfo.iIsFurcedUpdate << "下载地址=" << versionInfo.strDownloadUrl<<"是否卸载"<< GlobalData::bNeedForcedUpdateApp;
                            break;
                        }
                    }
                }
            }
        }
        reply->deleteLater();
        });

    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

void ToolObject::HttpGetNoticeListInfo(NOTICE_TYPE enType, int iPage, int iPageSize)
{
    //已授权列表
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_NOTICE_LIST;
    //platform PC端值为1
    strUrl += QString::asprintf("?page=%d&pageSize=%d&platform=%d", iPage, iPageSize, 1);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);
        
    //发出GET请求
    QNetworkReply* reply = manager->get(request);
    QEventLoop loop;    
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
                    QMap<int, S_NOTICE_INFO> mapNotice;
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iTotal = data["total"].toInt();
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_NOTICE_INFO noticeInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                noticeInfo.iId = recordObj["id"].toInt();
                                noticeInfo.iType = recordObj["type"].toInt();
                                noticeInfo.strTitle = recordObj["title"].toString();
                                noticeInfo.strCreateTime = recordObj["createTime"].toString();
                                noticeInfo.strRemark = recordObj["remark"].toString();
                                qDebug() << "公告 title=" << noticeInfo.strTitle << "remark=" << noticeInfo.strRemark;
                                noticeInfo.iCreateBy = recordObj["createBy"].toInt();
                                noticeInfo.bIsRead = recordObj["isRead"].toBool();
                                mapNotice.insert(i, noticeInfo);
                            }
                        }
                    }
                    //显示
                    emit noticeListInfoSignals(enType, mapNotice);
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
}

//获取我的实例级别
void ToolObject::HttpGetMyInstanceLevel(int iPhoneId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MYINSTANCE_LEVEL;
    strUrl += QString("/%1").arg(iPhoneId);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        int iDataSize = dataArray.size();
                        if (0 == iDataSize)
                        {
                            return;
                        }
                        else
                        {
                            QJsonObject data;
                            for (int i = 0; i < iDataSize; i++)
                            {
                                data = dataArray[i].toObject();
                                QString strAshIcon = data["ashIcon"].toString();
                                QString strColorIcon = data["colorIcon"].toString();
                                int iId = data["id"].toInt();
                                bool isEnabled = data["isEnabled"].toBool();
                                QString strlevelName = data["name"].toString();
                                QString strRemark = data["remark"].toString();
                                qDebug() << "获取我的实例级别 id=" << iId << "strlevelName" << strlevelName << "remark=" << strRemark << "isEnabled=" << isEnabled;
                            }
                        }

                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//实例重命名
void ToolObject::HttpPostInstanceRename(int iId, QString strName)
{
    //实例名称修改
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_SET_INSTANCE_NAME;
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
    obj.insert("id", iId);
    obj.insert("name", strName);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode && data)
                {
                    emit HttpPostInstanceRenameSignals(iId, strName);
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

void ToolObject::HttpDeleteGroup(int iGroupId)//删除分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_DELETE_GROUP;
    strUrl += QString("/%1").arg(iGroupId);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功                    
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        //on_btnGroupRefresh_clicked();
                        emit HttpGroupRefreshSignals();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

void ToolObject::HttpLogout()
{
    //关闭窗口并且退出登录
    qDebug() << "注销";
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_YSY_LOGOUT;
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
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
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
                qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    qDebug() << "注销成功";
                    //this->close();
                    emit HttpLogoutSignals();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//设置实例分组
void ToolObject::HttpPostInstanceSetGroup(int iGroupId, QStringList strList)
{
    int iSize = strList.size();
    if (iSize <= 0)
        return;
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_SET_INSTANCE_GROUP;
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
    QJsonObject jsonObj;
    jsonObj["groupId"] = iGroupId;

    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strList.at(i).toInt());
    }
    //doc.setObject(listArray);
    jsonObj["ids"] = listArray;
    //doc.setArray(listArray);
    QJsonDocument doc(jsonObj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
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
                bool bData = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode && bData)
                {
                    //on_btnGroupRefresh_clicked();
                    emit HttpGroupRefreshSignals();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

void ToolObject::HttpCreateGroup(QString strGroupName)//创建分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_GROUP;
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
    obj.insert("name", strGroupName);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //界面添加该组
                    //on_btnGroupRefresh_clicked();
                    emit HttpGroupRefreshSignals();
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
void ToolObject::HttpUpdateGroup(int iGroupId, QString strNewName)//修改分组
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_UPDATE_GROUP;
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
    obj.insert("createBy", "");
    obj.insert("id", iGroupId);
    obj.insert("name", strNewName);
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
                bool data = obj["data"].toBool();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << data << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (data)
                    {
                        //界面直接修改名称不需要重新请求
                        //on_btnGroupRefresh_clicked();
                        emit HttpGroupRefreshSignals();
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
//激活码接口
void ToolObject::HttpPostActivateCode(QMap<int, S_ACTIVE_CODE_INFO> mapActiveCode)
{
    if (mapActiveCode.size() <= 0)
        return;

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_ACTIVE_CODE;
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
    QJsonArray jsonArray;
    QJsonObject rootObject;

    QMap<int, S_ACTIVE_CODE_INFO>::iterator iter = mapActiveCode.begin();
    //for (const QString& strActiveCode : strActiveCodeList)
    for (; iter != mapActiveCode.end(); iter++)
    {
        obj.insert("code", iter->strRenewActiveCode);
        if (iter->iRelateId != 0)
        {
            obj.insert("relateId", iter->iRelateId);
        }
        //obj.insert("relateId", iRelateId);
        jsonArray.append(obj);
    }


    rootObject.insert("activateList", jsonArray);
    doc.setObject(rootObject);
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
                qDebug() << "response = " << response;
                QString strMessage = obj["message"].toString();
                if (obj["data"].isArray())
                {
                    QJsonArray dataArray = obj["data"].toArray();
                    int iSize = dataArray.size();
                    QString strCode;
                    bool bSuccess = false;
                    QJsonObject obj;
                    QMap<QString, bool> mapStatus;
                    for (int i = 0; i < iSize; i++)
                    {
                        obj = dataArray[i].toObject();
                        strCode = obj["code"].toString();
                        bSuccess = obj["isSuccess"].toBool();
                        mapStatus.insert(strCode, bSuccess);
                    }
                    emit activeCodeStatusSignals(mapStatus);
                }
            }
        }
        reply->deleteLater();
        });
}
//订单接口-创建订单
void ToolObject::HttpCreateOrder(int iChannel, int iMemberId, int iNum, int iPayType, QString strRelateId)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_CREATE_ORDER;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit());
    request.setUrl(url);
    QJsonDocument doc;
    QJsonObject obj;

    obj.insert("channel", iChannel);
    obj.insert("memberId", iMemberId);
    obj.insert("num", iNum);
    obj.insert("payType", iPayType);
    if (!strRelateId.isEmpty())
    {
        obj.insert("relateId", strRelateId);
    }
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);

    QNetworkReply* reply = manager->post(request, postData);
    //连接请求完成的信号
    connect(reply, &QNetworkReply::finished, this, [=] {
        //读取响应数据
        QByteArray response = reply->readAll();
        qDebug() << "response=" << response;

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
                qDebug() << "Code=" << iCode << "message=" << strMessage;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    QString strData = obj["data"].toString();
                    qDebug() << strData;

                    doc = QJsonDocument::fromJson(strData.toUtf8(), &parseError);
                    if (parseError.error != QJsonParseError::NoError)
                    {
                        qDebug() << response;
                        qWarning() << "Json parse error:" << parseError.errorString();
                    }
                    else
                    {
                        obj = doc.object();
                        QJsonObject objResponse = obj["alipay_trade_precreate_response"].toObject();
                        QString strCode = objResponse["code"].toString();
                        QString strMsg = objResponse["msg"].toString();
                        QString strOutTradeNo = objResponse["out_trade_no"].toString();
                        QString strQrCode = objResponse["qr_code"].toString();

                        qDebug() << strQrCode;
                        emit HttpCreateOrderSignals(strQrCode);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
//关闭订单
void ToolObject::HttpCloseOrder(QString strOutTradeNo)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += "/api/order/closeOrder";// HTTP_CLOSE_ORDER;
    //strUrl += QString("?outTradeNo=%1").arg(strOutTradeNo);
    strUrl += QString("/%1").arg(strOutTradeNo);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

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
                bool bData = obj["data"].toBool();
                QString strMessage = obj["message"].toString();
                qDebug() << "Code=" << iCode << "message=" << strMessage << "json=" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (bData)
                        qDebug() << "关闭订单成功";
                    else
                        qDebug() << "关闭订单失败";
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//会员级别接口
void ToolObject::HttpLevelList()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_LEVEL_LIST;
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
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strCode);
    obj.insert("password", strPassword);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    QByteArray postData = "";
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
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        QJsonObject objData;
                        int iSize = dataArray.size();
                        //int会员等级
                        QMap<int, S_LEVEL_INFO> mapLevelList;
                        mapLevelList.clear();
                        S_LEVEL_INFO info;
                        for (int i = 0; i < iSize; i++)
                        {
                            objData = dataArray.at(i).toObject();
                            info.iLevelId = objData["id"].toInt();
                            info.strLevelName = objData["name"].toString();
                            info.strColorIcon = objData["colorIcon"].toString();
                            /*if (!info.strColorIcon.isEmpty())
                            {
                                startDownload(info.strColorIcon);
                            }*/
                            //安卓用
                            info.strAshIcon = objData["ashIcon"].toString();
                            /*if (!info.strAshIcon.isEmpty())
                            {
                                startDownload(info.strAshIcon);
                            }*/
                            //isEnabled true能用;false禁用
                            info.bIsEnabled = objData["isEnable"].toBool();
                            info.strLevelRemark = objData["remark"].toString();
                            info.strFucImg = objData["describeUrl"].toString();
                            qDebug() << "等级" << info.iLevelId << " name=" << info.strLevelName;
                            if (info.bIsEnabled)
                            {
                                mapLevelList.insert(info.iLevelId, info);
                            }
                        }

                        emit HttpLevelListSignals(mapLevelList);                        
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//会员相关接口
void ToolObject::HttpMemberLevelListData()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_MEMBER_LEVEL_LIST_DATA;
    //strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        QMap<int, S_LEVEL_DATA_INFO> mapData;
                        S_LEVEL_DATA_INFO sLevelData;
                        QJsonObject data;

                        QJsonArray memberList;
                        QJsonObject member;
                        int iMemberIndex = 0;
                        QMap<int, QMap<int, S_LEVEL_DATA_INFO>> mapLevel;
                        for (int i = 0; i < dataArray.size(); i++)
                        {
                            data = dataArray[i].toObject();
                            sLevelData.iLevelId = data["level"].toInt();
                            memberList = data["memberList"].toArray();
                            mapData.clear();
                            for (iMemberIndex = 0; iMemberIndex < memberList.size(); iMemberIndex++)
                            {
                                member = memberList[iMemberIndex].toObject();
                                sLevelData.iMemberId = member["id"].toInt();
                                sLevelData.strMemberName = member["name"].toString();
                                sLevelData.fPrice = member["price"].toDouble();
                                sLevelData.fActivityPrice = member["activityPrice"].toDouble();
                                sLevelData.strUrl = member["url"].toString();
                                sLevelData.strRemark = member["remark"].toString();
                                sLevelData.strInstanceLevel = member["instanceLevel"].toString();
                                sLevelData.iUseDay = member["useDay"].toInt();
                                sLevelData.strLevelName = member["levelName"].toString();
                                sLevelData.strColorIcon = member["colorIcon"].toString();
                                sLevelData.strAshIcon = member["ashIcon"].toString();
                                sLevelData.strLevelRemark = member["levelRemark"].toString();
                                mapData.insert(sLevelData.iMemberId, sLevelData);
                            }
                            mapLevel.insert(sLevelData.iLevelId, mapData);
                        }

                        HttpMemberLevelListDataSignals(mapLevel);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//查询全部分组
void ToolObject::HttpQueryAllGroup()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_QUERY_ALL_GROUP;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    qDebug() << "strToken=" << strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    request.setUrl(url);
    /*QJsonDocument doc;
    QJsonObject obj;
    obj.insert("code", strSMSCode);
    obj.insert("mobile", strPhone);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);*/
    //QByteArray postData = QString("{\"mobile\":\"%1\",\"code\":\"%2\"}").arg(strPhone).arg(strSMSCode).toLocal8Bit();
    //发出GET请求
    QByteArray postData = "";
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
                qDebug() << "Code=" << iCode << "message=" << strMessage << "response:" << response;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isArray())
                    {
                        QJsonArray dataArray = obj["data"].toArray();
                        int iGroupSize = dataArray.size();
                        if (0 == iGroupSize)
                        {
                            return;
                        }
                        else
                        {
                            QMap<int, S_GROUP_INFO> mapGroupInfo;                            
                            //树形列表显示
                            S_GROUP_INFO groupInfo;
                            QJsonObject dataObj;
                            for (int i = 0; i < iGroupSize; i++)
                            {
                                dataObj = dataArray[i].toObject();
                                groupInfo.iGroupId = dataObj["id"].toInt();
                                groupInfo.iGroupNum = dataObj["num"].toInt();
                                groupInfo.strGroupName = dataObj["name"].toString();
                                qDebug() << "iGroupId=" << groupInfo.iGroupId << "strGroupName=" << groupInfo.strGroupName << "iGroupCount=" << groupInfo.iGroupNum;

                                mapGroupInfo.insert(i, groupInfo);
                            }
                            emit HttpQueryAllGroupSignals(mapGroupInfo);
                        }
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//获取我的手机实例
void ToolObject::HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_PHONE_INSTANCE;
    //level不传值,返回该 组下面所有的level
    if (iLevel > 0)
        strUrl += QString::asprintf("?level=%d&page=%d&pageSize=%d", iLevel, iPage, iPageSize);
    else if (iGroupId == -1 && iLevel == -1)
    {
        strUrl += QString::asprintf("?page=%d&pageSize=%d", iPage, iPageSize);
    }
    else
        strUrl += QString::asprintf("?groupId=%d&page=%d&pageSize=%d", iGroupId, iPage, iPageSize);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
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
                        qDebug() << "iTotal=" << iTotal << "iGroupId=" << iGroupId << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();
                        QMap<int, S_PHONE_INFO> map;
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                memset(&phoneInfo, 0, sizeof(S_PHONE_INFO));
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iAuthType = recordObj["type"].toInt();
                                phoneInfo.strGrantControl = recordObj["grantControl"].toString();
                                phoneInfo.bIsAuth = recordObj["isAuth"].toBool();
                                map.insert(phoneInfo.iId, phoneInfo);
                                qDebug() << "iTotal=" << iTotal << "iGroupId=" << iGroupId << "name" << phoneInfo.strName << "strInstanceNo=" << phoneInfo.strInstanceNo << "phoneInfo.strCreateTime=" << phoneInfo.strCreateTime << "phoneInfo.strCurrentTime=" << phoneInfo.strCurrentTime << "phoneInfo.strExpireTime=" << phoneInfo.strExpireTime << "id=" << phoneInfo.iId << "authType=" << phoneInfo.iAuthType << "level=" << phoneInfo.iLevel;
                            }
                        }
                        emit HttpGetMyPhoneInstanceSignals(iLevel, iGroupId, map);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}