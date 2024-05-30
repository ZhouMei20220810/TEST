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
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
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

