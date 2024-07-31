#include "queuetableitem.h"
#include "ui_queuetableitem.h"
#include <QFileInfo>
#include "global.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetipsdialog.h"
#include <alibabacloud/oss/OssClient.h>
#include <fstream>
#include "tuploadfilethread.h"
#include <QThread>

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

    ui->progressBar->setMaximum(fileInfo.size());
    ui->progressBar->setValue(0);
    ui->labelProgressStatus->setText("等待中");

    qDebug() << "QueueTableItem所在线程 " << QThread::currentThreadId();
    m_Timer = new QTimer();
    m_Timer->start(3000);
    connect(m_Timer, &QTimer::timeout, this, [this]()
        {
            m_Timer->stop();
            TUploadFileThread* uploadThread = new TUploadFileThread(this);
            connect(uploadThread, &TUploadFileThread::started, this, []() {
                qDebug() << "开始下载";
                });
            connect(uploadThread, &TUploadFileThread::finished, this, []() {
                qDebug() << "上传完成";
                });
            /*connect(uploadThread, &TUploadFileThread::finished, this, [this](bool success, QString errorMessage)
                {
                    if (success)
                    {
                        QString url = m_levelInfo.strColorIcon;
                        QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
                        QString strTmp = GlobalData::strPictureTempDir + strFileName;
                        qDebug() << "url=" << url << "strTmp=" << strTmp;
                        //ui->labelVersion->setPixmap(QPixmap(strTmp));
                        //startDownload(url);
                        int width = ui->labelVersion->width();
                        int height = ui->labelVersion->height();
                        QFile file(strTmp);
                        if (file.exists())
                        {
                            ui->labelVersion->setPixmap(QPixmap(strTmp).scaled(QSize(width, height), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        }
                    }
                    else
                    {
                        qDebug() << "download=" << m_levelInfo.strColorIcon << "errorMessage = " << errorMessage;
                    }

                });*/
            uploadThread->setUploadFileParam(m_strFilePath, m_strPhoneList, 1, ui->progressBar);
            uploadThread->start();
            //上传
            /*if (uploadFile(m_strFilePath, m_strPhoneList))
            {
                ui->toolBtnCancel->setVisible(false);
                ui->toolBtnFinish->setVisible(true);
            }*/
        });
    
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

    TUploadFileThread* uploadThread = new TUploadFileThread(this);
    connect(uploadThread, &TUploadFileThread::started, this, []() {
        qDebug() << "开始下载";
        });
    connect(uploadThread, &TUploadFileThread::finished, this, []() {
        qDebug() << "上传完成";
        });
    /*connect(uploadThread, &TUploadFileThread::finished, this, [this](bool success, QString errorMessage)
        {
            if (success)
            {
                QString url = m_levelInfo.strColorIcon;
                QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
                QString strTmp = GlobalData::strPictureTempDir + strFileName;
                qDebug() << "url=" << url << "strTmp=" << strTmp;
                //ui->labelVersion->setPixmap(QPixmap(strTmp));
                //startDownload(url);
                int width = ui->labelVersion->width();
                int height = ui->labelVersion->height();
                QFile file(strTmp);
                if (file.exists())
                {
                    ui->labelVersion->setPixmap(QPixmap(strTmp).scaled(QSize(width, height), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }
            }
            else
            {
                qDebug() << "download=" << m_levelInfo.strColorIcon << "errorMessage = " << errorMessage;
            }

        });*/
    uploadThread->setUploadFileParam(m_strFilePath, m_strPhoneList, 1, ui->progressBar);
    uploadThread->start();
    //重新上传
    /*if (uploadFile(m_strFilePath, m_strPhoneList))
    {
        ui->toolBtnCancel->setVisible(false);
        ui->toolBtnFinish->setVisible(true);
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
