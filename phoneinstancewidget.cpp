#include "phoneinstancewidget.h"
#include "ui_phoneinstancewidget.h"
#include <QDir>
#include <QClipboard>
#include "messagetips.h"
#include "Keyboard.h"
#include "SWDataSource.h"
#include "messagetipsdialog.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetips.h"
#include <QSettings>
#include <QDateTime>
#include <QStandardItemModel>
#include <QGeoPositionInfoSource>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QSettings>
#include "filedownloader.h"
#include "recentcopycutcontentdialog.h"
#define         TOOLBUTTON_WIDTH            (40)
#define         TOOLBUTTON_HEIGHT           (40)

PhoneInstanceWidget::PhoneInstanceWidget(S_PHONE_INFO sPhoneInfo,bool bIsMasterOrNot,QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::PhoneInstanceWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowTitle(sPhoneInfo.strInstanceNo);

    m_bIsMasterOrNot = bIsMasterOrNot;
    m_bHasChildControl = false;
    m_GeoSource = NULL;
    
    ui->frameTool->setVisible(false);   

    m_PhoneInfo = sPhoneInfo;
    m_strPhoneList.clear();
    m_strPhoneList << sPhoneInfo.strInstanceNo;//同步操作时，同时传入所有选中的item
    //主控设备才需要同步
    if (GlobalData::bIsSyncOperation)
    {
        m_strPhoneList.clear();
        QMap<int, S_PHONE_INFO>::iterator iter = GlobalData::mapSyncPhoneList.begin();
        for (; iter != GlobalData::mapSyncPhoneList.end(); iter++)
        {
            qDebug() << "同步操作,添加需要同步设备" << iter->strInstanceNo;
            m_strPhoneList << iter->strInstanceNo;
        }
    }
    ui->toolBtnShow->setVisible(false);

    ui->toolBtnPhoneInstance->setText(sPhoneInfo.strInstanceNo);    

    m_manager = new QNetworkAccessManager(this);
    m_toolObject = new ToolObject(this);
    m_getScreenshotsTimer = NULL;
    m_getScreenshotsTimer = new QTimer();
    connect(m_getScreenshotsTimer, &QTimer::timeout, this, [=]()
        {
            m_getScreenshotsTimer->stop();
            this->m_toolObject->HttpPostInstanceScreenshot(m_strPhoneList);
        });
    connect(m_toolObject, &ToolObject::startTimerShowScreenshotSignals, this, [=]() {
        qDebug() << "获取图片刷新成功,间隔三秒钟下载图片";
        m_getScreenshotsTimer->start(DOWNLOAD_SCREENSHOT_INTERVAL);
        });
    connect(m_toolObject, &ToolObject::getScreenshortSignals, this, [=](QMap<QString, S_TASK_INFO> mapScreenshotTask) 
        {
        if (mapScreenshotTask.size() <= 0)
            return;

        QString strOutputFile = "";
        QMap<QString, S_TASK_INFO>::iterator iter = mapScreenshotTask.begin();
        for (; iter != mapScreenshotTask.end(); iter++)
        {
            qDebug() << "download url=" << iter->strUrl;
            if (iter->strUrl.isEmpty())
            {
                MessageTips* tips = new MessageTips("截图失败", this);
                tips->show();
                continue;
            }

            FileDownloader* downloader = new FileDownloader(this);
            if (downloader != NULL)
            {
                QDateTime dateTime = QDateTime::currentDateTime();
                QString strTime = dateTime.toString("MMddhhmmss");
                strOutputFile = GlobalData::strPhoneInstanceScreenshotDir + "/" + iter->strPadCode + "_" + strTime + ".png";
                downloader->setUrlOutputFile(iter->strUrl, strOutputFile);
                downloader->start();
            }            
        }

        });

    m_Player = NULL;

    QString strUUID = GlobalData::strAccount;
    QString strPadCode = sPhoneInfo.strInstanceNo;
    QString strOnlineTime = sPhoneInfo.strExpireTime;
    QDateTime time = QDateTime::fromString(strOnlineTime,"yyyy-MM-dd hh:mm:ss");
    qint64 i64Time = time.toSecsSinceEpoch();//time.toMSecsSinceEpoch();
    qDebug() << "i64Time(s)=" << i64Time<<"i64Time(ms)="<< time.toMSecsSinceEpoch();

    m_bIsVertical = GlobalData::bVerticalPhoneInstance;
    do_HorizontalSignals(m_bIsVertical);
    if (m_bIsVertical)
    {
        int height = PHONE_INSTANCE_VERTICAL_HEIGHT;
        int iwidth = calculateWidth(height);
        resize(iwidth, height);
    }        
    else
    {
        int height = PHONE_INSTANCE_HORIZONTAL_WIDTH;
        int width = calculateWidth(height - 40) + 40;
        resize(height, width);
    }
    // 使用QStandardItemModel，因为它支持设置数据的不同角色
    ui->comboBox->setIconSize(QSize(27,45));
    QStandardItemModel* model = new QStandardItemModel(ui->comboBox);
    QStandardItem* item = NULL;
    // 添加图片和文本到下拉列表的每一项
    //（0：自动，1：高清，2：标清，3：流畅）
    for (int i = 0; i < 4; ++i)
    {
        // 创建QStandardItem
        item = new QStandardItem;

        // 设置项的文本
        //item->setText(QString("Item %1").arg(i + 1));

        // 加载图片并设置为项的装饰角色（一般用于显示图片）
        //QPixmap pixmap(QString(":/resource/instance/%1.png").arg(i + 1)); // 假设你有image1.png到image5.png
        //（0：自动，1：高清，2：标清，3：流畅）
        QIcon icon(QString(":/resource/instance/%1.png").arg(i+1));
        item->setIcon(icon);
        //item->setData(Qt::UserRole, i);

        // 将项添加到模型
        model->appendRow(item);
    }

    // 设置模型到组合框
    ui->comboBox->setModel(model);
    // 可以设置当前索引，以便初始显示的项
    //ui->comboBox->setCurrentIndex(0);
    //同步设置界面
    ui->comboBox->setCurrentIndex(GlobalData::enPictrueQuality);

    if (!GlobalData::bVerticalPhoneInstanceCenter)
    {
        QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
        GlobalData::pointPhoneInstance = setting.value("PhoneInstancePoint").toPoint();
        this->move(GlobalData::pointPhoneInstance);
    }

    HttpGetInstanceSession(sPhoneInfo.iId);
}

void PhoneInstanceWidget::HttpGetInstanceSession(int id)/*QString strUUID, qint64 i64OnlineTime, QString strPadCode*/
{
    QString strUrl = HTTP_SERVER_DOMAIN_ADDRESS;
    strUrl += HTTP_GET_INSTANCE_SESSION;
    strUrl += QString("/%1").arg(id);
    //QString strUrl = QString("%1%2{%3}").arg(HTTP_SERVER_DOMAIN_ADDRESS).arg(HTTP_DELETE_GROUP).arg(iGroupId);//.toLocal8Bit();
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
                qDebug() << "Code=" << iCode << "message=" << strMessage;
                if (HTTP_SUCCESS_CODE == iCode)
                {
                    //true操作成功
                    if (obj["data"].isObject())
                    {
                        S_PAD_INFO padInfo;
                        padInfo.iPhoneId = id;
                        QJsonObject dataObj = obj["data"].toObject();
                        QJsonObject data = dataObj["data"].toObject();
                        int iServerCode = dataObj["code"].toInt();
                        QString strMessage = dataObj["message"].toString();
                        if (iServerCode != HTTP_SUCCESS_CODE)
                        {
                            MessageTips* tips = new MessageTips(strMessage, this);
                            tips->show();
                        }
                        else
                        {
                            //m_strSessionId = data["sessionId"].toString();
                            padInfo.strSessionID = data["sessionId"].toString();
                            //qDebug() << "m_strSessionId = " << m_strSessionId;
                            QJsonObject control;
                            QJsonArray controlArray = data["controlList"].toArray();
                            for (int iControl = 0; iControl < controlArray.size(); iControl++)
                            {
                                control = controlArray.at(iControl).toObject();

                                if (control["controlInfoList"].isArray())
                                {
                                    QJsonArray controlList = control["controlInfoList"].toArray();
                                    int iSize = controlList.size();
                                    QJsonObject info;
                                    for (int i = 0; i < iSize; i++)
                                    {
                                        info = controlList.at(i).toObject();
                                        padInfo.strControlAddr = info["controlIp"].toString();
                                        padInfo.fControlPort = info["controlPort"].toDouble();
                                        padInfo.iUserID = GlobalData::id;
                                        padInfo.iHWaccel = 1;

                                        m_mapPadInfo.insert(id, padInfo);
                                        //m_strTraceServer = info["traceServer"].toString();
                                        //qDebug() << "m_strControlIp=" << m_strControlIp << "m_dControlPort=" << m_dControlPort << "strTraceServer=" << m_strTraceServer;
                                    }
                                }
                            }

                            onPlayStart(padInfo);
                        }                        
                    }
                }
                else
                {
                    strMessage = QString("%1(%2)").arg(obj["error"].toString()).arg(obj["status"].toInt());
                    MessageTips* tips = new MessageTips(strMessage, this);
                    tips->show();
                }
            }
        }
        reply->deleteLater();
        });
}

PhoneInstanceWidget::~PhoneInstanceWidget()
{
    if (m_GeoSource != NULL)
    {
        m_GeoSource->stopUpdates();
    }
    if (m_getScreenshotsTimer->isActive())
    {
        m_getScreenshotsTimer->stop();
    }
	Mutex::Autolock lock(m_Mutex);
	onPlayStop(true);
    delete ui;
}

void PhoneInstanceWidget::on_toolBtnPhoneInstance_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->toolBtnPhoneInstance->text());

    MessageTips* tips = new MessageTips("已复制云号至剪贴板", this);
    tips->show();
}

void PhoneInstanceWidget::on_toolBtnPictureQuality_clicked()
{

}


void PhoneInstanceWidget::on_toolBtnTopLevel_clicked()
{
    //切换窗口状态
    /*if (window()->isMaximized()) {
        return;
    }*/
    // 获取当前窗口状态
    Qt::WindowFlags flags = window()->windowFlags();

    // 切换置顶状态
    flags ^= Qt::WindowStaysOnTopHint;

    // 更新窗口状态
    window()->setWindowFlags(flags);

    // 检查窗口是否可见
    if (!window()->isVisible()) {
        // 窗口不可见，尝试重新显示窗口
        window()->show();
    }
    if (flags & Qt::WindowStaysOnTopHint) {
        ui->toolBtnTopLevel->setText("➴");
        /*QString styleSheet = QString("QToolButton{background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
        ui->toolBtnTopLevel->setStyleSheet(styleSheet);*/
    }
    else {
        ui->toolBtnTopLevel->setText("↘");
        /*QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
        ui->toolBtnTopLevel->setStyleSheet(styleSheet);*/
    }
}

void PhoneInstanceWidget::on_toolBtnMin_clicked()
{
    this->showMinimized();
}


void PhoneInstanceWidget::on_toolBtnMax_clicked()
{
    //最大化、常态
    QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);}QToolButton:hover {background-color: rgb(60, 63, 71);border:none;border-radius:5px;color: rgb(255, 255, 255);}");
    ui->toolBtnTopLevel->setStyleSheet(styleSheet);
    if (window()->isMaximized()) {
        ui->toolBtnMax->setIcon(QIcon(":/resource/instance/max.png"));        
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);margin-bottom:3px;}QToolButton:hover {color: rgb(255, 255, 255);}");
        ui->toolBtnMax->setStyleSheet(styleSheet);
        //this->showNormal();
        move(m_leftTopPoint);
        resize(m_remmberSize);
    }
    else {
        ui->toolBtnMax->setIcon(QIcon(":/resource/instance/restore.png"));
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);margin-bottom:2px;}QToolButton:hover {color: rgb(255, 255, 255);}");
        ui->toolBtnMax->setStyleSheet(styleSheet);
        m_leftTopPoint = this->pos();
        m_remmberSize = this->size();
        this->geometry();
        this->showMaximized();
    }
}

void PhoneInstanceWidget::do_closePhoneInstanceWidgetSignals()
{
    QSettings setting(ORGANIZATION_NAME, APPLICATION_NAME);
    setting.setValue("PhoneInstancePoint", QVariant::fromValue(this->pos()));

    this->close();
}
void PhoneInstanceWidget::on_toolBtnClose_clicked()
{
    if (m_bIsMasterOrNot)
    {
        //是否有副控
        if (hasChildControl() && !GlobalData::bIsTipsCloseMasterInstance)
        {
            //弹窗提示是否关闭
            MessageTipsDialog* dialog = new MessageTipsDialog("关闭主控时,将同时关闭非主控云机", nullptr, MESSAGE_NOT_TIPS_CLOSE_MASTER_INSTANCE, "关闭主控云机");
            if (QDialog::Accepted == dialog->exec())
            {
                emit closePhoneInstanceWidgetSignals();
            }
        }
        else
        {
            emit closePhoneInstanceWidgetSignals();
        }
    }
    else
    {
        emit closeNotMasterPhoneSignals();
        this->close();
    }
}


void PhoneInstanceWidget::on_toolBtnShow_clicked()
{
    ui->toolBtnShow->setVisible(false);
    ui->frame_2->setVisible(true);
}


void PhoneInstanceWidget::on_toolBtnHide_clicked()
{
    ui->toolBtnShow->setVisible(true);
    ui->frame_2->setVisible(false);
}

void PhoneInstanceWidget::do_ReturnSignals()
{
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_BACK);
        }
    }
}
void PhoneInstanceWidget::do_HomeSignals()
{
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_HOMEPAGE);
        }
    }
}
void PhoneInstanceWidget::do_ChangePageSignals()
{
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL) {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL) {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_MENU);
        }
    }
}

bool PhoneInstanceWidget::onPlayStart(S_PAD_INFO padInfo)
{
	Mutex::Autolock lock(m_Mutex);
	if (m_Player == NULL) 
	{
		do {
            int picQualityIndex = ui->comboBox->currentIndex();//ui->comboBox->itemData(ui->comboBox->currentIndex(), Qt::UserRole).toInt();
            qDebug() << "select index=" << picQualityIndex;
            //PAAS(试玩): BusinessType为0; 主营: BusinessType为1; 百度: BusinessType为2
            int businessType = 1;//m_OptionPlayType->IsSelected() ? 0 : 1;

			std::string padcode = ui->toolBtnPhoneInstance->text().toStdString();
            std::string packageName = "";//可以为空
            std::string controlAddr = padInfo.strControlAddr.toStdString();//"120.26.132.153";

            int controlPort = padInfo.fControlPort;//端口
            int userID = GlobalData::id;
            std::string sessionID = padInfo.strSessionID.toStdString();

			if (padcode.empty()) {
				break;
			}
			if (controlAddr.empty()) {
				break;
			}
			if (controlPort <= 0 || controlPort > 65535) {
				break;
			}

			VideoLevel videoLevels[4];
			videoLevels[0].encodetype = 2;
			videoLevels[0].width = 720;
			videoLevels[0].height = 1280;
			videoLevels[0].maxfps = 20;
			videoLevels[0].minfps = 15;
			videoLevels[0].bitrate = 4096;
			videoLevels[0].gop = videoLevels[0].maxfps * 4;
			videoLevels[0].resolutionLevel = 1;
			videoLevels[0].videoQuality = 1;
			videoLevels[0].maxDelay = 100;
			videoLevels[0].minDelay = 50;

			memcpy(&videoLevels[1], &videoLevels[0], sizeof(VideoLevel));
			videoLevels[1].width = 576;
			videoLevels[1].height = 1024;
			videoLevels[1].bitrate = 2048;
			videoLevels[1].resolutionLevel = 2;
			videoLevels[1].videoQuality = 2;
			memcpy(&videoLevels[2], &videoLevels[0], sizeof(VideoLevel));
			videoLevels[2].width = 432;
			videoLevels[2].height = 768;
			//videoLevels[2].width = 144;
			//videoLevels[2].height = 256;
			videoLevels[2].bitrate = 1024;
			videoLevels[2].resolutionLevel = 3;
			videoLevels[2].videoQuality = 3;
			memcpy(&videoLevels[3], &videoLevels[0], sizeof(VideoLevel));
			videoLevels[3].width = 288;
			videoLevels[3].height = 512;
			//videoLevels[3].width = 96;
			//videoLevels[3].height = 112;
			videoLevels[3].maxfps = 10;
			videoLevels[3].minfps = 10;
			videoLevels[3].gop = videoLevels[0].maxfps * 4;
			videoLevels[3].bitrate = 512;
			videoLevels[3].resolutionLevel = 4;
			videoLevels[3].videoQuality = 4;

			int videoLevelCount = sizeof(videoLevels) / sizeof(VideoLevel);

			int encodetype = -1;//2;
			int resolutionLevel = -1;//3;
			int videoQuality = -1;
			int apiLevel = businessType == 0 ? 2 : 1;
			int playType = PLAYTYPE_AV;

			m_Player = new SWPlayer();
            UINT32 playerId = m_Player->getId();
            qDebug() << "playerId = " << playerId;
            SWDataSource* datasource =  new SWDataSource(playerId, this);
			int ret = datasource->setLoginParams(controlAddr.c_str(), controlPort, userID, sessionID.c_str(), padcode.c_str(), 0);
            if (ret != 0)
            {
                qDebug() << "设置连接参数失败ret=" << ret;
            }
            /**
             * 设置投屏参数
             * @param appName       应用包名
             * @param encodetype    1：软解，2：硬解
             * @param width         width
             * @param height        height
             * @param maxfps        最大帧率
             * @param minfps        最小帧率
             * @param bitrate       码率
             * @param gop           I帧间隔
             * @param resolutionLevel   分辨率等，1-4
             * @param videoQuality  不用填
             * @param playType     	播放类型， 0：无，1：视频， 2：音频, 3：音视频
             * @param apiLevel      1：旧接口，2：新接口
             * @param useSSL        1:使用SSL加密，0：不加密
             * @return 0为成功，其它为失败
             */
			ret = datasource->setPlayParams(packageName.c_str(), encodetype, 0, 0,
				0, 0, 0, 0, resolutionLevel, videoQuality, playType, apiLevel, 0);
            if (ret != 0)
            {
                qDebug() << "设置播放参数失败ret=" << ret;
            }
            /**
            设置视频码流的配置档数（高清，标清，流畅三档配置信息）
            @param videoLevel 视频码流的配置档
            @return 0为成功，其它为失败
            */
			datasource->setVideoLevels((VideoLevel*)&videoLevels[0], videoLevelCount);
            /**
            设置投屏用哪一档
            @param levelIndex （0：自动，1：高清，2：标清，3：流畅）
            @return 0为成功，其它为失败
            */
			datasource->setVideoLevel(picQualityIndex);

			datasource->setBusinessType(businessType);

			m_Player->setDataSource(datasource);
            connect(ui->videoViewWidget, &VideoViewWidget::syncTouchEventSignals, this, &PhoneInstanceWidget::do_syncTouchEventSignals);
            connect(this, &PhoneInstanceWidget::dealTouchEventSignals, ui->videoViewWidget, &VideoViewWidget::do_syncTouchEventSignals);
            connect(this, &PhoneInstanceWidget::changeVerOrHorScreenSignals, ui->videoViewWidget, &VideoViewWidget::do_changeVerOrHorScreenSignals);
            m_Player->setDisplay(ui->videoViewWidget);
            //开始投屏
			m_Player->start();
			return true;
		} while (0);
	}
	return true;
}

void PhoneInstanceWidget::do_syncTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[])
{
    if (m_bIsMasterOrNot)
    {
        emit TouchEventSignals(eventAction, pointerCount, x, y, force);
    }
    else
    {
        ui->videoViewWidget->do_syncTouchEventSignals(eventAction, pointerCount, x, y, force);
    }
}

void PhoneInstanceWidget::onPlayStop(bool bQuit)
{
	if (m_Player != NULL) {
		if (!bQuit) {
			//m_EditPadcode->SetReadOnly(false);
			//m_BtnStart->SetText(L"开始投屏");
		}

		m_Player->stop();
		//m_Player.reset();
	}
}

void PhoneInstanceWidget::onReconnecting(int NthTime)
{
    QString strTips = QString("第%1次重连").arg(NthTime);
    qDebug() << strTips;
    //MessageTips* tips = new MessageTips(strTips);
    //tips->show();
}

void PhoneInstanceWidget::onConnected()
{
    QString strTips = QString("已连接");
    qDebug() << strTips;
    //MessageTips* tips = new MessageTips(strTips);
    //tips->show();
}

void PhoneInstanceWidget::onDisconnected(int errcode)
{
    QString strTips = QString("已断开:%1").arg(errcode);
    qDebug() << strTips;
    //MessageTips* tips = new MessageTips(strTips);
    //tips->show();
}

void PhoneInstanceWidget::onDisconnected(int errcode, const char* errmsg)
{
    QString strTips = QString("已断开:%1,%2").arg(errcode).arg(errmsg);
    qDebug() << strTips;
    //MessageTips* tips = new MessageTips(strTips);
    //tips->show();
}

void PhoneInstanceWidget::onPlayInfo(const char* info)
{
    //QString strTips = QString("onPlayInfo:%1").arg(info);
    //qDebug() << strTips;
}

/**
     * 传感器输入
     * @param inputtype     传感器类型:
     *                         位置数据:201, 加速度:202, 高度计:203, 陀螺仪:204, 磁力计:205,
     *                         音频输入:211, 视频输入:212, 重力感应:213
     * @param state          传感器状态，1：开，0：关
     */
void PhoneInstanceWidget::onSensorInput(int inputtype, int state)
{
    //qDebug() << "inputtype=" << inputtype << "state=" << state;
}

void PhoneInstanceWidget::on_toolBtnMore_clicked()
{
    bool bVisible = ui->frameTool->isVisible();
    ui->frameTool->setParent(ui->videoViewWidget);
    ui->frameTool->move(ui->videoViewWidget->width()-ui->frameTool->width()-2, 10);
    ui->frameTool->setVisible(!bVisible);
}

void PhoneInstanceWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if ((event->pos().x() >= 0 && event->pos().y() >= 0 && event->pos().x() <= ui->frame->width() && event->pos().y() <= ui->frame->height())
        || (event->pos().x() >= ui->frame->width() && event->pos().y() >= 0 && event->pos().x() <= (ui->frame->width()+ui->frame_2->width()) && event->pos().y() <= ui->frame_2->height()))
        {
            m_bMoving = true;
            m_LastPos = event->globalPos() - this->frameGeometry().topLeft();
        }
    }
    return QWidget::mousePressEvent(event);
}

void PhoneInstanceWidget::mouseReleaseEvent(QMouseEvent *event)
{

    m_bMoving = false;
}

void PhoneInstanceWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint globalPosition = event->globalPosition().toPoint();
    if(m_bMoving && (event->buttons()&Qt::LeftButton)
        && (globalPosition-m_LastPos-pos()).manhattanLength() > QApplication::startDragDistance()) //控制移动的距离，多少距离执行拖拽
    {
        move(globalPosition-m_LastPos);
        m_LastPos = globalPosition-pos();
    }
    return QWidget::mouseMoveEvent(event);
}

void PhoneInstanceWidget::do_VolumeUpSignals()
{
    //音量加大
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_VOLUMEUP);
        }
    }
}
void PhoneInstanceWidget::do_VolumeDownSignals()
{
    //音量减小
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_VOLUMEDOWN);
        }
    }
}
void PhoneInstanceWidget::on_toolBtnVolumnUp_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit VolumeUpSignals();
    }
    else
    {
        do_VolumeUpSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnVolumnDown_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit VolumeDownSignals();
    }
    else
    {
        do_VolumeDownSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnHorOrVer_clicked()
{
    m_bIsVertical = !m_bIsVertical;
    emit changeVerOrHorScreenSignals(m_bIsVertical);
    if (m_bIsMasterOrNot)
    {
        //其他窗口不适应关闭新建，需要重新初始化，先发送closePhoneInstanceWidgetSignals信号通知其他同步窗口，给服务器后台同步横屏
        emit HorizontalSignals(m_bIsVertical);
    }
    else
    {
        do_HorizontalSignals(m_bIsVertical);
    }
}


void PhoneInstanceWidget::on_toolBtnClipboard_clicked()
{
    //主控、副控剪切板一致，其他的情况副控只能控制自己。
    qDebug() << "do clipboard signals";
    RecentCopyCutContentDialog* dialog = new RecentCopyCutContentDialog();
    //传递需要拷贝的文字
    connect(dialog, &RecentCopyCutContentDialog::DirectCopyToPhoneSignals, this, &PhoneInstanceWidget::do_DirectCopyToPhoneSignals);
    dialog->exec();
}

void PhoneInstanceWidget::do_DirectCopyToPhoneSignals(QString strSelectText)
{
    //直接拷贝到云手机
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            //手机剪贴板
            source->copyToRemote(strSelectText.toStdString().c_str(), strSelectText.length());
        }
    }
}

void PhoneInstanceWidget::on_Screenshot_clicked(bool checked)
{
    if (m_bIsMasterOrNot)
    {
        //截图一次请求，可以同时发送多个手机InstanceNo
        m_toolObject->HttpPostInstanceScreenshotRefresh(m_strPhoneList);
    }
    else
    {
        //单个手机实例编号，截图自己
        QStringList strList;
        strList<<m_PhoneInfo.strInstanceNo;
        m_toolObject->HttpPostInstanceScreenshotRefresh(strList);
    }    
}
void PhoneInstanceWidget::on_toolBtnScreenshotDir_clicked()
{
    //截图目录
    QUrl url(QString("file:///") + GlobalData::strPhoneInstanceScreenshotDir);
    if (QDesktopServices::openUrl(url)) {
        // 成功打开目录
    }
    else {
        // 打开目录失败
        qDebug() << "open fail." << GlobalData::strPhoneInstanceScreenshotDir;
    }
}
void PhoneInstanceWidget::on_toolBtnReboot_clicked()
{
    if (m_bIsMasterOrNot)
    {
        //截图一次请求，可以同时发送多个手机InstanceNo
        m_toolObject->HttpPostInstanceReboot(m_strPhoneList);
    }
    else
    {
        QStringList strList;
        strList << m_PhoneInfo.strInstanceNo;
        m_toolObject->HttpPostInstanceReboot(strList);
    }
}

void PhoneInstanceWidget::on_toolBtnFactoryDataReset_clicked()
{
    if (m_bIsMasterOrNot)
    {
        m_toolObject->HttpPostInstanceReset(m_strPhoneList);
    }
    else
    {
        //只能控制自己
        QStringList strList;
        strList << m_PhoneInfo.strInstanceNo;
        m_toolObject->HttpPostInstanceReset(strList);
    }
}

void PhoneInstanceWidget::on_toolBtnRoot_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit RootSignals();
    }
    else
    {
        //只能控制自己
        do_rootSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnShark_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit SharkSignals();
    }
    else
    {
        //只能控制自己
        do_SharkSignals();
    }
}
void PhoneInstanceWidget::on_toolBtnGPS_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit GPSSignals();
    }
    else
    {
        //只能控制自己
        do_GPSSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnReturn_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit ReturnSignals();
    }
    else
    {
        //只能控制自己
        do_ReturnSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnHome_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit HomeSignals();
    }
    else
    {
        //只能控制自己
        do_HomeSignals();
    }
}

void PhoneInstanceWidget::on_toolBtnChangePage_clicked()
{
    if (m_bIsMasterOrNot)
    {
        emit ChangePageSignals();
    }
    else
    {
        //只能控制自己
        do_ChangePageSignals();
    }
}

void PhoneInstanceWidget::do_rootSignals()
{
    qDebug() << "do root signals";
}

void PhoneInstanceWidget::onPositionUpdated(const QGeoPositionInfo& info) 
{
    qDebug() << "QGeoPositionInfoSource::positionUpdated 响应";
    if (info.isValid()) {
        QGeoCoordinate coordinate = info.coordinate();
        double  dLongitude = coordinate.longitude();
        double  dlatitude = coordinate.latitude();        
        double  dAltitude = coordinate.altitude();
        double dDirection = info.attribute(QGeoPositionInfo::Direction);
        double dGroundSpeed = info.attribute(QGeoPositionInfo::GroundSpeed);
        double dVerticalSpeed = info.attribute(QGeoPositionInfo::VerticalSpeed);
        double dHorizontalAccuracy = info.attribute(QGeoPositionInfo::HorizontalAccuracy);
        double dVerticalAccuracy = info.attribute(QGeoPositionInfo::VerticalAccuracy);
        double dDirectionAccuracy = info.attribute(QGeoPositionInfo::DirectionAccuracy);
        qDebug() << "Longitude=" << dLongitude << "Latitude=" << dlatitude <<"Altitude="<<dAltitude<<"dHorizontalAccuracy=" << dHorizontalAccuracy<<"dVerticalAccuracy="<< dVerticalAccuracy<<"speed="<<dGroundSpeed<<"Direction="<< dDirection;
        if (m_Player != NULL)
        {
            DataSource* source = m_Player->getDataSource();
            if (source != NULL)
            {
                /*int sendInputLocation(float longitude,
                    float latitude,
                    float altitude,
                    float floor,
                    float horizontalaccuracy,
                    float verticalaccuracy,
                    float speed,
                    float direction,
                    const char* timestamp)*/
                source->sendInputLocation(dLongitude, dlatitude, dAltitude, 1, dHorizontalAccuracy, dVerticalAccuracy, dGroundSpeed, dDirection, QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss").toStdString().c_str());
            }
        }
    }
}
void PhoneInstanceWidget::do_HorizontalSignals(bool bIsVertical)
{
    int height = PHONE_INSTANCE_VERTICAL_HEIGHT;
    int width = calculateWidth(height);
    QSize btnMinSize(TOOLBUTTON_WIDTH, TOOLBUTTON_HEIGHT);
    QSize btnMaxSize(40, 57);
    QSize iconMinSize(24,24);

    m_tBtnHide = new QToolButton(ui->frame_2);
    connect(m_tBtnHide, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnHide_clicked);
    m_tBtnHide->setIcon(QIcon(":/resource/instance/Shouqi.png"));
    m_tBtnHide->setIconSize(btnMinSize);
    m_tBtnHide->setFixedSize(btnMinSize);

    m_tBtnRetrun = new QToolButton(ui->frame_2);
    connect(m_tBtnRetrun, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnReturn_clicked);
    m_tBtnRetrun->setIcon(QIcon(":/resource/instance/reback.png"));
    m_tBtnRetrun->setIconSize(btnMinSize);
    m_tBtnRetrun->setFixedSize(btnMinSize);

    m_tBtnHome = new QToolButton(ui->frame_2);
    connect(m_tBtnHome, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnHome_clicked);
    m_tBtnHome->setIcon(QIcon(":/resource/instance/home.png"));
    m_tBtnHome->setIconSize(btnMinSize);
    m_tBtnHome->setFixedSize(btnMinSize);

    m_tBtnChangeMenu = new QToolButton(ui->frame_2);
    connect(m_tBtnChangeMenu, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnChangePage_clicked);
    m_tBtnChangeMenu->setIcon(QIcon(":/resource/instance/change.png"));
    m_tBtnChangeMenu->setIconSize(btnMinSize);
    m_tBtnChangeMenu->setFixedSize(btnMinSize);
    
    //清空
    QLayoutItem* item = NULL;
    QLayout* layout = ui->frame_2->layout();
    if (layout != NULL)
    {
        int itemCount = ui->frame_2->layout()->count();
        for (int i = itemCount - 1; i >= 0; --i)
        {
            item = ui->frame_2->layout()->takeAt(i);
            if (item != NULL)
            {
                ui->frame_2->layout()->removeWidget(item->widget());
                delete item->widget();
            }
        }
        delete layout;
        layout = NULL;
    }
    layout = ui->frameTool->layout();
    if (layout != NULL)
    {
        int itemCount = ui->frameTool->layout()->count();
        for (int i = itemCount - 1; i >= 0; --i)
        {
            item = ui->frameTool->layout()->takeAt(i);
            if (item != NULL)
            {
                ui->frameTool->layout()->removeWidget(item->widget());
                delete item->widget();
            }
        }
        delete layout;
        layout = NULL;
    }

    m_vBox = new QVBoxLayout(this);
    m_vBox->setContentsMargins(0, 0, 0, 0);
    m_vBox->setSpacing(5);
    m_vBox->addWidget(m_tBtnHide);

    m_vBox2 = new QVBoxLayout(this);
    m_vBox2->setContentsMargins(0, 0, 0, 0);
    m_vBox2->setSpacing(5);

    //横屏
    if (m_bIsVertical)
    {        
        if (!GlobalData::strToolButtonList.isEmpty())
        {
            QStringList strList = GlobalData::strToolButtonList.split(',');
            int iToolIndex = 0;
            for (int i = 0; i < PHONE_INSTANCE_VER_DEFAULT_TOOL_NUM; i++)
            {
                iToolIndex = strList.at(i).toInt();
                InitToolButtonList(iToolIndex, ui->frame_2, m_vBox);
            }
        }
        m_vBox->addStretch();
        this->setMinimumSize(width, height);
        this->setMaximumSize(width, height);
    }
    else
    {
        QStringList strList = GlobalData::strToolButtonList.split(',');
        int iSize = strList.size();
        int iToolIndex = 0;
        int j = 0;
        for (int i = 0; i < iSize; i++)
        {
            iToolIndex = strList.at(i).toInt();
            if (i < PHONE_INSTANCE_HOR_DEFAULT_TOOL_NUM)
            {
                InitToolButtonList(iToolIndex, ui->frame_2, m_vBox);    
            }
            else
            {
                InitToolButtonList(iToolIndex, ui->frameTool, m_vBox2);
            }   
        }

        //添加更多按钮
        m_tBtnMoreTool = new QToolButton(this);
        connect(m_tBtnMoreTool, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnMore_clicked);
        m_tBtnMoreTool->setIcon(QIcon(":/resource/instance/more.png"));
        m_tBtnMoreTool->setIconSize(btnMinSize);
        m_tBtnMoreTool->setFixedSize(btnMinSize);
        m_tBtnMoreTool->setText("更多");
        m_tBtnMoreTool->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        m_vBox->addWidget(m_tBtnMoreTool);
        m_vBox->addStretch();

        m_vBox2->addStretch();
        ui->frameTool->setLayout(m_vBox2);

        this->setMinimumSize(height, width);
        this->setMaximumSize(height, width);
    }

    m_vBox->addWidget(m_tBtnRetrun);
    m_vBox->addWidget(m_tBtnHome);
    m_vBox->addWidget(m_tBtnChangeMenu);
    ui->frame_2->setLayout(m_vBox);

}

void PhoneInstanceWidget::InitToolButtonList(int iToolIndex,QFrame* frame, QVBoxLayout* vBox)
{
    QSize btnMinSize(TOOLBUTTON_WIDTH, TOOLBUTTON_HEIGHT);
    QSize btnMaxSize(40, 57);
    QSize iconMinSize(24, 24);
    switch (iToolIndex)
    {
    case TYPE_VOLUMN_UP:
        m_tBtnVolumnUp = new QToolButton(frame);
        connect(m_tBtnVolumnUp, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnVolumnUp_clicked);
        m_tBtnVolumnUp->setIcon(QIcon(":/resource/instance/volumeAdd.png"));
        m_tBtnVolumnUp->setIconSize(iconMinSize);
        m_tBtnVolumnUp->setText("音量+");
        m_tBtnVolumnUp->setFixedSize(btnMinSize);
        m_tBtnVolumnUp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnVolumnUp);
        break;
    case TYPE_VOLUMN_DOWN:
        m_tBtnVolumnDown = new QToolButton(frame);
        connect(m_tBtnVolumnDown, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnVolumnDown_clicked);
        m_tBtnVolumnDown->setIcon(QIcon(":/resource/instance/volumeSub.png"));
        m_tBtnVolumnDown->setIconSize(iconMinSize);
        m_tBtnVolumnDown->setText("音量-");
        m_tBtnVolumnDown->setFixedSize(btnMinSize);
        m_tBtnVolumnDown->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnVolumnDown);
        break;
    case TYPE_HOR_VER_SCREEN:
        m_tBtnHorVerScreen = new QToolButton(frame);
        connect(m_tBtnHorVerScreen, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnHorOrVer_clicked);
        m_tBtnHorVerScreen->setIcon(QIcon(":/resource/instance/HorVerScreen.png"));
        m_tBtnHorVerScreen->setIconSize(iconMinSize);
        m_tBtnHorVerScreen->setText("横竖屏");
        m_tBtnHorVerScreen->setFixedSize(btnMinSize);
        m_tBtnHorVerScreen->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnHorVerScreen);;
        break;
    case TYPE_CLIPBOARD:
        m_tBtnClipboard = new QToolButton(frame);
        connect(m_tBtnClipboard, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnClipboard_clicked);
        m_tBtnClipboard->setIcon(QIcon(":/resource/instance/clipboard.png"));
        m_tBtnClipboard->setIconSize(iconMinSize);
        m_tBtnClipboard->setText("剪贴板");
        m_tBtnClipboard->setFixedSize(btnMinSize);
        m_tBtnClipboard->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnClipboard);
        break;
    case TYPE_SCREENSHOTS:
        m_tBtnScreenshots = new QToolButton(frame);
        connect(m_tBtnScreenshots, &QToolButton::clicked, this, &PhoneInstanceWidget::on_Screenshot_clicked);
        m_tBtnScreenshots->setIcon(QIcon(":/resource/instance/screenshots.png"));
        m_tBtnScreenshots->setIconSize(iconMinSize);
        m_tBtnScreenshots->setText("截图");
        m_tBtnScreenshots->setFixedSize(btnMinSize);
        m_tBtnScreenshots->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnScreenshots);
        break;
    case TYPE_SCREENSHOTS_DIR:
        m_tBtnScreenshotsFolder = new QToolButton(frame);
        connect(m_tBtnScreenshotsFolder, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnScreenshotDir_clicked);
        m_tBtnScreenshotsFolder->setIcon(QIcon(":/resource/instance/screenshotsDir.png"));
        m_tBtnScreenshotsFolder->setIconSize(iconMinSize);
        m_tBtnScreenshotsFolder->setText("截图\n目录");
        m_tBtnScreenshotsFolder->setFixedSize(btnMaxSize);
        m_tBtnScreenshotsFolder->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnScreenshotsFolder);
        break;
    case TYPE_RESTART:
        m_tBtnRestart = new QToolButton(frame);
        connect(m_tBtnRestart, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnReboot_clicked);
        m_tBtnRestart->setIcon(QIcon(":/resource/instance/restart.png"));
        m_tBtnRestart->setIconSize(iconMinSize);
        m_tBtnRestart->setText("重启");
        m_tBtnRestart->setFixedSize(btnMinSize);
        m_tBtnRestart->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnRestart);
        break;
    case TYPE_RESET_FACTORY_DATA:
        m_tBtnResetFactoryData = new QToolButton(frame);
        connect(m_tBtnResetFactoryData, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnFactoryDataReset_clicked);
        m_tBtnResetFactoryData->setIcon(QIcon(":/resource/instance/factoryDataReset.png"));
        m_tBtnResetFactoryData->setIconSize(iconMinSize);
        m_tBtnResetFactoryData->setText("恢复\n出厂");
        m_tBtnResetFactoryData->setFixedSize(btnMaxSize);
        m_tBtnResetFactoryData->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnResetFactoryData);
        break;
    case TYPE_ROOT:
        m_tBtnRoot = new QToolButton(frame);
        connect(m_tBtnRoot, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnRoot_clicked);
        m_tBtnRoot->setIcon(QIcon(":/resource/instance/Root.png"));
        m_tBtnRoot->setIconSize(iconMinSize);
        m_tBtnRoot->setText("Root");
        m_tBtnRoot->setFixedSize(btnMinSize);
        m_tBtnRoot->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnRoot);
        break;
    case TYPE_SHARK:
        m_tBtnShark = new QToolButton(frame);
        connect(m_tBtnShark, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnShark_clicked);
        m_tBtnShark->setIcon(QIcon(":/resource/instance/shake.png"));
        m_tBtnShark->setIconSize(iconMinSize);
        m_tBtnShark->setText("摇一摇");
        m_tBtnShark->setFixedSize(btnMinSize);
        m_tBtnShark->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnShark);
        break;
    case TYPE_GPS:
        m_tBtnGPS = new QToolButton(frame);
        connect(m_tBtnGPS, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnGPS_clicked);
        m_tBtnGPS->setIcon(QIcon(":/resource/instance/GPS.png"));
        m_tBtnGPS->setIconSize(iconMinSize);
        m_tBtnGPS->setText("GPS");
        m_tBtnGPS->setFixedSize(btnMinSize);
        m_tBtnGPS->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnGPS);
        break;
    case TYPE_CHANGE_KEYBOARD:
        m_tBtnChangeKeyBoard = new QToolButton(frame);
        connect(m_tBtnChangeKeyBoard, &QToolButton::clicked, this, &PhoneInstanceWidget::on_toolBtnChangeKeyBoard_clicked);
        m_tBtnChangeKeyBoard->setIcon(QIcon(":/resource/instance/key_show.png"));
        m_tBtnChangeKeyBoard->setIconSize(iconMinSize);
        m_tBtnChangeKeyBoard->setText("切换\n键盘");
        m_tBtnChangeKeyBoard->setFixedSize(btnMaxSize);
        m_tBtnChangeKeyBoard->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        vBox->addWidget(m_tBtnChangeKeyBoard);
        break;
    default:
        break;
    }
}
void PhoneInstanceWidget::do_SharkSignals()
{
    //摇一摇
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            float fx[] = { 18.140408f, -18.2266f, -18.2266f, 13.803864f };
            float fy[] = { -13.018726f, 12.674904f,14.300858f, -12.225403f };
            float fz[] = { 14.384073f, 0.493712f, 14.386272f, 13.7585411 };

            //SendAccelerometer(fx[m], fy[m], fz[m]);
            for (int m = 0; m < 4; m++)
            {
                int iRet = source->sendInputAccelerometer(fx[m], fy[m], fz[m]);
                qDebug() << "摇一摇 iRet=" << iRet;
            }

        }
    }
}
void PhoneInstanceWidget::do_GPSSignals()
{
    qDebug() << "this->width" << this->width() << "this.height=" << this->height();
    //gps
    Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL)
        {
            m_GeoSource = QGeoPositionInfoSource::createDefaultSource(0);
            if (NULL != m_GeoSource)
            {
                // 连接信号槽，当位置更新时获取经纬度
                QObject::connect(m_GeoSource, &QGeoPositionInfoSource::positionUpdated, this, &PhoneInstanceWidget::onPositionUpdated);
                // 开始获取位置信息
                m_GeoSource->setUpdateInterval(1000); // 设置更新间隔为1000毫秒
                m_GeoSource->startUpdates();
                QGeoPositionInfoSource::Error err = m_GeoSource->error();
                switch (err)
                {
                case QGeoPositionInfoSource::AccessError:
                {
                    MessageTips* tips = new MessageTips("定位权限未打开", this);
                    tips->show();
                }
                break;
                case QGeoPositionInfoSource::ClosedError:
                {
                    MessageTips* tips = new MessageTips("远程定位后端关闭连接，这种情况发生在用户将位置服务切换为off的情况下,一旦位置服务重新启用，定期更新将恢复。", this);
                    tips->show();
                }
                break;
                case QGeoPositionInfoSource::UnknownSourceError:
                {
                    MessageTips* tips = new MessageTips("未知错误", this);
                    tips->show();
                }
                break;
                case QGeoPositionInfoSource::NoError:
                    break;
                case QGeoPositionInfoSource::UpdateTimeoutError:
                {
                    MessageTips* tips = new MessageTips("当前位置不能在指定的超时内检索", this);
                    tips->show();
                }
                break;
                default:
                    break;
                }
                qDebug() << "gps error=" << err;
            }
        }
    }
}

void PhoneInstanceWidget::on_toolBtnChangeKeyBoard_clicked()
{
    //键盘KEY_KEYBOARD
    qDebug() << "do change keyboard signals";
    qDebug() << "this->width" << this->width() << "this.height=" << this->height();
}

