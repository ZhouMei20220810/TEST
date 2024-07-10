#include "toolobject.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"

ToolObject::ToolObject(QObject *parent)
    : QObject{parent}
{

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
                            //qDebug() << "任务返回数据 No" << taskInfo.strPadCode << "下载图片地址:" << taskInfo.strUrl;
                            m_mapScreenshotTask.insert(taskInfo.strPadCode, taskInfo);
                        }
                    }
                    emit getScreenshortSignals(m_mapScreenshotTask);
                    
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
                    MessageTips* tips = new MessageTips("实例重置操作成功!");
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