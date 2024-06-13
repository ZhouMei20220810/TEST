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
#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
#include "webrtc/SWDataSourceWebrtc.h"
#include "customerp2p/CustomerP2PInterface.h"
#endif

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

class OnPlayerErrorListener{
public:
    OnPlayerErrorListener(){}
    virtual ~OnPlayerErrorListener(){}
    virtual void onPlayError(SWPlayer* player, int errorCode, const char* errorMsg){}
private:
    DISALLOW_EVIL_CONSTRUCTORS(OnPlayerErrorListener);
};

enum {
    PLAY_DECODER_INIT_FAILED    = 65560,
    PLAY_DECODER_DECODE_FAILED  = 65561,
};

class AWE_DECLARE_DATA SWPlayer
#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
    : public customerp2p::CustomerP2PObserver
#endif
{
public:
    SWPlayer();
    virtual ~SWPlayer();

    uint32_t getId();

    DataSource* getDataSource();

    void setDataSource(DataSource* dataSource);
    void setDataSource(const std::shared_ptr<DataSource>& dataSource);

    void setDisplay(VideoDisplay* display);
    void setOnVideoSizeChangedListener(OnVideoSizeChangedListener* l);
    void setOnVideoSizeChangedListener(const std::shared_ptr<OnVideoSizeChangedListener>& l);

    void setOnPlayerErrorListener(OnPlayerErrorListener* l);
    void setOnPlayerErrorListener(const std::shared_ptr<OnPlayerErrorListener>& l);

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
    void onAudioStop();
    void onVideoStreamChanged(int width, int height, const char *sps, int sps_len, const char *pps, int pps_len);
    void onScreenRotation(int rotation);

#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
    void onRecvOffer(const char* offer);
    void onRecvCandidate(const char* sdpMid, int sdpMLineIndex, const char* candidate);

    // CustomerP2PObserver impl
    void OnAnswer(const char* answer) override;
    void OnCandidate(const char* mid, int mline_index, const char* candidate) override;
    void OnIceConnected() override;
    void OnIceFailed() override;
    void OutputVideo(customerp2p::VideoData& data) override;
    void OnDataChannelOpen() override;
    void OnDataChannelClose() override;
    void OnRecvData(const void* data, size_t size) override;
#endif
private:
    class OnAVStreamChangedListener
        : public OnAudioStreamChangedListener
        , public OnVideoStreamChangedListener
        , public OnDisplaySizeChangedListener
#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
        , public OnWebRTCInfoListener
#endif
    {
    public:
        OnAVStreamChangedListener(SWPlayer* handler);
        ~OnAVStreamChangedListener();
        //OnAudioStreamChangedListener impl
        void onAudioStreamChanged(DataSource* dataSource,
                int type, int profile, int sampleRate, int channelCount) override;
        void onAudioStop();
        //OnVideoStreamChangedListener impl
        void onVideoStreamChanged(DataSource* dataSource,
                int width, int height, const char* sps, int sps_len, const char* pps, int pps_len) override;
        void onScreenRotation(DataSource* dataSource, int rotation) override;

        void onRenderedFirstFrame(int width, int height) override;
        void onVideoSizeChanged(int width, int height) override;

#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
        // OnWebRTCInfoListener impl
        void onRecvOffer(const char* offer) override;
        void onRecvCandidate(const char* sdpMid, int sdpMLineIndex, const char* candidate) override;
#endif
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

    OnPlayerErrorListener* mOnPlayerErrorListener = NULL;
    std::shared_ptr<OnPlayerErrorListener> mOnPlayerErrorListenerSharedPtr;

    TimerID mVideoDecID = NULL;

#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
    customerp2p::CustomerP2PInterface* mCustomerP2P = NULL;
#endif

    DISALLOW_EVIL_CONSTRUCTORS(SWPlayer);
};

//-------------------------------------------------------
AWE_DECLARE(void) audio_decode_example(const char* aac_file, int samplerate, int channels);

#endif /* SWPLAYER_H_ */
