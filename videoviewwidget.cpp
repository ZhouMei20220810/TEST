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

    //int mouseX = event->
}
void VideoViewWidget::mouseReleaseEvent(QMouseEvent *event)
{

}
void VideoViewWidget::mouseMoveEvent(QMouseEvent *event)
{

}
void VideoViewWidget::paintEvent(QPaintEvent *event)
{
    //QPainter painter(this);
}
