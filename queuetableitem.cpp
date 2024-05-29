#include "queuetableitem.h"
#include "ui_queuetableitem.h"
#include <QFileInfo>
#include "global.h"
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetipsdialog.h"
#include <alibabacloud/oss/OssClient.h>
#include <fstream>

using namespace AlibabaCloud::OSS;
QueueTableItem::QueueTableItem(QStringList strPhoneList, QString strFilePath,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QueueTableItem)
{
    ui->setupUi(this);
    ui->toolBtnCancel->setVisible(true);
    ui->labelCancelling->setVisible(false);
    ui->labelFinish->setVisible(false);
    ui->toolBtnDelete->setVisible(false);
    ui->toolBtnReupload->setVisible(false);
    /* 初始化网络等资源 */
    InitializeSdk();

    m_strPhoneList = strPhoneList;
    m_strFilePath = strFilePath;
    QFileInfo fileInfo(strFilePath);
    ui->labelFileName->setText(fileInfo.fileName());    

    //上传
    if (uploadFile(m_strFilePath, m_strPhoneList))
    {
        ui->toolBtnCancel->setVisible(false);
        ui->labelFinish->setVisible(true);
    }
}

QueueTableItem::~QueueTableItem()
{
    /* 释放网络等资源 */
    ShutdownSdk();
    delete ui;
}

void QueueTableItem::on_toolBtnCancel_clicked()
{
    ui->toolBtnCancel->setVisible(false);
    ui->labelCancelling->setVisible(true);
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
    //重新上传
    if (uploadFile(m_strFilePath, m_strPhoneList))
    {
        ui->toolBtnCancel->setVisible(false);
        ui->labelFinish->setVisible(true);
    }
}

bool QueueTableItem::uploadFile(const QString& filePath, QStringList strPhoneList)
{
    int iSize = strPhoneList.size();    
    if (iSize <= 0)
        return false;

    /* 初始化OSS账号信息 */

    std::string Endpoint = HTTP_ALIBABA_OSS_ENDPOINT;//"yourEndpoint";
    /* 填写Bucket名称，例如examplebucket */
    std::string BucketName = "yishunyun-file";
    /* 填写Object完整路径，完整路径中不能包含Bucket名称，例如exampledir/exampleobject.txt。 */
    QFileInfo fileInfo(filePath);
    std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
    std::string ServerName = HTTP_ALIBABA_OSS_CALLBACK;
    ClientConfiguration conf;
    conf.verifySSL = false;
    /* 设置连接池数，默认为16个 */
    conf.maxConnections = 20;
    /* 设置请求超时时间，超时没有收到数据就关闭连接，默认为10000ms */
    conf.requestTimeoutMs = 8000;
    /* 设置建立连接的超时时间，默认为5000ms */
    conf.connectTimeoutMs = 8000;

    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);
    //token验证
    OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(),GlobalData::strSecurityToken.toStdString(), conf);
    //OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), conf);

    InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);
    /*（可选）请参见如下示例设置存储类型 */
    //initUploadRequest.MetaData().addHeader("x-oss-storage-class", "Standard");

    /* 初始化分片上传事件 */
    InitiateMultipartUploadOutcome uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
    /* 根据UploadId执行取消分片上传事件或者列举已上传分片的操作。*/
    /* 如果您需要根据您需要UploadId执行取消分片上传事件的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后获取uploadId。*/
    /* 如果您需要根据您需要UploadId执行列举已上传分片的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后，且在调用CompleteMultipartUpload完成分片上传之前获取uploadId。*/
    const std::string uploadId = uploadIdResult.result().UploadId();
    m_UploadId = uploadId;
    std::string fileToUpload = filePath.toStdString();//"yourLocalFilename";
    int64_t partSize = 100 * 1024;
    PartList partETagList;    
    auto fileSize = fileInfo.size();
    //auto fileSize = getFileSize(fileToUpload);
    qDebug() << "fileSize=" << fileSize;
    int partCount = static_cast<int>(fileSize / partSize);
    /* 计算分片个数 */
    if (fileSize % partSize != 0) {
        partCount++;
    }

    /* 对每一个分片进行上传 */
    for (int i = 1; i <= partCount; i++) {
        auto skipBytes = partSize * (i - 1);
        auto size = (partSize < fileSize - skipBytes) ? partSize : (fileSize - skipBytes);
        std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(fileToUpload, std::ios::in | std::ios::binary);
        content->seekg(skipBytes, std::ios::beg);

        UploadPartRequest uploadPartRequest(BucketName, ObjectName, content);
        uploadPartRequest.setContentLength(size);
        uploadPartRequest.setUploadId(uploadId);
        uploadPartRequest.setPartNumber(i);
        auto uploadPartOutcome = client.UploadPart(uploadPartRequest);
        if (uploadPartOutcome.isSuccess()) {
            Part part(i, uploadPartOutcome.result().ETag());
            partETagList.push_back(part);
        }
        else {
            qDebug() << "uploadPart fail" <<
                ",code:" << uploadPartOutcome.error().Code() <<
                ",message:" << uploadPartOutcome.error().Message() <<
                ",requestId:" << uploadPartOutcome.error().RequestId();
        }

    }

    
    std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
    *content << "Thank you for using Aliyun Object Storage Service!";
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strPhoneList.at(i));
    }
    QJsonObject jsonObj;
    QJsonDocument doc(jsonObj);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
    //QString callbackBody = QString("bucket=${bucket}");//
        //.arg(0).arg(GlobalData::id).arg(GlobalData::getFileMd5(filePath)).arg(fileInfo.fileName()).arg(postData);
    //qDebug() << callbackBody;

    /*QJsonObject jsonBody;
    QJsonDocument docBody;
    jsonBody.insert("callbackUrl", HTTP_ALIBABA_OSS_CALLBACK);
    jsonBody.insert("callbackHost", "www.ysyos.com");
    jsonBody.insert("callbackBody")
    docBody.setObject(jsonBody);
    QByteArray postData = docBody.toJson(QJsonDocument::Compact);*/

    /*ObjectCallbackBuilder builder(ServerName, GlobalData::QStringToBase64(callbackBody).toStdString(), "www.ysyos.com", ObjectCallbackBuilder::Type::URL);
    std::string value = builder.build();
    //ObjectCallbackVariableBuilder varBuilder;
    //varBuilder.addCallbackVariable("x:var1", "value1");
    //std::string varValue = varBuilder.build();
    *content << "Thank you for using Aliyun Object Storage Service!";
    PutObjectRequest Putrequest(BucketName, ObjectName, content);
    request.MetaData().addHeader("x-oss-callback", value);
    request.setUploadId()*/
    //request.MetaData().addHeader("x-oss-callback-var", varValue);
    //putrequest.MetaData().addHeader("Content-Type", "application/json");
    /* 设置上传回调参数。*/
       
    /* 完成分片上传 */
    //同安卓
    QString strcallbackBody = "{\"fileMd5\":\"${md5}\",\"autoInstall\":${autoInstall},\"instanceCodes\":${map},\"createBy\":${CacheUtil.getUserInfo()?.userDetailVO?.id},\"mimeType\":\${mimeType},\"size\":\${size},\"imageInfo\":\${\"imageInfo.format\"},\"bucket\":\${bucket},\"fileName\":\${object}}";
    qDebug() <<"strcallbackBody="<< strcallbackBody;
    QString strcallbackBody2 = QString("{\"fileMd5\":\"%1\",\"autoInstall\":%2,\"instanceCodes\":\"%3\",\"createBy\":%4,\"mimeType\":\${mimeType},\"size\":\${size},\"imageInfo\":\${\"imageInfo\"},\"bucket\":\${bucket},\"fileName\":\"%5\"}")
        .arg(GlobalData::getFileMd5(filePath)).arg(0).arg(postData).arg(GlobalData::id).arg(fileInfo.fileName());
    qDebug() << "strcallbackBody2=" << strcallbackBody2;
    QString strNewcallbackBody = "bucket=${bucket}&object=${object}";
    /* 在执行完成分片上传操作时，需要提供所有有效的partETags。OSS收到提交的partETags后，会逐一验证每个分片的有效性。当所有的数据分片验证通过后，OSS将把这些分片组合成一个完整的文件。*/
    //QString strBody = "{\"callbackUrl\":\"https://www.ysyos.com/api/file/callback/instance\",\"callbackHost\":\"www.ysyos.com\",\"callbackBody\":\"bucket=${bucket}&object=${object}\",\"callbackBodyType\":\"application/x-www-form-urlencoded\"}";

    QString strBody = "{\"callbackUrl\":\"https://www.ysyos.com/api/file/callback/instance\",\"callbackHost\":\"www.ysyos.com\",\"callbackBody\":\"" + strNewcallbackBody+"\",\"callbackBodyType\":\"application/x-www-form-urlencoded\"}";
    qDebug()<<"strBody="<<strBody;
    CompleteMultipartUploadRequest putrequest(BucketName, ObjectName);
    putrequest.setUploadId(uploadId);
    putrequest.setPartList(partETagList);
    qDebug() << "strBody = " << strBody;
    putrequest.setCallback(GlobalData::QStringToBase64(strBody).toStdString());
    qDebug() << "64base strBody=" << GlobalData::QStringToBase64(strBody).toStdString();

    //auto outcome = client.PutObject(request);

    
    CompleteMultipartUploadOutcome outcome = client.CompleteMultipartUpload(putrequest);

    if (!outcome.isSuccess()) {
        /* 异常处理 */
        qDebug() << "CompleteMultipartUpload fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId();
        return false;
    }
    else
    {
        //成功结果
        qDebug() << "成功：location=" << outcome.result().Location() <<"Tag="<<outcome.result().ETag()<<"CRC64="<<outcome.result().CRC64();
    }
    return true;
}
bool QueueTableItem::cancelUploadFile(const QString& filePath, QStringList strPhoneList)
{
    /* 初始化OSS账号信息 */

    std::string Endpoint = HTTP_ALIBABA_OSS_ENDPOINT;//"yourEndpoint";
    /* 填写Bucket名称，例如examplebucket */
    std::string BucketName = "yishunyun-file";
    /* 填写Object完整路径，完整路径中不能包含Bucket名称，例如exampledir/exampleobject.txt。 */
    QFileInfo fileInfo(filePath);
    std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
    std::string ServerName = HTTP_ALIBABA_OSS_CALLBACK;
     /* 填写UploadId。UploadId来源于调用InitiateMultipartUpload完成初始化分片之后的返回结果。*/
    std::string UploadId = m_UploadId;

    ClientConfiguration conf;
    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);
    OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), GlobalData::strSecurityToken.toStdString(), conf);

    InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);

    /* 初始化分片上传事件 */
    auto uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
    auto uploadId = uploadIdResult.result().UploadId();
    qDebug() << "m_uploadId=" << m_UploadId << "   uploadId=" << UploadId;
    /* 取消分片上传 */
    AbortMultipartUploadRequest  abortUploadRequest(BucketName, ObjectName, UploadId);
    auto outcome = client.AbortMultipartUpload(abortUploadRequest);

    if (!outcome.isSuccess()) {
        /* 异常处理 */
        qDebug()<< "AbortMultipartUpload fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId();
        return false;
    }

    return true;
}

bool QueueTableItem::uploadFileCallback(QString filePath, QStringList strPhoneList, int iAutoInstall)
{
    int iSize = strPhoneList.size();
    if (iSize <= 0 || filePath.isEmpty())
        return false;
    /* 初始化OSS账号信息。*/
    std::string Endpoint = HTTP_ALIBABA_OSS_ENDPOINT;//"yourEndpoint";
    /* 填写Bucket名称，例如examplebucket */
    std::string BucketName = "yishunyun-file";
    /* 填写Object完整路径，完整路径中不能包含Bucket名称，例如exampledir/exampleobject.txt。 */
    QFileInfo fileInfo(filePath);
    std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
    std::string ServerName = HTTP_ALIBABA_OSS_CALLBACK;//"https://example.aliyundoc.com:23450";

    /* 初始化网络等资源。*/
    //InitializeSdk();

    ClientConfiguration conf;
    conf.verifySSL = false;
    OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), GlobalData::strSecurityToken.toStdString(), conf);
    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);

    std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
    *content << "Thank you for using Aliyun Object Storage Service!";

    //设置上传回调参数
    //fileMd5 获取文件md5
    //autoInstall 是否自动安装
    //instanceCodes 手机实例
    //createBy 用户id
    //std::string callbackBody = "{\"fileMd5\":\"${md5}\",\"autoInstall\":${autoInstall},\"instanceCodes\":${map},\"createBy\":${CacheUtil.getUserInfo()?.userDetailVO?.id}," + "\"mimeType\":\${mimeType},\"size\":\${size}," + "\"imageInfo\":" + "\${\"imageInfo.format\"},\"bucket\":" + "\${bucket},\"fileName\":\${object}}";
    QJsonArray listArray;
    for (int i = 0; i < iSize; i++)
    {
        listArray.append(strPhoneList.at(i));
    }

    QJsonObject jsonObj;
    QJsonDocument doc(jsonObj);
    doc.setArray(listArray);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << postData;
    QString callbackBody = QString("autoInstall=%1&createBy=%2&fileMd5=%3&fileName=%4&instanceCodes=%5&bucket=${bucket}&object=${object}&size=${size}&mimeType=${mimeType}&size=${size}&imageInfo=${imageInfo}&bucket=${bucket}&etag=${etag}&my_var1=${x:var1}")
        .arg(0).arg(GlobalData::id).arg(GlobalData::getFileMd5(filePath)).arg(fileInfo.fileName()).arg(postData);
    qDebug() << callbackBody;
    ObjectCallbackBuilder builder(ServerName, GlobalData::QStringToBase64(callbackBody).toStdString(), "www.ysyos.com", ObjectCallbackBuilder::Type::JSON);
    std::string value = builder.build();
    ObjectCallbackVariableBuilder varBuilder;
    varBuilder.addCallbackVariable("x:var1", "value1");
    std::string varValue = varBuilder.build();
    PutObjectRequest request(BucketName, ObjectName, content);
    request.MetaData().addHeader("x-oss-callback", value);
    request.MetaData().addHeader("x-oss-callback-var", varValue);
    qDebug() << "default=" << request.MetaData().ContentType();
    //request.MetaData().setContentType("application/json");
    request.MetaData().setContentType("application/x-www-form-urlencoded");

    //request.MetaData().setContentType("text/plain");
    qDebug() << "default=" << request.MetaData().ContentType() << GlobalData::getContentType(filePath);

    //request.MetaData().setContentType("application/json");//("multipart/form-data");
    auto outcome = client.PutObject(request);
    if (!outcome.isSuccess()) {
        /* 异常处理 */
        qDebug() << "PutObjectRequest fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId();
        return false;
    }
    return true;
    /* 释放网络等资源。*/
    //ShutdownSdk();
}
