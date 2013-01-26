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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /Qwt/5.2.1/include
INCLUDEPATH += /usr/local/Qxt/include/QxtCore
DEPENDPATH += /Qwt/5.2.1/lib
greaterThan(QT_MAJOR_VERSION, 4):INCLUDEPATH += /home/arcnexus/Qt5.0.0/5.0.0/gcc_64/include/QtWebKitWidgets



LIBS += -LC:/Qt/Qwt/5.2.1/lib
LIBS += -L/usr/local/Qxt/lib
greaterThan(QT_MAJOR_VERSION, 4):LIBS += -L/home/arcnexus/Qt5.0.0/Tools/QtCreator/lib/qtcreator/ -lQt5WebKitWidgets


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
    Busquedas/frmbuscarcliente.cpp \
    frmfacturas.cpp \
    factura.cpp \
    Busquedas/frmbuscarpoblacion.cpp \
    configuracion.cpp \
    empresa.cpp \
    articulo.cpp \
    Busquedas/frmBuscarFactura.cpp \
    frmmodificarlin_fac.cpp \
    columnafecha.cpp \
    Zona_Administrador/frmempresas.cpp \
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
    Busquedas/frmbuscarcie.cpp \
    vademecum.cpp \
    farmacologia.cpp \
    frminformacionfarmaco.cpp \
    frmanadirimagen.cpp \
    imagenesdiagnostico.cpp \
    prueba.cpp \
    interconsulta.cpp \
    frmanalitica.cpp \
    frmanalitica2.cpp \
    analitica.cpp \
    frmveranalitica.cpp \
    frmvisitas.cpp \
    Zona_Administrador/arearestringida_form.cpp \
    block_terra_form.cpp \
    Zona_Administrador/copy_db_progressfrm.cpp \
    visitas.cpp \
    Gestion_Almacen/gestion_seccionalmacen.cpp \
    db_table_view.cpp

HEADERS  += mainwindow.h \
    frmClientes.h \
    login.h \
    frmproveedores.h \
    frmarticulos.h \
    cliente.h \
    proveedor.h \
    Busquedas/frmbuscarcliente.h \
    frmfacturas.h \
    factura.h \
    Busquedas/frmbuscarpoblacion.h \
    configuracion.h \
    empresa.h \
    articulo.h \
    Busquedas/frmBuscarFactura.h \
    frmmodificarlin_fac.h \
    columnafecha.h \
    Zona_Administrador/frmempresas.h \
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
    Busquedas/frmbuscarcie.h \
    vademecum.h \
    farmacologia.h \
    frminformacionfarmaco.h \
    frmanadirimagen.h \
    imagenesdiagnostico.h \
    prueba.h \
    interconsulta.h \
    frmanalitica.h \
    frmanalitica2.h \
    analitica.h \
    frmveranalitica.h \
    frmvisitas.h \
    Zona_Administrador/arearestringida_form.h \
    block_terra_form.h \
    Zona_Administrador/copy_db_progressfrm.h \
    visitas.h \
    Gestion_Almacen/gestion_seccionalmacen.h \
    db_table_view.h

FORMS    += mainwindow.ui \
    frmClientes.ui \
    login.ui \
    frmproveedores.ui \
    frmarticulos.ui \
    Busquedas/frmbuscarcliente.ui \
    frmfacturas.ui \
    pruebas.ui \
    Busquedas/frmbuscarpoblacion.ui \
    Busquedas/frmBuscarFactura.ui \
    frmmodificarlin_fac.ui \
    Zona_Administrador/frmempresas.ui \
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
    Busquedas/frmbuscarcie.ui \
    frminformacionfarmaco.ui \
    frmanadirimagen.ui \
    frmanalitica.ui \
    frmanalitica2.ui \
    frmveranalitica.ui \
    prueba.ui \
    frmvisitas.ui \
    Zona_Administrador/arearestringida_form.ui \
    block_terra_form.ui \
    Zona_Administrador/copy_db_progressfrm.ui \
    Gestion_Almacen/gestion_seccionalmacen.ui \
    db_table_view.ui

RESOURCES += \
    terra.qrc

OTHER_FILES += \
    TODO.txt
