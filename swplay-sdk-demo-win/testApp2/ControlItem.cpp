#include "framework.h"
#include "Keyboard.h"
#include "SWDataSource.h"
#include "SWPlayer.h"

#include "item.h"
#include "dui_video_view.h"
#include "ControlItem.h"

ControlItem::ControlItem(const std::string& padcode) {
	m_ControlType = kNoneWindow;
	m_Padcode = padcode;
}

ControlItem::~ControlItem() {

}

void ControlItem::onReconnecting(int NthTime) {

}
void ControlItem::onConnected() {

}
void ControlItem::onDisconnected(int errcode) {

}

void ControlItem::onDisconnected(int errcode, const char* errmsg) {

}

void ControlItem::onPlayInfo(const char* info) {
	//std::wstring msg = nbase::StringPrintf(L"%s", info);
	//nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainForm::OnShowMessage, this, msg));
}

void ControlItem::onControlVideo(int videoQuality, int fps) {
	VideoLevel videoLevel;
	int rs = m_DataSource->getVideoLevelResult(&videoLevel);
	if (rs == 0) {
		m_VideoWidth = videoLevel.width;
		m_VideoHeight = videoLevel.height;
	}
}

void ControlItem::changleTo(ControlType type) {
	if (m_ControlType == type) {
		return;
	}
	if (m_ControlType == kMainWindow) {
		//nothing todo
	}else if (m_ControlType == kSubWindow) {
		if (m_DataSource.get() != NULL) {
			m_DataSource->screenCast(0);
		}
		if (m_Player.get() != NULL) {
			m_Player->detachDataSource();

			m_Player->stop();
			m_Player.reset();
		}
	}
	else {
		//nothing todo
	}

	if (type == kMainWindow) {
		//nothing todo
	}
	else if (type == kSubWindow) {
		if (m_Control != NULL && m_DataSource.get() != NULL) {
			if (m_Player.get() == NULL) {
				m_Player = std::make_shared<SWPlayer>();
				m_Player->setDataSource(m_DataSource);
				m_Player->setDisplay(m_Control->getDisplay());

				if (m_EnableHwaccel) {
					m_Player->enableHWaccel();
				}

				m_Player->start();

				m_DataSource->screenCast(1);
				m_DataSource->setVideoLevel(4);
			}
		}
	}
	else {
		if (m_DataSource.get() != NULL) {
			m_DataSource->screenCast(0);
		}
	}

	m_ControlType = type;
}

void ControlItem::setDataSource(const std::shared_ptr<DataSource>& dataSource) {
	m_DataSource = dataSource;
}

int ControlItem::start(bool enableHwaccel) {
	if(m_Control != NULL){
		if (m_Player.get() == NULL) {
			m_Player = std::make_shared<SWPlayer>();
			m_Player->setDataSource(m_DataSource);
			m_Player->setDisplay(m_Control->getDisplay());

			if (enableHwaccel) {
				m_Player->enableHWaccel();
			}

			m_Player->start();
		}
	}
	else {
		if (m_DataSource != NULL) {
			m_DataSource->start();
		}
	}

	m_EnableHwaccel = enableHwaccel;
	return 0;
}
void ControlItem::stop() {
	if(m_Player.get() != NULL){
		m_Player->stop();
		m_Player.reset();
	}

	if (m_DataSource.get() != NULL) {
		m_DataSource->stop();
		m_DataSource.reset();
	}
	if (m_Control != NULL) {
		m_ControlType = kSubWindow;
	}
	else {
		m_ControlType = kNoneWindow;
	}
}

void ControlItem::OnTouchEvent(int eventAction, int pointerCount, float percentX, float percentY, float force) {
	if (m_DataSource.get() != NULL) {
		int x[1] = { -1 };
		int y[1] = { -1 };
		float f[1] = { 0 };

		if(eventAction != SW_ACTION_UP){
			x[0] = (int)(m_VideoWidth * percentX);
			y[0] = (int)(m_VideoHeight * percentY);
		}
		else {
			x[0] = -1;
			y[0] = -1;
		}

		m_DataSource->sendTouchEvent(eventAction, pointerCount, x, y, f);
	}
}
