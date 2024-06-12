#include "videoviewwidget.h"
#include "ui_videoviewwidget.h"
#include "global.h"
#include <QFile>
#include <QDataStream>
//#include <QMatrix>
VideoViewWidget::VideoViewWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::VideoViewWidget)
{
    ui->setupUi(this);

	setStyleSheet("background-color:gray");
    //ui->label->setText("Hello world");
	m_strTempFile = GlobalData::strPictureTempDir +"1.png";
	
	QFile file1(m_strTempFile);
	QString strUrl;
	if (!file1.exists())
		strUrl = ":/main/resource/main/defaultSceenShot.png";
	else
		strUrl = m_strTempFile;
	QPixmap pixmap;
	QImage image(strUrl);
	if (!image.isNull())
	{
		if (!GlobalData::bVerticalPhoneInstance)
		{
			QTransform transform;
			transform.rotate(270);
			image = image.transformed(transform);
		}
	}

	pixmap = QPixmap::fromImage(image);
	if (pixmap.isNull())
	{
		pixmap = QPixmap(strUrl);
	}
	
	ui->label->setPixmap(pixmap.scaled(QSize(ui->label->width(), ui->label->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	//m_strPicturePath = GlobalData::strFileTempDir + "/" + m_PhoneInfo.strInstanceNo + ".png";
}

VideoViewWidget::~VideoViewWidget()
{
    delete ui;
}

// VideoDisplayWinBase implementation
void VideoViewWidget::onRenderer()
{
    //更新界面
    update();
}

void VideoViewWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint point = event->globalPos();
    QPointF localPoint = event->localPos();
    qDebug() << "Global point.x=" << point.x() << "point.y=" << point.y();
    qDebug() << "Local point.x=" << localPoint.x() << "point.y=" << localPoint.y();

	int mouseX = localPoint.x()-this->rect().left();
	int mouseY = localPoint.y()-this->rect().top();

	int viewWidth = this->rect().right() -this->rect().left();
	int viewHeight = this->rect().bottom() - this->rect().top();
	qDebug() << "viewWidth=" << viewWidth << "viewHeight=" << viewHeight;
	int videoWidth = getSrcWidth();
	int videoHeight = getSrcHeight();
	qDebug() << "videoWidth=" << videoWidth << "videoHeight=" << videoHeight;

	float percentX = mouseX / (float)(viewWidth * 1.0f);
	float percentY = mouseY / (float)(viewHeight * 1.0f);

	int x[1] = { -1 };
	int y[1] = { -1 };
	float force[1] = { 0 };

	x[0] = (int)(videoWidth * percentX);
	y[0] = (int)(videoHeight * percentY);

	OnTouchEvent(SW_ACTION_DOWN, 1, x, y, force);
	clicktype = SW_ACTION_DOWN;
	
	QWidget::mousePressEvent(event);
}
void VideoViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	int x[1] = { -1 };
	int y[1] = { -1 };
	float force[1] = { 0 };
	x[0] = -1;
	y[0] = -1;
	OnTouchEvent(SW_ACTION_UP, 1, x, y, force);
	clicktype = SW_ACTION_UP;
	QWidget::mousePressEvent(event);
}
void VideoViewWidget::mouseMoveEvent(QMouseEvent *event) 
{
	if (clicktype == SW_ACTION_DOWN)
	{
		QPointF mousePoint = event->localPos();
		if (mousePoint.x() > this->rect().right()|| mousePoint.y() > this->rect().bottom()) 
		{
			event->ignore();
			return;
		}
		int mouseX = mousePoint.x() - this->rect().left();
		int mouseY = mousePoint.y() - this->rect().top();

		int viewWidth = this->rect().right() - this->rect().left();
		int viewHeight = this->rect().bottom() - this->rect().top();

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
		event->accept();
		return;
	}
	QWidget::mouseMoveEvent(event);
}

void  VideoViewWidget::Show_RGB(const uchar* data, uchar Per_port_number, uchar frame_len)//data帧数组
{
	ui->label->setAutoFillBackground(true);

	//保持原图片的长宽比，且不限制矩形框大小
	//pixmap2 = pixmap2.scaled(ui->label->size(), Qt::KeepAspectRatio);//自适应/等比例

	ui->label->setStyleSheet("background:black;");  // 标签背景
	ui->label->setAlignment(Qt::AlignCenter);  // 图片居中
	//QImage image(data, ui->label->width(), frame_len, ui->label->height(), QImage::Format_RGB888);//data数组 //355宽度 //frame_len 高度//每行1005字节数//格式
    //QImage image(data, getSrcWidth(), frame_len, 1005, QImage::Format_RGB888);//data数组 //355宽度 //frame_len 高度//每行1005字节数//格式
	if (!GlobalData::bVerticalPhoneInstance)
	{
		QImage image(data, ui->label->width(), ui->label->height(), QImage::Format_RGBA8888);
		if (!image.isNull())
		{
			if (!GlobalData::bVerticalPhoneInstance)
			{
				QTransform transform;
				transform.rotate(-90);
				image = image.transformed(transform);
			}
		}		
		ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->size()));//自适应/等比例
	}
	else
	{
		QImage image(data, ui->label->width(), ui->label->height(), QImage::Format_RGBA8888);
		ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->size(), Qt::KeepAspectRatio));//自适应/等比例
	}
	
}

void VideoViewWidget::paintEvent(QPaintEvent *event)
{
	try
	{
		//int64_t t1 = get_time_now_ms();
		//if (!::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem))
		//	return;
		QWidget::paintEvent(event);
		QRect rcPaint = event->rect();
		ui->label->setPixmap(QPixmap(m_strTempFile).scaled(QSize(ui->label->width(), ui->label->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		ui->label->resize(this->size());
		int item_w = ui->label->width();// m_rcItem.right - m_rcItem.left;
		int item_h = ui->label->height();//m_rcItem.bottom - m_rcItem.top;
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
			int item_x = ui->label->rect().left();
			int item_y = ui->label->rect().top();//m_rcItem.top;
			int source_w = getFrameWidth();
			int source_h = getFrameHeight();

			char* src_data = (char*)getFrameBuffer();

            Show_RGB((const uchar*)src_data, 20,10);


            //ui->label->setPixmap(QPixmap(src_data));
			//Window* parent_wnd = GetWindow();
			/*QFile file(GlobalData::strPictureTempDir + "1.png");
			if (file.open(QIODevice::WriteOnly))
			{
				QDataStream stream(&file);
				stream << src_data;
				file.flush();
				file.close();
			}*/
			

			if (src_data /*&& parent_wnd */)
			{
				//居中
				item_x += (item_w - source_w) / 2;
				item_y += (item_h - source_h) / 2;
				//UiRect rcClient;
				//::GetClientRect(parent_wnd->GetHWND(), &rcClient);
				QRect rcClient = ui->label->rect();
				int width = rcClient.right() - rcClient.left();
				int height = rcClient.bottom() - rcClient.top();

				//计算实际绘制区域坐标
				/*int draw_x = max(rcPaint.left(), item_x);
				draw_x = max(ui->label.left(), draw_x);
				int draw_y = max(rcPaint.top(), item_y);
				draw_y = max(ui->label.top(), draw_y);
				int draw_h = min(rcPaint.bottom() - draw_y, min(item_y + source_h, ui->label.bottom()) - draw_y);
				draw_h = max(draw_h, 0);
				int src_x = draw_x - item_x;
				int src_y = draw_y - item_y;
				int src_w = min(rcPaint.right() - draw_x, min(item_x + source_w, ui->label.right()) - draw_x);
				src_w = max(src_w, 0);

				int dest_byte_width = width * 4;
				int src_byte_width = source_w * 4;
				int paint_byte_width = src_w * 4;
				char* dest_data = ui->label //(char*)parent_wnd->GetRenderContext()->GetBits();
				int bottom = draw_y;// height - draw_y - 1;
				dest_data += bottom * dest_byte_width + draw_x * 4;
				//char* src_data = (char*)getFrameBuffer();
				src_data += src_y * src_byte_width + src_x * 4;
				for (int i = 0; i < draw_h; ++i)
				{
					memcpy(dest_data, src_data, paint_byte_width);
					dest_data += dest_byte_width;//-=
					src_data += src_byte_width;
				}*/
			}
			/*Window* parent_wnd = GetWindow();
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
			}*/

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
