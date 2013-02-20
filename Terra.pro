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

# CONFIG  += qxt
# QXT     += core gui


SOURCES += main.cpp\
        mainwindow.cpp \
    Zona_Pacientes/frmClientes.cpp \
    login.cpp \
    frmproveedores.cpp \
    Zona_Pacientes/cliente.cpp \
    proveedor.cpp \
    Busquedas/frmbuscarcliente.cpp \
    frmfacturas.cpp \
    factura.cpp \
    Busquedas/frmbuscarpoblacion.cpp \
    configuracion.cpp \
    empresa.cpp \
    Busquedas/frmBuscarFactura.cpp \
    columnafecha.cpp \
    Zona_Administrador/frmempresas.cpp \
    frmconfiguracion.cpp \
    albaran.cpp \
    frmalbaran.cpp \
    frmpedidos.cpp \
    pedidos.cpp \
    frmpresupuestoscli.cpp \
    presupuesto.cpp \
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
    db_table_view.cpp \
    Auxiliares/search_lineedit.cpp \
    Auxiliares/table_helper.cpp \
    Auxiliares/readonlydelegate.cpp \
    Auxiliares/spinboxdelegate.cpp \
    Auxiliares/searchdelegate.cpp \
    Zona_Pacientes/visitas.cpp \
    Auxiliares/comboboxdelegate.cpp \
    TPV/frmcajaminuta.cpp \
    TPV/ticket.cpp \
    TPV/lineaticket.cpp \
    Almacen/frmtarifas.cpp \
    Almacen/frmarticulos.cpp \
    Almacen/articulo.cpp \
    Auxiliares/init_form.cpp \
    Auxiliares/toolbarbutton.cpp \
    Zona_Compras/frmpedidosproveedor.cpp \
    Zona_Compras/frmalbaranproveedor.cpp \
    Zona_Compras/frmfacturasproveedor.cpp \
    Zona_Ventas/frmfacturaralabaranes.cpp \
    Zona_Pacientes/frmaddtipocliente.cpp \
    Zona_Compras/frmorden_pedido_producto.cpp \
    openrptLibs/Auxiliares/reportwriterwindow.cpp \
    Zona_Administrador/frmconfigterra.cpp \
    Agenda/graphicstable.cpp \
    Agenda/graphicsevent.cpp \
    Agenda/editeventform.cpp \
    Agenda/agendaform.cpp \
    Zona_Pacientes/frmpersonascontactocliente.cpp

HEADERS  += mainwindow.h \
    Zona_Pacientes/analitica.h \
    Zona_Pacientes/frmClientes.h \
    login.h \
    frmproveedores.h \
    Zona_Pacientes/cliente.h \
    proveedor.h \
    Busquedas/frmbuscarcliente.h \
    frmfacturas.h \
    factura.h \
    Busquedas/frmbuscarpoblacion.h \
    configuracion.h \
    empresa.h \
    Busquedas/frmBuscarFactura.h \
    columnafecha.h \
    Zona_Administrador/frmempresas.h \
    frmconfiguracion.h \
    albaran.h \
    frmalbaran.h \
    frmpedidos.h \
    pedidos.h \
    frmpresupuestoscli.h \
    presupuesto.h \
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
    db_table_view.h \
    Auxiliares/search_lineedit.h \
    Auxiliares/table_helper.h \
    Auxiliares/readonlydelegate.h \
    Auxiliares/spinboxdelegate.h \
    Auxiliares/searchdelegate.h \
    Auxiliares/Globlal_Include.h\
    Zona_Pacientes/visitas.h \
    Auxiliares/comboboxdelegate.h \
    TPV/frmcajaminuta.h \
    TPV/ticket.h \
    TPV/lineaticket.h \
    Almacen/frmtarifas.h \
    Almacen/frmarticulos.h \
    Almacen/articulo.h \
    Auxiliares/init_form.h \
    Auxiliares/toolbarbutton.h \
    Zona_Compras/frmpedidosproveedor.h \
    Zona_Compras/frmalbaranproveedor.h \
    Zona_Compras/frmfacturasproveedor.h \
    Zona_Ventas/frmfacturaralabaranes.h \
    Zona_Pacientes/frmaddtipocliente.h \
    Zona_Compras/frmorden_pedido_producto.h \
    openrptLibs/Auxiliares/reportwriterwindow.h \
    Zona_Administrador/frmconfigterra.h \   
     Agenda/graphicstable.h \
    Agenda/graphicsevent.h \
    Agenda/editeventform.h \
    Agenda/agendaform.h \
    Zona_Pacientes/frmpersonascontactocliente.h


FORMS    += mainwindow.ui \
    Zona_Pacientes/frmClientes.ui \
    login.ui \
    frmproveedores.ui \
    Busquedas/frmbuscarcliente.ui \
    frmfacturas.ui \
    pruebas.ui \
    Busquedas/frmbuscarpoblacion.ui \
    Busquedas/frmBuscarFactura.ui \
    Zona_Administrador/frmempresas.ui \
    frmconfiguracion.ui \
    frmalbaran.ui \
    frmpedidos.ui \
    frmpresupuestoscli.ui \
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
    db_table_view.ui \
    Auxiliares/search_lineedit.ui \
    TPV/frmcajaminuta.ui \
    Almacen/frmtarifas.ui \
    Almacen/frmarticulos.ui \
    Auxiliares/init_form.ui \
    Auxiliares/toolbarbutton.ui \
    Zona_Compras/frmpedidosproveedor.ui \
    Zona_Compras/frmalbaranproveedor.ui \
    Zona_Compras/frmfacturasproveedor.ui \
    Zona_Ventas/frmfacturaralabaranes.ui \
    Zona_Pacientes/frmaddtipocliente.ui \
    Zona_Compras/frmorden_pedido_producto.ui \
    Zona_Administrador/frmconfigterra.ui \
    Agenda/editeventform.ui \
    Agenda/agendaform.ui \
    Zona_Pacientes/frmpersonascontactocliente.ui

RESOURCES += \
    terra.qrc \

OTHER_FILES += \
    TODO.txt

unix:!macx: LIBS += -L$$PWD/openrptLibs/linux/ -lcommon

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

unix:!macx: LIBS += -L$$PWD/openrptLibs/linux/ -lrenderer

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

unix:!macx: LIBS += -L$$PWD/openrptLibs/linux/ -lMetaSQL

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

unix:!macx: LIBS += -L$$PWD/openrptLibs/linux/ -lwrtembed

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/openrptLibs/win32/release/ -lcommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/openrptLibs/win32/debug/ -lcommon

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/release/common.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/debug/common.lib


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/openrptLibs/win32/release/ -lMetaSQL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/openrptLibs/win32/debug/ -lMetaSQL

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/release/MetaSQL.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/debug/MetaSQL.lib


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/openrptLibs/win32/release/ -lrenderer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/openrptLibs/win32/debug/ -lrenderer

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/release/renderer.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/debug/renderer.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/openrptLibs/win32/release/ -lwrtembed
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/openrptLibs/win32/debug/ -lwrtembed

INCLUDEPATH += $$PWD/openrptLibs/include
DEPENDPATH += $$PWD/openrptLibs/include

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/release/wrtembed.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/openrptLibs/win32/debug/wrtembed.lib
