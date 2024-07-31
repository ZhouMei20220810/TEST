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

    // increment��ʾ���λص����͵����ݴ�С��
    // transfered��ʾ���ϴ������ݴ�С��
    // total��ʾ�ϴ��ļ����ܴ�С��
    /*if (userData != NULL)
    {
        //��userDataת��Ϊָ��QPrograssBar��ָ��
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

    qDebug() << "TUploadFileThread�����߳� " << QThread::currentThreadId();
    /*if (m_IsAbort)
    {
        ui->progressBar->setValue(50);
        ui->labelProgressStatus->setText("��ȡ��");
        return;
    }
    else
    {
        ui->progressBar->setValue(50);
        ui->labelProgressStatus->setText("�ϴ���");
    }*/
    emit refreshUISignals(10);

    do
    {
        QFileInfo fileInfo(m_filePath);
        std::string ObjectName = fileInfo.fileName().toStdString();//"exampledir/exampleobject.txt";
        ClientConfiguration conf;
        conf.verifySSL = false;
        /* �������ӳ�����Ĭ��Ϊ16�� */
        conf.maxConnections = 20;
        /* ��������ʱʱ�䣬��ʱû���յ����ݾ͹ر����ӣ�Ĭ��Ϊ10000ms */
        conf.requestTimeoutMs = 8000;
        /* ���ý������ӵĳ�ʱʱ�䣬Ĭ��Ϊ5000ms */
        conf.connectTimeoutMs = 8000;

        /* �ӻ��������л�ȡ����ƾ֤�����б�����ʾ��֮ǰ����ȷ�������û�������OSS_ACCESS_KEY_ID��OSS_ACCESS_KEY_SECRET��*/
        //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
        //OssClient client(Endpoint, credentialsProvider, conf);
        //token��֤
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
        /*����ѡ����μ�����ʾ�����ô洢���� */
        //initUploadRequest.MetaData().addHeader("x-oss-storage-class", "Standard");

        /* ��ʼ����Ƭ�ϴ��¼� */
        InitiateMultipartUploadOutcome uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
        emit refreshUISignals(20);
        /*if (m_IsAbort)
        {
            break;
        }*/
        /* ����UploadIdִ��ȡ����Ƭ�ϴ��¼������о����ϴ���Ƭ�Ĳ�����*/
        /* �������Ҫ��������ҪUploadIdִ��ȡ����Ƭ�ϴ��¼��Ĳ���������Ҫ�ڵ���InitiateMultipartUpload��ɳ�ʼ����Ƭ֮���ȡuploadId��*/
        /* �������Ҫ��������ҪUploadIdִ���о����ϴ���Ƭ�Ĳ���������Ҫ�ڵ���InitiateMultipartUpload��ɳ�ʼ����Ƭ֮�����ڵ���CompleteMultipartUpload��ɷ�Ƭ�ϴ�֮ǰ��ȡuploadId��*/
        const std::string uploadId = uploadIdResult.result().UploadId();
        m_UploadId = uploadId;
        std::string fileToUpload = m_filePath.toLocal8Bit().constData();//filePath.toStdString();//"yourLocalFilename";
        int64_t partSize = 100 * 1024;
        PartList partETagList;
        auto fileSize = fileInfo.size();
        //auto fileSize = getFileSize(fileToUpload);
        qDebug() << "fileSize=" << fileSize;
        int partCount = static_cast<int>(fileSize / partSize);
        /* �����Ƭ���� */
        if (fileSize % partSize != 0) {
            partCount++;
        }

        /* ��ÿһ����Ƭ�����ϴ� */
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

        /* ��ɷ�Ƭ�ϴ� */
        /* ��ִ����ɷ�Ƭ�ϴ�����ʱ����Ҫ�ṩ������Ч��partETags��OSS�յ��ύ��partETags�󣬻���һ��֤ÿ����Ƭ����Ч�ԡ������е����ݷ�Ƭ��֤ͨ����OSS������Щ��Ƭ��ϳ�һ���������ļ���*/
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
        //autoInstall=1�Զ���װ
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
            /* �쳣���� */
            qDebug() << "CompleteMultipartUpload fail" <<
                ",code:" << outcome.error().Code() <<
                ",message:" << outcome.error().Message() <<
                ",requestId:" << outcome.error().RequestId();
            emit refreshUISignals(80);
        }
        else
        {
            //�ɹ����
            qDebug() << "�ɹ���location=" << outcome.result().Location() << "Tag=" << outcome.result().ETag() << "CRC64=" << outcome.result().CRC64();
        }
        emit refreshUISignals(100);
    } while (false);
}
