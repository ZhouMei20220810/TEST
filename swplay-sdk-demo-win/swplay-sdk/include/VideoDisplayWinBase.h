/*
 * VideoDisplayWinBase.h
 *
 */

#ifndef VIDEODISPLAY_WIN_BASE_H_
#define VIDEODISPLAY_WIN_BASE_H_

#include <common/Utils.h>
#include <common/Condition.h>

#include <VideoDisplay.h>

typedef struct AVPic {
#define _NUM_DATA_POINTERS 8
    uint8_t *data[_NUM_DATA_POINTERS];    ///< pointers to the image data planes
    int linesize[_NUM_DATA_POINTERS];     ///< number of bytes per line
} AVPic;

struct SwsContext;

class AWE_DECLARE_DATA VideoDisplayWinBase: public VideoDisplay{
public:
	VideoDisplayWinBase();
	virtual ~VideoDisplayWinBase();

	void setLogId(uint32_t id) override;
	void render(VideoFrame *vf) override;

#define DST_FMT_YUV420P	0
#define DST_FMT_BGRA	-1
	void setDstFormat(int fmt, int width, int height, int flip);

	inline uint8_t* getFrameBuffer(){ return mFrameBuffer; }
	inline int getFrameWidth(){ return mDstWidth; }
	inline int getFrameHeight(){ return mDstHeight; }

	inline int getSrcWidth() { return mSrcWidth; }
	inline int getSrcHeight() { return mSrcHeight; }

	inline bool havePic() { return mSrcWidth != 0 && mSrcHeight != 0 && mFrameBuffer != NULL; }

	void lockFrameBuffer();
	void unlockFrameBuffer();

	virtual void onRenderer();
private:
	Mutex mLock;

	struct SwsContext *mSwsContext = nullptr;

	AVPic mPicture;
	uint8_t* mFrameBuffer = nullptr;
	int mFrameBufferSize = 0;

	int	mSrcWidth = 0;
	int mSrcHeight = 0;

	int mDstFormat = 0;
	int	mDstWidth = 0;
	int mDstHeight = 0;

	int mFlip = 0;

	uint32_t mId = 0;

	DISALLOW_EVIL_CONSTRUCTORS(VideoDisplayWinBase);
};

#endif /* VIDEODISPLAY_WIN_BASE_H_ */
