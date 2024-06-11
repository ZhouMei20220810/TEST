/*
 * SWDisplayIOS.h
 *
 */

#ifndef SW_DISPLAY_IOS_H_
#define SW_DISPLAY_IOS_H_

#import <SWViewIOS.h>

#include <common/Utils.h>
#include <common/Condition.h>

#include <VideoDisplay.h>


class SWDisplayIOS: public VideoDisplay{
public:
	SWDisplayIOS(SWView* view);
	virtual ~SWDisplayIOS();

	void setLogId(uint32_t id) override;
	void render(VideoFrame *vf) override;

	inline int getFrameWidth(){ return mWidth; }
	inline int getFrameHeight(){ return mHeight; }

private:
//    Mutex mMutex;
    static void onTouchEventHandle(int eventAction, int pointerCount, int x[], int y[], float force[], void* userdata);
    
    uint32_t mId = 0;
    
	int	mWidth = 0;
	int mHeight = 0;
    
    bool mRenderedFirstFrame = true;
    
    SWView* mView = NULL;

    struct SwsContext *mSwsContext = NULL;
    uint8_t* mBuffer = NULL;
    int mBufferSize = 0;
    
	DISALLOW_EVIL_CONSTRUCTORS(SWDisplayIOS);
};

#endif /* SW_DISPLAY_IOS_H_ */
