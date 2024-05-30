#pragma once

namespace ui {
	class DuiVideoView;
}

class MainForm;

// 从 ui::ListContainerElement 中继承所有可用功能
class Item : public ui::Box//ui::ListContainerElement
{
public:
	Item(MainForm* mainForm, const std::string& padcode);
	~Item();

	// 提供外部调用来初始化 item 数据
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

