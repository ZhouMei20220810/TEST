#ifndef VIDEOVIEWWIDGET_H
#define VIDEOVIEWWIDGET_H

#include <QLabel>
#include <QMouseEvent>
#include "VideoDisplayWinBase.h"

namespace Ui {
class VideoViewWidget;
}

class VideoViewWidget : public QWidget,public VideoDisplayWinBase
{
    Q_OBJECT

public:
    explicit VideoViewWidget(QWidget* parent = nullptr);
    ~VideoViewWidget();

    // VideoDisplayWinBase implementation
    void onRenderer();
signals:
    void syncTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
public slots:
    void do_syncTouchEventSignals(int eventAction, int pointerCount, int x[], int y[], float force[]);
private:
    void  Show_RGB(const uchar* data, uchar Per_port_number, uchar frame_len);

    Ui::VideoViewWidget *ui;

    int viewWidth = 0;
    int viewHeight = 0;

    int clicktype = -1;

    QString m_strTempFile;
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

    QWidget* m_parent;
};

#endif // VIDEOVIEWWIDGET_H
