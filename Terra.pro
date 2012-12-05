#-------------------------------------------------
#
# Project created by QtCreator 2012-06-04T22:00:11
# y por ArcNexus
#-------------------------------------------------

QT       += core \
gui
QT       += sql
QT       += network
QT       += xml
QT       += webkit


INCLUDEPATH += /Qwt/5.2.1/include
INCLUDEPATH += /usr/local/Qxt/include/QxtCore
DEPENDPATH += /Qwt/5.2.1/lib



LIBS += -LC:/Qt/Qwt/5.2.1/lib
LIBS += -L/usr/local/Qxt/lib
TARGET = Terra
TEMPLATE = app

 CONFIG  += qxt
 QXT     += core gui


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
    frmbuscarpoblacion.cpp \
    configuracion.cpp \
    empresa.cpp \
    articulo.cpp \
    frmBuscarFactura.cpp \
    frmmodificarlin_fac.cpp \
    columnamoneda.cpp \
    columnafecha.cpp \
    frmempresas.cpp \
    frmconfiguracion.cpp \
    albaran.cpp \
    frmalbaran.cpp \
    frmmodificarlin_alb.cpp \
    frmpedidos.cpp \
    pedidos.cpp \
    frmmodificarlin_ped.cpp \
    frmpresupuestoscli.cpp \
    presupuesto.cpp \
    frmmodificarlin_precli.cpp \
    frmcajaminuta.cpp \
    frmfichapaciente.cpp \
    frmagendavisitas.cpp \
    sqlcalls.cpp \
    paciente.cpp \
    episodio.cpp \
    frmanadirmedicamento.cpp \
    frmbuscarcie.cpp \
    vademecum.cpp \
    farmacologia.cpp \
    frminformacionfarmaco.cpp \
    frmanadirimagen.cpp \
    imagenesdiagnostico.cpp \
    prueba.cpp

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
    frmbuscarpoblacion.h \
    configuracion.h \
    empresa.h \
    articulo.h \
    frmBuscarFactura.h \
    frmmodificarlin_fac.h \
    columnamoneda.h \
    columnafecha.h \
    frmempresas.h \
    frmconfiguracion.h \
    albaran.h \
    frmalbaran.h \
    frmmodificarlin_alb.h \
    frmpedidos.h \
    pedidos.h \
    frmmodificarlin_ped.h \
    frmpresupuestoscli.h \
    presupuesto.h \
    frmmodificarlin_precli.h \
    frmcajaminuta.h \
    frmfichapaciente.h \
    frmagendavisitas.h \
    sqlcalls.h \
    paciente.h \
    episodio.h \
    frmanadirmedicamento.h \
    frmbuscarcie.h \
    vademecum.h \
    farmacologia.h \
    frminformacionfarmaco.h \
    frmanadirimagen.h \
    imagenesdiagnostico.h \
    prueba.h

FORMS    += mainwindow.ui \
    frmClientes.ui \
    login.ui \
    frmproveedores.ui \
    frmarticulos.ui \
    frmbuscarcliente.ui \
    frmdecision.ui \
    frmfacturas.ui \
    pruebas.ui \
    frmbuscarpoblacion.ui \
    frmBuscarFactura.ui \
    frmmodificarlin_fac.ui \
    frmempresas.ui \
    frmconfiguracion.ui \
    frmalbaran.ui \
    frmmodificarlin_alb.ui \
    frmpedidos.ui \
    frmmodificarlin_ped.ui \
    frmpresupuestoscli.ui \
    frmmodificarlin_precli.ui \
    frmcajaminuta.ui \
    frmagendavisitas.ui \
    frmfichapaciente.ui \
    frmanadirmedicamento.ui \
    frmbuscarcie.ui \
    frminformacionfarmaco.ui \
    frmanadirimagen.ui

RESOURCES += \
    terra.qrc
