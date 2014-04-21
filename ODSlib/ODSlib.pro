#-------------------------------------------------
#
# Project created by QtCreator 2014-04-17T13:42:54
#
#-------------------------------------------------

QT       += sql xml

QT       -= gui

TARGET = ODSlib
TEMPLATE = lib

DEFINES += ODSLIB_LIBRARY

SOURCES += ods.cpp

HEADERS += ods.h\
        odslib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../quazip/debug/ -lquazip
else:unix: LIBS += -L$$OUT_PWD/../quazip/ -lquazip

INCLUDEPATH += $$PWD/../quazip
DEPENDPATH += $$PWD/../quazip

RESOURCES += \
    resource.qrc
