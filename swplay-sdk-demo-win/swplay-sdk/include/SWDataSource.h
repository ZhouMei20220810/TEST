/*
 * SWDataSource.h
 *
 */

#ifndef SWDATASOURCE_H_
#define SWDATASOURCE_H_

#include <string.h>

#include <list>
//#include <common/linkedlist.h>

#include <common/Utils.h>
#include <common/Condition.h>
#include <DataSource.h>
#include <SWDataSourceListener.h>
#include <VideoDisplay.h>

class APacket;
class PlayDataSource;
class MyPlayDataSourceListenerInner;
class PlayEventListener;
class PlayAudioListener;
class PlayVideoListener;

class AWE_DECLARE_DATA SWDataSource: public DataSource, public SWTouchEvent{
public:
	SWDataSource(uint32_t id, SWDataSourceListener* l);
	SWDataSource(uint32_t id, const std::shared_ptr<PlayEventListener> l);

	~SWDataSource();

	SWDataSourceListener* getListener(){
		return mListener;
	}

    /**
     * 用于测试的, 在setLoginParams方法前调用
     * @param controlIp     control server IP
     * @param port          control server port
     */
    void specificControl(const char* controlIp, uint16_t port);

	/**
	 * 设置连接参数
	 * @param controlIp     control server IP
	 * @param port          control server port
	 * @param userID        userID
	 * @param sessionID     sessionID
	 * @param padCode       padCode
	 * @param onlineType       onlineType
	 * @return 0为成功，其它为失败
	 */
	int setLoginParams(const char* controlIp, uint16_t port, int userID,
			const char* sessionID, const char* padCode, int onlineType);

#define PLAYTYPE_VIDEO	1//播放视频
#define PLAYTYPE_AUDIO	2//播放音频
#define PLAYTYPE_AV		3//播放音视频

	/**
	 * 设置投屏参数
	 * @param appName       应用包名
	 * @param encodetype    1：软解，2：硬解
	 * @param width         width
	 * @param height        height
	 * @param maxfps        最大帧率
	 * @param minfps        最小帧率
	 * @param bitrate       码率
	 * @param gop           I帧间隔
	 * @param resolutionLevel   分辨率等，1-4
	 * @param videoQuality  不用填
	 * @param playType     	播放类型， 0：无，1：视频， 2：音频, 3：音视频
	 * @param apiLevel      1：旧接口，2：新接口
	 * @param useSSL        1:使用SSL加密，0：不加密
	 * @return 0为成功，其它为失败
	 */
	int setPlayParams(const char* appName, int encodetype, int width, int height,
				int maxfps, int minfps, int bitrate, int gop, int resolutionLevel,
				int videoQuality, int playType, int apiLevel, int useSSL) ;

	void setExtraData(int extraType, const char* extraData);

	/**
	 * 设置断开时是否重连
	 * @param b 是否重连
	 */
	void setReconnectable(bool b);

	/**
	 * 设置业务类型
	 * @param type  试玩=0, 主营=1
	 */
	void setBusinessType(int type);

	/**
	 * 设置缓冲区大小
	 * @param size 大于0，小于((1024 + 512) * 1024)
	 */
	int setBufferSize(int size);

	/**
	 * 保存视频
	 */
	void saveVideo(const char* filename);
	/**
	 * 保存音频
	 */
	void saveAudio(const char* filename);
	/**
	 * 保存音视频
	 */
	void saveAV(const char* filename);

	/**
	 * 开始
	 * @return 0为成功，其它为失败
	 */
	int start() override;
	/**
	 * 停止
	 */
	void stop() override;

	/**
	 * 开启关闭投屏
	 * @param playType 0：关闭，1：开启视频，2，开启音频，3：开启音视频
	 * @return 0为成功，其它为失败
	 */
	int screenCast(int playType) override;

	/**
	 * 设置视频码流的配置档数（高清，标清，流畅三档配置信息）
	 * @param videoLevel 视频码流的配置档
	 * @return 0为成功，其它为失败
	 */
	int setVideoLevels(VideoLevel* videoLevel, int count) override;
	/**
	 * 设置投屏用哪一档
	 * @param levelIndex （0：自动，1：高清，2：标清，3：流畅）
	 * @return 0为成功，其它为失败
	 */
	int setVideoLevel(int levelIndex) override;
	/**
	 * 获取当前投屏用哪一档
	 * @return 0：自动，1：高清，2：标清，3：流畅
	 */
	int getVideoLevel() override;
	/**
	 * 获取当前投屏画质信息
	 * @return 0为成功，其它为失败
	 */
	int getVideoLevelResult(VideoLevel* videoLevel) override;

	/**
	 * 设置为固定的分辨率
	 * @param b true:固定的，false:非固定的
	 */
	void setFixedResolution(bool b) override;

	/**
	 * 发送AAC音频数据到设备
	 * @param frametype 0:AAC special data, 1:AAC frame
	 * @param data AAC音频数据
	 * @return 0为成功，其它为失败
	 */
	int sendAudio(int frametype, const char* data, int len) override;
	/**
	 * 发送H264视频数据到设备
	 * @param frametype 0:sps, 1:pps, 2:I frame, 3:P frame
	 * @param data H264视频数
	 * @return 0为成功，其它为失败
	 */
	int sendVideo(int frametype, const char* data, int len) override;

	/**
	 * 发送按键的键值
	 * @param action
	 * @param scanCode
	 * @return 0为成功，其它为失败
	 */
	int sendKeyEvent(int action, int keyCode) override;

    /**
     * 触摸事件
     * @param eventAction	事件类型
     * @param pointerCount	触摸点个数
     * @param x
     * @param y
     * @param force
     */
	int sendTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]) override;

	/**
	 * 位置输入消息
	 * @param longitude
	 * @param latitude
	 * @param altitude
	 * @param floor
	 * @param horizontalaccuracy
	 * @param verticalaccuracy
	 * @param speed
	 * @param direction
	 * @param timestamp
	 * @return 0为成功，负数为失败
	 */
	int sendInputLocation(float longitude,
    		float latitude,
    		float altitude,
    		float floor,
    		float horizontalaccuracy,
    		float verticalaccuracy,
    		float speed,
    		float direction,
    		const char* timestamp) override;
	/**
	 * 加速度输入消息
	 * @param x
	 * @param y
	 * @param z
	 * @return 0为成功，负数为失败
	 */
	int sendInputAccelerometer(float x, float y, float z) override;
	/**
	 * 高度变化输入消息
	 * @param altitude
	 * @param pressure
	 * @return 0为成功，负数为失败
	 */
	int sendInputAltimeter(float altitude, float pressure) override;
	/**
	 * 陀螺仪输入消息
	 * @param x
	 * @param y
	 * @param z
	 * @return 0为成功，负数为失败
	 */
	int sendInputGyro(float x, float y, float z) override;
	/**
	 * 磁力变化输入消息
	 * @param x
	 * @param y
	 * @param z
	 * @return 0为成功，负数为失败
	 */
	int sendInputMagnetometer(float x, float y, float z) override;
	/**
	 * 重力感应输入消息
	 * @param x
	 * @param y
	 * @param z
	 * @return 0为成功，负数为失败
	 */
	int sendInputGravity(float x, float y, float z) override;

	/**
	 * @return 获取屏幕方向
	 */
	int getScreenRotation() override;
	/**
	 * 当前控制模式
	 * @return 大于0为成功，1:可控制，2:仅观看。负数为失败
	 */
	int currentControlMode() override;
	/**
	 * 授权请求控制
	 * @param bControl
	 * @return 0为成功，负数为失败
	 */
	int sendControlGrant(bool bControl) override;
	/**
	 * 剪贴板
	 * @param value
	 * @return 0为成功，负数为失败
	 */
	int copyToRemote(const char* value, int len) override;

	/**
	 * 屏幕分享
	 * @param playtype
	 * @return 0为成功，负数为失败
	 */
	int screenSharing(int playtype) override;

	/**
	 * 设置静音
	 * @param playtype  静音：1    取消静音：3
	 * @return 0为成功，负数为失败
	 */
	int AVTransReq(int playtype) override;

	/**
	* 传输图片数据（如：云控二维码传入摄像头处理功能）
	* @param data  图片数据
	* @return 0为成功，其它为失败
	*/
	int sendPicture(const char* data, int len) override;

	int sendInputEmpty(const char* data, int len) override;
public:
	bool started() override;

	uint32_t getId() override;
	void setId(uint32_t id) override;

	SWTouchEvent* getEventHandler() override;

	void collectDecodeTime(int dtime) override;

    int getAudioFrameCount() override;
    int popAudioFrame(std::shared_ptr<APacket>* buffer, int waitMs) override;
//    int popAudioFrame(packet** buffer, int waitMs) override;
    int getVideoFrameCount() override;
    int popVideoFrame(std::shared_ptr<APacket>* buffer, int waitMs) override;
//    int popVideoFrame(packet** buffer, int waitMs) override;

    void OnTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]) override;

	bool attach(uint32_t id) override;
	bool detach(uint32_t id) override;
public:
	void onAudioStreamChanged(int type, int profile, int sampleRate, int channelCount);
	void onVideoStreamChanged(int videoWidth, int videoHeight,
			const char *sps, int sps_len, const char *pps, int pps_len);
	void onScreenRotation(int rotation);

	void pushAudioFrame(const char *data, int length);
	//0:sps, 1:pps, 2:I frame, 3:P frame
	void pushVideoFrame(int frametype, const char *data, int length);

private:
	Mutex mMutex;

	Mutex mAMutex;
	Mutex mVMutex;

	Condition mACondition;
	Condition mVCondition;

	std::list<std::shared_ptr<APacket> > mAudioList;
	std::list<std::shared_ptr<APacket> > mVideoList;

//	linkedlist mVideoList;
//	linkedlist mAudioList;

	std::shared_ptr<PlayEventListener> mPlayEventListenerSharedPtr;

	MyPlayDataSourceListenerInner* mInnerListener = NULL;
	SWDataSourceListener* mListener = NULL;
	PlayDataSource* mSource = NULL;

    char mSpecificControlIp[64];
    uint16_t mSpecificPort = 0;

    uint32_t mPlayerID = 0;

	FILE* mH264file = NULL;
	FILE* mAacfile = NULL;
	FILE* mAVfile = NULL;

	DISALLOW_EVIL_CONSTRUCTORS(SWDataSource);
};

#endif /* SWDATASOURCE_H_ */
