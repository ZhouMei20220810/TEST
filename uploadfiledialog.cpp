#include "uploadfiledialog.h"
#include "ui_uploadfiledialog.h"
#include <QFileDialog>
#include "uploadfileitemwidget.h"
#include "global.h"
#include <QProgressBar>
#include "queuetableitem.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "messagetipsdialog.h"
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>

UploadFileDialog::UploadFileDialog(QStringList strList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UploadFileDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //透明背景
    //setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_strPhoneList = strList;

    ui->toolBtnUpload->setDisabled(true);

    ui->stackedWidget->setCurrentWidget(ui->page);
    InitWidget(ui->listWidgetChooseFile);

    GetOSSInfo();
}

UploadFileDialog::~UploadFileDialog()
{
    delete ui;
}

void UploadFileDialog::InitWidget(QListWidget* listWidget)
{
    listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    listWidget->setSpacing(ITEM_WIDGET_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    listWidget->setMovement(QListWidget::Static);
    //设置单选
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void UploadFileDialog::on_btnClose_clicked()
{
    this->close();
}


void UploadFileDialog::on_toolBtnAddUploadFile_clicked()
{
    ui->toolBtnAddUploadFile->setStyleSheet("QToolButton {border:none;color:#FF505465;	background-color:#FFE6E9F2;border-radius:1px;padding-left:8px;}QToolButton:hover {color: #FF505465;;background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadQueue->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    if(ui->listWidgetChooseFile->count() > 0)
        ui->stackedWidget->setCurrentWidget(ui->pageChooseFile);
    else
        ui->stackedWidget->setCurrentWidget(ui->page);
}


void UploadFileDialog::on_toolBtnUploadQueue_clicked()
{
    ui->toolBtnUploadQueue->setStyleSheet("QToolButton {border:none;color:#FF505465;	background-color:#FFE6E9F2;border-radius:1px;padding-left:8px;}QToolButton:hover {color: #FF505465;;background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnAddUploadFile->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageQueue);
}


void UploadFileDialog::on_toolBtnUploadHistory_clicked()
{
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton {border:none;color:#FF505465;	background-color:#FFE6E9F2;border-radius:1px;padding-left:8px;}QToolButton:hover {color: #FF505465;;background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnAddUploadFile->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageHistory);
}


void UploadFileDialog::on_toolBtnSelectFile_2_clicked()
{
    SelectFile();
}


void UploadFileDialog::on_toolBtnUpload_2_clicked()
{
    uploadFile();
}


void UploadFileDialog::on_toolBtnSelectFile_clicked()
{
    SelectFile();
}


void UploadFileDialog::on_toolBtnUpload_clicked()
{
    uploadFile();
}

void UploadFileDialog::SelectFile()
{
    QString strCurPath = QDir::currentPath();
    QStringList strFileList = QFileDialog::getOpenFileNames(this,"选择文件",strCurPath,"请选择上传文件(*.*)");
    int iFileSize = strFileList.size();
    if(iFileSize > 0)
    {
        QListWidgetItem* item = NULL;
        UploadFileItemWidget* fileItem = NULL;
        for(int i = 0; i < iFileSize; i++)
        {
            qDebug()<<"第"<<i<<"个："<<strFileList.at(i);
            item = new QListWidgetItem(ui->listWidgetChooseFile);

            fileItem = new UploadFileItemWidget(strFileList.at(i),this);
            connect(fileItem, &UploadFileItemWidget::deleteFileItemSignal, this, &UploadFileDialog::do_deleteFileItemSignal);
            item->setData(Qt::UserRole, strFileList.at(i));
            item->setSizeHint(QSize(fileItem->size()));
            ui->listWidgetChooseFile->addItem(item);
            ui->listWidgetChooseFile->setItemWidget(item, fileItem);
        }
        ui->stackedWidget->setCurrentWidget(ui->pageChooseFile);
    }
    else
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}

void UploadFileDialog::uploadFile()
{
    ui->stackedWidget->setCurrentWidget(ui->pageQueue);
    //初始化列表
    ui->listWidgetQueue->setViewMode(QListWidget::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetQueue->setSpacing(5);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetQueue->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetQueue->setMovement(QListWidget::Static);

    //获取上传列表
    int iUploadCount = ui->listWidgetChooseFile->count();
    if (iUploadCount <= 0)
        return;
    QListWidgetItem* item = NULL;
    QListWidgetItem* queueItem = NULL;
    QString strFilePath;
    QueueTableItem* widget = NULL;
    for (int i = 0; i < iUploadCount; i++)
    {
        item = ui->listWidgetChooseFile->item(i);
        if (item != NULL)
        {
            strFilePath = item->data(Qt::UserRole).toString();
            queueItem = new QListWidgetItem(ui->listWidgetQueue);
            queueItem->setData(Qt::UserRole, strFilePath);
            queueItem->setSizeHint(QSize(QUEUE_ITEM_WIDTH, QUEUE_ITEM_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
            //tableitem* widget = new tableitem(dataObj,this);
            widget = new QueueTableItem(m_strPhoneList,strFilePath,this);
            ui->listWidgetQueue->addItem(queueItem);
            ui->listWidgetQueue->setItemWidget(queueItem, widget);
        }
    }
}

void UploadFileDialog::do_deleteFileItemSignal(QString strFilePath)
{
    qDebug() << "delete " << strFilePath;
    int iCount = ui->listWidgetChooseFile->count();
    QListWidgetItem* item = NULL;
    QString strFile;
    for (int i = 0; i < iCount; i++)
    {
        item = ui->listWidgetChooseFile->item(i);
        if (item != NULL)
        {
            strFile = item->data(Qt::UserRole).toString();
            if (strFile.compare(strFilePath) == 0)
            {
                qDebug() << "list remove item" << strFile;
                ui->listWidgetChooseFile->takeItem(i);
                //ui->listWidgetChooseFile->removeItemWidget(item);
                break;
            }            
        }
    }

    iCount = ui->listWidgetChooseFile->count();
    if(iCount <= 0)
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}

void UploadFileDialog::GetOSSInfo()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_SERVER_OSS_INFO;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
    request.setUrl(url);
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
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    if (obj["data"].isObject())
                    {
                        QJsonObject dataObj = obj["data"].toObject();
                        GlobalData::strSecurityToken = dataObj["securityToken"].toString();
                        GlobalData::strAccessKeySecret = dataObj["accessKeySecret"].toString();
                        GlobalData::strAccessKeyId = dataObj["accessKeyId"].toString();
                        GlobalData::strExpiration = dataObj["expiration"].toString();
                    }
                    qDebug() << "Code=" << iCode << "message=" << strMessage;
                    qDebug() << "securityToken=" << GlobalData::strSecurityToken;
                    qDebug()<< "strAccessKeySecret=" << GlobalData::strAccessKeySecret;
                    qDebug() << "strAccessKeyId=" << GlobalData::strAccessKeyId;
                    qDebug() << "strExpiration=" << GlobalData::strExpiration;
                }
                else
                {
                    MessageTipsDialog* tips = new MessageTipsDialog(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}
