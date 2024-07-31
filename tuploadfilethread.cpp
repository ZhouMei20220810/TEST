#include "tuploadfilethread.h"
#include <QFileInfo>
#include "global.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <fstream>
#include <alibabacloud/oss/OssClient.h>
using namespace AlibabaCloud::OSS;

TUploadFileThread::TUploadFileThread(QObject *parent)
    : QThread{parent}
{}

void TUploadFileThread::setUploadFileParam(const QString& filePath, QStringList strPhoneList, int iIsAutoInstall)
{
    m_filePath = filePath;
    m_strPhoneList = strPhoneList;
    m_iIsAutoInstall = iIsAutoInstall;
    
    QString strPhone="";
    int iSize = strPhoneList.size();
    for (int i = 0; i < iSize; i++)
    {
        if (!strPhone.isEmpty())
            strPhone += ",";
        strPhone += strPhoneList.at(i);
    }
    qDebug() << "m_filePath=" << m_filePath <<"m_strPhoneList="<< strPhone << "m_iIsAutoInstall=" << m_iIsAutoInstall;
}

void ProgressCallback(size_t increment, int64_t transfered, int64_t total, void* userData)
{
    qDebug() << "ProgressCallback[" << userData << "] => " << increment << " ," << transfered << "," << total;

    // increment表示本次回调发送的数据大小。
    // transfered表示已上传的数据大小。
    // total表示上传文件的总大小。
    /*if (userData != NULL)
    {
        //将userData转换为指向QPrograssBar的指针
        QueueTableItem* widget = static_cast<QueueTableItem*>(userData);
        if(widget!= NULL)
            widget->RefreshPrograssBarValue(transfered, total);
    }*/
}

void TUploadFileThread::run()
{
    int iSize = m_strPhoneList.size();
    if (iSize <= 0)
        return;

    qDebug() << "TUploadFileThread所在线程 " << QThread::currentThreadId();
    /*if (m_IsAbort)
    {
        ui->progressBar->setValue(50);
        ui->labelProgressStatus->setText("已取消");
        return;
    }
    else
    {
        ui->progressBar->setValue(50);
        ui->labelProgressStatus->setText("上传中");
    }*/
    emit refreshUISignals(10);

    do
    {
        QFileInfo fileInfo(m_filePath);
        std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
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
        /*if (m_IsAbort)
        {
            break;
        }*/
        OssClient client(GlobalData::Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), GlobalData::strSecurityToken.toStdString(), conf);
        //OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(), conf);

        InitiateMultipartUploadRequest initUploadRequest(GlobalData::BucketName, ObjectName);
        /*if (m_IsAbort)
        {
            break;
        }*/
        /*（可选）请参见如下示例设置存储类型 */
        //initUploadRequest.MetaData().addHeader("x-oss-storage-class", "Standard");

        /* 初始化分片上传事件 */
        InitiateMultipartUploadOutcome uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
        emit refreshUISignals(20);
        /*if (m_IsAbort)
        {
            break;
        }*/
        /* 根据UploadId执行取消分片上传事件或者列举已上传分片的操作。*/
        /* 如果您需要根据您需要UploadId执行取消分片上传事件的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后获取uploadId。*/
        /* 如果您需要根据您需要UploadId执行列举已上传分片的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后，且在调用CompleteMultipartUpload完成分片上传之前获取uploadId。*/
        const std::string uploadId = uploadIdResult.result().UploadId();
        m_UploadId = uploadId;
        std::string fileToUpload = m_filePath.toLocal8Bit().constData();//filePath.toStdString();//"yourLocalFilename";
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
            /*if (m_IsAbort)
            {
                break;
            }*/
            auto skipBytes = partSize * (i - 1);
            auto size = (partSize < fileSize - skipBytes) ? partSize : (fileSize - skipBytes);
            std::shared_ptr<std::iostream> content = std::make_shared<std::fstream>(fileToUpload, std::ios::in | std::ios::binary);
            content->seekg(skipBytes, std::ios::beg);

            UploadPartRequest uploadPartRequest(GlobalData::BucketName, ObjectName, content);
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
        emit refreshUISignals(50);

        /* 完成分片上传 */
        /* 在执行完成分片上传操作时，需要提供所有有效的partETags。OSS收到提交的partETags后，会逐一验证每个分片的有效性。当所有的数据分片验证通过后，OSS将把这些分片组合成一个完整的文件。*/
        /*if (m_IsAbort)
        {
            break;
        }*/
        CompleteMultipartUploadRequest request(GlobalData::BucketName, ObjectName);
        /*if (m_IsAbort)
        {
            break;
        }*/
        request.setUploadId(uploadId);
        request.setPartList(partETagList);
        TransferProgress progressCallback = { ProgressCallback , nullptr};
        request.setTransferProgress(progressCallback);
        std::shared_ptr<std::iostream> content = std::make_shared<std::stringstream>();
        *content << "Thank you for using Aliyun Object Storage Service!";
        QJsonArray listArray;
        for (int i = 0; i < iSize; i++)
        {
            listArray.append(m_strPhoneList.at(i));
        }

        QJsonObject jsonObj;
        QJsonDocument doc(jsonObj);
        doc.setArray(listArray);
        QByteArray postData = doc.toJson(QJsonDocument::Compact);
        //autoInstall=1自动安装
        QString strJson = QString("{\"fileMd5\":\"%1\",\"autoInstall\":%2,\"instanceCodes\":%3,\"createBy\":%4,\"fileName\":\"%5\",\"mimeType\":\"%6\",\"size\":%7,\"bucket\":\"yishunyun-file\",\"imageInfo\":\"%8\"}")
            .arg(GlobalData::getFileMd5(m_filePath)).arg(m_iIsAutoInstall).arg(postData).arg(GlobalData::id).arg(fileInfo.fileName()).arg(GlobalData::getContentType(m_filePath)).arg(fileInfo.size()).arg(fileInfo.absoluteFilePath());
        qDebug() << "strJson=" << strJson;
        QJsonDocument DocCallBody;
        QJsonObject obj;
        obj.insert("callbackUrl", "https://www.ysyos.com/api/file/callback/instance");
        obj.insert("callbackHost", "www.ysyos.com");
        obj.insert("callbackBodyType", "application/json");
        obj.insert("callbackBody", strJson);
        DocCallBody.setObject(obj);
        QString postDataCallBody = DocCallBody.toJson(QJsonDocument::Compact);
        qDebug() << "postDataCallBody=" << postDataCallBody;
        QString strBase64 = GlobalData::QStringToBase64(postDataCallBody);
        request.MetaData().addHeader("x-oss-callback", strBase64.toStdString());
        /*if (m_IsAbort)
        {
            break;
        }*/
        emit refreshUISignals(75);
        CompleteMultipartUploadOutcome outcome = client.CompleteMultipartUpload(request);
        /*if (m_IsAbort)
        {
            break;
        }*/
        if (!outcome.isSuccess()) {
            /* 异常处理 */
            qDebug() << "CompleteMultipartUpload fail" <<
                ",code:" << outcome.error().Code() <<
                ",message:" << outcome.error().Message() <<
                ",requestId:" << outcome.error().RequestId();
            emit refreshUISignals(80);
        }
        else
        {
            //成功结果
            qDebug() << "成功：location=" << outcome.result().Location() << "Tag=" << outcome.result().ETag() << "CRC64=" << outcome.result().CRC64();
        }
        emit refreshUISignals(100);
    } while (false);
}
