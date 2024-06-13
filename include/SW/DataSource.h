/*
 * DataSource.h
 *
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

//#include <string>
#include <memory>

#include <SWCommon.h>

class DataSource;

class OnAudioStreamChangedListener{
public:
    OnAudioStreamChangedListener(){}
    virtual ~OnAudioStreamChangedListener(){}

    virtual void onAudioStreamChanged(DataSource* dataSource,
            int type, int profile, int sampleRate, int channelCount) = 0;
    
    virtual void onAudioStop() = 0;
private:
    OnAudioStreamChangedListener(const OnAudioStreamChangedListener &);
    OnAudioStreamChangedListener &operator=(const OnAudioStreamChangedListener &);
};

class OnVideoStreamChangedListener{
public:
    OnVideoStreamChangedListener(){}
    virtual ~OnVideoStreamChangedListener(){}

    virtual void onVideoStreamChanged(DataSource* dataSource,
            int width, int height, const char *sps, int sps_len, const char *pps, int pps_len) = 0;
    virtual void onScreenRotation(DataSource* dataSource, int rotation) = 0;

private:
    OnVideoStreamChangedListener(const OnVideoStreamChangedListener &);
    OnVideoStreamChangedListener &operator=(const OnVideoStreamChangedListener &);
};

class SWTouchEvent;
class APacket;

class AWE_DECLARE_DATA DataSource{
public:
    DataSource(){}
    virtual ~DataSource(){}

    void setOnAudioStreamChangedListener(OnAudioStreamChangedListener* l){
        mOnAudioStreamChangedListener = l;
    }
    void setOnVideoStreamChangedListener(OnVideoStreamChangedListener* l){
        mOnVideoStreamChangedListener = l;
    }

    virtual uint32_t getId(){return 0;}
    virtual void setId(uint32_t id){}

    virtual SWTouchEvent* getEventHandler(){return NULL;}

    virtual int start(){return -1;}
    virtual void stop(){}

    virtual bool started(){return false;}

    virtual int screenCast(int playType){return -1;}

    virtual void collectDecodeTime(int dtime){}
    virtual void collectVideoRenderer() {}

    virtual int setVideoLevels(VideoLevel* videoLevel, int count){return -1;}
    virtual int setVideoLevel(int levelIndex){return -1;}
    virtual int getVideoLevel(){return -1;}

    virtual int getVideoLevelResult(VideoLevel* videoLevel) { return -1; }

    virtual void setFixedResolution(bool b){}

    virtual int sendAudio(int frametype, const char* data, int len){return -1;}
    virtual int sendVideo(int frametype, const char* data, int len){return -1;}

#define SW_ACTION_KEY_DOWN    1
#define SW_ACTION_KEY_UP    (1 << 1)
    virtual int sendKeyEvent(int action, int keyCode){return -1;}

    virtual int sendTouchEvent(int eventAction, int pointerCount, int x[], int y[], float force[]) {return -1;}

    virtual int sendKeyboardSwitch(int type) { return -1;}

    virtual int sendInputLocation(float longitude,
            float latitude,
            float altitude,
            float floor,
            float horizontalaccuracy,
            float verticalaccuracy,
            float speed,
            float direction,
            const char* timestamp){return -1;}
    virtual int sendInputAccelerometer(float x, float y, float z){return -1;}
    virtual int sendInputAltimeter(float altitude, float pressure){return -1;}
    virtual int sendInputGyro(float x, float y, float z){return -1;}
    virtual int sendInputMagnetometer(float x, float y, float z){return -1;}
    virtual int sendInputGravity(float x, float y, float z){return -1;}
    virtual int sendInputCompass(float x, float y, float z){return -1;}
    virtual int sendInputStepDetector(float x){return -1;}
    virtual int sendInputStepCount(float x){return -1;}
    virtual int sendInputLight(float x){return -1;}
    virtual int sendInputPressure(float x){return -1;}
    virtual int sendInputProximity(float x){return -1;}
    virtual int sendInputTemperature(float x){return -1;}

    virtual int getAudioFrameCount() {return -1;}
    virtual int popAudioFrame(std::shared_ptr<APacket>* buffer, int waitMs) {return -1;}
//    virtual int popAudioFrame(packet** buffer, int waitMs) {return -1;}
    virtual int getVideoFrameCount() {return -1;}
    virtual int popVideoFrame(std::shared_ptr<APacket>* buffer, int waitMs) {return -1;}
//    virtual int popVideoFrame(packet** buffer, int waitMs) {return -1;}

    virtual bool attach(uint32_t lid){return false;}
    virtual bool detach(uint32_t lid){return false;}

    /**
     * return (value % 2 == 0) ? PORTRAIT : LANDSCAPE;
     */
    virtual int getScreenRotation(){return 0;}
    virtual int currentControlMode(){return -1;}
    virtual int sendControlGrant(bool bControl){return -1;}
    virtual int sendInputString(const char* value, int len){return -1;}
    virtual int copyToRemote(const char* value, int len){return -1;}

    virtual int screenSharing(int playtype){return -1;}
    virtual int AVTransReq(int playtype){return -1;}
    virtual int sendPicture(const char* data, int len) { return -1; }

    virtual int sendInputEmpty(const char* data, int len){return -1;}

    virtual int sendTransparentMsgReq(int type, const char* data, const char* binderService) {return -1;}

    virtual int sendTransparentMsgRes(int result, int type, const char* binderService) {return -1;}

    virtual int sendInputGameController(uint8_t index, uint8_t pressed, uint16_t buttons, uint8_t lt, uint8_t rt, uint16_t lx, uint16_t ly, uint16_t rx, uint16_t ry) {return -1;}

    virtual int sendSdkInformation(const char* sdk_info) { return -1;}
    virtual int sendActiveKeyboard(int platform, int active) {return -1;}
protected:
//    int mId = 0;
    bool mStarted = false;

    OnAudioStreamChangedListener* mOnAudioStreamChangedListener = NULL;
    OnVideoStreamChangedListener* mOnVideoStreamChangedListener = NULL;
private:
    DataSource(const DataSource &);
    DataSource &operator=(const DataSource &);
};

#endif /* DATASOURCE_H_ */
