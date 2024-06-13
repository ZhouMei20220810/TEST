/*
 * DataSourceListener.h
 *
 */

#ifndef DATASOURCE_LISTENER_H_
#define DATASOURCE_LISTENER_H_

enum{
    PLAY_ONRECONNECTING        = 199,
    PLAY_ONCONNECTED        = 200,
    PLAY_ONDISCONNECTED        = 201,
    PLAY_ONDISCONNECTED2    = 2011,
    PLAY_ONSCREENROTATION    = 202,
    PLAY_ONSENSORINPUT        = 203,
    PLAY_ONINFO                = 204,

    PLAY_ONCONTROLUSERCOUNT            = 205,
    PLAY_ONCONTROLQUERYAUTH_REQ        = 206,
    PLAY_ONCONTROLAUTHCHANGE        = 207,
    PLAY_ONCONTROLTIME                = 208,
    PLAY_ONCOPYTOREMOTE_RES            = 209,
    PLAY_ONCONTROLVIDEO_RES            = 210,
    PLAY_ONSCREEN_SHARED_RES        = 211,

    PLAY_ONTRANSPARENTMSG_REQ        = 231,
    PLAY_ONTRANSPARENTMSG_RES        = 232,

    PLAY_ONOUTPUTSTRING                = 233,
    PLAY_ONOUTPUTBRIGHT                = 234,
    PLAY_ONOUTPUTCLIPER                = 235,
    PLAY_ONOUTPUTCURSOR                = 236,

    PLAY_ONOUT_FILE_URL                = 240,
    PLAY_ONOUT_GAME_VIDEOFILE_URL      = 241,

    PLAY_ONSENSORINPUT_ON            = 245,
    PLAY_ONSENSORINPUT_OFF           = 246,
    PLAY_ONKEYBOARDTYPE              = 247,
    PLAY_CLOUD_NOTIFY              	 = 248,
    PLAY_ONSDKINFO_RES               = 249,

    PLAY_CLOUDAPP_STATE              = 250,
    PLAY_CLOUD_PHONE_EVENT           = 251,

    PLAY_ONWSSHAKE_SUCCESSED        = 252,
    PLAY_ONGATEWAY_NOTIFY        = 253,
    PLAY_ONP2P_NOTIFY        = 254,
    PLAY_TCP_CONNECTED      = 255,
    PLAY_VIDEO_QUALITY_CHANGED      = 256,
    PLAY_NAV_BAR_STATE      = 257,
    PLAY_REMOTE_KEYBOARD_RES     = 258,
};

#define BASE_OUTPUTBRIGHT    1000000.0f

class PlayEventListener{
public:
    PlayEventListener(){}
    virtual ~PlayEventListener(){}

    virtual void onReconnecting(int nthTime, int errcode){}
    virtual void onConnected(){}
    virtual void onDisconnected(int errcode){}
    virtual void onDisconnected(int errcode, const char* errmsg){}

    virtual void onScreenRotation(int rotation){}

    virtual void onSensorInput(int inputtype, int state, int width, int height , const char* data){}

    virtual void onControlUserCount(int count){}
    virtual void onControlQueryAuthReq(int controlMode){}
    virtual void onControlAuthChangeNotify(int controlMode){}
    virtual void onControlTime(int remainingTime){}

    virtual void onCopyToRemoteRes(int result){}

    virtual void onControlVideo(int videoQuality, int fps){}

    virtual void onScreenSharing(bool video, bool audio){}

    //json format
    virtual void onPlayInfo(const char* info){}

    virtual void onTransparentMsgReq(int type, const char* data, const char* binderService){}
    virtual void onTransparentMsgRes(int result, int type, const char* binderService){}

    virtual void onCloudAppNotification(const char* data, int len){}

    virtual void onOutputBright(float bright){}

    virtual void onCopyFromRemote(const char* data, int len){}

    virtual void onGameScreenshots(const char* filename, int filenameLen,
        const char* filecontent, int filecontentLen){}

    virtual void onRecvKeyboardType(int type){}

    virtual void onRecvSdkInfoR(const char* sdk_info, int len){}

    virtual void onOutputCursor(int opt) {}

    virtual void onRecvNavBarStateR(int state){}

    virtual void onRecvKeyboardActiveR(int active){}

    virtual void onScreenshotsFile(const char* fileurl, int fileurlLen, const char* filename, int filenameLen,
        int fileLen){}

    virtual void onGameVideoFile(const char* fileurl, int fileurlLen, const char* filename, int filenameLen,
        int fileLen){}

    virtual void onCloudAppEvent(int type, int state){}

	virtual void onCloudNotify(int type, const char* msg){}

	virtual void OutputCloudPhoneEvent(const char* phoneNum){}

    // for websocket
    virtual void onWsShakeNotification(int state, int reconnect){}
    //for gateWay
    virtual void onGatWayNotify(int state, const char* info){}
    //for P2P
    virtual void onP2pNotify(int state, const char* info){}
    // tcp 连上
    virtual void onTcpNotification(int state, int reconnect){}
    //画质切换
    virtual void onVideoQualityChange(char* info){}
private:
    PlayEventListener(const PlayEventListener &);
    PlayEventListener &operator=(const PlayEventListener &);
};

class PlayAudioListener{
public:
    PlayAudioListener(){}
    virtual ~PlayAudioListener(){}

    virtual void onAudioStreamChanged(int type, int profile, int sampleRate, int channelCount){}

    virtual void onAudioStop() {};

    virtual void incomingAudio(const char *data, int length){}
private:
    PlayAudioListener(const PlayAudioListener &);
    PlayAudioListener &operator=(const PlayAudioListener &);
};

class PlayVideoListener{
public:
    PlayVideoListener(){}
    virtual ~PlayVideoListener(){}

    virtual void onVideoStreamChanged(int width, int height, const char *sps, int sps_len, const char *pps, int pps_len){}
    virtual void onScreenRotation(int rotation){}

    //0:sps, 1:pps, 2:I frame, 3:P frame
    virtual void incomingVideo(int frametype, const char *data, int length){}

    virtual void cleanVideoQueue(){}
private:
    PlayVideoListener(const PlayVideoListener &);
    PlayVideoListener &operator=(const PlayVideoListener &);
};

#endif /* DATASOURCE_LISTENER_H_ */
