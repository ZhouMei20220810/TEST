/*
 * DataSourceListener.h
 *
 */

#ifndef DATASOURCE_LISTENER_H_
#define DATASOURCE_LISTENER_H_

enum{
	PLAY_ONRECONNECTING		= 199,
	PLAY_ONCONNECTED		= 200,
	PLAY_ONDISCONNECTED		= 201,
	PLAY_ONDISCONNECTED2	= 2011,
	PLAY_ONSCREENROTATION	= 202,
	PLAY_ONSENSORINPUT		= 203,
	PLAY_ONINFO				= 204,

	PLAY_ONCONTROLUSERCOUNT			= 205,
	PLAY_ONCONTROLQUERYAUTH_REQ		= 206,
	PLAY_ONCONTROLAUTHCHANGE		= 207,
	PLAY_ONCONTROLTIME				= 208,
	PLAY_ONCOPYTOREMOTE_RES			= 209,
	PLAY_ONCONTROLVIDEO_RES			= 210,
	PLAY_ONSCREEN_SHARED_RES		= 211,
};

class PlayEventListener{
public:
	PlayEventListener(){}
	virtual ~PlayEventListener(){}

	virtual void onReconnecting(int NthTime){}
	virtual void onConnected(){}
	virtual void onDisconnected(int errcode){}
	virtual void onDisconnected(int errcode, const char* errmsg){}

	virtual void onScreenRotation(int rotation){}

	virtual void onSensorInput(int inputtype, int state){}

	virtual void onControlUserCount(int count){}
	virtual void onControlQueryAuthReq(int controlMode){}
	virtual void onControlAuthChangeNotify(int controlMode){}
	virtual void onControlTime(int remainingTime){}

	virtual void onCopyToRemoteRes(int result){}

	virtual void onControlVideo(int videoQuality, int fps){}

	virtual void onScreenSharing(bool video, bool audio){}

	//json format
	virtual void onPlayInfo(const char* info){}

//	virtual void onAudioStreamChanged(int type, int profile, int sampleRate, int channelCount){}
//
//	virtual void onVideoStreamChanged(int videoWidth, int videoHeight,
//			const char *sps, int sps_len, const char *pps, int pps_len){}
//
//	virtual void incomingAudio(const char *data, int length){}
//	//0:sps, 1:pps, 2:I frame, 3:P frame
//	virtual void incomingVideo(int frametype, const char *data, int length){}
private:
	PlayEventListener(const PlayEventListener &);
	PlayEventListener &operator=(const PlayEventListener &);
};

class PlayAudioListener{
public:
	PlayAudioListener(){}
	virtual ~PlayAudioListener(){}

	virtual void onAudioStreamChanged(int type, int profile, int sampleRate, int channelCount){}

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
private:
	PlayVideoListener(const PlayVideoListener &);
	PlayVideoListener &operator=(const PlayVideoListener &);
};

#endif /* DATASOURCE_LISTENER_H_ */
