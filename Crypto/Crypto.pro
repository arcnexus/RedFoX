#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T18:11:44
#
#-------------------------------------------------

QT       -= gui

TARGET = Crypto
TEMPLATE = lib

DEFINES += CRYPTO_LIBRARY

SOURCES += crypto.cpp

HEADERS += crypto.h\
        crypto_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -lcryptopp
