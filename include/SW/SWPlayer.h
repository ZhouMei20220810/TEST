/*
 * SWPlayer.h
 *
 */

#ifndef SWPLAYER_H_
#define SWPLAYER_H_

#include <common/Utils.h>
#include <common/Condition.h>
#include <common/Timer.h>

#include <DataSource.h>
#include <VideoDisplay.h>

class AudioPlayer;
class VideoFrameSource;
class SWPlayer;

/**
 * 视频宽高监听器
 */
class OnVideoSizeChangedListener{
public:
	OnVideoSizeChangedListener(){}
	virtual ~OnVideoSizeChangedListener(){}
	/**
	 * 渲染第一帧时回调
	 */
	virtual void onRenderedFirstFrame(SWPlayer* player, int width, int height){}
	/**
	 * 视频宽高改变时回调
	 */
	virtual void onVideoSizeChanged(SWPlayer* player, int width, int height){}
private:
	DISALLOW_EVIL_CONSTRUCTORS(OnVideoSizeChangedListener);
};

class AWE_DECLARE_DATA SWPlayer{
public:
	SWPlayer();
	~SWPlayer();

	uint32_t getId();

	DataSource* getDataSource();

	void setDataSource(DataSource* dataSource);
	void setDataSource(const std::shared_ptr<DataSource>& dataSource);

	void setDisplay(VideoDisplay* display);
    void setOnVideoSizeChangedListener(OnVideoSizeChangedListener* l);
    void setOnVideoSizeChangedListener(const std::shared_ptr<OnVideoSizeChangedListener>& l);

    int attachDataSource(const std::shared_ptr<DataSource>& dataSource);
    int detachDataSource();

    void setExternalScheduler(std::shared_ptr<Timer>& timer);
	void enableHWaccel();

	int start();
	void stop();

	int changeDisplay(VideoDisplay* display);
private:
	static void onStopping(void *userdata, intptr_t arg);

    static void onDecodeHandle(void *userdata, intptr_t arg);
    void onDecode();

    void onAudioStreamChanged(int type, int profile, int sampleRate, int channelCount);
    void onVideoStreamChanged(int width, int height, const char *sps, int sps_len, const char *pps, int pps_len);
    void onScreenRotation(int rotation);
private:
	class OnAVStreamChangedListener: public OnAudioStreamChangedListener,
									 public OnVideoStreamChangedListener,
									 public OnDisplaySizeChangedListener{
	public:
		OnAVStreamChangedListener(SWPlayer* handler);
		~OnAVStreamChangedListener();
		//OnAudioStreamChangedListener impl
		void onAudioStreamChanged(DataSource* dataSource,
				int type, int profile, int sampleRate, int channelCount) override;
		//OnVideoStreamChangedListener impl
		void onVideoStreamChanged(DataSource* dataSource,
				int width, int height, const char* sps, int sps_len, const char* pps, int pps_len) override;
		void onScreenRotation(DataSource* dataSource, int rotation) override;

		void onRenderedFirstFrame(int width, int height) override;
		void onVideoSizeChanged(int width, int height) override;
	private:
		SWPlayer* mHandler = NULL;
	};

	std::unique_ptr<OnAVStreamChangedListener> mOnAVStreamChangedListener;
	friend class OnAVStreamChangedListener;
private:
    Mutex mMutex;
    Condition mCondition;

	uint32_t mId = 0;
	bool mExternalScheduler = false;
	bool mDetachDataSource = false;
	bool mHWaccel = false;
	bool mStarted = false;

	std::shared_ptr<AudioPlayer> mAudioPlayer;

	std::shared_ptr<Timer> mDecTimer;

	DataSource* mDataSource = NULL;
	std::shared_ptr<DataSource> mDataSourceSharedPtr;
	std::shared_ptr<VideoFrameSource> mVideoFrameSource;

	VideoDisplay* mVideoDisplay = NULL;

	OnVideoSizeChangedListener* mOnVideoSizeChangedListener = NULL;
	std::shared_ptr<OnVideoSizeChangedListener> mOnVideoSizeChangedListenerSharedPtr;

	TimerID mVideoDecID = NULL;

	DISALLOW_EVIL_CONSTRUCTORS(SWPlayer);
};

//-------------------------------------------------------
AWE_DECLARE(void) audio_decode_example(const char* aac_file, int samplerate, int channels);

#endif /* SWPLAYER_H_ */
