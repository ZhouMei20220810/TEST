QT       += core gui
QT += network
QT += webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#LIBS += -lssl -lcrypto

CONFIG += c++17

INCLUDEPATH += C:\Program Files\OpenSSL-Win64\include
LIBS += D:\zm\QtProject\TEST\MD\libssl.lib \
        D:\zm\QtProject\TEST\MD\libcrypto.lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logoutdialog.cpp \
    buyhistoryitemwidget.cpp \
    buyhistorywidget.cpp \
    creategroupwidget.cpp \
    forgetpwdialog.cpp \
    global.cpp \
    individualcenterwidget.cpp \
    levelchildwidget.cpp \
    levelitemwidget.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    messagetips.cpp \
    messagetipsdialog.cpp \
    passwordloginpage.cpp \
    phoneinstancewidget.cpp \
    phoneitemnodatawidget.cpp \
    phoneitemwidget.cpp \
    phonelistmodeitemwidget.cpp \
    policydialog.cpp \
    queuetableitem.cpp \
    registerpage.cpp \
    renewitemwidget.cpp \
    smsloginpage.cpp \
    toolobject.cpp \
    updategroupwidget.cpp \
    uploadfiledialog.cpp \
    uploadfileitemwidget.cpp \
    videoviewwidget.cpp \
    vipitemwidget.cpp

HEADERS += \
    Logoutdialog.h \
    buyhistoryitemwidget.h \
    buyhistorywidget.h \
    creategroupwidget.h \
    forgetpwdialog.h \
    global.h \
    individualcenterwidget.h \
    levelchildwidget.h \
    levelitemwidget.h \
    loginwindow.h \
    mainwindow.h \
    messagetips.h \
    messagetipsdialog.h \
    passwordloginpage.h \
    phoneinstancewidget.h \
    phoneitemnodatawidget.h \
    phoneitemwidget.h \
    phonelistmodeitemwidget.h \
    policydialog.h \
    queuetableitem.h \
    registerpage.h \
    renewitemwidget.h \
    smsloginpage.h \
    toolobject.h \
    updategroupwidget.h \
    uploadfiledialog.h \
    uploadfileitemwidget.h \
    videoviewwidget.h \
    vipitemwidget.h

FORMS += \
    Logoutdialog.ui \
    buyhistoryitemwidget.ui \
    buyhistorywidget.ui \
    creategroupwidget.ui \
    forgetpwdialog.ui \
    individualcenterwidget.ui \
    levelchildwidget.ui \
    levelitemwidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    messagetipsdialog.ui \
    passwordloginpage.ui \
    phoneinstancewidget.ui \
    phoneitemnodatawidget.ui \
    phoneitemwidget.ui \
    phonelistmodeitemwidget.ui \
    policydialog.ui \
    queuetableitem.ui \
    registerpage.ui \
    renewitemwidget.ui \
    smsloginpage.ui \
    updategroupwidget.ui \
    uploadfiledialog.ui \
    uploadfileitemwidget.ui \
    videoviewwidget.ui \
    vipitemwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = logo.ico
RC_FILE += logo.rc

DISTFILES += \
    logo.rc

MOC_DIR = $$DESTDIR

#DESTDIR = $$PWD/../bin #指定目标文件存放位置

win32: LIBS += -L$$PWD/libs/ -lqrencode

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/qrencode.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libqrencode.a

win32: LIBS += -L$$PWD/libs/ -lalibabacloud-oss-cpp-sdk

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/alibabacloud-oss-cpp-sdk.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libalibabacloud-oss-cpp-sdk.a

win32: LIBS += -L$$PWD/libs/ -llibcurl

INCLUDEPATH += $$PWD/include/curl
DEPENDPATH += $$PWD/include/curl

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/libcurl.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/liblibcurl.a

win32: LIBS += -L$$PWD/libs/ -lssleay32

INCLUDEPATH += $$PWD/include/openssl
DEPENDPATH += $$PWD/include/openssl

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/ssleay32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libssleay32.a

win32: LIBS += -L$$PWD/libs/ -llibeay32

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/libeay32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/liblibeay32.a

win32: LIBS += -L$$PWD/libs/ -lswplay_ud

INCLUDEPATH += $$PWD/include/SW
DEPENDPATH += $$PWD/include/SW

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libs/swplay_ud.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/libs/libswplay_ud.a
