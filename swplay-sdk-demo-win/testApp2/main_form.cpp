#include "framework.h"

#define SW_LOG_TAG "MainForm"
#include "common/log.h"
#include "common/common.h"
#include "common/time_c.h"

#include "SWRuntime.h"
#include "SWDataSource.h"

#include "ControlItem.h"
#include "dui_video_view.h"
#include "virtual_tile_box.h"
#include "control_form.h"
#include "item.h"

#include "main_form.h"

namespace ui {
	Control* MyCreateControlCallback(const std::wstring& sName)
	{
		if (sName == L"DuiVideoView")
		{
			return new DuiVideoView();
		}
		else if (sName == L"VirtualTileBox")
		{
			return new VirtualTileBox();
		}
		return nullptr;
	}
}

//----------------------------------------------------
static void testers_fill(std::list<std::string>& padcodeList, const char* filename, int fromIndex) {
	ALOGV("read padcode from file:%s", filename);
	FILE *fp = NULL;
	if ((fp = fopen(filename, "r")) == NULL) {
		return;
	}
	char buf[32] = { 0 };
	int i = 0;
	while (!feof(fp)) {
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), fp);
		if (i++ < fromIndex) {
			continue;
		}

		if (strlen(buf) > 8) {
			std::string padcode = nbase::StringTrim(buf);
			padcodeList.push_back(padcode);
		}
	}
	fclose(fp);
	ALOGV("read padcode end");
}

static int getDefaultVideoLevel(VideoLevel** vLevels, ControlType type) {
	static VideoLevel videoLevels[4];
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
	videoLevels[2].bitrate = 1024;
	videoLevels[2].resolutionLevel = 3;
	videoLevels[2].videoQuality = 3;
	memcpy(&videoLevels[3], &videoLevels[0], sizeof(VideoLevel));
	videoLevels[3].width = 288;
	videoLevels[3].height = 512;
	//videoLevels[3].width = 144;
	//videoLevels[3].height = 256;
	videoLevels[3].bitrate = 512;
	videoLevels[3].resolutionLevel = 4;
	videoLevels[3].videoQuality = 4;

	if (type == kSubWindow) {
		videoLevels[3].maxfps = 8;
		videoLevels[3].minfps = 8;
		videoLevels[3].gop = videoLevels[3].maxfps * 4;
	}

	*vLevels = videoLevels;
	int videoLevelCount = sizeof(videoLevels) / sizeof(VideoLevel);
	return videoLevelCount;
}

//----------------------------------------------------
const std::wstring MainForm::kClassName = L"MainForm";

MainForm::MainForm()
{
}

MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
	return L"testApp2";
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
	m_BtnAddToControl = dynamic_cast<ui::Button*>(FindControl(L"btn_addto_control"));
	m_PadcodeTree = dynamic_cast<ui::TreeView*>(FindControl(L"padcode_tree"));
	m_pTileList = dynamic_cast<VirtualTileBox*>(FindControl(L"tile_list"));

	ALOG_ASSERT(m_BtnAddToControl == NULL);
	ALOG_ASSERT(m_PadcodeTree == NULL);
	ALOG_ASSERT(m_pTileList == NULL);

	m_BtnAddToControl->AttachClick(nbase::Bind(&MainForm::OnAddToControl, this, std::placeholders::_1));
	m_pTileList->SetMouseFocused(false);
	m_pTileList->EnableScrollBar(false, false);
	m_pTileList->setMainForm(this);
	m_pTileList->DetachEvent(ui::EventType::kEventAll);

	std::wstring run_dir = nbase::win32::GetCurrentModuleDirectory();
	std::string runpath = std::string(run_dir.begin(), run_dir.end());
	std::string idpath = runpath + std::string("\\id.txt");
	testers_fill(m_PadcodeList, idpath.c_str(), 0);

	ui::TreeNode* group_default = new ui::TreeNode();
	group_default->SetClass(L"listitem");
	group_default->SetFixedHeight(30);
	group_default->SetText(L"默认分组");
	m_PadcodeTree->GetRootNode()->AddChildNode(group_default);

	std::list<std::string>::iterator it;
	for (it = m_PadcodeList.begin(); it != m_PadcodeList.end(); it++)
	{
		ui::TreeNode* node = new ui::TreeNode();
		node->SetClass(L"listitem");
		node->SetFixedHeight(20);
		node->SetText(nbase::UTF8ToUTF16((*it).c_str(), (*it).length()));
		node->SetMargin({ 10, 0, 0, 0 });
		group_default->AddChildNode(node);
	}

	std::string confpath = runpath + std::string("\\padinfo.conf");
	m_PadInfo = CPadInfo::InitFromConfigFile(confpath.c_str());
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool MainForm::OnAddToControl(ui::EventArgs* args) {
	if (m_ControlItemList.size() == 0) {
		std::list<std::string>::iterator it;
		for (it = m_PadcodeList.begin(); it != m_PadcodeList.end(); it++){
			std::shared_ptr<ControlItem> item = std::make_shared<ControlItem>((*it));
			m_ControlItemList.push_back(item);
		}

		m_pTileList->Refresh();
	}
	return true;
}

std::shared_ptr<ControlItem> MainForm::GetControlItem(const std::string& padcode) {
	std::list<std::shared_ptr<ControlItem> >::iterator it;
	for (it = m_ControlItemList.begin(); it != m_ControlItemList.end(); it++) {
		std::shared_ptr<ControlItem> item = *it;
		if (padcode == item->getPadcode()) {
			return item;
		}
	}
	std::shared_ptr<ControlItem> none;
	return none;
}

void MainForm::OnPlayStart(const std::string& padcode) {
	std::shared_ptr<ControlItem> bigControlItem = GetControlItem(padcode);
	if (bigControlItem->getControlType() == kMainWindow) {
		return;
	}
	bigControlItem->changleTo(kMainWindow);

	std::string packageName = "";
	std::string controlAddr = m_PadInfo->GetControlAddr();

	int controlPort = m_PadInfo->GetControlPort();
	int userID = m_PadInfo->GetUserID();
	std::string sessionID = m_PadInfo->GetSessionID();

	int encodetype = -1;//2;
	int resolutionLevel = -1;//3;
	int videoQuality = -1;
	int apiLevel = 1;
	int playType = 3;

	VideoLevel* videoLevels = NULL;
	int videoLevelCount = getDefaultVideoLevel(&videoLevels, kMainWindow);
	std::shared_ptr<SWDataSource> datasource = std::make_shared<SWDataSource>(SWRuntime::getInstance()->obtainID(), bigControlItem.get());
	datasource->setLoginParams(controlAddr.c_str(), controlPort, userID, sessionID.c_str(), bigControlItem->getPadcode().c_str(), 0);
	datasource->setPlayParams(packageName.c_str(), encodetype, 0, 0,
		0, 0, 0, 0, resolutionLevel, videoQuality, playType, apiLevel, 0);

	datasource->setVideoLevels(videoLevels, videoLevelCount);
	datasource->setVideoLevel(0);
	datasource->setBusinessType(1);

	bigControlItem->setDataSource(datasource);

	// 创建一个主播放窗口
	ControlForm* window = new ControlForm(this, bigControlItem);
	window->Create(NULL, ControlForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
	m_WindowCount++;

	nbase::ThreadManager::PostDelayedTask(kThreadGlobalMisc, nbase::Bind(&MainForm::OnGroupControlStart, this), nbase::TimeDelta::FromMilliseconds(400));
}

void MainForm::OnPlayStop(std::shared_ptr<ControlItem>& controlItem) {
	--m_WindowCount;
	if(m_WindowCount > 0){
		controlItem->changleTo(kSubWindow);
	}else{
		OnGroupControlStop();
		//nbase::ThreadManager::PostTask(kThreadGlobalMisc, nbase::Bind(&MainForm::OnGroupControlStop, this));
	}
}

void MainForm::OnGroupControlStart() {
	Mutex::Autolock lock(m_Mutex);
	if (!(m_GroupControlStatus == GROUP_CONTROL_NOP || MODULE_STATUS_ISSET(m_GroupControlStatus, GROUP_CONTROL_STOPED))) {
		return;
	}
	int64_t t1 = get_time_now_ms();
	MODULE_STATUS_SET(m_GroupControlStatus, GROUP_CONTROL_STARTING);
	std::string packageName = "";
	std::string controlAddr = m_PadInfo->GetControlAddr();

	int controlPort = m_PadInfo->GetControlPort();
	int userID = m_PadInfo->GetUserID();
	std::string sessionID = m_PadInfo->GetSessionID();

	int encodetype = -1;//2;
	int resolutionLevel = -1;//3;
	int videoQuality = -1;
	int apiLevel = 1;
	
	int businessType = 1;
	if (businessType == 0) {
		apiLevel = 2;
	}

	VideoLevel* videoLevels = NULL;
	int videoLevelCount = getDefaultVideoLevel(&videoLevels, kSubWindow);
	int usingLevel = 4;
	VideoLevel* usingVideoLevel = &videoLevels[usingLevel];

	std::list<std::shared_ptr<ControlItem> >::iterator it;
	for (it = m_ControlItemList.begin(); it != m_ControlItemList.end(); it++) {
		int playType = 1;
		std::shared_ptr<ControlItem>& item = *it;
		if (item->getControlType() == kMainWindow) {
			continue;
		}
		else if (item->getControlType() == kNoneWindow) {
			playType = 0;
		}

		std::shared_ptr<SWDataSource> datasource = std::make_shared<SWDataSource>(SWRuntime::getInstance()->obtainID(), item.get());
		datasource->setLoginParams(controlAddr.c_str(), controlPort, userID, sessionID.c_str(), item->getPadcode().c_str(), 0);
		datasource->setPlayParams(packageName.c_str(), encodetype, 0, 0,
			0, 0, 0, 0, resolutionLevel, videoQuality, playType, apiLevel, 0);

		datasource->setVideoLevels((VideoLevel*)&videoLevels[0], videoLevelCount);
		datasource->setVideoLevel(usingLevel);
		datasource->setBusinessType(businessType);
		datasource->setBufferSize(512 * 1024);
		item->setDataSource(datasource);
		item->setVideoResolution(usingVideoLevel->width, usingVideoLevel->height);
		item->start(m_PadInfo->haveHWaccel());
	}

	MODULE_STATUS_CLR(m_GroupControlStatus, GROUP_CONTROL_STARTING);
	MODULE_STATUS_SET(m_GroupControlStatus, GROUP_CONTROL_STARTED);
	int64_t t2 = get_time_now_ms();
	ALOGI("OnGroupControlStart, using time:%d", (int)(t2-t1));
}

void MainForm::OnGroupControlStop() {
	Mutex::Autolock lock(m_Mutex);
	MODULE_STATUS_CLR(m_GroupControlStatus, GROUP_CONTROL_STARTED);
	MODULE_STATUS_SET(m_GroupControlStatus, GROUP_CONTROL_STOPPING);
	int64_t t1 = get_time_now_ms();
	std::list<std::shared_ptr<ControlItem> >::iterator it;
	for (it = m_ControlItemList.begin(); it != m_ControlItemList.end(); it++) {
		std::shared_ptr<ControlItem>& item = *it;

		item->stop();
	}
	MODULE_STATUS_CLR(m_GroupControlStatus, GROUP_CONTROL_STOPPING);
	MODULE_STATUS_SET(m_GroupControlStatus, GROUP_CONTROL_STOPED);
	int64_t t2 = get_time_now_ms();
	ALOGI("OnGroupControlStop, using time:%d", (int)(t2 - t1));
}

