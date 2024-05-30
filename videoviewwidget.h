#ifndef VIDEOVIEWWIDGET_H
#define VIDEOVIEWWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "VideoDisplayWinBase.h"

namespace Ui {
class VideoViewWidget;
}

class VideoViewWidget : public QWidget,public VideoDisplayWinBase
{
    Q_OBJECT

public:
    explicit VideoViewWidget(QWidget *parent = nullptr);
    ~VideoViewWidget();

    // VideoDisplayWinBase implementation
    void onRenderer();
private:
    Ui::VideoViewWidget *ui;

    int viewWidth = 0;
    int viewHeight = 0;

    int clicktype = -1;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // VIDEOVIEWWIDGET_H
