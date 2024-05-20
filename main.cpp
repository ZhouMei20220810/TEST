#include "loginwindow.h"

#include <QApplication>
#include <QJsonParseError>
#include <QJsonObject>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 假设这是您接收到的JSON字符串
    /*QString jsonString = "{\"code\":200,\"message\":\"\xE6\x88\x90\xE5\x8A\x9F\",\"data\":\"<form name=\\\"punchout_form\\\" method=\\\"post\\\" action=\\\"https://openapi-sandbox.dl.alipaydev.com/gateway.do?charset=UTF8&method=alipay.trade.page.pay&sign=K9yaNssrdwkXsb5vIz5gCQLvXzHVKmwltGvH0UPGwV%2FZw0KLgF1BTFWHQ%2FEb7%2BTTc8j2lRu0S3wFE6ZPwQzXhobYsY4iY0HFlm7qUOEO3%2BdZHni91J3u4Lq7xGLfVQye7Zz%2F1a55%2FbWn5gsBs6%2BKPQ8DUS64c8%2BD391RreZmpUOftfv3MigtxLu6G88jn7pn%2F8mpLBeAJeP3HW0nNb3%2B16MaiL9RcbFfZQXqQwCqgRo5jcxMYJSBdN%2BPn7FZ8nlm7Q3%2B4DX5SAchgHYYErvHuYq1PEmrSTChMyR4dwqpS3crM3KZSNDL%2BHwmt6VJqkkREypsPRYiS3%2BqymzDLieXyw%3D%3D&return_url=http%3A%2F%2F120.26.132.153%3A8080%2Fapi%2Fpay%2Fcallback%2Fzfb&notify_url=http%3A%2F%2F120.26.132.153%3A8080%2Fapi%2Fpay%2Fcallback%2Fzfb&version=1.0&app_id=9021000136629279&sign_type=RSA2&timestamp=2024-05-17+12%3A52%3A34&alipay_sdk=alipay-sdk-java-4.35.171.ALL&format=json\\\">\\n<input type=\\\"hidden\\\" name=\\\"biz_content\\\" value=\\\"{&quot;out_trade_no&quot;:&quot;69a8a31f7bb94d81bc142649f656c508&quot;,&quot;total_amount&quot;:&quot;19.90&quot;,&quot;subject&quot;:&quot;VIP30\xE5\xA4\xA9-\xE8\xB4\xAD\xE4\xB9\xB0\xE8\xAE\xA2\xE5\x8D\x95-20240517045234&quot;,&quot;body&quot;:&quot;VIP30\xE5\xA4\xA9-\xE8\xB4\xAD\xE4\xB9\xB0\xE8\xAE\xA2\xE5\x8D\x95-20240517045234&quot;,&quot;product_code&quot;:&quot;FAST_INSTANT_TRADE_PAY&quot;}\\\">\\n<input type=\\\"submit\\\" value=\\\"\xE7\xAB\x8B\xE5\x8D\xB3\xE6\x94\xAF\xE4\xBB\x98\\\" style=\\\"display:none\\\" >\\n</form>\\n<script>document.forms[0].submit();</script>\"}";

    // 将JSON字符串解析成QJsonDocument
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &jsonError);

    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "解析错误：" << jsonError.errorString();
        return -1;
    }

    // 转换为QJsonObject以便访问
    QJsonObject jsonObj = jsonDoc.object();

    // 提取message字段的值
    QString message = jsonObj.value("message").toString();
    QString strData = jsonObj.value("data").toString();

    // 输出message的值
    qDebug() << "Message: " << message;
    qDebug() << "data: " << strData;
    QDir dir;
    QString strFilePath = dir.tempPath() + "/" + OPEN_ZHIFUBAO_TEMP_FILE_NAME;
    QFile file(strFilePath);
    if (file.exists())
        file.remove();
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QDataStream stream(&file);
        stream << strData;
        file.flush();
        file.close();

        //PayWidget* pay = new PayWidget(strFilePath);
        //pay->show();
    }

    return a.exec();*/
    LoginWindow w;
    w.show();
    return a.exec();
}
