#include "loginwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //����Ŀ¼
    QString strDir = QDir::tempPath() + "/"+ SCREENSHOT_PICTRUE_FLODER;
    QDir dir(strDir);
    if (!dir.exists(strDir))
    {
        if (!dir.mkdir(strDir))
            qDebug() << "����Ŀ¼ʧ��:" << strDir;
    }
    LoginWindow w;
    w.show();
    return a.exec();
}
