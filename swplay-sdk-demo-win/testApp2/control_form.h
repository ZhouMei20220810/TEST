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

