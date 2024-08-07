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
    activecodehistorydialog.cpp \
    activecodehistoryitem.cpp \
    activecodeitem.cpp \
    activecoderenewitem.cpp \
    addactivecodedialog.cpp \
    addauthorizationdialog.cpp \
    authorizationitem.cpp \
    authorizationmanagedialog.cpp \
    buyhistoryitemwidget.cpp \
    buyhistorywidget.cpp \
    creategroupwidget.cpp \
    customtoolbutton.cpp \
    factorydataresetdialog.cpp \
    filedownloader.cpp \
    forgetpwdialog.cpp \
    global.cpp \
    individualcenterwidget.cpp \
    levelitemwidget.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    messagecenterdialog.cpp \
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
    replacecloudphonedialog.cpp \
    smsloginpage.cpp \
    systemsettingwidget.cpp \
    tcustomdragdroplistwidget.cpp \
    toolobject.cpp \
    transferphonedialog.cpp \
    transferphonehistorydialog.cpp \
    transfertipsdialog.cpp \
    updategroupwidget.cpp \
    updatesoftwaredialog.cpp \
    uploadfiledialog.cpp \
    uploadfilehistoryitem.cpp \
    uploadfileitemwidget.cpp \
    videoviewwidget.cpp \
    vipitemwidget.cpp

HEADERS += \
    Logoutdialog.h \
    activecodehistorydialog.h \
    activecodehistoryitem.h \
    activecodeitem.h \
    activecoderenewitem.h \
    addactivecodedialog.h \
    addauthorizationdialog.h \
    authorizationitem.h \
    authorizationmanagedialog.h \
    buyhistoryitemwidget.h \
    buyhistorywidget.h \
    creategroupwidget.h \
    customtoolbutton.h \
    factorydataresetdialog.h \
    filedownloader.h \
    forgetpwdialog.h \
    global.h \
    individualcenterwidget.h \
    levelitemwidget.h \
    loginwindow.h \
    mainwindow.h \
    messagecenterdialog.h \
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
    replacecloudphonedialog.h \
    smsloginpage.h \
    systemsettingwidget.h \
    tcustomdragdroplistwidget.h \
    toolobject.h \
    transferphonedialog.h \
    transferphonehistorydialog.h \
    transfertipsdialog.h \
    updategroupwidget.h \
    updatesoftwaredialog.h \
    uploadfiledialog.h \
    uploadfilehistoryitem.h \
    uploadfileitemwidget.h \
    videoviewwidget.h \
    vipitemwidget.h

FORMS += \
    Logoutdialog.ui \
    activecodehistorydialog.ui \
    activecodehistoryitem.ui \
    activecodeitem.ui \
    activecoderenewitem.ui \
    addactivecodedialog.ui \
    addauthorizationdialog.ui \
    authorizationitem.ui \
    authorizationmanagedialog.ui \
    buyhistoryitemwidget.ui \
    buyhistorywidget.ui \
    creategroupwidget.ui \
    factorydataresetdialog.ui \
    forgetpwdialog.ui \
    individualcenterwidget.ui \
    levelitemwidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    messagecenterdialog.ui \
    messagetipsdialog.ui \
    passwordloginpage.ui \
    phoneinstancewidget.ui \
    phoneitemwidget.ui \
    phonelistmodeitemwidget.ui \
    policydialog.ui \
    queuetableitem.ui \
    registerpage.ui \
    renewitemwidget.ui \
    replacecloudphonedialog.ui \
    smsloginpage.ui \
    systemsettingwidget.ui \
    transferphonedialog.ui \
    transferphonehistorydialog.ui \
    transfertipsdialog.ui \
    updategroupwidget.ui \
    updatesoftwaredialog.ui \
    uploadfiledialog.ui \
    uploadfilehistoryitem.ui \
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
