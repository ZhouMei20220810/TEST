/*
 * SWDataSourceListener.h
 *
 */

#ifndef SWDATASOURCE_LISTENER_H_
#define SWDATASOURCE_LISTENER_H_

class AWE_DECLARE_DATA SWDataSourceListener{
public:
	SWDataSourceListener(){}
	virtual ~SWDataSourceListener(){}

    /**
     * 正在重连
     * @param NthTime  第几次尝试重连
     */
	virtual void onReconnecting(int NthTime){}
    /**
     * 连接成功
     */
	virtual void onConnected(){}
    /**
     * (主营)设备连接断开
     * @param errcode 错误码
     */
	virtual void onDisconnected(int errcode){}
	/**
	 * (试玩)设备连接断开
	 * @param errcode 错误码
	 */
	virtual void onDisconnected(int errcode, const char* errmsg) {}

    /**
     * Not used
     * @param rotation rotation
     */
	virtual void onScreenRotation(int rotation){}

    /**
     * 传感器输入
     * @param inputtype     传感器类型:
     *                         位置数据:201, 加速度:202, 高度计:203, 陀螺仪:204, 磁力计:205,
     *                         音频输入:211, 视频输入:212, 重力感应:213
     * @param state          传感器状态，1：开，0：关
     */
	virtual void onSensorInput(int inputtype, int state){}
    /**
     * 投屏时的统计信息
     * @param info 统计信息json format
     */
	virtual void onPlayInfo(const char* info){}

	/**
	 * @param count User count
	 */
	virtual void onControlUserCount(int count){}
	/**
	 * 是否取回控制权
	 * @param controlMode 控制权
	 */
	virtual void onControlQueryAuthReq(int controlMode){}
	/**
	 * 控制权改变通知
	 * @param controlMode 控制权
	 */
	virtual void onControlAuthChangeNotify(int controlMode){}
	/**
	 * 剩余控制时间
	 * @param remainingTime 剩余控制时间
	 */
	virtual void onControlTime(int remainingTime){}
	/**
	 * 剪贴板copy到remote的结果
	 * @param result 结果
	 */
	virtual void onCopyToRemoteRes(int result){}

	/**
	 * 视频画质
	 * @param videoQuality	画质等级
	 * @param fps			帧率
	 */
	virtual void onControlVideo(int videoQuality, int fps){}

	/**
	 * 屏幕分享结果
	 * @param video	视频
	 * @param audio	音频
	 */
	virtual void onScreenSharing(bool video, bool audio){}
private:
	SWDataSourceListener(const SWDataSourceListener &);
	SWDataSourceListener &operator=(const SWDataSourceListener &);
};

#endif /* SWDATASOURCE_LISTENER_H_ */
