#include "framework.h"

#define SW_LOG_TAG "MainForm"
#include "common/log.h"

#include "Keyboard.h"
#include "SWDataSource.h"
#include "SWPlayer.h"

#include "dui_video_view.h"
#include "main_form.h"

const std::wstring MainForm::kClassName = L"MainForm";

const std::wstring kPicQuality[] = { L"自动", L"高清", L"普清", L"快速", L"极速" };
const int kPicQualityLen = 5;

MainForm::MainForm()
{
}

MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
	return L"testApp";
}

std::wstring MainForm::GetSkinFile()
{
	return L"main_form.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
	return kClassName;
}

void MainForm::InitWindow()
{
	m_EditPadcode = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_padcode"));
	m_EditPackageName = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_package_name"));
	m_EditControlAddr = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_control_addr"));
	m_EditControlPort = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_control_port"));
	m_EditUserID = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_user_id"));
	m_EditSessionID = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_session_id"));
	m_PicQuality = dynamic_cast<ui::Combo*>(FindControl(L"combo_pic_quality"));
	m_OptionPlayType = dynamic_cast<ui::Option*>(FindControl(L"option_is_sw"));

	m_BtnStart = dynamic_cast<ui::Button*>(FindControl(L"btn_start"));

	m_BtnBack = dynamic_cast<ui::Button*>(FindControl(L"btn_back"));
	m_BtnHome = dynamic_cast<ui::Button*>(FindControl(L"btn_home"));
	m_BtnMenu = dynamic_cast<ui::Button*>(FindControl(L"btn_menu"));

	m_MsgShow = dynamic_cast<ui::Label*>(FindControl(L"label_msg_show"));
	m_ConMsg = dynamic_cast<ui::Label*>(FindControl(L"label_conn_msg"));

	ALOG_ASSERT(m_EditPadcode == NULL);
	ALOG_ASSERT(m_EditPackageName == NULL);
	ALOG_ASSERT(m_EditControlAddr == NULL);
	ALOG_ASSERT(m_EditControlPort == NULL);
	ALOG_ASSERT(m_EditUserID == NULL);
	ALOG_ASSERT(m_EditSessionID == NULL);
	ALOG_ASSERT(m_BtnStart == NULL);
	ALOG_ASSERT(m_BtnBack == NULL);
	ALOG_ASSERT(m_BtnHome == NULL);
	ALOG_ASSERT(m_BtnMenu == NULL);
	ALOG_ASSERT(m_PicQuality == NULL);
	ALOG_ASSERT(m_OptionPlayType == NULL);
	ALOG_ASSERT(m_MsgShow == NULL);
	ALOG_ASSERT(m_ConMsg == NULL);


	/** 
	设置测试信息
	*/
	m_EditPadcode->SetText(L"VM192168102212");
	m_EditControlAddr->SetText(L"10.100.0.253");
	m_EditControlPort->SetText(L"19955");
	m_EditUserID->SetText(L"10000");
	m_EditSessionID->SetText(L"session");


	for (auto i = 0; i < kPicQualityLen; i++){
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(kPicQuality[i]);
		m_PicQuality->Add(element);
	}
	m_PicQuality->SelectItem(0);
	m_PicQuality->AttachSelect(nbase::Bind(&MainForm::OnPicQualitySelect, this, std::placeholders::_1));

	m_BtnStart->AttachClick(nbase::Bind(&MainForm::OnPlayStart, this, std::placeholders::_1));
	m_BtnBack->AttachClick(nbase::Bind(&MainForm::OnClickBack, this, std::placeholders::_1));
	m_BtnHome->AttachClick(nbase::Bind(&MainForm::OnClickHome, this, std::placeholders::_1));
	m_BtnMenu->AttachClick(nbase::Bind(&MainForm::OnClickMenu, this, std::placeholders::_1));

	m_Display = dynamic_cast<ui::DuiVideoView*>(FindControl(L"video_view"));
	ALOG_ASSERT(m_Display == NULL);
	m_Display->InitMouseEvents();

	//m_Display->AttachDoubleClick(nbase::Bind(&MainForm::OnItemDoubleClick, this, std::placeholders::_1));
}

bool MainForm::OnItemDoubleClick(ui::EventArgs* args) {
	printf("OnItemDoubleClick");
	return true;
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Mutex::Autolock lock(m_Mutex);
	OnPlayStop(true);

	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void MainForm::onReconnecting(int NthTime) {
	std::wstring msg = nbase::StringPrintf(L"第%d次重连\n", NthTime);
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnConnMessage, this, msg));
}
void MainForm::onConnected() {
	std::wstring msg = nbase::StringPrintf(L"已连接\n");
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnConnMessage, this, msg));
}
void MainForm::onDisconnected(int errcode) {
	std::wstring msg = nbase::StringPrintf(L"已断开:%d\n", errcode);
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnDisconnMessage, this, msg));
}

void MainForm::onDisconnected(int errcode, const char* errmsg) {
	std::wstring msg = nbase::StringPrintf(L"已断开:%d, %s\n", errcode, errmsg);
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnDisconnMessage, this, msg));
}

void MainForm::onPlayInfo(const char* info) {
	//std::wstring msg = nbase::StringPrintf(L"%s", info);
	//nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnShowMessage, this, msg));
}

void MainForm::OnShowMessage(const std::wstring& msg) {
	m_ConMsg->SetText(msg);
}

void MainForm::OnConnMessage(const std::wstring& msg) {
	std::wstring text = m_ConMsg->GetText();
	text += msg;
	m_ConMsg->SetText(text);
}

void MainForm::OnDisconnMessage(const std::wstring& msg) {
	std::wstring text = m_ConMsg->GetText();
	text += msg;
	m_ConMsg->SetText(text);

	OnPlayStop(false);
}

bool MainForm::OnPlayStart(ui::EventArgs* args)
{
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() == NULL) {
		do {
			std::wstring edit_padcode = m_EditPadcode->GetText();
			std::wstring edit_packageName = m_EditPackageName->GetText();
			std::wstring edit_controlAddr = m_EditControlAddr->GetText();
			std::wstring edit_controlPort = m_EditControlPort->GetText();
			std::wstring edit_userID = m_EditUserID->GetText();
			std::wstring edit_sessionID = m_EditSessionID->GetText();

			int picQualityIndex = m_PicQuality->GetCurSel();
			int businessType = m_OptionPlayType->IsSelected() ? 0 : 1;

			std::string padcode;
			std::string packageName;
			std::string controlAddr;
			
			int controlPort = 0;
			int userID = 0;
			std::string sessionID;

			if (!edit_padcode.empty()) {
				padcode = std::string(edit_padcode.begin(), edit_padcode.end());
			}
			if (!edit_packageName.empty()) {
				packageName = std::string(edit_packageName.begin(), edit_packageName.end());
			}
			if (!edit_controlAddr.empty()) {
				controlAddr = std::string(edit_controlAddr.begin(), edit_controlAddr.end());
			}
			if (!edit_controlPort.empty()) {
				std::string str_port = std::string(edit_controlPort.begin(), edit_controlPort.end());
				controlPort = (!str_port.empty()) ? atoi(str_port.c_str()) : 19955;
			}
			if (!edit_userID.empty()) {
				std::string str_userID = std::string(edit_userID.begin(), edit_userID.end());
				userID = (!str_userID.empty()) ? atoi(str_userID.c_str()) : 10000;
			}
			if (!edit_sessionID.empty()) {
				sessionID = std::string(edit_sessionID.begin(), edit_sessionID.end());
			}

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
			int playType = 3;

			m_Player = std::make_shared<SWPlayer>();
			std::shared_ptr<SWDataSource> datasource = std::make_shared<SWDataSource>(m_Player->getId(), this);
			datasource->setLoginParams(controlAddr.c_str(), controlPort, userID, sessionID.c_str(), padcode.c_str(), 0);
			datasource->setPlayParams(packageName.c_str(), encodetype, 0, 0,
				0, 0, 0, 0, resolutionLevel, videoQuality, playType, apiLevel, 0);

			datasource->setVideoLevels((VideoLevel*)&videoLevels[0], videoLevelCount);
			datasource->setVideoLevel(picQualityIndex);

			datasource->setBusinessType(businessType);

			m_Player->setDataSource(datasource);
			m_Player->setDisplay(m_Display);

			m_Player->start();

			m_EditPadcode->SetReadOnly(true);
			m_BtnStart->SetText(L"结束投屏");

			m_MsgShow->SetText(L"");
			m_ConMsg->SetText(L"");

			return true;
		} while (0);
	}
	else {
		OnPlayStop(false);
	}
	return true;
}

void MainForm::OnPlayStop(bool quit) {
	if (m_Player.get() != NULL) {
		if(!quit){
			m_EditPadcode->SetReadOnly(false);
			m_BtnStart->SetText(L"开始投屏");
		}

		m_Player->stop();
		m_Player.reset();
	}
}

bool MainForm::OnPicQualitySelect(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			int picQualityIndex = m_PicQuality->GetCurSel();
			source->setVideoLevel(picQualityIndex);
		}
	}
	return true;
}

bool MainForm::OnClickBack(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN| SW_ACTION_KEY_UP, KEY_BACK);
		}
	}
	return true;
}
bool MainForm::OnClickHome(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_HOMEPAGE);
		}
	}
	return true;
}
bool MainForm::OnClickMenu(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_MENU);
		}
	}
	return true;
}
