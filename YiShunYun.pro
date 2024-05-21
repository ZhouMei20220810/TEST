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
    levelchildgvipwidget.cpp \
    levelchildkvipwidget.cpp \
    levelchildwidget.cpp \
    levelenhancementwidget.cpp \
    levelitemwidget.cpp \
    levelnormalwidget.cpp \
    levelpremierwidget.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    messagetipsdialog.cpp \
    passwordloginpage.cpp \
    phoneitemwidget.cpp \
    policydialog.cpp \
    registerpage.cpp \
    smsloginpage.cpp \
    updategroupwidget.cpp \
    vipitemwidget.cpp

HEADERS += \
    Logoutdialog.h \
    buyhistoryitemwidget.h \
    buyhistorywidget.h \
    creategroupwidget.h \
    forgetpwdialog.h \
    global.h \
    individualcenterwidget.h \
    levelchildgvipwidget.h \
    levelchildkvipwidget.h \
    levelchildwidget.h \
    levelenhancementwidget.h \
    levelitemwidget.h \
    levelnormalwidget.h \
    levelpremierwidget.h \
    loginwindow.h \
    mainwindow.h \
    messagetipsdialog.h \
    passwordloginpage.h \
    phoneitemwidget.h \
    policydialog.h \
    registerpage.h \
    smsloginpage.h \
    updategroupwidget.h \
    vipitemwidget.h

FORMS += \
    Logoutdialog.ui \
    buyhistoryitemwidget.ui \
    buyhistorywidget.ui \
    creategroupwidget.ui \
    forgetpwdialog.ui \
    individualcenterwidget.ui \
    levelchildgvipwidget.ui \
    levelchildkvipwidget.ui \
    levelchildwidget.ui \
    levelenhancementwidget.ui \
    levelitemwidget.ui \
    levelnormalwidget.ui \
    levelpremierwidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    messagetipsdialog.ui \
    passwordloginpage.ui \
    phoneitemwidget.ui \
    policydialog.ui \
    registerpage.ui \
    smsloginpage.ui \
    updategroupwidget.ui \
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
