#include "framework.h"

#define SW_LOG_TAG "ControlForm"
#include "common/log.h"
#include "common/time_c.h"

#include "Keyboard.h"
#include "SWDataSource.h"
#include "SWPlayer.h"

#include "dui_video_view.h"
#include "ControlItem.h"
#include "main_form.h"

#include "control_form.h"

const std::wstring ControlForm::kClassName = L"ControlForm";

const std::wstring kPicQuality[] = { L"自动", L"高清", L"普清", L"快速", L"极速" };
const int kPicQualityLen = 5;

ControlForm::ControlForm(MainForm* mainForm, std::shared_ptr<ControlItem> controlItem)
{
	m_MainForm = mainForm;
	m_ControlItem = controlItem;
}

ControlForm::~ControlForm()
{
}

std::wstring ControlForm::GetSkinFolder()
{
	return L"testApp2";
}

std::wstring ControlForm::GetSkinFile()
{
	return L"control_form.xml";
}

std::wstring ControlForm::GetWindowClassName() const
{
	return kClassName;
}

void ControlForm::InitWindow()
{
	m_Display = dynamic_cast<ui::DuiVideoView*>(FindControl(L"video_view"));

	m_BtnBack = dynamic_cast<ui::Button*>(FindControl(L"btn_back"));
	m_BtnHome = dynamic_cast<ui::Button*>(FindControl(L"btn_home"));
	m_BtnMenu = dynamic_cast<ui::Button*>(FindControl(L"btn_menu"));

	m_PicQuality = dynamic_cast<ui::Combo*>(FindControl(L"combo_pic_quality"));
	m_GroupControl = dynamic_cast<ui::CheckBox*>(FindControl(L"cbx_group_control"));

	ALOG_ASSERT(m_Display == NULL);
	ALOG_ASSERT(m_GroupControl == NULL);
	ALOG_ASSERT(m_BtnBack == NULL);
	ALOG_ASSERT(m_BtnHome == NULL);
	ALOG_ASSERT(m_BtnMenu == NULL);
	ALOG_ASSERT(m_PicQuality == NULL);


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
	m_PicQuality->AttachSelect(nbase::Bind(&ControlForm::OnPicQualitySelect, this, std::placeholders::_1));

	m_BtnBack->AttachClick(nbase::Bind(&ControlForm::OnClickBack, this, std::placeholders::_1));
	m_BtnHome->AttachClick(nbase::Bind(&ControlForm::OnClickHome, this, std::placeholders::_1));
	m_BtnMenu->AttachClick(nbase::Bind(&ControlForm::OnClickMenu, this, std::placeholders::_1));

	m_Display = dynamic_cast<ui::DuiVideoView*>(FindControl(L"video_view"));
	ALOG_ASSERT(m_Display == NULL);

	m_Display->InitMouseEvents(this);

	std::shared_ptr<DataSource> datasource = m_ControlItem->getDataSource();
	m_Player = std::make_shared<SWPlayer>();
	m_Player->setDataSource(datasource);
	m_Player->setDisplay(m_Display);

	m_Player->start();

	datasource->screenCast(1);
}

LRESULT ControlForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Mutex::Autolock lock(m_Mutex);
	if(m_Player.get() != NULL){
		m_Player->getDataSource()->screenCast(0);
		m_Player->detachDataSource();

		m_Player->stop();
		m_Player.reset();
	}

	m_MainForm->OnPlayStop(m_ControlItem);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool ControlForm::OnPicQualitySelect(ui::EventArgs* args) {
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

bool ControlForm::OnClickBack(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN| SW_ACTION_KEY_UP, KEY_BACK);
		}

		OnKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_BACK);
	}
	return true;
}
bool ControlForm::OnClickHome(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_HOMEPAGE);
		}

		OnKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_HOMEPAGE);
	}
	return true;
}
bool ControlForm::OnClickMenu(ui::EventArgs* args) {
	Mutex::Autolock lock(m_Mutex);
	if (m_Player.get() != NULL) {
		DataSource* source = m_Player->getDataSource();
		if (source != NULL) {
			source->sendKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_MENU);
		}

		OnKeyEvent(SW_ACTION_KEY_DOWN | SW_ACTION_KEY_UP, KEY_MENU);
	}
	return true;
}

void ControlForm::OnKeyEvent(int action, int keyCode) {
	if (!m_GroupControl->IsSelected()) {
		return;
	}
	std::list<std::shared_ptr<ControlItem> > itemList = m_MainForm->GetControlItemList();
	std::list<std::shared_ptr<ControlItem> >::iterator it;
	for (it = itemList.begin(); it != itemList.end(); it++) {
		std::shared_ptr<ControlItem>& item = *it;
		if (item.get() == m_ControlItem.get()) {
			continue;
		}

		std::shared_ptr<DataSource> datasource = item->getDataSource();
		if (datasource.get() != NULL) {
			datasource->sendKeyEvent(action, keyCode);
		}
	}
}

void ControlForm::OnTouchEvent(int eventAction, int pointerCount, float percentX, float percentY, float force) {
	if (!m_GroupControl->IsSelected()) {
		return;
	}
	//int64_t t1 = get_time_now_ms();
	std::list<std::shared_ptr<ControlItem> > itemList = m_MainForm->GetControlItemList();
	std::list<std::shared_ptr<ControlItem> >::iterator it;
	for (it = itemList.begin(); it != itemList.end(); it++) {
		std::shared_ptr<ControlItem>& item = *it;
		if (item.get() == m_ControlItem.get()) {
			continue;
		}

		item->OnTouchEvent(eventAction, pointerCount, percentX, percentY, force);

		//std::shared_ptr<DataSource> datasource = item->getDataSource();
		//if (datasource.get() != NULL) {
		//	datasource->sendTouchEvent(eventAction, pointerCount, x, y, force);
		//}
	}
	//int64_t t2 = get_time_now_ms();
	//ALOGI("OnTouchEvent, using time:%d", (int)(t2-t1));
}
