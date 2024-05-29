/*
 * VideoDisplay.h
 *
 */

#ifndef VIDEODISPLAY_H_
#define VIDEODISPLAY_H_

#define SW_ACTION_DOWN             0
#define SW_ACTION_UP               1
#define SW_ACTION_MOVE             2
#define SW_ACTION_CANCEL           3

class AWE_DECLARE_DATA SWTouchEvent{
public:
	SWTouchEvent(){}
	virtual ~SWTouchEvent(){}
	virtual void OnTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]) {}
private:
    SWTouchEvent(const SWTouchEvent &);
    SWTouchEvent &operator=(const SWTouchEvent &);
};

class OnDisplaySizeChangedListener{
public:
	OnDisplaySizeChangedListener(){}
	virtual ~OnDisplaySizeChangedListener(){}
	virtual void onRenderedFirstFrame(int width, int height){}
	virtual void onVideoSizeChanged(int width, int height){}
private:
	DISALLOW_EVIL_CONSTRUCTORS(OnDisplaySizeChangedListener);
};

struct VideoFrame;

class AWE_DECLARE_DATA VideoDisplay{
public:
	VideoDisplay(){}
	virtual ~VideoDisplay(){}

	virtual void setLogId(uint32_t id){}
	/**
	 * @param rotation	(rotation % 2 == 0) ? PORTRAIT : LANDSCAPE;
	 */
	virtual void onScreenRotation(int rotation){}
	virtual void render(VideoFrame *vf){}

	void setEventHandler(SWTouchEvent* e){touchEvent = e;}
	void OnTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]){
		if(touchEvent != NULL){
			touchEvent->OnTouchEvent(eventAction, pointerCount, x, y, force);
		}
	}
	void setOnVideoSizeChangedListener(OnDisplaySizeChangedListener* l){
		mOnDisplaySizeChangedListener = l;
	}
protected:
	OnDisplaySizeChangedListener* mOnDisplaySizeChangedListener = NULL;
private:
	SWTouchEvent* touchEvent = NULL;
	VideoDisplay(const VideoDisplay &);
	VideoDisplay &operator=(const VideoDisplay &);
};

#endif /* VIDEODISPLAY_H_ */
