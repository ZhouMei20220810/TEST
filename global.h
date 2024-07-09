#ifndef GLOBAL_H
#define GLOBAL_H

#include <QToolButton>
#include <QString>
#include <QSystemTrayIcon>
#define HTTP_SERVER_DOMAIN_ADDRESS  "http://192.168.1.6:8080" //"http://120.26.132.153:8080"       
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

//会员列表
#define HTTP_LEVEL_LIST             "/api/level/list"       
//会员相关接口
#define HTTP_MEMBER_LEVEL_LIST_DATA "/api/member/data"      //获取会员等级列表(全量)
#define HTTP_MEMBER_LIST_BY_LEVELID "/api/member/list"      //获取会员等级列表通过levelId

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
#define HTTP_INSTANCE_RESET_CALLBACK "/api/instance/callback/reset-fb" //实例重置回调
#define HTTP_INSTANCE_SCREENSHOT    "/api/user/screenshot"  //获取实例截图
#define HTTP_INSTANCE_SCREENSHOT_REFRESH "/api/user/screenshot/refresh"  //刷新实例截图
#define HTTP_SET_INSTANCE_GROUP     "/api/user/setGroup"    //设置实例分组
#define HTTP_SET_INSTANCE_NAME      "/api/user/setName"     // 实例名称修改
#define HTTP_UPLOAD_FILE_TO_INSTANCE "/api/user/upload"     //上传文件到实例
#define HTTP_GET_INSTANCE_SESSION    "/api/user/getSessionId" //获取连接session

//文件相关接口,停用
#define HTTP_POST_SERVER_OSS_INFO   "/api/file/getSts"      //获取文件上传sts

//云手机
#define HTTP_CLOUD_PHONE_SERVER         "https://platform.armvm.com"
#define HTTP_CLOUD_PHONE_GET_INSTANCE   "/auth/device/connect.html?s=da67906d06e8d334d1c8308fbba8af67&appkey=4a8311be034943cfaa6a357fdc9f0461&auth_ver=2&nonce=1576737589993" //实例申请



#define     SCREENSHOT_PICTRUE_FLODER "YiShunYun"
#define     INSTANCE_TEMP_DIR         "/Instance/"
#define     DOWNLOAD_SCREENSHOT_INTERVAL (3*1000)       //下载截图间隔
#define     TIMER_INTERVAL          (6*1000)               //正式时间
#define     ORGANIZATION_NAME       "YSY"
#define     APPLICATION_NAME        "YSY STUDIO"
#define OPEN_ZHIFUBAO_TEMP_FILE_NAME "ysy.html"

#define HTTP_SUCCESS_CODE           200
//phone item
#define         ITEM_PHONE_VERTICAL_WIDTH       207 //竖屏宽度
#define         ITEM_PHONE_VERTICAL_HEIGHT      396 //竖屏高度
//phone item
#define         ITEM_PHONE_HORIZONTAL_WIDTH       368 //横屏宽度
#define         ITEM_PHONE_HORIZONTAL_HEIGHT      235 //横屏高度
//instance item
#define         PHONE_INSTANCE_VERTICAL_WIDTH       520 //竖屏宽度
#define         PHONE_INSTANCE_VERTICAL_HEIGHT      840 //竖屏高度
//instance item
#define         PHONE_INSTANCE_HORIZONTAL_WIDTH       840 //横屏宽度
#define         PHONE_INSTANCE_HORIZONTAL_HEIGHT      520 //横屏高度
#define         ITEM_WIDGET_SPACING             15
//level item
#define         ITEM_WIDGET_LEVEL_WIDTH         400
#define         ITEM_WIDGET_LEVEL_HEIGHT        160
#define         ITEM_LEVEL_LABELCHECK_WIDTH     36
#define         ITEM_LEVEL_LABELCHECK_HEIGHT    36
//queue item
#define          QUEUE_ITEM_WIDTH           100
#define          QUEUE_ITEM_HEIGHT          30
//renew item
#define          RENEW_ITEM_WIDTH           100
#define          RENEW_ITEM_HEIGHT          20
//listmode item
#define          LISTMODE_ITEM_HEGITH       20
#define          LISTMODE_ITEM_WIDTH        100
#define          LIST_WIDGET_LISTMODE_ITEM_SPACING          5

#define         PHONE_INSTANCE_TOOL_COUNT                    (12)
#define         PHONE_INSTANCE_VER_DEFAULT_TOOL_NUM          (7)
#define         PHONE_INSTANCE_HOR_DEFAULT_TOOL_NUM          (5)

#define         ICON_WIDTH          40
#define         ICON_HEIGHT         46

typedef struct LEVEL_INFO
{
    int     iLevelId;
    QString strLevelName;       //等级名称
    QString strLevelRemark;     //等级描述
    QString strColorIcon;       //彩图
    QString strAshIcon;         //灰图
    bool    bIsEnabled;         //是否启动
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

//vip item
#define         ITEM_WIDGET_VIP_WIDTH           100
#define         ITEM_WIDGET_VIP_HEIGHT          120
#define         ITEM_WIDGET_VIP_COUNT           5//列表总数

QString getMessageByCode(long code);
std::string sha256(const std::string str);
int calculateWidth(int fixedHeight);
void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
enum ENUM_ACTIVE_CODE_STATUS
{
    TYPE_INIT = -1,
    TYPE_SUCCESS = 0,
    TYPE_FAILED = 1
};
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

enum ENUM_PICTURE_QUALITY
{
    TYPE_QUALITY_AUTO = 0,//（0：自动，1：高清，2：标清，3：流畅）
    TYPE_QUALITY_TOPSPEED=3,
    TYPE_QUALITY_NORMAL=2,
    TYPE_QUALITY_HIGH_DEFINITION=1
};

//实例窗口按钮类型
enum ENUM_TOOLBUTTON_TYPE
{
    TYPE_VOLUMN_UP = 1,
    TYPE_VOLUMN_DOWN,
    TYPE_HOR_VER_SCREEN,
    TYPE_CLIPBOARD,
    TYPE_SCREENSHOTS,
    TYPE_SCREENSHOTS_DIR,
    TYPE_RESTART,
    TYPE_RESET_FACTORY_DATA,
    TYPE_ROOT,
    TYPE_SHARK,
    TYPE_GPS,
    TYPE_CHANGE_KEYBOARD
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
    bool    bChecked;
    bool    bUsed;              //是否使用
    int     iAuthStatus;        //是否授权
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

typedef struct Task_INFO
{
    float fTaskId;
    float fTaskStatus;
    QString strUrl;
    QString strPadCode;
    Task_INFO()
    {
        memset(this, 0, sizeof(Task_INFO));
    }
}S_TASK_INFO,*PS_TASK_INFO;

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

typedef struct PAD_INFO
{
    int         iPhoneId;
    QString     strInstanceNo;
    QString     strControlAddr;
    float       fControlPort;
    int         iUserID;
    QString     strSessionID;
    int         iHWaccel;
    PAD_INFO()
    {
        memset(this, 0, sizeof(PAD_INFO));
    }
}S_PAD_INFO,*PS_PAD_INFO;
typedef struct ACTIVE_CODE_INFO
{
    int         iRelateId;//PhoneId
    QString     strRenewActiveCode;//续费激活码
    ACTIVE_CODE_INFO()
    {
        memset(this, 0, sizeof(ACTIVE_CODE_INFO));
    }
}S_ACTIVE_CODE_INFO,*PS_ACTIVE_CODE_INFO;

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
    static QString getFileMd5(const QString& fileName);
    static QString getContentType(const QString& filePath);
    static QString QStringToBase64(QString str);
    static QString Base64ToQString(QString base64Str);

    static int      iPhoneItemWidth;
    static int      iPhoneItemHeight;

    static bool     bVerticalScreen;

    static ENUM_PICTURE_QUALITY enPictrueQuality;
    static bool     bVerticalPhoneInstance;
    static bool     bVerticalPhoneInstanceCenter;
    static QPoint   pointPhoneInstance;
    static bool     bCloseMainWindowExit;
    static bool     bShowSystemTrayIcon;
    static QString  strToolButtonList;
    static bool     bBootstrapAutoStart;
    static bool     bIsTopWindow;
    static bool     bIsPageMode;

    static bool     bIsSyncOperation; //    同步操作
    static QMap<int, S_PHONE_INFO> mapSyncPhoneList;
    //static void setToolButtonSize(QToolButton* btn);

    static QString strSecurityToken;
    static QString strAccessKeySecret;
    static QString strAccessKeyId;
    static QString strExpiration;

    static QString strPictureTempDir;
    static QString strFileTempDir;
    static QString strPhoneInstanceScreenshotDir;

    static std::string Endpoint;
    static std::string BucketName;

    static bool bHideIndividualCenterWidget;
};
#endif // GLOBAL_H
