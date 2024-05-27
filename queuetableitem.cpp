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

    m_strFilePath = strFilePath;
    QFileInfo fileInfo(strFilePath);
    ui->labelFileName->setText(fileInfo.fileName());

    uploadFile(strFilePath,strPhoneList);
}

QueueTableItem::~QueueTableItem()
{
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

void QueueTableItem::uploadFile(const QString& filePath, QStringList strPhoneList)
{
    int iSize = strPhoneList.size();
    if (iSize <= 0)
        return;

    //QString filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr("Images (*.png *.jpg *.jpeg)"));
    if (!filePath.isEmpty()) {
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
    }
}
/*void QueueTableItem::uploadFile(const QString& filePath, QStringList strPhoneList)
{
    int iSize = strPhoneList.size();
    if (iSize <= 0)
        return;

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_UPLOAD_FILE_TO_INSTANCE;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    //request.setRawHeader("Authorization", m_userInfo.strMobile.toUtf8());
    //request.setUrl(url);

    QJsonObject jsonObj;
    jsonObj["autoInstall"] = 0;
    jsonObj["customizeFilePath"] = "";
    jsonObj["fileMd5"] = "";
    jsonObj["fileName"] = "";
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
    //发出GET请求
    //QNetworkReply* reply = manager->post(request, postData);


    // 添加文件部分
    QHttpPart filePart;
    QFile file(filePath);
    //filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, postData);
    filePart.setBodyDevice(&file);
    multiPart->append(filePart);
    request.setUrl(url);
    //QNetworkRequest request(url);
    QNetworkReply* reply = manager->post(request, multiPart);

    QObject::connect(reply, &QNetworkReply::finished,this, [multiPart, reply]() 
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
        });
}*/