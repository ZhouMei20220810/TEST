#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#define HTTP_SERVER_DOMAIN_ADDRESS "http://120.26.132.153:8080"
#define HTTP_YSY_PASSWORD_LOGIN     "/api/login"           //密码登录
#define HTTP_YSY_REGISTER           "/api/login/register"   //注册
#define HTTP_YSY_LOGOUT             "/api/login/logout"     //注销
#define HTTP_YSY_SMS_LOGIN          "/api/login/sms"        //短信验证码登录
#define HTTP_YSY_GET_SMS_CODE       "/api/sms"             //获取验证码

#define HTTP_QUERY_ALL_GROUP        "/api/group/list"       //查询全部分组
#define HTTP_CREATE_GROUP           "/api/group/create"     //创建分组
#define HTTP_UPDATE_GROUP           "/api/group/update"     //修改分组
#define HTTP_DELETE_GROUP           "/api/group/delete/{id}" //删除分组

#define HTTP_SUCCESS_CODE           200

QString getMessageByCode(long code);


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
#endif // GLOBAL_H
