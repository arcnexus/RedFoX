#-------------------------------------------------
#
# Project created by QtCreator 2012-06-04T22:00:11
# y por ArcNexus
#-------------------------------------------------

QT       += core gui
QT       += sql

INCLUDEPATH += /Qwt/5.2.1/include
DEPENDPATH += /Qwt/5.2.1/lib

LIBS += -LC:/Qt/Qwt/5.2.1/lib

TARGET = Terra
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    frmClientes.cpp \
    login.cpp \
    frmproveedores.cpp \
    frmarticulos.cpp \
    cliente.cpp \
    proveedor.cpp \
    frmbuscarcliente.cpp \
    frmdecision.cpp \
    frmfacturas.cpp \
    factura.cpp \
    pruebas.cpp \
    frmbuscarpoblacion.cpp \
    configuracion.cpp \
    empresa.cpp \
    conexionterra.cpp

HEADERS  += mainwindow.h \
    frmClientes.h \
    login.h \
    frmproveedores.h \
    frmarticulos.h \
    cliente.h \
    proveedor.h \
    frmbuscarcliente.h \
    frmdecision.h \
    frmfacturas.h \
    factura.h \
    pruebas.h \
    frmbuscarpoblacion.h \
    configuracion.h \
    empresa.h \
    conexionterra.h

FORMS    += mainwindow.ui \
    frmClientes.ui \
    login.ui \
    frmproveedores.ui \
    frmarticulos.ui \
    frmbuscarcliente.ui \
    frmdecision.ui \
    frmfacturas.ui \
    pruebas.ui \
    frmbuscarpoblacion.ui

RESOURCES += \
    terra.qrc
