QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++17

DEFINES += HAVE_CONFIG_H

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bitstream.c \
    mask.c \
    mmask.c \
    mqrspec.c \
    qrencode.c \
    qrencode_main.cpp \
    qrinput.c \
    qrspec.c \
    rsecc.c \
    split.c

HEADERS += \
    bitstream.h \
    config.h \
    mask.h \
    mmask.h \
    mqrspec.h \
    qrencode.h \
    qrencode_inner.h \
    qrencode_main.h \
    qrinput.h \
    qrspec.h \
    rsecc.h \
    split.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists.txt \
    COPYING \
    ChangeLog \
    Doxyfile \
    Makefile.am \
    Makefile.in \
    NEWS \
    README \
    TODO \
    acinclude.m4 \
    aclocal.m4 \
    autogen.sh \
    configure \
    configure.ac \
    libqrencode.pc.in \
    qrencode.1.in \
    qrencode.pro.user
