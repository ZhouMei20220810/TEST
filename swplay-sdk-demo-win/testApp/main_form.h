#pragma once

#include <SWDataSourceListener.h>
#include <common/Condition.h>

namespace ui {
class DuiVideoView;
}

class SWPlayer;

class MainForm : public ui::WindowImplBase, public SWDataSourceListener
{
public:
	MainForm();
	~MainForm();

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

	// SWDataSourceListener implementation
	void onReconnecting(int NthTime) override;
	void onConnected() override;
	void onDisconnected(int errcode) override;
	void onDisconnected(int errcode, const char* errmsg) override;
	void onPlayInfo(const char* info) override;

	bool OnItemDoubleClick(ui::EventArgs* args);
private:
	void OnShowMessage(const std::wstring& msg);
	void OnConnMessage(const std::wstring& msg);
	void OnDisconnMessage(const std::wstring& msg);

	bool OnPlayStart(ui::EventArgs* args);
	void OnPlayStop(bool quit);

	bool OnPicQualitySelect(ui::EventArgs* args);
	bool OnClickBack(ui::EventArgs* args);
	bool OnClickHome(ui::EventArgs* args);
	bool OnClickMenu(ui::EventArgs* args);

	Mutex m_Mutex;

	ui::RichEdit* m_EditPadcode;
	ui::RichEdit* m_EditPackageName;
	ui::RichEdit* m_EditControlAddr;
	ui::RichEdit* m_EditControlPort;

	ui::RichEdit* m_EditUserID;
	ui::RichEdit* m_EditSessionID;

	ui::Combo* m_PicQuality;
	ui::Option* m_OptionPlayType;

	ui::Button* m_BtnStart;
	ui::Button* m_BtnBack;
	ui::Button* m_BtnHome;
	ui::Button* m_BtnMenu;

	ui::Label* m_MsgShow;
	ui::Label* m_ConMsg;

	ui::DuiVideoView* m_Display;

	std::shared_ptr<SWPlayer> m_Player;
};

