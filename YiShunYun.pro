QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    creategroupwidget.cpp \
    forgetpwdialog.cpp \
    global.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    passwordloginpage.cpp \
    registerpage.cpp \
    smsloginpage.cpp \
    updategroupwidget.cpp

HEADERS += \
    creategroupwidget.h \
    forgetpwdialog.h \
    global.h \
    loginwindow.h \
    mainwindow.h \
    passwordloginpage.h \
    registerpage.h \
    smsloginpage.h \
    updategroupwidget.h

FORMS += \
    creategroupwidget.ui \
    forgetpwdialog.ui \
    loginwindow.ui \
    mainwindow.ui \
    passwordloginpage.ui \
    registerpage.ui \
    smsloginpage.ui \
    updategroupwidget.ui

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
