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

    //QString filePath = "/path/to/your/file";
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

void QueueTableItem::uploadFile(const QString& filePath,QStringList strPhoneList)
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
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(filePath).fileName() + "\""));
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
}