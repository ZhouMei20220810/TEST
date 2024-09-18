#ifndef TOOLOBJECT_H
#define TOOLOBJECT_H

#include <QObject>
#include "global.h"
class ToolObject : public QObject
{
    Q_OBJECT
public:
    explicit ToolObject(QObject *parent = nullptr);
    static ToolObject* getInstance();
    //手机实例相关接口
    //获取实例截图
    void HttpPostInstanceScreenshot(QStringList strList);
    //刷新实例截图
    void HttpPostInstanceScreenshotRefresh(QStringList strList);
    //实例重启
    void HttpPostInstanceReboot(QStringList strList);
    //实例重置
    void HttpPostInstanceReset(QStringList strList);

    //取消授权
    void HttpPostCancelAuth(int iPhoneId);
    //授权详情
    void HttpPostAuthDetail(int iPhoneId);
    //版本检测
    void HttpPostCheckAppVersion();
    //获取通知列表
    void HttpGetNoticeListInfo(NOTICE_TYPE enType, int iPage, int iPageSize);
    //获取我的实例级别
    void HttpGetMyInstanceLevel(int iPhoneId);

    //实例重命名
    void HttpPostInstanceRename(int iId, QString strName);
    //删除分组
    void HttpDeleteGroup(int iGroupId);
    //注销登录
    void HttpLogout();
    //设置实例分组
    void HttpPostInstanceSetGroup(int iGroupId, QStringList strList);
    //创建分组
    void HttpCreateGroup(QString strGroupName); 
    //修改分组
    void HttpUpdateGroup(int iGroupId, QString strNewName);
    //激活码接口
    void HttpPostActivateCode(QMap<int, S_ACTIVE_CODE_INFO> mapActiveCode);
    //创建订单
    void HttpCreateOrder(int iChannel, int iMemberId, int iNum, int iPayType, QString strRelateId);
    //关闭订单
    void HttpCloseOrder(QString strOutTradeNo);
    //会员级别接口
    void HttpLevelList();
    //会员相关接口
    void HttpMemberLevelListData();
    //查询全部分组
    void HttpQueryAllGroup();  
    //获取我的手机实例
    void HttpGetMyPhoneInstance(int iGroupId, int iPage, int iPageSize, int iLevel = 0);
signals:
    void startTimerShowScreenshotSignals();
    void getScreenshortSignals(QMap<QString, S_TASK_INFO> mapScreenshotTask);
    void ShowAuthDetailSignals(S_AUTHOR_INFO authInfo);
    void closeAuthDialogOrGroupRefreshSignals();
    void noticeListInfoSignals(NOTICE_TYPE enType, QMap<int, S_NOTICE_INFO> mapNotice);

    //实例重命名
    void HttpPostInstanceRenameSignals(int iId,QString strName);
    //删除分组
    void HttpGroupRefreshSignals();
    //注销登录
    void HttpLogoutSignals();
    //激活码
    void activeCodeStatusSignals(QMap<QString, bool> mapActiveCodeStatus);
    //创建订单信号
    void HttpCreateOrderSignals(QString strQrCode);
    //会员级别响应
    void HttpLevelListSignals(QMap<int, S_LEVEL_INFO> mapLevelList);
    //会员相关接口响应
    void HttpMemberLevelListDataSignals(QMap<int, QMap<int, S_LEVEL_DATA_INFO>> mapLevel);
    //查询全部分组
    void HttpQueryAllGroupSignals(QMap<int, S_GROUP_INFO> mapGroupInfo);
    //手机实例响应
    void HttpGetMyPhoneInstanceSignals(int iLevel, int iGroupId, QMap<int, S_PHONE_INFO> map);
};

#endif // TOOLOBJECT_H
