#include "global.h"
#include "openssl/ssl.h"

int GlobalData::id = 10;
QString GlobalData::strMaxExpirationDate = "";
QString GlobalData::strToken = "";
QString GlobalData::strAccount = "";
QString GlobalData::strMobile = "";
QString GlobalData::strName = "";
QString GlobalData::strPhotoUrl = "";
//QString GlobalData::apiurl = FORMAL_SERVER_DOMAIN_NAME_URL;

QString getLevelTypeToText(LEVEL_TYPE levelType)
{
    QString strText;
    switch (levelType)
    {
    case LEVEL_NOMAL_LEVEL:
        strText = "VIP";
        break;
    case LEVEL_ENHANCEMENT_TYPE:
        strText = "GVIP";
        break;
    case LEVEL_PREMIER_TYPE:
        strText = "KVIP";
        break;
    default:
        strText = "VIP";
        break;
    }
    return strText;
}

std::string sha256(const std::string str)
{
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string newString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        newString = newString + buf;
    }
    return newString;
}
QString getMessageByCode(long code)
{
    QString strMsg;
    switch(code)
    {
    case 11100050:
        strMsg="系统异常，请联系客服人员";
        break;
    case 11101100:
        strMsg="商户信息异常";
        break;
    case 11101118:
        strMsg="商户类型错误,只允许云手机客户使用";
        break;
    case 11102027:
        strMsg="serverToken无效";
        break;
    case 11102032:
        strMsg="serverToken无效，[serverToken]";
        break;
    case 11103062:
        strMsg="实例不存在";
        break;
    case 11103078:
        strMsg="实例不属于此用户";
        break;
    case 11103103:
        strMsg="无设备可绑定";
        break;
    case 11103180:
        strMsg="实例池不存在";
        break;
    case 11103183:
        strMsg="实例池中无实例";
        break;
    case 11103184:
        strMsg="实例编号和实例池编码不能同时为空";
        break;
    case 11103185:
        strMsg="实例池不存在或无实例";
        break;
    case 11103190:
        strMsg="实例池不存在，[实例池编号]";
        break;
    case 11108003:
        strMsg="工作任务不存在";
        break;
    case 11108009:
        strMsg="找不到脚本[脚本名称],请确定是否有配置";
        break;
    case 11109002:
        strMsg="内部服务通信失败";
        break;
    default:
        strMsg="未知错误";
        break;
    }
    return strMsg;
}
