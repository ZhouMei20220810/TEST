#include "framework.h"
#define SW_LOG_TAG "Item"
#include "common/log.h"
#include "common/time_c.h"

#include "dui_video_view.h"
#include "main_form.h"
#include "virtual_tile_box.h"
#include "item.h"

Item::Item(MainForm* mainForm, const std::string& padcode){
	ALOGV("ctor(%p)", this);
	m_MainForm = mainForm;
	m_Padcode = padcode;
}

Item::~Item() {
	ALOGV("dtor(%p)", this);
}

void Item::InitSubControls()
{
	// 查找 Item 下的控件
	if (m_LabelTitle == nullptr)
	{
		m_LabelTitle = dynamic_cast<ui::Label*>(FindSubControl(L"sub_label_title"));
		m_Display = dynamic_cast<ui::DuiVideoView*>(FindSubControl(L"sub_video_view"));

		ALOG_ASSERT(m_LabelTitle == NULL);
		ALOG_ASSERT(m_Display == NULL);

		m_LabelTitle->SetText(nbase::UTF8ToUTF16(m_Padcode.c_str(), m_Padcode.length()));
		m_Display->AttachButtonUp(nbase::Bind(&Item::OnItemDoubleClick, this, std::placeholders::_1));
	}
}

bool Item::OnItemDoubleClick(ui::EventArgs* args) {
	int64_t now = get_time_now_ms();
	if (now - m_LastClickTime < 300) {
		m_MainForm->OnPlayStart(m_Padcode);
	}
	else {
		m_LastClickTime = now;
	}
	return true;
}

ui::DuiVideoView* Item::getDisplay() {
	return m_Display;
}