QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#LIBS += -lssl -lcrypto

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Logoutdialog.cpp \
    creategroupwidget.cpp \
    forgetpwdialog.cpp \
    global.cpp \
    levelitemwidget.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    passwordloginpage.cpp \
    registerpage.cpp \
    smsloginpage.cpp \
    updategroupwidget.cpp \
    vipitemwidget.cpp

HEADERS += \
    Logoutdialog.h \
    creategroupwidget.h \
    forgetpwdialog.h \
    global.h \
    levelitemwidget.h \
    loginwindow.h \
    mainwindow.h \
    passwordloginpage.h \
    registerpage.h \
    smsloginpage.h \
    updategroupwidget.h \
    vipitemwidget.h

FORMS += \
    Logoutdialog.ui \
    creategroupwidget.ui \
    forgetpwdialog.ui \
    levelitemwidget.ui \
    loginwindow.ui \
    mainwindow.ui \
    passwordloginpage.ui \
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
