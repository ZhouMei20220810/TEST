#include "loginwindow.h"

#include <QApplication>
#include <QDir>
#include "SWRuntime.h"
#include "common/log.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    QString strDir = GlobalData::strFileTempDir;
    QDir dir(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }
    strDir = GlobalData::strPictureTempDir;
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "failed:" << strDir;
    }

    SWRuntime::getInstance()->init(ANDROID_LOG_DEFAULT, strDir.toStdString().c_str());
    LoginWindow w;
    w.show();

    int iResult = a.exec();
    SWRuntime::getInstance()->deinit();
    return iResult;
}
