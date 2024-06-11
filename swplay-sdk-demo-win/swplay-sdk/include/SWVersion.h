//sw_version.h  

#ifndef SW_VERSION_H_
#define SW_VERSION_H_

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
4, 处理MSG_CONTROL_INPUT,	// 控制是否可以input(试玩)
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

*/

//2019-01-22 183000
#define SW_VERSION "3.1.25.61"

#endif /* SW_VERSION_H_ */
