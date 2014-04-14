CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(blinkinkplugin)
TEMPLATE    = lib

HEADERS     = blinkinkplugin.h
SOURCES     = blinkinkplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

DEFINES += SHAREDLIB_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(blinkink.pri)
