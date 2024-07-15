#include "replacecloudphonedialog.h"
#include "ui_replacecloudphonedialog.h"
#include "messagetips.h"
#include <QCheckBox>

ReplaceCloudPhoneDialog::ReplaceCloudPhoneDialog(S_PHONE_INFO phoneInfo, QMap<int, S_LEVEL_INFO> mapLevelList,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceCloudPhoneDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    m_mapLevelList = mapLevelList;
    //imageList->resize(365,400);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    //ui->imageList->setIconSize(QSize(100,100));
    //设置QListWidget中单元项的间距
    ui->listWidget->setSpacing(0);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //设置不能移动
    ui->listWidget->setMovement(QListWidget::Static);
    //设置单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    //按等级拉去数据
    HttpGetMyPhoneInstance(0, 1, 1000, phoneInfo.iLevel);
}

ReplaceCloudPhoneDialog::~ReplaceCloudPhoneDialog()
{
    delete ui;
}

void ReplaceCloudPhoneDialog::on_btnClose_clicked()
{
    this->close();
}


void ReplaceCloudPhoneDialog::on_btnOk_clicked()
{
    qDebug()<<"确定更换";
}
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

void ReplaceCloudPhoneDialog::HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel)
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_MY_PHONE_INSTANCE;
    //level不传值,返回该 组下面所有的level
    if (iLevel != 0)
        strUrl += QString::asprintf("?level=%d&page=%d&pageSize=%d", iLevel, iPage, iPageSize);
    else
        strUrl += QString::asprintf("?groupId=%d&page=%d&pageSize=%d", iGroupId, iPage, iPageSize);
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
                        m_mapPhoneInfo.clear();
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
                            S_PHONE_INFO phoneInfo;
                            for (int i = 0; i < iRecordsSize; i++)
                            {
                                memset(&phoneInfo, 0, sizeof(S_PHONE_INFO));
                                recordObj = records[i].toObject();
                                phoneInfo.strCreateTime = recordObj["createTime"].toString();
                                phoneInfo.strCurrentTime = recordObj["current"].toString();
                                phoneInfo.strExpireTime = recordObj["expireTime"].toString();
                                phoneInfo.iId = recordObj["id"].toInt();
                                phoneInfo.iLevel = recordObj["level"].toInt();
                                phoneInfo.strName = recordObj["name"].toString();
                                phoneInfo.strInstanceNo = recordObj["no"].toString();
                                phoneInfo.strServerToken = recordObj["serverToken"].toString();
                                phoneInfo.iType = recordObj["type"].toInt();
                                phoneInfo.strGrantControl = recordObj["grantControl"].toString();
                                phoneInfo.bIsAuth = recordObj["isAuth"].toBool();
                                m_mapPhoneInfo.insert(i, phoneInfo);
                                qDebug() << "name" << phoneInfo.strName << "strInstanceNo=" << phoneInfo.strInstanceNo << "phoneInfo.strCreateTime=" << phoneInfo.strCreateTime << "phoneInfo.strCurrentTime=" << phoneInfo.strCurrentTime << "phoneInfo.strExpireTime=" << phoneInfo.strExpireTime << "id=" << phoneInfo.iId << "type=" << phoneInfo.iType << "level=" << phoneInfo.iLevel;
                            }
                        }
                        //if (iLevel != 0)
                            //    ShowActiveCodeItemInfo(iLevel, m_mapPhoneInfo);
                            //else 
                        ShowPhoneInfo(m_mapPhoneInfo);
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

void ReplaceCloudPhoneDialog::ShowPhoneInfo(QMap<int, S_PHONE_INFO> mapPhoneInfo)
{
    ui->listWidget->clear();
    if (mapPhoneInfo.size() <= 0)
        return;

    QString strStyleSheet = "QCheckBox{spacing:5px;}QCheckBox::indicator{width:16px;height:16px;}QCheckBox::indicator:unchecked{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:hover{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:unchecked:pressed{image:url(:/login/resource/login/option_normal.png);}QCheckBox::indicator:checked{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:hover{image:url(:/login/resource/login/option_select.png);}QCheckBox::indicator:checked:pressed{image:url(:/login/resource/login/option_select.png);}";    
    QListWidgetItem* item = NULL;
    QCheckBox* checkBox = NULL;
    QString strLevelName;
    QMap<int, S_LEVEL_INFO>::iterator iterFind;
    QMap<int, S_PHONE_INFO>::iterator iter = mapPhoneInfo.begin();
    for (; iter != mapPhoneInfo.end(); iter++)
    {
        iterFind = m_mapLevelList.find(iter->iLevel);
        strLevelName = "";
        if (iterFind != m_mapLevelList.end())
        {
            strLevelName = iterFind->strLevelName;
        }

        checkBox = new QCheckBox(this);
        checkBox->setStyleSheet(strStyleSheet);
        checkBox->setIcon(QIcon(QString(":/main/resource/main/%1.png").arg(strLevelName)));
        if (iter->strName.isEmpty())
            checkBox->setText(iter->strInstanceNo);
        else
            checkBox->setText(iter->strName);
        checkBox->setFixedSize(200, LISTMODE_ITEM_HEGITH);
        //widget2 = new PhoneListModeItemWidget(phoneInfo, this);
        //connect(widget2, &PhoneListModeItemWidget::ShowPhoneInstanceWidgetSignals, this, &MainWindow::on_ShowPhoneInstanceWidgetSignals);
        //connect(widget2, &PhoneListModeItemWidget::stateChanged, this, &MainWindow::do_stateChanged);
        item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(LISTMODE_ITEM_WIDTH, LISTMODE_ITEM_HEGITH));
        item->setData(Qt::UserRole, QVariant::fromValue(*iter));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, checkBox);
    }
    
}