#pragma once

/*
2018-07-03 3.0.0
1，项目初始建立。

2018-09-29 3.1.1
1, 增加重连功能
2, 增加SSL加密

2018-11-28 3.1.3
1, 增加音视频输入接口

2018-12-05 3.1.4
1, 优化音视频缓存

2019-01-22 3.1.5
1, 增加收回控制权与控制权改变通知的接口
2, 增加回调剩余控制时间的接口
3, 增加剪贴板功能
4, 处理MSG_CONTROL_INPUT,    // 控制是否可以input(试玩)
   与MSG_CONTROL_USERS_R,   // number of users(主营)
   的问题。

2019-05-21 3.1.5.6
1, 修改错误码与主营一样。

2019-05-29 3.1.6.7
1, 增加双链路支持

2019-05-30 3.1.7.8
1, 增加自动升降帧，自动画质支持
2, 增加Android软解支持

2019-06-06 3.1.7.11
1, 修复被kick时导致的崩溃

2019-06-06 3.1.7.12
1, 增加屏幕分享返回接口

2019-06-06 3.1.7.14
1, 增加获取软解解码时间

2019-06-11 3.1.7.16
1, 修改关闭时音频播放会导致阻塞的问题。

2019-06-11 3.1.7.17
1, 修改关闭时player时可能导致崩溃的问题。

2019-06-13 3.1.7.20
1, 修改连接时可能导致崩溃的问题。

2019-06-17 3.1.8.25
1, 增加域名异步解析

2019-06-26 3.1.9.27
1, 增加PlayStatus原子操作

2019-07-23 3.1.10.29
1, 修改bugly上报的问题

2019-07-26 3.1.10.30
1, 修改误判延时的问题

2019-07-26 3.1.10.34
1, 去掉dns缓存

2019-08-01 3.1.11.37
1, dns解析改用gethostbyname
2, 增加dns缓存

2019-08-08 3.1.12.39
1，修改Android视频渲染方式

2019-08-14 3.1.12.40
1，心跳包如果没有及时回复，延时时间无法计算时，则定为500ms.

2019-08-21 3.1.12.43
1，fix HSZ-13881自动升降画质

2019-08-23 3.1.13.44
1，修复因上行网络拥堵，重连时发送不出数据的问题。

2019-09-05 3.1.13.45
1，支持 arm64-v8a

2019-10-31 3.1.13.46
1，修改TCP不延时发送

2019-11-07 3.1.14.47
1，增加iOS支持
 
2019-11-22 3.1.14.48
1，修复软解时出现的崩溃

2019-12-11 3.1.15.49
1，海外版SDK增加一键静音功能接口

2019-12-26 3.1.16.50
1，修改atomic操作接口

2020-01-20 3.1.17.51
1, shakeandplay增加extra字

2020-02-07 3.1.17.52
1, HSZ-17662修改对aac音频帧的判断

2020-02-27 3.1.18.53
1.HSZ-19661_新版SDK增加图片传输接口

2020-03-16 3.1.19.54
1.修复在XP下定时任务线程还没启动就被stop掉的问题

2020-03-20 3.1.20.56
1. fix Android delete Listener
2, 修复对string变量memset导致崩溃

2020-04-09 3.1.21.57
1. 增加Input空指令

2020-05-06 3.1.22.58
1. 弃用智能指针, 尝试修复iOS的崩溃问题

2020-07-10 3.1.23.59
1. 修复不能在win-x86_64运行的问题
2, 合并TransparentMsg协议

2020-07-22 3.1.24.60
1. 给试玩新增重载onDisconnect接口

2020-07-28 3.1.25.61
1. windows下视频解码增加硬件加速

2020-08-31 3.1.26.62
1. 修复音频解码可能会崩溃的问题

2020-09-07 3.1.27.63
1. 增加和虚拟机的通信通道的接口

2020-09-16 3.1.28.66
1. 增加手柄接口
2, 增加亮度输出接口
3, 增加UOS arm64支持

2020-11-19 3.1.29.67
1, 增加设置是否自动控制视频画质接口

2020-12-17 3.1.30.68
1, 增加Linux音频播放

2021-01-20 3.1.31.69
1, 增加从设备复制内容到本地接口
2, ControlResolution修改为支持传自定义分辨率

2021-02-02 3.1.32.70
1, 增加BusinessType(2)兼容旧接口传感器类型

2021-03-19 3.1.33.73
1, 增加云手机消息回调接口,onCloudAppNotification
2, 增加字符串输入接口,sendInputString
3, 修改音视频缓存大小超过最大值时, 删除缓存.(之前删除最大值的一半会出现花屏情况)

2021-04-25 3.1.33.74
1, 拉起应用协议, 增加可传入应用的数据

2021-05-08 3.1.34.79
1, 增加统计RTT协议;
2, 增加统计编码时间;
3, 修改ControlVideo接口, 支持传具体的码率值;
4, 增加投屏过程中直接设置具体分辨率码率和帧率的接口;
5, 修复win释放未padding内存时崩溃的问题;
6, table ControlVideo恢复原来的默认值;

2021-07-23 3.1.35.1
1, 增加截图传输接口

2021-09-16 3.1.36.1
1, 摄像头分辨率回传

2021-11-12 3.1.37.1
1, 添加传感器支持:陀螺仪：204,计步器:206,测距仪:207,光感器 :208,步数检测器:215,压力计 :216,罗盘:217

2022-03-28 3.1.38.1
1, Android硬解码的渲染方式改为用ffmpeg转换yuv为RGBA(bitmap)后再渲染, 解决多开渲染问题.
2022-05-23 3.1.38.2
1, 修改上个版在Android 6.0渲染时转码出现崩溃的问题.
2022-07-06 3.2.0
1, 规范版本号
2, 修复Linux下系统中断导致连接断开的问题.

2022-08-29 3.3.0
1.添加软硬键盘切换功能
2.pc端硬解加上控制宏，兼容xp版本
3.修复接收键盘切换类型协议
4.添加发送平台协议

2023-01-03 3.3.1
1. ARM-XQ-15966, 增加session数组长度到128
2. pc端硬解加上控制宏，兼容xp版本

2023-03-22 3.4.0
1. WCI-7559 ，修复条件变量释放时没有销毁创建的资源.

2023-06-29 3.5.0
1. ARM-XQ-19199 ，合并master分支代码，整合Android端的sdkInformation，增加开启软硬键盘的协议
1. WCI-7618， 增加PC端输入法透传接口

2023-07-21 3.6.0
1. ARM-XQ-19609 so接口协议统一使用加锁的方式进行发送，解决P2P推流发送协议失败的问题

2023-07-21 3.7.0
1. ARM-XQ-19609 webrtc接口协议统一使用加锁的方式进行发送，解决线上P2P推流异常崩溃

2023-08-02 3.8.0
1. Android端修复重连过程出现sslHandshaking超时后没有发起重连
2. Android端P2P模式在发送avFmt后面发送avTrans，发起关闭音频流推流的消息

2023-08-17 3.9.0
1. Android端修复P2P连接时从java发起重连，异步操作调用disconnect释放读写内存导致出现野指针异常

2023-08-24 3.10.0
1. Android端增加设置三键导航显示和隐藏的协议
2. Android端修改网关模式不能拉起APP

2023-09-19 3.11.0
1. Android端修复Android P以上机型会出现FORTIFY: pthread_mutex_lock called on a destroyed mutex
2. Android端SO增加AES_CBC_PKCS5加密

2023-10-19 3.12.0
1. Android端AES_CBC_PKCS5加密接口改为传递byte数组
2. Android端支持本地输入法和云机输入法切换

2023-11-09 3.13.0
1.webrtc支持pc客户端
2.合并android sdk的代码

2024-02-21 3.13.1
1.修复mac画面不同步
2.ARM-XQ-22878 增加screenCast时切换只获取视频时关闭音频解码和音频播放器

2024-01-19 3.14.0
1.webrtc支持ios客户端

2024-03-11 3.14.1
1.修复ios客户端加密连接失败问题
2.ARM-XQ-22878 修复screenCast切换音视频后没有声音的问题
*/

#define SW_VERSION "3.14.1"
