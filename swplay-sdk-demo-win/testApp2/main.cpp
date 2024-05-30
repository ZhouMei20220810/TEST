// testApp.cpp : 定义应用程序的入口点。
//

#include "framework.h"

#define SW_LOG_TAG "testApp2"
#include "common/log.h"

#include "SWRuntime.h"

#include "main_form.h"


std::string w2str(LPCTSTR src) {
#ifdef _UNICODE
	std::wstring w = src;
	return std::string(w.begin(), w.end()); // magic here
#else
	return src;
#endif
}

class MiscThread : public nbase::FrameworkThread
{
public:
	MiscThread(enum ThreadId thread_id, const char *name)
		: FrameworkThread(name)
		, thread_id_(thread_id) {}

	~MiscThread(void) {}

private:
	/**
	* 虚函数，初始化线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;

private:
	enum ThreadId thread_id_;
};

void MiscThread::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void MiscThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}

class MainThread : public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}

private:
	/**
	* 虚函数，初始化主线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，主线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;

	std::unique_ptr<MiscThread>	misc_thread_;
};

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 启动杂事处理线程
	misc_thread_.reset(new MiscThread(kThreadGlobalMisc, "Global Misc Thread"));
	misc_thread_->Start();

	// 获取资源路径，初始化全局参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
//#ifdef _DEBUG
	// Debug 模式下使用本地文件夹作为资源
	// 默认皮肤使用 resources\\themes\\default
	// 默认语言使用 resources\\lang\\zh_CN
	// 如需修改请指定 Startup 最后两个参数
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::MyCreateControlCallback, false);
//#else
//	// Release 模式下使用资源中的压缩包作为资源
//	// 资源被导入到资源列表分类为 THEME，资源名称为 IDR_THEME
//	// 如果资源使用的是本地的 zip 文件而非资源中的 zip 压缩包
//	// 可以使用 OpenResZip 另一个重载函数打开本地的资源压缩包
//	ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_THEME), L"THEME", "");
//	// ui::GlobalManager::OpenResZip(L"resources.zip", "");
//	ui::GlobalManager::Startup(L"resources\\", ui::MyCreateControlCallback, false);
//#endif

	std::string logpath = std::string(theme_dir.begin(), theme_dir.end());
	SWRuntime::getInstance()->init(ANDROID_LOG_INFO, logpath.c_str());

	// 创建一个默认带有阴影的居中窗口
	MainForm* window = new MainForm();
	window->Create(NULL, MainForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	misc_thread_->Stop();
	misc_thread_.reset(nullptr);

	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();

	SWRuntime::getInstance()->deinit();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 创建主线程
	MainThread thread;

	// 执行主线程循环
	thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}


