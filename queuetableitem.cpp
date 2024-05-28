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

    m_strFilePath = strFilePath;
    QFileInfo fileInfo(strFilePath);
    ui->labelFileName->setText(fileInfo.fileName());

    if (uploadFile(strFilePath, strPhoneList))
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

    ClientConfiguration conf;
    /* 设置连接池数，默认为16个 */
    conf.maxConnections = 20;
    /* 设置请求超时时间，超时没有收到数据就关闭连接，默认为10000ms */
    conf.requestTimeoutMs = 8000;
    /* 设置建立连接的超时时间，默认为5000ms */
    conf.connectTimeoutMs = 8000;

    /* 从环境变量中获取访问凭证。运行本代码示例之前，请确保已设置环境变量OSS_ACCESS_KEY_ID和OSS_ACCESS_KEY_SECRET。*/
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);
    
    //auto credentialsProvider = std::make_shared<EnvironmentVariableCredentialsProvider>();
    //OssClient client(Endpoint, credentialsProvider, conf);
    OssClient client(Endpoint, GlobalData::strAccessKeyId.toStdString(), GlobalData::strAccessKeySecret.toStdString(),GlobalData::strSecurityToken.toStdString(), conf);

    InitiateMultipartUploadRequest initUploadRequest(BucketName, ObjectName);
    /*（可选）请参见如下示例设置存储类型 */
    //initUploadRequest.MetaData().addHeader("x-oss-storage-class", "Standard");

    /* 初始化分片上传事件 */
    InitiateMultipartUploadOutcome uploadIdResult = client.InitiateMultipartUpload(initUploadRequest);
    /* 根据UploadId执行取消分片上传事件或者列举已上传分片的操作。*/
    /* 如果您需要根据您需要UploadId执行取消分片上传事件的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后获取uploadId。*/
    /* 如果您需要根据您需要UploadId执行列举已上传分片的操作，您需要在调用InitiateMultipartUpload完成初始化分片之后，且在调用CompleteMultipartUpload完成分片上传之前获取uploadId。*/
    const std::string uploadId = uploadIdResult.result().UploadId();
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

    /* 完成分片上传 */
    /* 在执行完成分片上传操作时，需要提供所有有效的partETags。OSS收到提交的partETags后，会逐一验证每个分片的有效性。当所有的数据分片验证通过后，OSS将把这些分片组合成一个完整的文件。*/
    CompleteMultipartUploadRequest request(BucketName, ObjectName);
    request.setUploadId(uploadId);
    request.setPartList(partETagList);
    /*（可选）请参见如下示例设置读写权限ACL */
    //request.setAcl(CannedAccessControlList::Private);

    CompleteMultipartUploadOutcome outcome = client.CompleteMultipartUpload(request);

    if (!outcome.isSuccess()) {
        /* 异常处理 */
        qDebug() << "CompleteMultipartUpload fail" <<
            ",code:" << outcome.error().Code() <<
            ",message:" << outcome.error().Message() <<
            ",requestId:" << outcome.error().RequestId();
        return false;
    }    
    return true;

    //QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg)"));
    /*if (!filePath.isEmpty()) {
        //localimage = filePath;
        //mLoadingDialog1->show();
        QNetworkAccessManager* manager = new QNetworkAccessManager(this);

        //QUrl url(strApiRomain + "api/oss/upload");
        QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
        strUrl += HTTP_UPLOAD_FILE_TO_INSTANCE;
        QUrl url(strUrl);

        QFile* file = new QFile(filePath); // 创建一个QFile对象，用于读取文件内容
        if (file->open(QIODevice::ReadOnly)) { // 打开文件，只读模式
            QHttpMultiPart* multiPart = new QHttpMultiPart(this); // 创建QHttpMultiPart对象
            QHttpPart filePart; // 创建QHttpPart对象来包含文件内容
            //filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("multipart/form-data"));
            filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
            QFileInfo info(filePath);
            QString xlname = info.fileName();
            QJsonObject jsonObj;
            jsonObj["autoInstall"] = 0;
            jsonObj["customizeFilePath"] = "";
            jsonObj["fileMd5"] = "";
            jsonObj["fileName"] = xlname;
            jsonObj["fileUrl"] = filePath;
            QJsonArray listArray;
            for (int i = 0; i < iSize; i++)
            {
                listArray.append(strPhoneList.at(i));
            }
            //doc.setObject(listArray);
            jsonObj["instanceCodes"] = listArray;
            //doc.setArray(listArray);
            QJsonDocument doc(jsonObj);
            QByteArray postData = doc.toJson(QJsonDocument::Compact);
            qDebug() << postData;
            //filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + xlname + "\""));;
            filePart.setHeader(QNetworkRequest::ContentDispositionHeader, postData);
            // 使用QFile::readAll来读取文件内容，并将其设置为HTTP部分的内容
//            QByteArray fileData = file->readAll(); // 使用适当的MIME类型
//            filePart.setBody(fileData); // 使用适当的MIME类型
//            file->setParent(multiPart); // 设置文件的父对象为multiPart，确保文件在multiPart删除时被关闭
//            multiPart->append(filePart); // 将文件部分添加到multiPart中


            file->open(QIODevice::ReadOnly);
            filePart.setBodyDevice(file);
            file->setParent(multiPart);
            multiPart->append(filePart);


            QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
            QNetworkRequest request(url); // 创建网络请求
            //request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data"); // 设置content-type为multipart/form-data
            request.setRawHeader("Authorization", strToken.toLocal8Bit()); // 设置token头
            qDebug() << "token="<< strToken;
            QNetworkReply* reply = manager->post(request, multiPart); // 发送包含multiPart的请求
            //imageurl = "";
            //imgKey = "";
            connect(reply, &QNetworkReply::finished, this, [multiPart, reply]()
                {
                    if (reply->error() == QNetworkReply::NoError) {
                        // 处理响应数据
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
                                    //HttpQueryAllGroup();
                                }
                                else
                                {
                                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage);
                                    tips->show();
                                }
                            }
                        }
                    }
                    else {
                        // 处理错误
                        qDebug() << "Error:" << reply->errorString();
                    }

                    multiPart->deleteLater();
                    reply->deleteLater();
                }); // 连接reply的finished信号到你的槽函数
        }
        else {
            // 处理文件打开失败的情况，例如显示错误消息等
            qDebug() << "Failed to open file:" << filePath;
            delete file; // 确保在出错时删除file对象，防止内存泄漏
        }
    }*/
}