#pragma once

#include <SWDataSourceListener.h>

enum ControlType{
	kNoneWindow = 0,
	kMainWindow = 1,
	kSubWindow = 2,
};

class DataSource;
class SWPlayer;
class Item;

class ControlItem: public SWDataSourceListener
{
public:
	ControlItem(const std::string& padcode);
	~ControlItem();

	// SWDataSourceListener implementation
	void onReconnecting(int NthTime) override;
	void onConnected() override;
	void onDisconnected(int errcode) override;
	void onDisconnected(int errcode, const char* errmsg) override;
	void onPlayInfo(const char* info) override;
	void onControlVideo(int videoQuality, int fps) override;

	Item* getControl() { return m_Control; }
	void setControl(Item* control) { m_Control = control; }

	const std::string& getPadcode() { return m_Padcode; }

	ControlType getControlType() { return m_ControlType; }

	void changleTo(ControlType type);

	std::shared_ptr<DataSource> getDataSource() { return m_DataSource; }
	void setDataSource(const std::shared_ptr<DataSource>& dataSource);

	void setVideoResolution(uint16_t width, uint16_t height) {
		m_VideoWidth = width;
		m_VideoHeight = height;
	}

	int start(bool enableHwaccel);
	void stop();

	void OnTouchEvent(int eventAction, int pointerCount, float percentX, float percentY, float force);
private:
	Item* m_Control = NULL;

	ControlType m_ControlType;
	std::string m_Padcode;

	uint16_t m_VideoWidth = 0;
	uint16_t m_VideoHeight = 0;

	bool m_EnableHwaccel = false;
	
	std::shared_ptr<DataSource> m_DataSource;
	std::shared_ptr<SWPlayer> m_Player;
};

