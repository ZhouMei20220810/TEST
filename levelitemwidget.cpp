#include "levelitemwidget.h"
#include "ui_levelitemwidget.h"
#include "global.h"
#include "filedownloader.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"

LevelItemWidget::LevelItemWidget(S_LEVEL_INFO levelInfo, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LevelItemWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    QSize size(ITEM_WIDGET_LEVEL_WIDTH, ITEM_WIDGET_LEVEL_HEIGHT);
    this->resize(size);

    m_levelInfo = levelInfo;

    m_toolBtn = new QToolButton(this);
    QString strIcon = QString(":/main/resource/buy/%1_level_bg_normal.png").arg(levelInfo.strLevelName);
    //m_toolBtn->setIcon(QIcon(":/main/resource/main/gvip_bg_normal.png"));
    m_toolBtn->resize(size);
    m_toolBtn->setIcon(QIcon(strIcon));
    m_toolBtn->setIconSize(QSize(ITEM_WIDGET_LEVEL_WIDTH - 2, ITEM_WIDGET_LEVEL_HEIGHT));
    connect(m_toolBtn, &QToolButton::clicked, this, &LevelItemWidget::on_toolButtonBG_clicked);

    ui->labelVersion->setParent(m_toolBtn);
    ui->labelFunction->setParent(m_toolBtn);
    ui->frame->setParent(m_toolBtn);

    QVBoxLayout* vBox = new QVBoxLayout();
    vBox->addWidget(ui->labelVersion);
    vBox->addWidget(ui->labelFunction);
    vBox->addWidget(ui->frame);
    vBox->setContentsMargins(24,24,24,24);
    m_toolBtn->setLayout(vBox);

    ui->labelVersion->setText(levelInfo.strLevelName);
    ui->labelFunction->setText(levelInfo.strLevelRemark);

    QString url = levelInfo.strColorIcon;
    QString strFileName = url.right(url.size() - url.lastIndexOf('/') - 1);
    
    QString strTmp = GlobalData::strPictureTempDir + strFileName;
    qDebug() << "url=" << url << "strTmp=" << strTmp;
    //ui->labelVersion->setPixmap(QPixmap(strTmp));
    startDownload(url);
    int width = ui->labelVersion->width();
    int height = ui->labelVersion->height();
    QFile file(strTmp);
    if(file.exists())
        ui->labelVersion->setPixmap(QPixmap(strTmp).scaled(QSize(width, height), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

LevelItemWidget::~LevelItemWidget()
{
    qDebug()<<"delete LevelItemWidget";
    delete ui;
}

void LevelItemWidget::startDownload(QString strUrl)
{
    FileDownloader* downloader = new FileDownloader(this);
    connect(downloader, &FileDownloader::downloadFinished, this, [this](bool success, QString errorMessage)
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
                qDebug() <<"download="<< m_levelInfo.strColorIcon<< "errorMessage = " << errorMessage;
            }
                            
        });
    downloader->setUrl(strUrl, "xxx.png");
    downloader->start();
}

S_LEVEL_INFO LevelItemWidget::getLevelInfo()
{
    return m_levelInfo;
}

void LevelItemWidget::setLabelCheckStatus(bool bCheck)
{
    QString strIcon;
    if (bCheck)
    {
        strIcon = QString(":/main/resource/buy/%1_level_bg_select.png").arg(m_levelInfo.strLevelName);
    }
    else
    {
        strIcon = QString(":/main/resource/buy/%1_level_bg_normal.png").arg(m_levelInfo.strLevelName);
    }
    m_toolBtn->setIcon(QIcon(strIcon));

    qDebug()<<"LevelItemWidget labelCheck status="<<bCheck;
}

void LevelItemWidget::on_toolButtonBG_clicked()
{
    QString strIcon = QString(":/main/resource/buy/%1_level_bg_select.png").arg(m_levelInfo.strLevelName);
    m_toolBtn->setIcon(QIcon(strIcon));
    HttpMemberListByLevelId();
    emit selectLevelTypeSignals(m_levelInfo);
}


//会员相关接口
void LevelItemWidget::HttpMemberListByLevelId()
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_MEMBER_LIST_BY_LEVELID;
    strUrl += QString("?level=%1&page=%2&pageSize=%3").arg(m_levelInfo.iLevelId).arg(1).arg(100);
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
                    QJsonObject dataObj = obj["data"].toObject();
                    int iTotal = dataObj["total"].toInt();
                    int iSize = dataObj["size"].toInt();
                    int iCurrent = dataObj["current"].toInt();

                    if (dataObj["records"].isArray())
                    {
                        QJsonArray recordArray = dataObj["records"].toArray();
                        m_mapData.clear();
                        S_LEVEL_DATA_INFO sLevelData;
                        QJsonObject member;
                        for (int i = 0; i < recordArray.size(); i++)
                        {
                            member = recordArray[i].toObject();
                            sLevelData.iMemberId = member["id"].toInt();
                            sLevelData.strMemberName = member["name"].toString();
                            sLevelData.iLevelId = member["level"].toInt();
                            sLevelData.fPrice = member["price"].toDouble();
                            sLevelData.fActivityPrice = member["activityPrice"].toDouble();
                            sLevelData.strUrl = member["url"].toString();
                            sLevelData.strRemark = member["remark"].toString();
                            sLevelData.strInstanceLevel = member["instanceLevel"].toString();
                            sLevelData.iUseDay = member["useDay"].toInt();
                            sLevelData.strLevelName = member["levelName"].toString();
                            sLevelData.strColorIcon = member["colorIcon"].toString();
                            sLevelData.strAshIcon = member["ashIcon"].toString();
                            sLevelData.strLevelRemark = member["levelRemark"].toString();
                            m_mapData.insert(sLevelData.iMemberId, sLevelData);                                
                        }
                        emit refreshMemberListSignals(m_levelInfo.iLevelId,m_mapData);
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