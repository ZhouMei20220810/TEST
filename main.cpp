#include "loginwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //创建目录
    QString strDir = QDir::tempPath() + "/"+ SCREENSHOT_PICTRUE_FLODER;
    QDir dir(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "创建目录失败:" << strDir;
    }
    LoginWindow w;
    w.show();
    return a.exec();
}
