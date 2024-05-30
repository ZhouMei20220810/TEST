#include <sys/timeb.h>

#include "framework.h"

#define SW_LOG_TAG "DuiVideoView"
#include "common/log.h"
#include "common/time_c.h"
#include "dui_video_view.h"

#define TIMER_FOR_CHECK_BITMAP	163
namespace ui
{

DuiVideoView::DuiVideoView()
{
	//timestamp_ = 0;
	//width_ = 0;
	//height_ = 0;
	//auto_size_ = false;
}

DuiVideoView::~DuiVideoView(void)
{
	Clear();
}

void DuiVideoView::Paint(IRenderContext* pRender, const UiRect& rcPaint)
{
	try
	{
		//int64_t t1 = get_time_now_ms();
		if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem))
			return;
		Control::Paint(pRender, rcPaint);

		int item_w = m_rcItem.right - m_rcItem.left;
		int item_h = m_rcItem.bottom - m_rcItem.top;
		if (item_w != viewWidth || item_h != viewHeight) 
		{
			viewWidth = item_w;
			viewHeight = item_h;
			setDstFormat(DST_FMT_BGRA, viewWidth, viewHeight, 0);
		}
		else if (havePic())//paint bitmap
		{
			lockFrameBuffer();
			//int item_w = m_rcItem.right - m_rcItem.left;
			//int item_h = m_rcItem.bottom - m_rcItem.top;
			int item_x = m_rcItem.left;
			int item_y = m_rcItem.top;
			int source_w = getFrameWidth();
			int source_h = getFrameHeight();
			
			char* src_data = (char*)getFrameBuffer();
			Window* parent_wnd = GetWindow();
			if (src_data && parent_wnd)
			{
				//居中
				item_x += (item_w - source_w) / 2;
				item_y += (item_h - source_h) / 2;
				UiRect rcClient;
				::GetClientRect(parent_wnd->GetHWND(), &rcClient);
				int width = rcClient.right - rcClient.left;
				int height = rcClient.bottom - rcClient.top;

				//计算实际绘制区域坐标
				int draw_x = max(rcPaint.left, item_x);
				draw_x = max(m_rcItem.left, draw_x);
				int draw_y = max(rcPaint.top, item_y);
				draw_y = max(m_rcItem.top, draw_y);
				int draw_h = min(rcPaint.bottom - draw_y, min(item_y + source_h, m_rcItem.bottom) - draw_y);
				draw_h = max(draw_h, 0);
				int src_x = draw_x - item_x;
				int src_y = draw_y - item_y;
				int src_w = min(rcPaint.right - draw_x, min(item_x + source_w, m_rcItem.right) - draw_x);
				src_w = max(src_w, 0);

				int dest_byte_width = width * 4;
				int src_byte_width = source_w * 4;
				int paint_byte_width = src_w * 4;
				char* dest_data = (char*)parent_wnd->GetRenderContext()->GetBits();
				int bottom = draw_y;// height - draw_y - 1;
				dest_data += bottom * dest_byte_width + draw_x * 4;
				//char* src_data = (char*)getFrameBuffer();
				src_data += src_y * src_byte_width + src_x * 4;
				for (int i = 0; i < draw_h; ++i)
				{
					memcpy(dest_data, src_data, paint_byte_width);
					dest_data += dest_byte_width;//-=
					src_data += src_byte_width;
				}
			}

			unlockFrameBuffer();
		}
		
		////绘制子控件
		//for (auto it = m_items.begin(); it != m_items.end(); it++)
		//{
		//	Control* pControl = *it;
		//	if (!pControl->IsVisible()) continue;
		//	UiRect controlPos = pControl->GetPos();
		//	if (!::IntersectRect(&m_rcPaint, &rcPaint, &controlPos)) continue;
		//	pControl->AlphaPaint(pRender, rcPaint);
		//}

		//int64_t t2 = get_time_now_ms();
		//ALOGV("DoPaint, using time:%d", (int)(t2-t1));
	}
	catch (...)
	{
		throw "DuiVideoView::DoPaint";
	}
}

//清理失效数据
void DuiVideoView::Clear()
{
	//timestamp_ = 0;
	//width_ = 0;
	//height_ = 0;
}

bool DuiVideoView::InitMouseEvents() {
	OnEvent[kEventMouseMove] += nbase::Bind(&DuiVideoView::OnMouseMove, this, std::placeholders::_1);
	OnEvent[kEventMouseButtonDown] += nbase::Bind(&DuiVideoView::OnMouseDown, this, std::placeholders::_1);
	OnEvent[kEventMouseButtonUp] += nbase::Bind(&DuiVideoView::OnMouseUp, this, std::placeholders::_1);

	return true;
}

bool DuiVideoView::OnMouseDown(EventArgs* args) {
	//ALOGV("OnMouseDown, x:%d, y:%d", args->ptMouse.x, args->ptMouse.y);
	int mouseX = args->ptMouse.x - m_rcItem.left;
	int mouseY = args->ptMouse.y - m_rcItem.top;

	int viewWidth = m_rcItem.right - m_rcItem.left;
	int viewHeight = m_rcItem.bottom - m_rcItem.top;

	int videoWidth = getSrcWidth();
	int videoHeight = getSrcHeight();

	float percentX = mouseX / (float)(viewWidth * 1.0f);
	float percentY = mouseY / (float)(viewHeight * 1.0f);

	int x[1] = { -1 };
	int y[1] = { -1 };
	float force[1] = { 0 };

	x[0] = (int)(videoWidth * percentX);
	y[0] = (int)(videoHeight * percentY);

	OnTouchEvent(SW_ACTION_DOWN, 1, x, y, force);
	clicktype = SW_ACTION_DOWN;
	return true;
}
bool DuiVideoView::OnMouseMove(EventArgs* args) {
	//ALOGV("OnMouseMove, x:%d, y:%d,  m_rcItem.right:%d, m_rcItem.bottom:%d", args->ptMouse.x, args->ptMouse.y, m_rcItem.right, m_rcItem.bottom);
	if(clicktype == SW_ACTION_DOWN){
		if (args->ptMouse.x > m_rcItem.right || args->ptMouse.y > m_rcItem.bottom) {
			return false;
		}
		int mouseX = args->ptMouse.x - m_rcItem.left;
		int mouseY = args->ptMouse.y - m_rcItem.top;

		int viewWidth = m_rcItem.right - m_rcItem.left;
		int viewHeight = m_rcItem.bottom - m_rcItem.top;

		int videoWidth = getSrcWidth();
		int videoHeight = getSrcHeight();

		float percentX = mouseX / (float)(viewWidth * 1.0f);
		float percentY = mouseY / (float)(viewHeight * 1.0f);

		int x[1] = { -1 };
		int y[1] = { -1 };
		float force[1] = { 0 };

		x[0] = (int)(videoWidth * percentX);
		y[0] = (int)(videoHeight * percentY);

		OnTouchEvent(SW_ACTION_MOVE, 1, x, y, force);
		return true;
	}
	return false;
}
bool DuiVideoView::OnMouseUp(EventArgs* args) {
	//ALOGV("OnMouseUp, x:%d, y:%d", args->ptMouse.x, args->ptMouse.y);
	int x[1] = { -1 };
	int y[1] = { -1 };
	float force[1] = { 0 };
	x[0] = -1;
	y[0] = -1;
	OnTouchEvent(SW_ACTION_UP, 1, x, y, force);
	clicktype = SW_ACTION_UP;
	return true;
}

void DuiVideoView::onRenderer() {
	Invalidate();
}

}


