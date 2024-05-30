#pragma once

namespace ui {
	class DuiVideoView;
}

class MainForm;

// �� ui::ListContainerElement �м̳����п��ù���
class Item : public ui::Box//ui::ListContainerElement
{
public:
	Item(MainForm* mainForm, const std::string& padcode);
	~Item();

	// �ṩ�ⲿ��������ʼ�� item ����
	void InitSubControls();

	const std::string& getPadcode() { return m_Padcode; }

	ui::DuiVideoView* getDisplay();
private:
	bool OnItemDoubleClick(ui::EventArgs* args);

	MainForm* m_MainForm;
	std::string m_Padcode;

	ui::Label* m_LabelTitle;
	ui::DuiVideoView* m_Display;
	int64_t m_LastClickTime = 0;
};

