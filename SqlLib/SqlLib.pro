#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T19:16:35
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = SqlLib
TEMPLATE = lib

DEFINES += SQLLIB_LIBRARY

SOURCES += \
    sqlcalls.cpp

HEADERS +=\
        sqllib_global.h \
    sqlcalls.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
