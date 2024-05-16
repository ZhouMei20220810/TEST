#ifndef GLOBAL_H
#define GLOBAL_H

#include <QToolButton>
#include <QString>
#define HTTP_SERVER_DOMAIN_ADDRESS "http://120.26.132.153:8080"
#define HTTP_YSY_PASSWORD_LOGIN     "/api/login/"           //密码登录
#define HTTP_YSY_REGISTER           "/api/login/register"   //注册
#define HTTP_YSY_LOGOUT             "/api/login/logout"     //注销
#define HTTP_YSY_SMS_LOGIN          "/api/login/sms"        //短信验证码登录
#define HTTP_YSY_GET_SMS_CODE       "/api/sms"             //获取验证码
//分组接口
#define HTTP_QUERY_ALL_GROUP        "/api/group/list"       //查询全部分组
#define HTTP_CREATE_GROUP           "/api/group/create"     //创建分组
#define HTTP_UPDATE_GROUP           "/api/group/update"     //修改分组
#define HTTP_DELETE_GROUP           "/api/group/delete"     //删除分组
#define HTTP_TOKEN_HEADER           "Bearer "               //token添加头

//订单接口
#define HTTP_CREATE_ORDER           "/api/order/create"     //创建订单
#define HTTP_CLOSE_ORDER            "/api/order/closeOrder/{outTradeNo}" //关闭订单
#define HTTP_DELETE_ORDER           "/api/order/delete"     //删除
#define HTTP_CLEAR_ORDER            "/api/order/empty"       //清空
#define HTTP_GET_MY_ORDER           "/api/order/my"          //我的支付订单

//手机实例相关接口
#define HTTP_GET_SERVER_TOKEN       "/api/user/serverToken" //获取serverToken,供测试使用
#define HTTP_GET_MYINSTANCE_LEVEL   "/api/user/getLevel"    //获取我的实例级别，可以不用获取级别
#define HTTP_GET_MY_PHONE_INSTANCE  "/api/user/myInstance"  //获取我的手机实例
#define HTTP_INSTANCE_REBOOT        "/api/user/reboot"      //实例重启
#define HTTP_INSTANCE_RESET         "/api/user/reset"       //实例重置
#define HTTP_INSTANCE_SCREENSHOT    "/api/user/screenshot"  //获取实例截图
#define HTTP_INSTANCE_SCREENSHOT_REFRESH "/api/user/screenshot/refresh"  //刷新实例截图
#define HTTP_SET_INSTANCE_GROUP     "/api/user/setGroup"    //设置实例分组
#define HTTP_UPLOAD_FILE_TO_INSTANCE "/api/user/upload"     //上传文件到实例

#define HTTP_SUCCESS_CODE           200
//phone item
#define         ITEM_PHONE_VERTICAL_WIDTH       207 //竖屏宽度
#define         ITEM_PHONE_VERTICAL_HEIGHT      396 //竖屏高度
//phone item
#define         ITEM_PHONE_HORIZONTAL_WIDTH       368 //横屏宽度
#define         ITEM_PHONE_HORIZONTAL_HEIGHT      235 //横屏高度

#define         ITEM_WIDGET_SPACING             15
//level item
#define         ITEM_WIDGET_LEVEL_WIDTH         416
#define         ITEM_WIDGET_LEVEL_HEIGHT        202
enum LEVEL_TYPE
{
    LEVEL_NOMAL_LEVEL = 0,          //标准型
    LEVEL_ENHANCEMENT_TYPE = 1,     //增强型
    LEVEL_PREMIER_TYPE = 2          //尊享型
};
QString getLevelTypeToText(LEVEL_TYPE levelType);
//vip item
#define         ITEM_WIDGET_VIP_WIDTH           100
#define         ITEM_WIDGET_VIP_HEIGHT          120
#define         ITEM_WIDGET_VIP_COUNT           5//列表总数
enum VIP_TYPE
{
    VIP_TYPE_0 = 0,
    VIP_TYPE_1 = 1,
    VIP_TYPE_2 = 2,
    VIP_TYPE_3 = 3,
    VIP_TYPE_4 = 4
};
typedef struct VIP_ITEM_INFO
{
    VIP_TYPE vipType;
    int iDayCount;
    QString strVipText;
    //QString strTotalTime;
    float fTotalPrice;
    float fDayPrice;
    VIP_ITEM_INFO()
    {
        memset(this,0,sizeof(VIP_ITEM_INFO));
    }
}S_VIP_ITEM_INFO,*PS_VIP_ITEM_INFO;


QString getMessageByCode(long code);
std::string sha256(const std::string str);


enum ENUM_LOGIN_PAGE_TYPE
{
    TYPE_PASSWORDLOGIN_PAGE = 0,
    TYPE_SMSLOGIN_PAGE = 1,
    TYPE_REGISTER_PAGE = 2
};

enum ENUM_LOGIN_STATUS
{
    TYPE_WINDOWS_HIDE = 0,
    TYPE_WINDOWS_SHOW = 1,
    TYPE_WINDOWS_CLOSE = 2
};

typedef struct USER_LOGIN_INFO
{
    int     id;
    QString strMaxExpirationDate;
    QString strToken;
    QString strAccount;
    QString strMobile;
    QString strName;
    QString strPhotoUrl;
    USER_LOGIN_INFO()
    {
        memset(this, 0, sizeof(USER_LOGIN_INFO));
    }
}S_USER_LOGIN_INFO,*P_USER_LOGIN_INFO;

typedef struct GROUP_INFO
{
    int     iGroupId;
    int     iGroupNum;
    QString strGroupName;
    GROUP_INFO()
    {
        memset(this, 0, sizeof(GROUP_INFO));
    }
}S_GROUP_INFO,*PS_GROUP_INFO;


typedef struct PHONE_INFO
{
    int     iId;
    int     iType;              //系统类型 安卓8，安卓10，
    int     iLevel;             //级别
    QString strInstanceNo;      //外部实例编号
    QString strCreateTime;      //创建时间
    QString strCurrentTime;     //当前时间
    QString strExpireTime;      //到期时间
    QString strName;            //实例名称
    QString strServerToken;     //第三方serverToken
    PHONE_INFO()
    {
        memset(this, 0, sizeof(PHONE_INFO));
    }
}S_PHONE_INFO,*PS_PHONE_INFO;

typedef struct ORDER_INFO
{
    int     iId;
    float   fActualAmount;      //实际支付金额
    int     iCreateBy;          //创建人
    QString strCreateTime;      //创建时间
    bool    bIsDelete;          //是否删除
    int     iMemberId;          //购买会员id
    QString strOrderTitle;      //订单标题
    int     iBuyNum;            //购买数量
    QString strOutTradeNo;      //订单号
    QString strPayTime;         //支付时间
    float   fPreferentialAmount; //优惠金额
    QString strRelateid;        //实例id,多个逗号分割
    QString strRemark;          //订单备注
    int     iStatus;            //支付状态0:未支付 1:已支付 2:已完成
    float   fTotalAmount;       //总金额
    QString strTradeNo;         //第三方支付唯一标识
    ORDER_INFO()
    {
        memset(this, 0, sizeof(ORDER_INFO));
    }
}S_ORDER_INFO, * PS_ORDER_INFO;

class GlobalData
{
public:
    static int userbox_x;
    static int userbox_y;
    static QString userphone;
    static QString useravator;
    static QString token;
    static int expire;
    static QString version;
    static QString apiurl;

    static void setToolButtonSize(QToolButton* btn);
};
#endif // GLOBAL_H
