#ifndef GLOBAL_H
#define GLOBAL_H

#include <QToolButton>
#include <QString>
#define HTTP_SERVER_DOMAIN_ADDRESS "http://120.26.132.153:8080"
#define HTTP_YSY_PASSWORD_LOGIN     "/api/login/"           //密码登录
#define HTTP_YSY_REGISTER           "/api/login/register"   //注册
#define HTTP_YSY_LOGOUT             "/api/login/logout"     //注销
#define HTTP_YSY_SMS_LOGIN          "/api/login/sms"        //短信验证码登录
#define HTTP_YSY_RESET_PASSWORD     "/api/user/setPassword" //重置密码
#define HTTP_YSY_GET_SMS_CODE       "/api/sms"              //获取验证码
//分组接口
#define HTTP_QUERY_ALL_GROUP        "/api/group/list"       //查询全部分组
#define HTTP_CREATE_GROUP           "/api/group/create"     //创建分组
#define HTTP_UPDATE_GROUP           "/api/group/update"     //修改分组
#define HTTP_DELETE_GROUP           "/api/group/delete"     //删除分组
#define HTTP_TOKEN_HEADER           "Bearer "               //token添加头

//会员相关接口
#define HTTP_MEMBER_LEVEL_LIST_DATA "/api/member/data"      //获取会员等级列表

//订单接口
#define HTTP_CREATE_ORDER           "/api/order/create"     //创建订单
#define HTTP_CLOSE_ORDER            "/api/order/closeOrder/{outTradeNo}" //关闭订单
#define HTTP_DELETE_ORDER           "/api/order/delete"     //删除
#define HTTP_CLEAR_ORDER            "/api/order/empty"       //清空
#define HTTP_GET_MY_ORDER           "/api/order/my"          //我的支付订单

#define HTTP_POST_ACTIVE_CODE       "/api/cdkey/activate"   //激活

//手机实例相关接口
#define HTTP_GET_SERVER_TOKEN       "/api/user/serverToken" //获取serverToken,供测试使用
#define HTTP_GET_MYINSTANCE_LEVEL   "/api/user/getLevel"    //获取我的实例级别，可以不用获取级别
#define HTTP_GET_MY_PHONE_INSTANCE  "/api/user/myInstance"  //获取我的手机实例
#define HTTP_INSTANCE_REBOOT        "/api/user/reboot"      //实例重启
#define HTTP_INSTANCE_RESET         "/api/user/reset"       //实例重置
#define HTTP_INSTANCE_SCREENSHOT    "/api/user/screenshot"  //获取实例截图
#define HTTP_INSTANCE_SCREENSHOT_REFRESH "/api/user/screenshot/refresh"  //刷新实例截图
#define HTTP_SET_INSTANCE_GROUP     "/api/user/setGroup"    //设置实例分组
#define HTTP_SET_INSTANCE_NAME      "/api/user/setName"     // 实例名称修改
#define HTTP_UPLOAD_FILE_TO_INSTANCE "/api/user/upload"     //上传文件到实例


#define OPEN_ZHIFUBAO_TEMP_FILE_NAME "ysy.html"

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
#define         ITEM_LEVEL_LABELCHECK_WIDTH     36
#define         ITEM_LEVEL_LABELCHECK_HEIGHT    36
enum LEVEL_TYPE
{
    LEVEL_NOMAL_LEVEL = 1,          //标准型
    LEVEL_ENHANCEMENT_TYPE = 2,     //增强型
    LEVEL_PREMIER_TYPE = 3          //尊享型
};

typedef struct LEVEL_INFO
{
    LEVEL_TYPE enType;
    QString strLevelName;       //等级名称
    QString strLevelRemark;     //等级描述
    QString strFunction;        //功能描述
    bool    bCloudManage;       //云端管理
    bool    bAndroidSystem;     //安卓系统
    bool    bCloudPhoneLicensing;//云手机授权
    LEVEL_INFO()
    {
        memset(this, 0, sizeof(LEVEL_INFO));
    }
}S_LEVEL_INFO,*PS_LEVEL_INFO;
//level data
typedef struct LEVEL_DATA_INFO
{
    int     iLevelId;           //等级id
    int     iMemberId;          //会员id
    int     iUseDay;            //使用天数
    int     fPrice;             //原价
    float   fActivityPrice;     //活动价格
    QString strUrl;             //会员url
    QString strRemark;          //备注
    QString strInstanceLevel;   //实例等级
    QString strLevelName;       //等级名称
    QString strColorIcon;       //彩色图标
    QString strAshIcon;         //灰色图标
    QString strLevelRemark;
    QString strMemberName;      //会员名称
    LEVEL_DATA_INFO()
    {
        memset(this,0,sizeof(LEVEL_DATA_INFO));
    }
}S_LEVEL_DATA_INFO,*PS_LEVEL_DATA_INFO;

QString getLevelTypeToText(LEVEL_TYPE levelType);
//vip item
#define         ITEM_WIDGET_VIP_WIDTH           100
#define         ITEM_WIDGET_VIP_HEIGHT          120
#define         ITEM_WIDGET_VIP_COUNT           5//列表总数

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

enum ENUM_CREATE_OR_UPDATA
{
    TYPE_CREATE_GROUP_WIDGET = 0,          // 创建分组
    TYPE_UPDATE_GROUP_WIDGET = 1,          // 编辑分组名称
    TYPE_PHONE_RENAME_WIDGET = 2           // 实例重命名
};

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
    int     iPayType;           //支付方式 1.支付宝 2.微信 3.激活码
    int     iOrderType;         //订单类型 1.购买 2.续费
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
    static int     id;
    static QString strMaxExpirationDate;
    static QString strToken;
    static QString strAccount;
    static QString strMobile;
    static QString strName;
    static QString strPhotoUrl;
    static QString strQrcode;
    static QString md5(const QString &str);
    //static void setToolButtonSize(QToolButton* btn);
};
#endif // GLOBAL_H
