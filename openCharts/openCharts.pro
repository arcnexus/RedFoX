CONFIG      += plugin #staticlib #debug_and_release
TARGET      = $$qtLibraryTarget(openchartplugin)
TEMPLATE    = lib

HEADERS     = openchartplugin.h
SOURCES     = openchartplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(openchart.pri)
