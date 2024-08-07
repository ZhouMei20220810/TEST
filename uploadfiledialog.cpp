#include "uploadfiledialog.h"
#include "ui_uploadfiledialog.h"
#include <QFileDialog>
#include "uploadfileitemwidget.h"
#include "global.h"
#include <QProgressBar>
#include "queuetableitem.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "messagetips.h"
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsDropShadowEffect>
#include "uploadfilehistoryitem.h"

UploadFileDialog::UploadFileDialog(QStringList strList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UploadFileDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    //透明背景
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowTitle("上传文件");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    m_strPhoneList = strList;

    ui->toolBtnUpload->setDisabled(true);

    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->listWidgetChooseFile->setVisible(false);
    InitWidget(ui->listWidgetChooseFile);

    //初始化列表
    ui->listWidgetQueue->setViewMode(QListWidget::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetQueue->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetQueue->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetQueue->setMovement(QListWidget::Static);

    //初始化列表
    ui->listWidgetHistory->setViewMode(QListWidget::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidgetHistory->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidgetHistory->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidgetHistory->setMovement(QListWidget::Static);

    m_LabelPoint = new QLabel(ui->frame_2);
    m_LabelPoint->resize(10, 10);
    m_LabelPoint->setPixmap(QPixmap(":/resource/upload/point.png").scaled(QSize(m_LabelPoint->width(), m_LabelPoint->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

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
    ui->stackedWidget->setCurrentWidget(ui->page);
    if (ui->listWidgetChooseFile->count() > 0)
    {
        ui->listWidgetChooseFile->setVisible(true);
        ui->frameNoData->setVisible(false);
    }
    else
    {
        ui->listWidgetChooseFile->setVisible(false);
        ui->frameNoData->setVisible(true);
    }
    QRect rect = ui->toolBtnUploadQueue->geometry();
    m_LabelPoint->move(rect.x() + rect.width() - 5, rect.y() - 5);
}


void UploadFileDialog::on_toolBtnUploadQueue_clicked()
{
    ui->toolBtnUploadQueue->setStyleSheet("QToolButton {border:none;color:#FF505465;	background-color:#FFE6E9F2;border-radius:1px;padding-left:8px;}QToolButton:hover {color: #FF505465;;background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnAddUploadFile->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageQueue);
    QRect rect = ui->toolBtnUploadHistory->geometry();
    m_LabelPoint->move(rect.x()+rect.width()-5, rect.y()-5);
}


void UploadFileDialog::on_toolBtnUploadHistory_clicked()
{
    ui->toolBtnUploadHistory->setStyleSheet("QToolButton {border:none;color:#FF505465;	background-color:#FFE6E9F2;border-radius:1px;padding-left:8px;}QToolButton:hover {color: #FF505465;;background-color: #FFE7E8EE;border-radius:1px;padding-left:8px;}");
    ui->toolBtnAddUploadFile->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");
    ui->toolBtnUploadQueue->setStyleSheet("QToolButton{background-color:#FfF4F6FA;border: 1px solid #FFE6E9F2;color: #FF505465;border-radius:1px;padding-left:8px;}QToolButton:hover {background-color: #FFE7E8EE;color: #FF505465;border-radius:1px;padding-left:8px;}");

    ui->stackedWidget->setCurrentWidget(ui->pageHistory);
    QRect rect = ui->toolBtnUploadQueue->geometry();
    m_LabelPoint->move(rect.x() + rect.width()-5, rect.y()-5);

    //获取文件历史记录
    HttpGetUploadFileHistory(1,1000);
}

//获取上传文件历史记录
void UploadFileDialog::HttpGetUploadFileHistory(int iPage, int iPageSize)
{
    //已授权列表
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_UPLOAD_FILE_HISTORY;
    //level不传值,返回该 组下面所有的level
    strUrl += QString::asprintf("?page=%d&pageSize=%d", iPage, iPageSize);
    qDebug() << "strUrl = " << strUrl;
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
    QNetworkReply* reply = manager->get(request);//manager->post(request, "");
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
                        int iCurrent = data["current"].toInt();
                        int iPages = data["pages"].toInt();
                        int iSize = data["size"].toInt();
                        int iSearchCount = data["searchCount"].toInt();
                        int iTotal = data["total"].toInt();
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();
                        QMap<int, S_UPLOADD_FILE_INFO> map;
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_UPLOADD_FILE_INFO uploadFileInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                uploadFileInfo.strBucket = recordObj["bucket"].toString();
                                uploadFileInfo.iCreateBy = recordObj["createBy"].toInt();
                                uploadFileInfo.id = recordObj["id"].toInt();
                                uploadFileInfo.strCreateTime = recordObj["createTime"].toString();
                                uploadFileInfo.strFileMd5 = recordObj["fileMd5"].toString();
                                uploadFileInfo.strFileName = recordObj["fileName"].toString();
                                uploadFileInfo.iStatus = recordObj["status"].toInt();
                                uploadFileInfo.iSize = recordObj["size"].toInt();
                                map.insert(i, uploadFileInfo);
                            }
                        }
                        LoadUploadFileHistory(map);
                    }
                }
                else
                {
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

//显示数据
void UploadFileDialog::LoadUploadFileHistory(QMap<int, S_UPLOADD_FILE_INFO> map)
{
    ui->listWidgetHistory->clear();
    if (map.size() <= 0)
        return;
    QListWidgetItem* item = NULL;
    QString strFilePath;
    UploadFileHistoryItem* widget = NULL;
    QMap<int, S_UPLOADD_FILE_INFO>::iterator iter = map.begin();
    for (; iter != map.end(); iter++)
    {
        item = new QListWidgetItem(ui->listWidgetHistory);
        item->setSizeHint(QSize(QUEUE_ITEM_WIDTH, QUEUE_ITEM_HEIGHT));	// 这里QSize第一个参数是宽度，无所谓值多少，只有高度可以影响显示效果
        widget = new UploadFileHistoryItem(*iter, this);
        ui->listWidgetHistory->addItem(item);
        ui->listWidgetHistory->setItemWidget(item, widget);
    }
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
        ui->listWidgetChooseFile->setVisible(true);
        ui->frameNoData->setVisible(false);
        ui->toolBtnUpload->setDisabled(false);

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

            QApplication::processEvents(); // 允许处理待处理事件
        }
    }
    else
    {
        ui->listWidgetChooseFile->setVisible(false);
        ui->frameNoData->setVisible(true);
        if (ui->listWidgetChooseFile->count() > 0)
        {
            ui->toolBtnUpload->setDisabled(false);
        }
        else
        {
            ui->toolBtnUpload->setDisabled(true);
        }
    }
}

void UploadFileDialog::uploadFile()
{
    on_toolBtnUploadQueue_clicked();

    ui->stackedWidget->setCurrentWidget(ui->pageQueue);    

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
        ui->listWidgetChooseFile->setVisible(false);
        ui->frameNoData->setVisible(true);
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
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}


void UploadFileDialog::showEvent(QShowEvent *event)
{
    QRect rect = ui->toolBtnUploadQueue->geometry();
    m_LabelPoint->move(rect.x() + rect.width()-5, rect.y()-5);
}
