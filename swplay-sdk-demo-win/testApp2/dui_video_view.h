#pragma once

#include <VideoDisplayWinBase.h>

namespace ui
{
class TouchEventListener {
public:
	TouchEventListener() {}
	virtual ~TouchEventListener() {}
	virtual void OnTouchEvent(int eventAction, int pointerCount, float percentX, float percentY, float force) {}
private:
	TouchEventListener(const TouchEventListener &);
	TouchEventListener &operator=(const TouchEventListener &);
};

/** @class DuiVideoView
  * @brief 支持贴hbitmap的图片
  * @copyright (c) 2016, NetEase Inc. All rights reserved
  * @author gaoqi
  * @date 2014/3/10
  */
class DuiVideoView : public ui::Box, public VideoDisplayWinBase
{
public:
	/**
	* 构造函数
	* @param[in] video_frame_mng 视频帧管理器
	* @return void	无返回值
	*/
	DuiVideoView();
	~DuiVideoView();

	/**
	* 重写父类绘制函数
	* @param[in] hDC 目标DC
	* @param[in] rcPaint 可绘制区域
	* @return void	无返回值
	*/
	void Paint(IRenderContext* pRender, const UiRect& rcPaint) override;

	/**
	* 清理数据
	* @return void	无返回值
	*/
	void Clear();

	bool InitMouseEvents(TouchEventListener* e);

	// VideoDisplayWinBase implementation
	void onRenderer() override;
private:
	bool OnMouseMove(EventArgs* args);
	bool OnMouseDown(EventArgs* args);
	bool OnMouseUp(EventArgs* args);

	TouchEventListener* touchEvent = NULL;

	int viewWidth = 0;
	int viewHeight = 0;

	int clicktype = -1;

	int64_t lastMouseMoveTime = 0L;
};

}

