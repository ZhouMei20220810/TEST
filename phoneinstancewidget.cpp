#include "phoneinstancewidget.h"
#include "ui_phoneinstancewidget.h"
#include <QDir>
#include <QClipboard>
#include "messagetips.h"
#include "Keyboard.h"
#include "SWDataSource.h"
#include "messagetips.h"
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "messagetipsdialog.h"

PhoneInstanceWidget::PhoneInstanceWidget(S_PHONE_INFO sPhoneInfo,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PhoneInstanceWidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowFlag(Qt::FramelessWindowHint);
    
    ui->frameTool->setVisible(false);
    setToolBtnVisible(GlobalData::bVerticalPhoneInstance);
    if (GlobalData::bVerticalPhoneInstance)
        resize(PHONE_INSTANCE_VERTICAL_WIDTH, PHONE_INSTANCE_VERTICAL_WIDTH);
    else
        resize(PHONE_INSTANCE_HORIZONTAL_WIDTH, PHONE_INSTANCE_HORIZONTAL_HEIGHT);
    m_toolObject = new ToolObject(this);
    m_PhoneInfo = sPhoneInfo;
    m_strPhoneList.clear();
    m_strPhoneList << sPhoneInfo.strInstanceNo;//同步操作时，同时传入所有选中的item
    ui->toolBtnShow->setVisible(false);

    ui->toolBtnPhoneInstance->setText(sPhoneInfo.strInstanceNo);

    m_Player = NULL;
    //m_Display = new VideoViewWidget(this);
    //m_Display->move(0, 100);
    //m_Display->resize(ui->labelPhone->size());

    QString strUUID = GlobalData::strAccount;
    QString strPadCode = sPhoneInfo.strInstanceNo;
    QString strOnlineTime = sPhoneInfo.strExpireTime;
    QDateTime time = QDateTime::fromString(strOnlineTime,"yyyy-MM-dd hh:mm:ss");
    qint64 i64Time = time.toSecsSinceEpoch();//time.toMSecsSinceEpoch();
    qDebug() << "i64Time(s)=" << i64Time<<"i64Time(ms)="<< time.toMSecsSinceEpoch();

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
    request.setRawHeader("Authorization", strToken.toLocal8Bit()); //strToken.toLocal8Bit());
    qDebug() << "token:   " << strToken;
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
                        QJsonObject dataObj = obj["data"].toObject();
                        QJsonObject data = dataObj["data"].toObject();
                        m_strSessionId = data["sessionId"].toString();
                        m_strDomain = data["domain"].toString();
                        m_strControlTactics = data["controlTactics"].toString();
                        qDebug() << "m_strSessionId = " << m_strSessionId;
                        qDebug() << "m_strControlCode = " << m_strControlCode;
                        QJsonObject control;
                        QJsonArray controlArray = data["controlList"].toArray();
                        for (int iControl = 0; iControl < controlArray.size(); iControl++)
                        {
                            control = controlArray.at(iControl).toObject();
                            m_strControlCode = control["controlCode"].toString();
                            
                            if (control["controlInfoList"].isArray())
                            {
                                QJsonArray controlList = control["controlInfoList"].toArray();
                                int iSize = controlList.size();
                                QJsonObject info;
                                for (int i = 0; i < iSize; i++)
                                {
                                    info = controlList.at(i).toObject();
                                    m_strControlIp = info["controlIp"].toString();
                                    m_dControlPort = info["controlPort"].toDouble();
                                    m_strTraceServer = info["traceServer"].toString();
                                    qDebug() << "m_strControlIp=" << m_strControlIp << "m_dControlPort=" << m_dControlPort << "strTraceServer=" << m_strTraceServer;
                                }
                            }
                        }
                         
                        onPlayStart();
                    }
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

PhoneInstanceWidget::~PhoneInstanceWidget()
{
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
    if (window()->isMaximized()) {
        return;
    }
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
        this->showNormal();
    }
    else {
        ui->toolBtnMax->setIcon(QIcon(":/resource/instance/restore.png"));
        QString styleSheet = QString("QToolButton{border:none;color: rgb(204, 204, 204);margin-bottom:2px;}QToolButton:hover {color: rgb(255, 255, 255);}");
        ui->toolBtnMax->setStyleSheet(styleSheet);
        this->showMaximized();
    }
}


void PhoneInstanceWidget::on_toolBtnClose_clicked()
{
    this->close();
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


void PhoneInstanceWidget::on_toolBtnSignal_clicked()
{
    //信号
}
void PhoneInstanceWidget::setToolBtnVisible(bool bVisible)
{
    ui->toolBtnMore->setVisible(!bVisible);
    ui->toolBtnScreenshot->setVisible(bVisible);
    ui->toolBtnScreenshotsDir->setVisible(bVisible);
    ui->toolBtnClipboard->setVisible(bVisible);
    ui->toolBtnShark->setVisible(bVisible);
    ui->toolBtnRoot->setVisible(bVisible);
    ui->toolBtnKeyboard->setVisible(bVisible);
    ui->toolBtnADB->setVisible(bVisible);
}

void PhoneInstanceWidget::on_toolBtnVerOrHor_clicked()
{
    this->close();

    GlobalData::bVerticalPhoneInstance = !GlobalData::bVerticalPhoneInstance;
    PhoneInstanceWidget* nn = new PhoneInstanceWidget(m_PhoneInfo);
    nn->show();
}


void PhoneInstanceWidget::on_toolBtnScreenshot_clicked()
{
    //截图
    //只需要截图，不需要获取当前页面
    this->m_toolObject->HttpPostInstanceScreenshot(m_strPhoneList);
}


void PhoneInstanceWidget::on_toolBtnAddVolume_clicked()
{
    //音量调大
}


void PhoneInstanceWidget::on_toolBtnSubVolume_clicked()
{
    //音量调小
}


void PhoneInstanceWidget::on_toolBtnClipboard_clicked()
{
    //剪贴板
}

void PhoneInstanceWidget::on_toolBtnInput_clicked()
{
    //输入协助
}


void PhoneInstanceWidget::on_toolBtnScreenshotsDir_clicked()
{
    //截图目录
}


void PhoneInstanceWidget::on_toolBtnShark_clicked()
{
    //摇一摇
}


void PhoneInstanceWidget::on_toolBtnRoot_clicked()
{
    //root
}


void PhoneInstanceWidget::on_toolBtnKeyboard_clicked()
{
    //硬键盘
}


void PhoneInstanceWidget::on_toolBtnADB_clicked()
{
    //ADB
}

void PhoneInstanceWidget::on_toolBtnReturn_clicked()
{
	Mutex::Autolock lock(m_Mutex);
    if(m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if(source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN|SW_ACTION_KEY_UP, KEY_BACK);
        }
    }
}


void PhoneInstanceWidget::on_toolBtnHome_clicked()
{
	Mutex::Autolock lock(m_Mutex);
    if(m_Player != NULL)
    {
        DataSource* source = m_Player->getDataSource();
        if(source != NULL)
        {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN|SW_ACTION_KEY_UP, KEY_HOMEPAGE);
        }
    }
}


void PhoneInstanceWidget::on_toolBtnChangePage_clicked()
{
	Mutex::Autolock lock(m_Mutex);
    if (m_Player != NULL) {
        DataSource* source = m_Player->getDataSource();
        if (source != NULL) {
            source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_MENU);
        }
    }
}

bool PhoneInstanceWidget::onPlayStart()
{
	Mutex::Autolock lock(m_Mutex);
	if (m_Player == NULL) 
	{
		do {

            //QString strQuality = ui->comboBoxQuality->currentText();//ui->comboBoxQuality->GetCurSel();
            int picQualityIndex = 2;//1;//ui->comboBoxQuality->currentIndex();
            //PAAS(试玩): BusinessType为0; 主营: BusinessType为1; 百度: BusinessType为2
            int businessType = 1;//m_OptionPlayType->IsSelected() ? 0 : 1;

			std::string padcode = ui->toolBtnPhoneInstance->text().toStdString();
            std::string packageName = "";//可以为空
            std::string controlAddr = m_strControlIp.toStdString();//"120.26.132.153";

            int controlPort = m_dControlPort;//端口
            int userID = GlobalData::id;
            std::string sessionID=m_strSessionId.toStdString();

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
            m_Player->setDisplay(ui->videoViewWidget);
            //开始投屏
			m_Player->start();
			return true;
		} while (0);
	}
	return true;
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
    QString strTips = QString("onPlayInfo:%1").arg(info);
    qDebug() << strTips;
    //MessageTips* tips = new MessageTips(strTips);
    //tips->show();
    //std::wstring msg = nbase::StringPrintf(L"%s", info);
    //nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnShowMessage, this, msg));
}

void PhoneInstanceWidget::on_toolBtnMore_clicked()
{
    bool bVisible = ui->frameTool->isVisible();
    ui->frameTool->setParent(ui->videoViewWidget);
    ui->frameTool->move(ui->videoViewWidget->width()-ui->frameTool->width()-2, 50);
    ui->frameTool->setVisible(!bVisible);
}


void PhoneInstanceWidget::on_toolBtnScreenshot_2_clicked()
{
    on_toolBtnScreenshot_clicked();
}


void PhoneInstanceWidget::on_toolBtnScreenshotsDir_2_clicked()
{
    on_toolBtnScreenshotsDir_clicked();
}


void PhoneInstanceWidget::on_toolBtnClipboard_2_clicked()
{
    on_toolBtnClipboard_clicked();
}


void PhoneInstanceWidget::on_toolBtnShark_2_clicked()
{
    on_toolBtnShark_clicked();
}


void PhoneInstanceWidget::on_toolBtnRoot_2_clicked()
{
    on_toolBtnRoot_clicked();
}


void PhoneInstanceWidget::on_toolBtnKeyboard_2_clicked()
{
    on_toolBtnKeyboard_clicked();
}


void PhoneInstanceWidget::on_toolBtnADB_2_clicked()
{
    on_toolBtnADB_clicked();
}
