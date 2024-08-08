#include "messagecenterdialog.h"
#include "ui_messagecenterdialog.h"
#include "messagetips.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QGraphicsDropShadowEffect>
#include "toolobject.h"
#define  POINT_SIZE         6

void NoticeItem::setupUI(S_NOTICE_INFO info)
{
    /*QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout* hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);*/
    m_info = info;

    m_button = new QToolButton(this);
    if (info.bIsRead)
        m_button->setStyleSheet("QToolButton{padding-left:12px;font-size:13px;color:#A9ADB6}QToolButton:hover{background-color:#FFFFFF}");
    else
        m_button->setStyleSheet("QToolButton{padding-left:12px;font-size:13px;color:#505465}QToolButton:hover{background-color:#FFFFFF}");
    m_button->setFixedSize(QSize(NOTICE_ITEM_WIDTH, NOTICE_ITEM_HEIGHT));
    m_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
    QFontMetrics fontWidth2(m_button->font());
    QString strElideNote2 = fontWidth2.elidedText(info.strTitle, Qt::ElideRight, NOTICE_ITEM_WIDTH-20);
    m_button->setText(strElideNote2);
    connect(m_button, &QPushButton::clicked, this, &NoticeItem::do_NoticeItem_clicked);
    //hLayout->addWidget(m_LabelTransferStatus);
    //hLayout->setSpacing(22);

    //添加到垂直布局
    //vLayout->addLayout(hLayout);
};

void NoticeItem::do_NoticeItem_clicked(bool checked)
{
    emit itemSelectSignals(m_info);
    qDebug() << "clicked me 当前选中 id=" << m_info.iId << "创建人=" << m_info.iCreateBy << "公告类型 1.系统公告 2.活动 3.更新公告：" << m_info.iType << "标题：" << m_info.strTitle;
    if (!m_info.bIsRead)
    {
        HttpPostSetNoticeRead(m_info.iCreateBy, m_info.iId, (NOTICE_TYPE)m_info.iType);
    }
}

MessageCenterDialog::MessageCenterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessageCenterDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowTitle("消息中心");
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(5);//阴影模糊半径
    shadow->setXOffset(0);//水平偏移
    shadow->setYOffset(0); //垂直偏移
    shadow->setColor(Qt::gray);//阴影颜色
    this->setGraphicsEffect(shadow);

    ui->listWidget->setViewMode(QListView::ListMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(0);//ui->listWidget->setSpacing(LIST_WIDGET_LISTMODE_ITEM_SPACING);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    m_LabelActivityPoint = new QLabel(ui->frame_22);
    m_LabelActivityPoint->resize(POINT_SIZE, POINT_SIZE);
    m_LabelActivityPoint->setPixmap(QPixmap(":/resource/upload/point.png").scaled(QSize(m_LabelActivityPoint->width(), m_LabelActivityPoint->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QRect rectActivity = ui->btnActivity->geometry();
    m_LabelActivityPoint->move(rectActivity.x() + rectActivity.width() - POINT_SIZE/2, rectActivity.y());
    m_LabelActivityPoint->setVisible(false);

    m_LabelAnnouncementPoint = new QLabel(ui->frame_21);
    m_LabelAnnouncementPoint->resize(POINT_SIZE, POINT_SIZE);
    m_LabelAnnouncementPoint->setPixmap(QPixmap(":/resource/upload/point.png").scaled(QSize(m_LabelAnnouncementPoint->width(), m_LabelAnnouncementPoint->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    QRect rectAnnouncement = ui->btnAnnouncement->geometry();
    m_LabelAnnouncementPoint->move(rectAnnouncement.x() + rectAnnouncement.width() - POINT_SIZE/2, rectAnnouncement.y());
    m_LabelAnnouncementPoint->setVisible(false);

    //默认公告
    HttpGetNoticeListInfo(NOTICE_SYSTEM_ANNOUNCEMENT, 1, 1000);    
}

MessageCenterDialog::~MessageCenterDialog()
{
    delete ui;
}

void MessageCenterDialog::on_btnAnnouncement_clicked()
{
    qDebug() << "公告列表";
    ui->btnAnnouncement->setStyleSheet("QPushButton{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->btnActivity->setStyleSheet("QPushButton:hover{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}QPushButton{color:#A9ADB6;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->labelAnnouncement->setStyleSheet("background-color:#505465;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");
    ui->labelActivity->setStyleSheet("background-color:#F4F6FA;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");
    
    HttpGetNoticeListInfo(NOTICE_SYSTEM_ANNOUNCEMENT, 1, 1000);
}

void MessageCenterDialog::on_btnActivity_clicked()
{
    qDebug() << "活动列表";
    ui->btnActivity->setStyleSheet("QPushButton{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->btnAnnouncement->setStyleSheet("QPushButton:hover{border:none;color:#505465;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}QPushButton{color: #A9ADB6;background:transparent;border-radius:1px;padding-left:0px;font-weight:bold;font-size:13px;}");
    ui->labelActivity->setStyleSheet("background-color:#505465;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");
    ui->labelAnnouncement->setStyleSheet("background-color:#F4F6FA;max-height:2px;max-width:12px;min-height:2px;min-width:12px;border:none;");

    HttpGetNoticeListInfo(NOTICE_ACTIVE, 1, 1000);
}

void MessageCenterDialog::LoadNoticeInfoList(NOTICE_TYPE enType)
{
    ui->listWidget->clear();
    NoticeItem* widget = NULL;
    QListWidgetItem* item = NULL;
    QMap<int, S_NOTICE_INFO>::iterator iter;
    QPushButton* button = NULL;
    int iIsReadCount = 0;
    int iIsReadActivityCount = 0;
    for (iter = m_mapNotice.begin(); iter != m_mapNotice.end(); iter++)
    {
        switch (iter->iType)
        {
        case NOTICE_SYSTEM_ANNOUNCEMENT:
        {
            if (!iter->bIsRead)
            {
                iIsReadCount++;
            }
        }
        break;
        case NOTICE_UPDATE_APP:
        {            
            if (!iter->bIsRead)
            {
                //更新公告
                ToolObject toolObj;
                toolObj.HttpPostCheckAppVersion();

                iIsReadCount++;
            }    
        }
            break;
        case NOTICE_ACTIVE:
        {
            if (!iter->bIsRead)
            {
                iIsReadActivityCount++;
            }    
        }
            break;
        default:
            break;
        }
        if (enType == NOTICE_SYSTEM_ANNOUNCEMENT)
        {
            if (NOTICE_SYSTEM_ANNOUNCEMENT == iter->iType || NOTICE_UPDATE_APP == iter->iType)
            {
                item = new QListWidgetItem(ui->listWidget);
                item->setSizeHint(QSize(NOTICE_ITEM_WIDTH, NOTICE_ITEM_HEIGHT));
                ui->listWidget->addItem(item);

                item->setData(Qt::UserRole, QVariant::fromValue(*iter));
                widget = new NoticeItem(*iter, this);
                connect(widget, &NoticeItem::itemSelectSignals, this, [this](S_NOTICE_INFO info) {
                    //显示选中数据
                    ui->textEdit->RefreshUIData(info.strRemark);
                    });
                ui->listWidget->setItemWidget(item, widget);
            }
        }        
    }

    //设置默认第一条选中
    if (ui->listWidget->count() > 0)
    {
        ui->listWidget->setCurrentRow(0);
        QListWidgetItem* item = ui->listWidget->currentItem();
        if (item != NULL)
        {
            S_NOTICE_INFO info = item->data(Qt::UserRole).value<S_NOTICE_INFO>();
            ui->textEdit->RefreshUIData(info.strRemark);
        }        
    }

    if (iIsReadCount == 0)
    {
        m_LabelAnnouncementPoint->setVisible(false);
    }
    else
    {
        m_LabelAnnouncementPoint->setVisible(true);
    }

    if (iIsReadActivityCount == 0)
    {
        m_LabelActivityPoint->setVisible(false);
    }
    else
    {
        m_LabelActivityPoint->setVisible(true);
    }

    //没有新的公告
    if (iIsReadCount == 0 && iIsReadActivityCount == 0)
    {
        this->close();
    }
}

void MessageCenterDialog::HttpGetNoticeListInfo(NOTICE_TYPE enType,int iPage, int iPageSize)
{
    //已授权列表
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_NOTICE_LIST;
    //platform PC端值为1
    strUrl += QString::asprintf("?page=%d&pageSize=%d&platform=%d", iPage, iPageSize,1);
    qDebug() << "strUrl = " << strUrl;
    //创建网络访问管理器,不是指针函数结束会释放因此不会进入finished的槽
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    //创建请求对象
    QNetworkRequest request;
    QUrl url(strUrl);
    qDebug() << "url:" << strUrl;
    QString strToken = HTTP_TOKEN_HEADER + GlobalData::strToken;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader(LOGIN_DEVICE_TYPE, LOGIN_DEVICE_TYPE_VALUE);
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    request.setUrl(url);

    m_mapNotice.clear();
    //发出GET请求
    QNetworkReply* reply = manager->get(request);
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
                        int iTotal = data["total"].toInt();
                        qDebug() << "iTotal=" << iTotal << "iCurrent=" << iCurrent << "iPages=" << iPages << "iSize=" << iSize;
                        QJsonArray records = data["records"].toArray();                        
                        if (records.size() > 0)
                        {
                            int iRecordsSize = records.size();
                            QJsonObject recordObj;
                            //获取我的手机实例数据，暂未存储
                            S_NOTICE_INFO noticeInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                recordObj = records[i].toObject();
                                noticeInfo.iId = recordObj["id"].toInt();
                                noticeInfo.iType = recordObj["type"].toInt();
                                noticeInfo.strTitle = recordObj["title"].toString();
                                noticeInfo.strCreateTime = recordObj["createTime"].toString();
                                noticeInfo.strRemark = recordObj["remark"].toString();
                                qDebug() << "公告 title=" << noticeInfo.strTitle << "remark=" << noticeInfo.strRemark;
                                noticeInfo.iCreateBy = recordObj["createBy"].toInt();
                                noticeInfo.bIsRead = recordObj["isRead"].toBool();
                                m_mapNotice.insert(i, noticeInfo);
                            }
                        }

                        //显示
                        LoadNoticeInfoList(enType);
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

//设置已读
void NoticeItem::HttpPostSetNoticeRead(int iCreateBy, int iId, NOTICE_TYPE enType)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_POST_SET_NOTICE_READ;
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
    /*obj.insert("createBy", iCreateBy);
    obj.insert("id", iId);*/
    obj.insert("noticeId", iId);
    doc.setObject(obj);
    QByteArray postData = doc.toJson(QJsonDocument::Compact);
    qDebug() << "postData:" << postData;
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
                    qDebug() << "更新状态为已读";
                    m_info.bIsRead = true;
                    m_button->setStyleSheet("QToolButton{padding-left:12px;font-size:13px;color:#A9ADB6}QToolButton:hover{background-color:#FFFFFF}");
                }
                else
                {
                    /*MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();*/
                }
            }
        }
        reply->deleteLater();
        });
}

void MessageCenterDialog::on_btnClose_clicked()
{
    this->close();
}
