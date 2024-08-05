#include "queuetableitem.h"
#include "ui_queuetableitem.h"
#include <QFileInfo>
#include "global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "messagetipsdialog.h"
#include <alibabacloud/oss/OssClient.h>
#include <fstream>
#include "tuploadfilethread.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace AlibabaCloud::OSS;
QueueTableItem::QueueTableItem(QStringList strPhoneList, QString strFilePath, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::QueueTableItem)
{
    ui->setupUi(this);
    ui->toolBtnCancel->setVisible(true);
    ui->toolBtnFinish->setVisible(false);
    ui->toolBtnDelete->setVisible(false);
    ui->toolBtnReupload->setVisible(false);

    m_IsAbort = false;
    /* 初始化网络等资源 */
    InitializeSdk();

    m_strPhoneList = strPhoneList;
    m_strFilePath = strFilePath;
    QFileInfo fileInfo(strFilePath);
    QFontMetrics fontWidth(ui->labelFileName->font());
    QString strElideNote = fontWidth.elidedText(fileInfo.fileName(), Qt::ElideRight, 150);
    ui->labelFileName->setText(strElideNote);
    ui->labelFileName->setToolTip(fileInfo.fileName());

    m_i64FileSize = fileInfo.size();
    ui->progressBar->setValue(0);
    ui->labelProgressStatus->setText("等待中");


    qDebug() << "QueueTableItem所在线程 " << QThread::currentThreadId();
    m_Timer = new QTimer();
    m_Timer->start(1);
    connect(m_Timer, &QTimer::timeout, this, [this]()
        {
            m_Timer->stop();
            //先校验文件md5是否已存在
            HttpPostFileCheckMd5(GlobalData::getFileMd5(m_strFilePath));            
        });
    
}

void QueueTableItem::RefreshPrograssBarValue(int64_t transfered, int64_t total)
{
    if (total > transfered)
    {
        ui->progressBar->setValue(transfered/total*100);
    }
    else
    {
        ui->progressBar->setValue(100);
    }
    
    qDebug() << "transfered=" << transfered << "total=" << total;
}

QueueTableItem::~QueueTableItem()
{
    if (m_Timer->isActive())
    {
        m_Timer->stop();
    }
    /* 释放网络等资源 */
    ShutdownSdk();
    delete ui;
}

void QueueTableItem::on_toolBtnCancel_clicked()
{
    ui->toolBtnCancel->setVisible(false);
    ui->toolBtnFinish->setVisible(false);
    ui->toolBtnDelete->setVisible(true);
    ui->toolBtnReupload->setVisible(true);
    ui->labelProgressStatus->setText("已取消");
    ui->progressBar->setStyleSheet("QProgressBar{text-align:center;background-color: #FFC4C7D0;border: 0px solid #FFC4C7D0;border-radius:2px;}QProgressBar::chunk{background-color:#FFA9ADB6;border-radius:2px;}");
    ui->progressBar->setValue(80);
    //上传取消
    cancelUploadFile(m_strFilePath, m_strPhoneList);
}


void QueueTableItem::on_toolBtnDelete_clicked()
{
    //删除
    //qDebug() << "delete " << strFilePath;
    emit deleteQueueFileItemSignal(m_strFilePath);
}


void QueueTableItem::on_toolBtnReupload_clicked()
{
    m_IsAbort = false;
    ui->progressBar->setStyleSheet("QProgressBar{text-align:center;background-color: #FFC4C7D0;border: 0px solid #FFC4C7D0;border-radius:2px;}QProgressBar::chunk{background-color:#FF407AFF;border-radius:2px;}");
    ui->progressBar->setValue(0);

    startThreadUploadFile();
    //重新上传
    /*if (uploadFile(m_strFilePath, m_strPhoneList))
    {
        
    }*/
}

bool QueueTableItem::cancelUploadFile(const QString& filePath, QStringList strPhoneList)
{
    QFileInfo fileInfo(filePath);
    std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
    /* 填写UploadId。UploadId来源于调用InitiateMultipartUpload完成初始化分片之后的返回结果。*/
    std::string UploadId = m_UploadId;
    m_IsAbort = true;
    ClientConfiguration conf;
    conf.verifySSL = false;
    OssClient client(GlobalData::Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), GlobalData::strSecurityToken.toStdString(), conf);
     /*InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);

    //初始化分片上传事件 
    InitiateMultipartUploadOutcome uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
    std::string uploadId = uploadIdResult.result().UploadId();
    qDebug() << "m_uploadId=" << m_UploadId << "   uploadId=" << UploadId;*/
    /* 取消分片上传 */
    //一个分片上传事件被取消后，无法再使用同一个uploadId执行任何操作，已经上传的分片数据会被删除。
    AbortMultipartUploadRequest  abortUploadRequest(GlobalData::BucketName, ObjectName, UploadId);
    auto outcome = client.AbortMultipartUpload(abortUploadRequest);

    if (!outcome.isSuccess()) {
        /* 异常处理 */
        qDebug() << "AbortMultipartUpload fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId();
        return false;
    }

    return true;
}

//删除文件
bool QueueTableItem::DeleteFile(const QString& filePath, QStringList strPhoneList)
{
    QFileInfo fileInfo(filePath);
    std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt"; 

    ClientConfiguration conf;
    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);
    //token验证
    OssClient client(GlobalData::Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), GlobalData::strSecurityToken.toStdString(), conf);

    DeleteObjectRequest request(GlobalData::BucketName, ObjectName);
    auto outcome = client.DeleteObject(request);
    if (!outcome.isSuccess()) {
        /* 异常处理。*/
        std::cout << "DeleteObject fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId() << std::endl;
        return false;
    }
    return true;
}

//1、文件md5校验
void QueueTableItem::HttpPostFileCheckMd5(QString strFileMd5)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_FILE_CHECK_MD5;
    //strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
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
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("md5", strFileMd5);
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
                    //data不为空则已存在，为空则不存在
                    if (obj["data"].isObject())
                    {
                        QJsonObject data = obj["data"].toObject();
                        S_UPLOADD_FILE_INFO uploadInfo;
                        uploadInfo.iAutoInstall = 1;//默认安装
                        uploadInfo.iId = data["id"].toInt();
                        uploadInfo.strBucket = data["bucket"].toString();
                        uploadInfo.strFileName = data["fileName"].toString();
                        uploadInfo.strFileMd5 = data["fileMd5"].toString();
                        uploadInfo.i64FileSize = data["size"].toInteger();
                        uploadInfo.iStatus = data["status"].toInt();
                        uploadInfo.iCreateBy = data["createBy"].toInt();
                        uploadInfo.strCreateTime = data["createTime"].toString();

                        //已存在文件,直接上传到百度
                        qDebug() << "已存在文件,直接上传到百度";
                        HttpPostFileUploadBaidu(uploadInfo);
                    }
                    else
                    {
                        //没有文件需要上传
                        qDebug() << "没有文件需要上传";
                        startThreadUploadFile();
                    }
                }
            }
        }
        reply->deleteLater();
        });
}
//2、检查文件md5值
void QueueTableItem::HttpPostFileUploadBaidu(S_UPLOADD_FILE_INFO uploadInfo)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_FILE_UPLOAD_BAIDU;
    //strUrl += QString("?page=%1&pageSize=%2").arg(iPage).arg(iPageSize);
    qDebug() << "strUrl = " << strUrl;
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
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("autoInstall", uploadInfo.iAutoInstall);
    obj.insert("bucket", uploadInfo.strBucket);
    obj.insert("fileMd5", uploadInfo.strFileMd5);
    obj.insert("fileName", uploadInfo.strFileName);
    //obj.insert("instanceCodes", uploadInfo.iAutoInstall);
    obj.insert("size", QJsonValue(uploadInfo.i64FileSize));
    obj.insert("status", uploadInfo.iStatus);
    uploadInfo.strPhoneInstanceNo = m_strPhoneList;
    QJsonArray listArray;
    int iSize = m_strPhoneList.size();
    for (int i=0; i<iSize; i++)
    {
        listArray.append(m_strPhoneList.at(i));
    }
    obj.insert("instanceCodes", listArray);

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
                qDebug() << "Code=" << iCode << "message=" << strMessage << "data=" << bData;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (bData)
                    {
                        //ui->labelProgressStatus->setText("用户安装中");
                        qDebug() << "上传到百度成功";
                        downloadFinishRefreshUI();
                    }
                }
            }
        }
        reply->deleteLater();
        });
}

void QueueTableItem::startThreadUploadFile()
{
    TUploadFileThread* uploadThread = new TUploadFileThread(this);
    connect(uploadThread, &TUploadFileThread::started, this, [this]() 
        {
        qDebug() << "开始上传到阿里巴巴";
        });
    connect(uploadThread, &TUploadFileThread::finished, this, [this]() 
        {
        qDebug() << "上传到阿里巴巴完成";
        downloadFinishRefreshUI();
        });
    connect(uploadThread, &TUploadFileThread::refreshUISignals, this, [this](int value)
        {
            qDebug() << "refreshUISignals value=" << value;
            if (value == 80)//失败
            {
                ui->toolBtnCancel->setVisible(false);
                ui->toolBtnFinish->setVisible(false);
                ui->toolBtnDelete->setVisible(true);
                ui->toolBtnReupload->setVisible(true);
                ui->labelProgressStatus->setText("已取消");
                ui->progressBar->setStyleSheet("QProgressBar{text-align:center;background-color: #FFC4C7D0;border: 0px solid #FFC4C7D0;border-radius:2px;}QProgressBar::chunk{background-color:#FFA9ADB6;border-radius:2px;}");
            }
            ui->progressBar->setValue(value);
        });
    uploadThread->setUploadFileParam(m_strFilePath, m_strPhoneList, 1);
    uploadThread->start();
}

void QueueTableItem::downloadFinishRefreshUI()
{
    ui->labelProgressStatus->setText("解析中");
    ui->toolBtnCancel->setVisible(false);
    ui->toolBtnFinish->setVisible(true);
    ui->toolBtnDelete->setVisible(false);
    ui->toolBtnReupload->setVisible(false);
    ui->progressBar->setValue(100);
}