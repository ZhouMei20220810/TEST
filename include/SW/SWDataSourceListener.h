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
     * @param nthTime  第几次尝试重连
     */
    virtual void onReconnecting(int nthTime){}
    virtual void onReconnecting(int nthTime, int errcode){ onReconnecting(nthTime); }
    
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
    virtual void onDisconnected(int errcode, const char* errmsg) { onDisconnected(errcode); }

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
     * 传感器输入
     * @param inputtype     传感器类型:
     *                         位置数据:201, 加速度:202, 高度计:203, 陀螺仪:204, 磁力计:205,
     *                         音频输入:211, 视频输入:212, 重力感应:213
     * @param state          传感器状态，1：开，0：关
     * @param width         摄像头宽
     * @param height        摄像头高
     */
    virtual void onSensorInput(int inputtype, int state, int width, int height){
        onSensorInput(inputtype, state);
    }
    
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
     * @param videoQuality    画质等级
     * @param fps            帧率
     */
    virtual void onControlVideo(int videoQuality, int fps){}

    /**
     * 屏幕分享结果
     * @param video    视频
     * @param audio    音频
     */
    virtual void onScreenSharing(bool video, bool audio){}

    /**
     * 收到云手机发送的透传数据
     * @param type  数据类型
     * @param data  数据
     * @param binderService  服务名
     */
    virtual void onTransparentMsgReq(int type, const char* data, const char* binderService){}

    /**
     * 调用sendTransparentMsg后, 收到的回复.
     * @param result 错误码
     * @param type  数据类型
     * @param binderService  服务名
     */
    virtual void onTransparentMsgRes(int result, int type, const char* binderService){}

    /**
     * 云手机应用的消息通知
     * @param data 内容
     * @param len 长度
     */
    virtual void onCloudAppNotification(const char* data, int len){}
    
    /**
     * 亮度
     * @param bright 亮度
     */
    virtual void onOutputBright(float bright){}

    /**
     * @brief 
     * 
     * @param opt 输入法激活参数 0x00007FFF(32767):输入法激活, 0x00007FFC(32764):输入完成
     */
    virtual void onOutputCursor(int opt){}

    /**
     * 从设备复制出来的内容
     * @param data 内容
     * @param len 长度
     */
    virtual void onCopyFromRemote(const char* data, int len){}

    /**
     * 云手机游戏截图,(不能在此方法做耗时的事)
     * @param filename 截图文件名
     * @param filenameLen 截图文长度
     * @param filecontent 截图内容
     * @param filecontentLen 截图内容长度
     */
    virtual void onGameScreenshots(const char* filename, int filenameLen,
        const char* filecontent, int filecontentLen){}

    /**
     * @param type 键盘类型 0：硬键盘  1：软键盘
     *
     */
    virtual void onRecvKeyboardType(int type) {}

    /**
     * @param sdk_info sdk内容回传
     *
     */
    virtual void onRecvSdkInfoR(const char* sdk_info, int len){}

    /**
     * @param state 导航键状态 0: 导航键显示 1：导航键隐藏
     *
     */
    virtual void onRecvNavBarStateR(int state){}

    /**
     * @param active 云机输入法键盘激活状态 0: 导航键显示 1：导航键隐藏
     *
     */
    virtual void onRecvKeyboardActiveR(int active){}

    /**
     * websocket握手结果通知
     * @param state   1：成功，其他失败
     */
    virtual void onWsShakeNotification(int state, int reconnect){}

    /**
    *网关结果通知
    *@param state   1：成功，其他失败
    *@param info   json信息
    */
    virtual void onGatWayNotify(int state, const char* info){}

    /**
    *P2P结果通知
    *@param state   1：成功，其他失败
    *@param info   json信息
    */
    virtual void onP2pNotify(int state, const char* info){}

    /**
    *tcp 连通，埋点用
    *@param state   1：成功，其他失败
    */
    virtual void onTcpNotification(int state, int reconnect){}

    /**
    *画质切换回调
    *@param info   画质信息
    */
    virtual void onVideoQualityChange(char* info){}
private:
    SWDataSourceListener(const SWDataSourceListener &);
    SWDataSourceListener &operator=(const SWDataSourceListener &);
};

#endif /* SWDATASOURCE_LISTENER_H_ */
