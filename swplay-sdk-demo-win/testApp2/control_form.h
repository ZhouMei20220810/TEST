#pragma once

#include <common/Condition.h>

#include <SWDataSourceListener.h>
#include <VideoDisplayWinBase.h>

namespace ui {
class DuiVideoView;
}

class SWPlayer;
class ControlItem;
class MainForm;

class ControlForm : public ui::WindowImplBase, public ui::TouchEventListener
{
public:
	ControlForm(MainForm* mainForm, std::shared_ptr<ControlItem> controlItem);
	~ControlForm();

	static const std::wstring kClassName;

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;

	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;

	// TouchEventListener implementation
	void OnTouchEvent(int eventAction, int pointerCount, float percentX, float percentY, float force) override;

private:
	bool OnPicQualitySelect(ui::EventArgs* args);
	bool OnClickBack(ui::EventArgs* args);
	bool OnClickHome(ui::EventArgs* args);
	bool OnClickMenu(ui::EventArgs* args);

	void OnKeyEvent(int action, int keyCode);

	Mutex m_Mutex;

	MainForm* m_MainForm;

	ui::DuiVideoView* m_Display;

	ui::Button* m_BtnBack;
	ui::Button* m_BtnHome;
	ui::Button* m_BtnMenu;

	ui::Combo* m_PicQuality;
	ui::CheckBox* m_GroupControl;

	std::shared_ptr<ControlItem> m_ControlItem;
	std::shared_ptr<SWPlayer> m_Player;
};

