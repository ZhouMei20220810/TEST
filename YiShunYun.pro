QT       += core gui
QT += network
QT += webenginewidgets
QT += positioning
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#LIBS += -lssl -lcrypto

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logoutdialog.cpp \
    activecodeitem.cpp \
    addactivecodedialog.cpp \
    buyhistoryitemwidget.cpp \
    buyhistorywidget.cpp \
    creategroupwidget.cpp \
    factorydataresetdialog.cpp \
    filedownloader.cpp \
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
    phoneitemwidget.cpp \
    phonelistmodeitemwidget.cpp \
    policydialog.cpp \
    qmouseframe.cpp \
    qmouselistwidget.cpp \
    qmousetreewidget.cpp \
    qmousewidget.cpp \
    qmovewidget.cpp \
    queuetableitem.cpp \
    registerpage.cpp \
    renewitemwidget.cpp \
    smsloginpage.cpp \
    systemsettingwidget.cpp \
    tdragdroplistwidget.cpp \
    toolobject.cpp \
    updategroupwidget.cpp \
    uploadfiledialog.cpp \
    uploadfileitemwidget.cpp \
    videoviewwidget.cpp \
    vipitemwidget.cpp

HEADERS += \
    Logoutdialog.h \
    activecodeitem.h \
    addactivecodedialog.h \
    buyhistoryitemwidget.h \
    buyhistorywidget.h \
    creategroupwidget.h \
    factorydataresetdialog.h \
    filedownloader.h \
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
    phoneitemwidget.h \
    phonelistmodeitemwidget.h \
    policydialog.h \
    qmouseframe.h \
    qmouselistwidget.h \
    qmousetreewidget.h \
    qmousewidget.h \
    qmovewidget.h \
    queuetableitem.h \
    registerpage.h \
    renewitemwidget.h \
    smsloginpage.h \
    systemsettingwidget.h \
    tdragdroplistwidget.h \
    toolobject.h \
    updategroupwidget.h \
    uploadfiledialog.h \
    uploadfileitemwidget.h \
    videoviewwidget.h \
    vipitemwidget.h

FORMS += \
    Logoutdialog.ui \
    activecodeitem.ui \
    addactivecodedialog.ui \
    buyhistoryitemwidget.ui \
    buyhistorywidget.ui \
    creategroupwidget.ui \
    factorydataresetdialog.ui \
    forgetpwdialog.ui \
    individualcenterwidget.ui \
    levelchildwidget.ui \
    levelitemwidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    messagetipsdialog.ui \
    passwordloginpage.ui \
    phoneinstancewidget.ui \
    phoneitemwidget.ui \
    phonelistmodeitemwidget.ui \
    policydialog.ui \
    queuetableitem.ui \
    registerpage.ui \
    renewitemwidget.ui \
    smsloginpage.ui \
    systemsettingwidget.ui \
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lswplay_u
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lswplay_ud

INCLUDEPATH += $$PWD/include/SW
DEPENDPATH += $$PWD/include/SW

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libswplay_u.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libswplay_ud.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/swplay_u.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/swplay_ud.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/ -lalibabacloud-oss-cpp-sdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/ -lalibabacloud-oss-cpp-sdkd

INCLUDEPATH += $$PWD/include/alibabacloud
DEPENDPATH += $$PWD/include/alibabacloud

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/libalibabacloud-oss-cpp-sdk.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/libalibabacloud-oss-cpp-sdkd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/alibabacloud-oss-cpp-sdk.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/alibabacloud-oss-cpp-sdkd.lib
