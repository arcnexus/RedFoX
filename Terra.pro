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
    Zona_Pacientes/frmClientes.cpp \
    login.cpp \
    frmproveedores.cpp \
    frmarticulos.cpp \
    Zona_Pacientes/cliente.cpp \
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
    Zona_Pacientes/frmfichapaciente.cpp \
    frmagendavisitas.cpp \
    sqlcalls.cpp \
    Zona_Pacientes/paciente.cpp \
    Zona_Pacientes/episodio.cpp \
    Zona_Pacientes/frmanadirmedicamento.cpp \
    Busquedas/frmbuscarcie.cpp \
    vademecum.cpp \
    Zona_Pacientes/farmacologia.cpp \
    Zona_Pacientes/frminformacionfarmaco.cpp \
    Zona_Pacientes/frmanadirimagen.cpp \
    imagenesdiagnostico.cpp \
    prueba.cpp \
    interconsulta.cpp \
    Zona_Pacientes/frmanalitica.cpp \
    Zona_Pacientes/frmanalitica2.cpp \
    Zona_Pacientes/analitica.cpp \
    Zona_Pacientes/frmveranalitica.cpp \
    frmvisitas.cpp \
    Zona_Administrador/arearestringida_form.cpp \
    block_terra_form.cpp \
    Zona_Administrador/copy_db_progressfrm.cpp \
    visitas.cpp \
    Gestion_Almacen/gestion_seccionalmacen.cpp \
    db_table_view.cpp \
    Auxiliares/search_lineedit.cpp \
    Auxiliares/table_helper.cpp \
    Auxiliares/readonlydelegate.cpp \
    Auxiliares/spinboxdelegate.cpp \
    Auxiliares/searchdelegate.cpp \
    Zona_Pacientes/visitas.cpp

HEADERS  += mainwindow.h \
    Zona_Pacientes/analitica.h \
    Zona_Pacientes/frmClientes.h \
    login.h \
    frmproveedores.h \
    frmarticulos.h \
    Zona_Pacientes/cliente.h \
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
    Zona_Pacientes/frmfichapaciente.h \
    frmagendavisitas.h \
    sqlcalls.h \
    Zona_Pacientes/paciente.h \
    Zona_Pacientes/episodio.h \
    Zona_Pacientes/frmanadirmedicamento.h \
    Busquedas/frmbuscarcie.h \
    vademecum.h \
    Zona_Pacientes/farmacologia.h \
    Zona_Pacientes/frminformacionfarmaco.h \
    Zona_Pacientes/frmanadirimagen.h \
    imagenesdiagnostico.h \
    prueba.h \
    interconsulta.h \
    Zona_Pacientes/frmanalitica.h \
    Zona_Pacientes/frmanalitica2.h \
    Zona_Pacientes/frmveranalitica.h \
    frmvisitas.h \
    Zona_Administrador/arearestringida_form.h \
    block_terra_form.h \
    Zona_Administrador/copy_db_progressfrm.h \
    visitas.h \
    Gestion_Almacen/gestion_seccionalmacen.h \
    db_table_view.h \
    Auxiliares/search_lineedit.h \
    Auxiliares/table_helper.h \
    Auxiliares/readonlydelegate.h \
    Auxiliares/spinboxdelegate.h \
    Auxiliares/searchdelegate.h \
    Zona_Pacientes/visitas.h

FORMS    += mainwindow.ui \
    Zona_Pacientes/frmClientes.ui \
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
    Zona_Pacientes/frmfichapaciente.ui \
    Zona_Pacientes/frmanadirmedicamento.ui \
    Busquedas/frmbuscarcie.ui \
    Zona_Pacientes/frminformacionfarmaco.ui \
    Zona_Pacientes/frmanadirimagen.ui \
    Zona_Pacientes/frmanalitica.ui \
    Zona_Pacientes/frmanalitica2.ui \
    Zona_Pacientes/frmveranalitica.ui \
    prueba.ui \
    frmvisitas.ui \
    Zona_Administrador/arearestringida_form.ui \
    block_terra_form.ui \
    Zona_Administrador/copy_db_progressfrm.ui \
    Gestion_Almacen/gestion_seccionalmacen.ui \
    db_table_view.ui \
    Auxiliares/search_lineedit.ui

RESOURCES += \
    terra.qrc

OTHER_FILES += \
    TODO.txt
