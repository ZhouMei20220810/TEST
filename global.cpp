#include "global.h"
#include "openssl/ssl.h"
#include <QCryptographicHash>
#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFileInfo>
#include <QDir>

int GlobalData::id = 10;
QString GlobalData::strMaxExpirationDate = "";
QString GlobalData::strToken = "";
QString GlobalData::strAccount = "";
QString GlobalData::strMobile = "";
QString GlobalData::strName = "";
QString GlobalData::strPhotoUrl = "";
QString GlobalData::strQrcode = "";

QString GlobalData::strSecurityToken = "";
QString GlobalData::strAccessKeySecret = "";
QString GlobalData::strAccessKeyId = "";
QString GlobalData::strExpiration = "";

int GlobalData::iPhoneItemWidth = ITEM_PHONE_VERTICAL_WIDTH;
int GlobalData::iPhoneItemHeight = ITEM_PHONE_VERTICAL_HEIGHT;

bool GlobalData::bVerticalScreen = true;

ENUM_PICTURE_QUALITY GlobalData::enPictrueQuality = TYPE_QUALITY_HIGH_DEFINITION;
bool GlobalData::bVerticalPhoneInstance = true;
bool GlobalData::bVerticalPhoneInstanceCenter = true;
QPoint GlobalData::pointPhoneInstance = QPoint(0, 0);
bool GlobalData::bCloseMainWindowExit = true;
bool GlobalData::bShowSystemTrayIcon = true;
QString GlobalData::strToolButtonList = "";
bool GlobalData::bBootstrapAutoStart = false;
bool GlobalData::bIsTopWindow = false;

bool GlobalData::bIsSyncOperation = false;
QMap<int, S_PHONE_INFO> GlobalData::mapSyncPhoneList;

QString GlobalData::strFileTempDir = QDir::tempPath() + "/" + SCREENSHOT_PICTRUE_FLODER+"/";
QString GlobalData::strPictureTempDir = QDir::tempPath() + "/" + SCREENSHOT_PICTRUE_FLODER + INSTANCE_TEMP_DIR;

std::string GlobalData::Endpoint = "https://oss-cn-beijing.aliyuncs.com"; //去掉https,否则https会报认证问题
std::string GlobalData::BucketName = "yishunyun-file";
//QString GlobalData::apiurl = FORMAL_SERVER_DOMAIN_NAME_URL;

bool GlobalData::bHideIndividualCenterWidget = false;

void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString logText;
    switch (type) {
    case QtDebugMsg:
        logText = QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(context.line).arg(context.function);
        break;
        // 类似的处理其他类型...
    case QtCriticalMsg:
    case QtFatalMsg:
    case QtWarningMsg:
    default:
        // 处理其他日志类型
        break;
    }

    // 这里可以写入文件、发送网络请求、显示在界面上等
    QDate date = QDate::currentDate();
    QString strDate = QString("/YSY_Log_%1.txt").arg(date.toString("yyyy-MM-dd"));
    QFile file(QDir::tempPath()+strDate);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream out(&file);
        out << logText;
        file.close();
    }
}

QString GlobalData::md5(const QString &str) {
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str.toLocal8Bit());
    return hash.result().toHex();
}

QString GlobalData::getFileMd5(const QString& fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return "";

    QCryptographicHash hash(QCryptographicHash::Md5);
    while (!file.atEnd()) {
        hash.addData(file.read(1024 * 1024)); // 每次读取1MB
    }
    file.close();

    return hash.result().toHex(); // 转换为16进制字符串
}

QString GlobalData::getContentType(const QString& filePath)
{
    QMimeDatabase db;
    QMimeType mimeType = db.mimeTypeForFile(QFileInfo(filePath).canonicalFilePath());
    return mimeType.name();
}

QString GlobalData::QStringToBase64(QString str)
{
    QByteArray byteA;
    byteA = str.toUtf8();
    byteA = byteA.toBase64();
    char* cbyteA = byteA.data();

    return QString(cbyteA);
}

QString GlobalData::Base64ToQString(QString base64Str)
{
    QByteArray byteA;
    std::string stdStr = base64Str.toStdString();
    byteA = QByteArray(stdStr.c_str());
    byteA = byteA.fromBase64(byteA);

    return  QString::fromUtf8(byteA);
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
int calculateWidth(int fixedHeight)
{
    const double aspectRatio = 9.0 / 16.0;
    int calcWidth = static_cast<int>(fixedHeight * aspectRatio);
    return calcWidth;
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
