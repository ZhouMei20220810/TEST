#pragma once

#include <common/Condition.h>
#include "PadInfo.h"

namespace ui {
	Control* MyCreateControlCallback(const std::wstring& sName);
}

class VirtualTileBox;
class ControlItem;
class Item;

class MainForm : public ui::WindowImplBase
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

	std::shared_ptr<CPadInfo>& GetPadInfo() { return m_PadInfo; }

	std::list<std::shared_ptr<ControlItem> > GetControlItemList() { return m_ControlItemList; }
	std::shared_ptr<ControlItem> GetControlItem(const std::string& padcode);

	void OnPlayStart(const std::string& padcode);
	void OnPlayStop(std::shared_ptr<ControlItem>& controlItem);
private:
	enum {
		GROUP_CONTROL_NOP		= 0,
		GROUP_CONTROL_STARTING	= 1,
		GROUP_CONTROL_STARTED	= 1 << 1,
		GROUP_CONTROL_STOPPING	= 1 << 2,
		GROUP_CONTROL_STOPED	= 1 << 3,
	};

	bool OnAddToControl(ui::EventArgs* args);

	void OnGroupControlStart();
	void OnGroupControlStop();

	Mutex m_Mutex;
	int m_GroupControlStatus = 0;
	int m_WindowCount = 0;

	ui::Button* m_BtnAddToControl;
	ui::TreeView* m_PadcodeTree;
	VirtualTileBox*	m_pTileList;

	std::shared_ptr<CPadInfo> m_PadInfo;
	std::list<std::string> m_PadcodeList;
	std::list<std::shared_ptr<ControlItem> > m_ControlItemList;

	
};

