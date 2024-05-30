#include "videoviewwidget.h"
#include "ui_videoviewwidget.h"

VideoViewWidget::VideoViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoViewWidget)
{
    ui->setupUi(this);

    ui->label->setText("Hello world");
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
void VideoViewWidget::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
}
