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

class AWE_DECLARE_DATA SWDataSource : public DataSource, public SWTouchEvent{
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

#define PLAYTYPE_VIDEO    1//播放视频
#define PLAYTYPE_AUDIO    2//播放音频
#define PLAYTYPE_AV        3//播放音视频

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
     * @param playType         播放类型， 0：无，1：视频， 2：音频, 3：音视频
     * @param apiLevel      1：旧接口，2：新接口
     * @param useSSL        1:使用SSL加密，0：不加密
     * @return 0为成功，其它为失败
     */
    int setPlayParams(const char* appName, int encodetype, int width, int height,
                int maxfps, int minfps, int bitrate, int gop, int resolutionLevel,
                int videoQuality, int playType, int apiLevel, int useSSL) ;

    void setExtraData(int extraType, const char* extraData);

    /**
     * @brief Set the Keyboard Params object
     *
     * @param platform  1:安卓 2.js 3.webrtc 4.PC
     * @param active 0：屏蔽云手机键盘 1：开启云手机键盘
     * @return int
     */
    int setKeyboardParams(int platform, int active);

    /**
     * 设置断开时是否重连
     * @param b 是否重连
     */
    void setReconnectable(bool b);

    /**
     * 设置业务类型
     * @param type  试玩=0, 主营=1, 百度=2
     */
    void setBusinessType(int type);

    /**
     * 设置连接设备类型
    　* @param type  ToB=0, ToC=1
     */
    void setInstanceType(int type);

    /**
     * 设置是否自动控制视频画质
     * @param control  1:是, 0:否
     */
    void setAutoControlVideoQuality(int control);

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
     * 请求带时间戳的音视频推流
     */
    void requestTimeStamp(bool audioTimestamp, bool videoTimestamp);

    /**
     * 设置丢帧数
     */
    void setDiscardVideoFrameNum(int discardVideoFrameNum);

    /**
     * 获取丢弃最新视频帧的pts
     */
    int64_t getDiscardVideoPts();

    /**
     * 获取丢弃最新视频帧的dts
     */
    int64_t getDiscardVideoDts();

    /**

j=key    /**
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
     * 设置视频播放参数
     * @param encodeType        编码格式
     * @param resolutionLevel   分辨率等级, 优先使用. 传0表示不使用等级, 而是使用具体的宽高值.
     * @param width             分辨率宽
     * @param height            分辨率高
     * @param videoQuality      码率等级, 优先使用. 传0表示不使用等级, 而是使用具体的码率值.
     * @param bitrate           码率
     * @param fps               帧率
     */
    int setVideoLevel(int encodeType, int resolutionLevel, int width, int height, int videoQuality, int bitrate, int fps);
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
     * @param eventAction    事件类型
     * @param pointerCount    触摸点个数
     * @param x
     * @param y
     * @param force
     */
    int sendTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]) override;

    /**
     * @param type 键盘类型 0：硬键盘 1：软键盘
     *
     */
    int sendKeyboardSwitch(int type) override;

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
     * 罗盘输入消息
     * @param x
     * @param y
     * @param z
     * @return 0为成功，负数为失败
     */
    int sendInputCompass(float x, float y, float z) override;
    /**
     * 步数检测器输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputStepDetector(float x) override;
    /**
     * 计步器输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputStepCount(float x) override;
    /**
     * 光感器输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputLight(float x) override;
    /**
     * 压力计输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputPressure(float x) override;
    /**
     * 测距仪输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputProximity(float x) override;
    /**
     * 温度计输入消息
     * @param x
     * @return 0为成功，负数为失败
     */
    int sendInputTemperature(float x) override;

    /**
     * 发送游戏手柄控制消息. 参数解释参考https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
     * @param index 摇杆编号
     * @param pressed 按键状态, 1:down, 0:up
     * @param buttons 按键值
     * @param lt
     * @param rt
     * @param lx
     * @param ly
     * @param rx
     * @param ry
     *
     * enum
     * {
     *   JOY_BTN_SELECT    = 1,  //按键SELECT
     *   JOY_BTN_START    = 2,  //按键START
     *   JOY_BTN_A        = 3,  //按鍵A 
     *   JOY_BTN_B        = 4,  //按键B
     *   JOY_BTN_X        = 5,  //按键X 
     *   JOY_BTN_Y        = 6,  //按键Y
     *   JOY_BTN_LB       = 7,  //按键LB
     *   JOY_BTN_RB       = 8,  //按键RB
     *   JOY_BTN_LT       = 9,  //按键LT
     *   JOY_BTN_RT       = 10,  //按键RT
     *   JOY_BTN_LEFT     = 11,  //按键十字键左
     *   JOY_BTN_RIGH     = 12,  //按键十字键右
     *   JOY_BTN_UP       = 13,  //按键十字键上
     *   JOY_BTN_DOWN     = 14,  //按键十字键下
     *   JOY_BTN_LEFT_STICK   = 15,  //按键左摇杆
     *   JOY_BTN_RIGH_STICK   = 16,  //按键右摇杆
     *   JOY_STICK_LEFT     = 17,  //左摇杆移动
     *   JOY_STICK_RIGH      = 18,  //右摇杆移动
     * };
     * @return 0为成功，其它为失败
     */
    int sendInputGameController(uint8_t index, uint8_t pressed, uint16_t buttons, uint8_t lt, uint8_t rt, uint16_t lx, uint16_t ly, uint16_t rx, uint16_t ry) override;

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
     * 字符串输入
     * @param value
     * @return 0为成功，负数为失败
     */
    int sendInputString(const char* value, int len) override;
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

    /**
     * 发送透传消息到云手机
     * @param type  数据类型
     * @param data  数据
     * @param binderService  服务名
     * @return 0为成功，其它为失败
     */
    int sendTransparentMsgReq(int type, const char* data, const char* binderService) override;

    /**
     * 发送透传消息的回复到云手机
     * @param result  错误码
     * @param type  数据类型
     * @param binderService  服务名
     * @return 0为成功，其它为失败
     */
    int sendTransparentMsgRes(int result, int type, const char* binderService) override;

    /**
     * 设置sessionID
     */
    int setSessionId(const char* sessionId);

    /**
    * 发送平台消息到云手机
    * @param sdk_info 客户端消息
    */
    int sendSdkInformation(const char* sdk_info)override;

    /**发送是否激活云机键盘消息
     * @param platform 平台消息 1:安卓 2.js 3.webrtc 4.PC
     * @param active 是否激活云机键盘
     */

    int sendActiveKeyboard(int platform, int active) override;

    /**
     * 查询导航键的状态 0: 导航键显示 1：导航键隐藏
     * @return 0为成功，负数为失败
     */
    int getNavBarState();

    /**
     * 设置导航键的状态
     * @param state 1: 导航键显示 2：导航键隐藏
     * @return 0为成功，负数为失败
     */
    int setNavBarState(int state);

public:
    bool started() override;

    uint32_t getId() override;
    void setId(uint32_t id) override;

    SWTouchEvent* getEventHandler() override;

    void collectDecodeTime(int dtime) override;
    void collectVideoRenderer() override;

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
    void onAudioStop();
    void onVideoStreamChanged(int videoWidth, int videoHeight,
            const char *sps, int sps_len, const char *pps, int pps_len);
    void onScreenRotation(int rotation);

    void pushAudioFrame(const char *data, int length);
    //0:sps, 1:pps, 2:I frame, 3:P frame
    void pushVideoFrame(int frametype, const char *data, int length);

    // 清空视频帧
    void cleanVideoQueue();

    int getApiLevel();
protected:
    PlayDataSource* mSource = NULL;
private:
    Mutex mMutex;

    Mutex mAMutex;
    Mutex mVMutex;

    Condition mACondition;
    Condition mVCondition;

    std::list<std::shared_ptr<APacket> > mAudioList;
    std::list<std::shared_ptr<APacket> > mVideoList;

//    linkedlist mVideoList;
//    linkedlist mAudioList;

    std::shared_ptr<PlayEventListener> mPlayEventListenerSharedPtr;

    MyPlayDataSourceListenerInner* mInnerListener = NULL;
    SWDataSourceListener* mListener = NULL;

    char mSpecificControlIp[64];
    uint16_t mSpecificPort = 0;

    uint32_t mPlayerID = 0;

    FILE* mH264file = NULL;
    FILE* mAacfile = NULL;
    FILE* mAVfile = NULL;
    int mDiscardVideoFrameNum = 30;
    int64_t mDiscardVideoPts = 0;
    int64_t mDiscardVideoDts = 0;

    DISALLOW_EVIL_CONSTRUCTORS(SWDataSource);
};

#endif /* SWDATASOURCE_H_ */
