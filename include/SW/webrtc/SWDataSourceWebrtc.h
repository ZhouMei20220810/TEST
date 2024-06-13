//
// Created by Administrator on 2023/2/6.
//

#ifndef MCI_ANDROID_DEMO_SWDATASOURCEWEBRTC_H
#define MCI_ANDROID_DEMO_SWDATASOURCEWEBRTC_H
#include "SWDataSource.h"

class PlayDataSourceWebrtc;

#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
class OnWebRTCInfoListener{
public:
    OnWebRTCInfoListener(){}
    virtual ~OnWebRTCInfoListener(){}

    virtual void onRecvOffer(const char* offer) = 0;
    virtual void onRecvCandidate(const char* sdpMid, int sdpMLineIndex, const char* candidate) = 0;
private:
    OnWebRTCInfoListener(const OnWebRTCInfoListener &);
    OnWebRTCInfoListener &operator=(const OnWebRTCInfoListener &);
};
#endif

class AWE_DECLARE_DATA SWDataSourceWebrtc : public SWDataSource {
public:
    SWDataSourceWebrtc(uint32_t id, SWDataSourceListener* l) : SWDataSource(id, l){}
    SWDataSourceWebrtc(uint32_t id, const std::shared_ptr<PlayEventListener> l) : SWDataSource(id, l){}
    ~SWDataSourceWebrtc();

#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
    OnWebRTCInfoListener* getOnWebRTCInfoListener(){
        return mOnWebRTCInfoListener;
    }
    void setOnWebRTCInfoListener(OnWebRTCInfoListener* l){
        mOnWebRTCInfoListener = l;
    }
#endif

    void sendWebrtcSdp(const char* sdp);
    void sendWebrtcCandidate(const char* sdpMid, int sdpMLineIndex, const char* candidate);

    void setGateWayControlInfo(const char* controlIp, int controlPort);

    /**
     * ����webrtc p2p����
     * @param remoteUdpIp   �滻webrtc�Զ�candidate��IP
     * @param remoteUdpPort �滻webrtc�Զ�candidate�Ķ˿�
     */
    void setWebRTCP2PInfo(const char* remoteUdpIp, int remoteUdpPort);

    int requestReconnect();

protected:
#if defined(USING_WEBSOCKET) && (defined(_WIN32) || __APPLE__)
    OnWebRTCInfoListener* mOnWebRTCInfoListener = NULL;
#endif

private:
    PlayDataSourceWebrtc* mPlayDataSourceWebrtc = NULL;
};

#endif //MCI_ANDROID_DEMO_SWDATASOURCEWEBRTC_H
