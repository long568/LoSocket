#-------------------------------------------------
#
# Project created by QtCreator 2017-12-14T17:47:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoSocket
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        losocket.cpp \
    ls_page.cpp \
    ls_pageudp.cpp \
    ls_pagetcpclient.cpp \
    lotools.cpp \
    ls_pagetcpsrvitem.cpp \
    ls_pagetcpsrv.cpp \
    about.cpp

HEADERS += \
        losocket.h \
    myconfig.h \
    ls_page.h \
    ls_pageudp.h \
    ls_pagetcpclient.h \
    lotools.h \
    ls_pagetcpsrvitem.h \
    ls_pagetcpsrv.h \
    about.h

FORMS += \
        losocket.ui \
    ls_pageudp.ui \
    ls_pagetcpclient.ui \
    ls_pagetcpsrv.ui \
    about.ui

ICON = LoSocket.icns

TRANSLATIONS += Languages/zhCN.ts Languages/enUS.ts

RESOURCES += \
    languages.qrc
