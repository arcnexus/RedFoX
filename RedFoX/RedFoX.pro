

+--------------------------------------------------
#
# Project created by QtCreator 2012-06-04T22:00:11
# y por ArcNexus
#-------------------------------------------------
CONFIG += c++11
QT       += core \
 gui
QT       += sql
QT       += network
QT       += xml
QT += printsupport
#QT       += webkit
#QT+= webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#DEFINES+= staticSql
#QTPLUGIN+=qsqlite
#QTPLUGIN+=qsqlmysql

win32:RC_FILE = windowsRc.rc

TARGET = RedFoX
TEMPLATE = app

# CONFIG  += qxt
# QXT     += core gui

SOURCES += main.cpp\
        mainwindow.cpp \
    Zona_Pacientes/frmClientes.cpp \
    login.cpp \
    Zona_Compras/frmproveedores.cpp \
    Zona_Pacientes/cliente.cpp \
    Zona_Compras/proveedor.cpp \
    configuracion.cpp \
    empresa.cpp \
    columnafecha.cpp \
    Zona_Administrador/frmempresas.cpp \
    Zona_Pacientes/frmfichapaciente.cpp \
    Zona_Pacientes/paciente.cpp \
    Zona_Pacientes/episodio.cpp \
    Zona_Pacientes/frmanadirmedicamento.cpp \
    Busquedas/frmbuscarcie.cpp \
    vademecum.cpp \
    Zona_Pacientes/farmacologia.cpp \
    Zona_Pacientes/frminformacionfarmaco.cpp \
    Zona_Pacientes/frmanadirimagen.cpp \
    Zona_Pacientes/imagenesdiagnostico.cpp \
    interconsulta.cpp \
    Zona_Pacientes/frmanalitica.cpp \
    Zona_Pacientes/frmanalitica2.cpp \
    Zona_Pacientes/analitica.cpp \
    Zona_Pacientes/frmveranalitica.cpp \
    frmvisitas.cpp \
    Zona_Administrador/arearestringida_form.cpp \
    Zona_Administrador/block_Maya_form.cpp \
    Zona_Administrador/copy_db_progressfrm.cpp \
    Zona_Administrador/auxmodule.cpp \
    db_table_view.cpp \
    Auxiliares/search_lineedit.cpp \
    Auxiliares/readonlydelegate.cpp \
    Auxiliares/spinboxdelegate.cpp \
    Auxiliares/searchdelegate.cpp \
    Zona_Pacientes/visitas.cpp \
    Auxiliares/comboboxdelegate.cpp \
    Almacen/frmtarifas.cpp \
    Almacen/frmarticulos.cpp \
    Almacen/articulo.cpp \
    Auxiliares/init_form.cpp \
    Zona_Compras/frmpedidosproveedor.cpp \
    Zona_Compras/frmalbaranproveedor.cpp \
    Zona_Compras/frmfacturasproveedor.cpp \
    Zona_Ventas/frmfacturaralabaranes.cpp \
    Zona_Pacientes/frmaddtipocliente.cpp \
    Zona_Compras/frmorden_pedido_producto.cpp \
    Agenda/graphicstable.cpp \
    Agenda/graphicsevent.cpp \
    Agenda/editeventform.cpp \
    Agenda/agendaform.cpp \
    Zona_Pacientes/frmpersonascontactocliente.cpp \
    Agenda/permisosagendaform.cpp \
    Zona_Ventas/presupuesto.cpp \
    Zona_Ventas/pedidos.cpp \
    Zona_Ventas/frmpresupuestoscli.cpp \
    Zona_Ventas/frmpedidos.cpp \
    Zona_Ventas/frmfacturas.cpp \
    Zona_Ventas/frmalbaran.cpp \
    Zona_Ventas/factura.cpp \
    Zona_Ventas/albaran.cpp \
    Zona_Compras/pedidoproveedor.cpp \ 
    Almacen/frmtipostarifa.cpp \
    Almacen/frmasociarproveedor.cpp \
    Auxiliares/monetarydelegate.cpp \
    Almacen/frmlistadosarticulo.cpp \
    Zona_Compras/frmrecepcion_pedidos.cpp \
    Zona_Pacientes/frmmtcbase.cpp \
    Zona_Pacientes/frmanadirdiagnostico.cpp \
    Zona_Administrador/frmconfigmaya.cpp \
    Zona_Pacientes/frmanadirhierb.cpp \
    Auxiliares/datedelegate.cpp \
    Zona_Contabilidad/frmentrada_apuntes.cpp \
    Auxiliares/frmdialogoimprimir.cpp \
    Busquedas/db_consulta_view.cpp \
    Auxiliares/timedmessagebox.cpp\
    Zona_Compras/albaranproveedor.cpp \
    Zona_Compras/facturasproveedor.cpp \
    Auxiliares/frmaddentregascuenta.cpp \
    Auxiliares/entregascuenta.cpp \
    Zona_Ventas/frmgestioncobros.cpp \
    Auxiliares/SMPT/smtpclient.cpp \
    Auxiliares/SMPT/quotedprintable.cpp \
    Auxiliares/SMPT/mimetext.cpp \
    Auxiliares/SMPT/mimepart.cpp \
    Auxiliares/SMPT/mimemultipart.cpp \
    Auxiliares/SMPT/mimemessage.cpp \
    Auxiliares/SMPT/mimeinlinefile.cpp \
    Auxiliares/SMPT/mimehtml.cpp \
    Auxiliares/SMPT/mimefile.cpp \
    Auxiliares/SMPT/mimecontentformatter.cpp \
    Auxiliares/SMPT/mimeattachment.cpp \
    Auxiliares/SMPT/emailaddress.cpp \
    mayamodule.cpp \
    moduleextension.cpp \
    Almacen/divisionalmacenext.cpp \
    archivosgeneralesext.cpp \
    Zona_Pacientes/clinicaext.cpp \
    Zona_Contabilidad/apuntes.cpp \
    Auxiliares/monetarydelegate_contabilidad.cpp \
    Zona_Contabilidad/frmcuadro_cuentas.cpp \
    Zona_Contabilidad/cuentas_contables.cpp \
    Almacen/tipostarifa.cpp \
    Almacen/frminventario.cpp \
    EditorReports/sectioneditordlg.cpp \
    EditorReports/section.cpp \
    EditorReports/roundedrect.cpp \
    EditorReports/reportview.cpp \
    EditorReports/reportrenderer.cpp \
    EditorReports/reportline.cpp \
    EditorReports/reportimage.cpp \
    EditorReports/reportfield.cpp \
    EditorReports/relationalfield.cpp \
    EditorReports/paper.cpp \
    EditorReports/pageheadersection.cpp \
    EditorReports/edittextdlg.cpp \
    EditorReports/editrelationalfield.cpp \
    EditorReports/editrectdlg.cpp \
    EditorReports/editpaperdlg.cpp \
    EditorReports/editlinedlg.cpp \
    EditorReports/editimagedlg.cpp \
    EditorReports/editfielddlg.cpp \
    EditorReports/editdinamycitemdlg.cpp \
    EditorReports/editdetailsecdlg.cpp \
    EditorReports/editcodebardlg.cpp \
    EditorReports/detailsection.cpp \
    EditorReports/customlabel.cpp \
    EditorReports/container.cpp \
    EditorReports/codebar.cpp \
    EditorReports/reportdesigwin.cpp \
    EditorReports/repdesignmodule.cpp \
    EditorReports/savetobdfrm.cpp \
    Almacen/frmexcepciones.cpp \
    Almacen/frmkit.cpp \
    Zona_Maestros/frmtransportistas.cpp \
    Zona_Maestros/transportistas.cpp \
    Zona_Ventas/frmgestioncobros2.cpp \
    vencimientos.cpp \
    Zona_Maestros/frmformas_pago.cpp \
    Auxiliares/monetarydelegate_totals.cpp \
    TPV/frmtpv.cpp \
    TPV/tpv.cpp \
    barraavisos.cpp \
    Auxiliares/refresca_ofertas.cpp \
    Auxiliares/barrabusqueda.cpp \
    Auxiliares/frmcambiarusuario.cpp \
    Auxiliares/avisos.cpp \
    Auxiliares/frmnuevosavisos.cpp \
    Auxiliares/delegatekit.cpp \
    EditorReports/reportparama.cpp \
    EditorReports/editparamdlg.cpp \
    TPV/frmcierrecaja.cpp \
    TPV/frmextrascaja.cpp \
    TPV/frmretirardecaja.cpp \
    TPV/frminsertardinerocaja.cpp \
    TPV/frmdevolucionticket.cpp \
    TPV/frmcausadevolucion.cpp \
    TPV/frmcantidad.cpp \
    Zona_Administrador/frmconfigurar_terminal.cpp \
    TPV/frmcajasabiertas.cpp \
    Auxiliares/globoaviso.cpp \
    TPV/frmdevolucion2.cpp \
    TPV/frmcrearvale.cpp \
    Zona_Pacientes/vale.cpp \
    Auxiliares/frmeditaravisos.cpp \
    EditorReports/reportacumulator.cpp \
    EditorReports/editacum.cpp \
    Zona_Contabilidad/frmbuscarapuntecontable.cpp \
    Auxiliares/frmeditline.cpp \
    Auxiliares/numericdelegate.cpp \
    Almacen/frmaddlotes.cpp \
    Almacen/frmselectlotes.cpp \
    Zona_Maestros/frmcajas.cpp \
    Zona_Maestros/cajas.cpp \
    Zona_Maestros/frmagentes.cpp \
    Zona_Maestros/frmtipocliente.cpp\
    Auxiliares/frmgastos_ped_pro.cpp \
    Zona_Ventas/frmlistadospre.cpp \
    Auxiliares/dlgsetupmail.cpp \
    Zona_Ventas/frmlistadoped.cpp \
    Zona_Ventas/frmlistadoalbaran.cpp \
    Zona_Ventas/frmlistadofac.cpp \
    Zona_Compras/dlgpedidoalbfact.cpp \
    Almacen/dlgdivisionesalmacen.cpp \
    Almacen/importararticuloext.cpp \
    Almacen/dlgimportods.cpp \
    Zona_Ventas/frmcobrarfactura.cpp


HEADERS  += mainwindow.h \
    Zona_Pacientes/analitica.h \
    Zona_Pacientes/frmClientes.h \
    Zona_Administrador\block_Maya_form.h \
    login.h \
    Zona_Compras/frmproveedores.h \
    Zona_Pacientes/cliente.h \
    Zona_Compras/proveedor.h \
    configuracion.h \
    empresa.h \
    columnafecha.h \
    Zona_Administrador/frmempresas.h \
    Zona_Pacientes/frmfichapaciente.h \
    Zona_Pacientes/paciente.h \
    Zona_Pacientes/episodio.h \
    Zona_Pacientes/frmanadirmedicamento.h \
    Busquedas/frmbuscarcie.h \
    vademecum.h \
    Zona_Pacientes/farmacologia.h \
    Zona_Pacientes/frminformacionfarmaco.h \
    Zona_Pacientes/frmanadirimagen.h \
    Zona_Pacientes/imagenesdiagnostico.h \
    interconsulta.h \
    Zona_Pacientes/frmanalitica.h \
    Zona_Pacientes/frmanalitica2.h \
    Zona_Pacientes/frmveranalitica.h \
    frmvisitas.h \
    Zona_Administrador/arearestringida_form.h \
    Zona_Administrador/copy_db_progressfrm.h \
    db_table_view.h \
    Auxiliares/search_lineedit.h \
    Auxiliares/readonlydelegate.h \
    Auxiliares/spinboxdelegate.h \
    Auxiliares/searchdelegate.h \
    Auxiliares/Globlal_Include.h\
    Zona_Pacientes/visitas.h \
    Auxiliares/comboboxdelegate.h \
    Almacen/frmtarifas.h \
    Almacen/frmarticulos.h \
    Almacen/articulo.h \
    Auxiliares/init_form.h \
    Zona_Compras/frmpedidosproveedor.h \
    Zona_Compras/frmalbaranproveedor.h \
    Zona_Compras/frmfacturasproveedor.h \
    Zona_Ventas/frmfacturaralabaranes.h \
    Zona_Pacientes/frmaddtipocliente.h \
    Zona_Compras/frmorden_pedido_producto.h \
    Agenda/graphicstable.h \
    Agenda/graphicsevent.h \
    Agenda/editeventform.h \
    Agenda/agendaform.h \
    Zona_Pacientes/frmpersonascontactocliente.h \
    Agenda/permisosagendaform.h \
    Zona_Ventas/presupuesto.h \
    Zona_Ventas/pedidos.h \
    Zona_Ventas/frmpresupuestoscli.h \
    Zona_Ventas/frmpedidos.h \
    Zona_Ventas/frmfacturas.h \
    Zona_Ventas/frmalbaran.h \
    Zona_Ventas/factura.h \
    Zona_Ventas/albaran.h \
    Zona_Compras/pedidoproveedor.h \
    Almacen/frmtipostarifa.h \
    Almacen/frmasociarproveedor.h \
    Auxiliares/monetarydelegate.h \
    Almacen/frmlistadosarticulo.h \
    Zona_Compras/frmrecepcion_pedidos.h \
    Zona_Pacientes/frmmtcbase.h \
    Zona_Pacientes/frmanadirdiagnostico.h \
    Zona_Administrador/frmconfigmaya.h \
    Zona_Administrador/block_Maya_form.h \
    Zona_Administrador/auxmodule.h \
    Zona_Pacientes/frmanadirhierb.h \
    Auxiliares/datedelegate.h \
    Zona_Contabilidad/frmentrada_apuntes.h \
    Auxiliares/frmdialogoimprimir.h \
    Busquedas/db_consulta_view.h \
    Auxiliares/timedmessagebox.h\
    Zona_Compras/albaranproveedor.h \
    Zona_Compras/facturasproveedor.h \
    Auxiliares/frmaddentregascuenta.h \
    Auxiliares/entregascuenta.h \
    Zona_Ventas/frmgestioncobros.h \
    Auxiliares/SMPT/SmtpMime \
    Auxiliares/SMPT/smtpclient.h \
    Auxiliares/SMPT/quotedprintable.h \
    Auxiliares/SMPT/mimetext.h \
    Auxiliares/SMPT/mimepart.h \
    Auxiliares/SMPT/mimemultipart.h \
    Auxiliares/SMPT/mimemessage.h \
    Auxiliares/SMPT/mimeinlinefile.h \
    Auxiliares/SMPT/mimehtml.h \
    Auxiliares/SMPT/mimefile.h \
    Auxiliares/SMPT/mimecontentformatter.h \
    Auxiliares/SMPT/mimeattachment.h \
    Auxiliares/SMPT/emailaddress.h \
    mayamodule.h \
    moduleextension.h \
    Almacen/divisionalmacenext.h \
    archivosgeneralesext.h \
    Zona_Pacientes/clinicaext.h \
    Zona_Contabilidad/apuntes.h \
    Auxiliares/monetarydelegate_contabilidad.h \
    Zona_Contabilidad/frmcuadro_cuentas.h \
    Zona_Contabilidad/cuentas_contables.h \
    Almacen/tipostarifa.h \
    Almacen/frminventario.h \
    EditorReports/sectioneditordlg.h \
    EditorReports/section.h \
    EditorReports/roundedrect.h \
    EditorReports/reportview.h \
    EditorReports/reportrenderer.h \
    EditorReports/reportline.h \
    EditorReports/reportimage.h \
    EditorReports/reportfield.h \
    EditorReports/relationalfield.h \
    EditorReports/paper.h \
    EditorReports/pageheadersection.h \
    EditorReports/edittextdlg.h \
    EditorReports/editrelationalfield.h \
    EditorReports/editrectdlg.h \
    EditorReports/editpaperdlg.h \
    EditorReports/editlinedlg.h \
    EditorReports/editimagedlg.h \
    EditorReports/editfielddlg.h \
    EditorReports/editdinamycitemdlg.h \
    EditorReports/editdetailsecdlg.h \
    EditorReports/editcodebardlg.h \
    EditorReports/detailsection.h \
    EditorReports/customlabel.h \
    EditorReports/container.h \
    EditorReports/codebar.h \
    EditorReports/reportdesigwin.h \
    EditorReports/repdesignmodule.h \
    EditorReports/savetobdfrm.h \
    Almacen/frmexcepciones.h \
    Almacen/frmkit.h \
    Zona_Maestros/frmtransportistas.h \
    Zona_Maestros/transportistas.h \
    Zona_Ventas/frmgestioncobros2.h \
    vencimientos.h \
    Zona_Maestros/frmformas_pago.h \
    Auxiliares/monetarydelegate_totals.h \
    TPV/frmtpv.h \
    TPV/tpv.h \
    barraavisos.h \
    Auxiliares/refresca_ofertas.h \
    Auxiliares/barrabusqueda.h \
    Auxiliares/frmcambiarusuario.h \
    Auxiliares/avisos.h \
    Auxiliares/frmnuevosavisos.h \
    Auxiliares/delegatekit.h \
    EditorReports/reportparama.h \
    EditorReports/editparamdlg.h \
    TPV/frmcierrecaja.h \
    TPV/frmextrascaja.h \
    TPV/frmretirardecaja.h \
    TPV/frminsertardinerocaja.h \
    TPV/frmdevolucionticket.h \
    TPV/frmcausadevolucion.h \
    TPV/frmcantidad.h \
    Zona_Administrador/frmconfigurar_terminal.h \
    TPV/frmcajasabiertas.h \
    Auxiliares/globoaviso.h \
    TPV/frmdevolucion2.h \
    TPV/frmcrearvale.h \
    Zona_Pacientes/vale.h \
    Auxiliares/frmeditaravisos.h \
    EditorReports/reportacumulator.h \
    EditorReports/editacum.h \
    Zona_Contabilidad/frmbuscarapuntecontable.h \
    Auxiliares/frmeditline.h \
    Auxiliares/numericdelegate.h \
    Almacen/frmaddlotes.h \
    Almacen/frmselectlotes.h \
    Zona_Maestros/frmcajas.h \
    Zona_Maestros/cajas.h \
    Zona_Maestros/frmagentes.h \
    Zona_Maestros/frmtipocliente.h \
    Auxiliares/frmgastos_ped_pro.h \
    Zona_Ventas/frmlistadospre.h \
    Auxiliares/dlgsetupmail.h \
    Zona_Ventas/frmlistadoped.h \
    Zona_Ventas/frmlistadoalbaran.h \
    Zona_Ventas/frmlistadofac.h \
    Zona_Compras/dlgpedidoalbfact.h \
    Almacen/dlgdivisionesalmacen.h \
    Almacen/importararticuloext.h \
    Almacen/dlgimportods.h \
    Zona_Ventas/frmcobrarfactura.h



FORMS    += mainwindow.ui \
    Zona_Pacientes/frmClientes.ui \
    login.ui \
    Zona_Compras/frmproveedores.ui \
    Zona_Administrador/frmempresas.ui \
    Zona_Pacientes/frmfichapaciente.ui \
    Zona_Pacientes/frmanadirmedicamento.ui \
    Busquedas/frmbuscarcie.ui \
    Zona_Pacientes/frminformacionfarmaco.ui \
    Zona_Pacientes/frmanadirimagen.ui \
    Zona_Pacientes/frmanalitica.ui \
    Zona_Pacientes/frmanalitica2.ui \
    Zona_Pacientes/frmveranalitica.ui \
    frmvisitas.ui \
    Zona_Administrador/arearestringida_form.ui \
    Zona_Administrador/block_Maya_form.ui \
    Zona_Administrador/copy_db_progressfrm.ui \
    db_table_view.ui \
    Auxiliares/search_lineedit.ui \
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
    Agenda/editeventform.ui \
    Agenda/agendaform.ui \
    Zona_Pacientes/frmpersonascontactocliente.ui \
    Agenda/permisosagendaform.ui \
    Zona_Ventas/frmpresupuestoscli.ui \
    Zona_Ventas/frmpedidos.ui \
    Zona_Ventas/frmfacturas.ui \
    Zona_Ventas/frmalbaran.ui \
    Almacen/frmtipostarifa.ui \
    Almacen/frmasociarproveedor.ui \
    Almacen/frmlistadosarticulo.ui \
    Zona_Compras/frmrecepcion_pedidos.ui \
    Zona_Pacientes/frmmtcbase.ui \
    Zona_Pacientes/frmanadirdiagnostico.ui \
    Zona_Administrador/frmconfigmaya.ui \
    Zona_Pacientes/frmanadirhierb.ui \
    Zona_Contabilidad/frmentrada_apuntes.ui \
    Auxiliares/frmdialogoimprimir.ui \
    Busquedas/db_consulta_view.ui \
    Auxiliares/frmaddentregascuenta.ui \
    Zona_Ventas/frmgestioncobros.ui \
    Zona_Contabilidad/frmcuadro_cuentas.ui \
    Almacen/frminventario.ui \
    EditorReports/sectioneditordlg.ui \
    EditorReports/edittextdlg.ui \
    EditorReports/editrelationalfield.ui \
    EditorReports/editrectdlg.ui \
    EditorReports/editpaperdlg.ui \
    EditorReports/editlinedlg.ui \
    EditorReports/editimagedlg.ui \
    EditorReports/editfielddlg.ui \
    EditorReports/editdetailsecdlg.ui \
    EditorReports/editcodebardlg.ui \
    EditorReports/reportdesigwin.ui \
    EditorReports/savetobdfrm.ui \
    Almacen/frmexcepciones.ui \
    Almacen/frmkit.ui \
    Zona_Maestros/frmtransportistas.ui \
    Zona_Ventas/frmgestioncobros2.ui \
    Zona_Maestros/frmformas_pago.ui \
    TPV/frmtpv.ui \
    barraavisos.ui \
    Auxiliares/barrabusqueda.ui \
    Auxiliares/frmcambiarusuario.ui \
    Auxiliares/frmnuevosavisos.ui \
    EditorReports/editparamdlg.ui \
    TPV/frmcierrecaja.ui \
    TPV/frmextrascaja.ui \
    TPV/frmretirardecaja.ui \
    TPV/frminsertardinerocaja.ui \
    TPV/frmdevolucionticket.ui \
    TPV/frmcausadevolucion.ui \
    TPV/frmcantidad.ui \
    Zona_Administrador/frmconfigurar_terminal.ui \
    TPV/frmcajasabiertas.ui \
    TPV/frmdevolucion2.ui \
    TPV/frmcrearvale.ui \
    Auxiliares/frmeditaravisos.ui \
    EditorReports/editacum.ui \
    Zona_Contabilidad/frmbuscarapuntecontable.ui \
    Auxiliares/frmeditline.ui \
    Almacen/frmaddlotes.ui \
    Almacen/frmselectlotes.ui \
    Zona_Maestros/frmcajas.ui \
    Zona_Maestros/frmagentes.ui \
    Zona_Maestros/frmtipocliente.ui \
    Auxiliares/frmgastos_ped_pro.ui \
    Zona_Ventas/frmlistadospre.ui \
    Auxiliares/dlgsetupmail.ui \
    Zona_Ventas/frmlistadoped.ui \
    Zona_Ventas/frmlistadoalbaran.ui \
    Zona_Ventas/frmlistadofac.ui \
    Zona_Compras/dlgpedidoalbfact.ui \
    Almacen/dlgdivisionesalmacen.ui \
    Almacen/dlgimportods.ui \
    Zona_Ventas/frmcobrarfactura.ui

RESOURCES += \
    maya.qrc \
    EditorReports/resource.qrc \
    EditorReports/resource.qrc

OTHER_FILES += \
    TODO.txt \
    windowsRc.rc \
    EditorReports/Graphics.pro.user \


!win32: LIBS += -lz

DEFINES += NOMINMAX

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Crypto/release/ -lCrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Crypto/debug/ -lCrypto
else:unix: LIBS += -L$$OUT_PWD/../Crypto/ -lCrypto

INCLUDEPATH += $$PWD/../Crypto
DEPENDPATH += $$PWD/../Crypto

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../SqlLib/release/ -lSqlLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../SqlLib/debug/ -lSqlLib
else:unix: LIBS += -L$$OUT_PWD/../SqlLib/ -lSqlLib

INCLUDEPATH += $$PWD/../SqlLib
DEPENDPATH += $$PWD/../SqlLib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../openCharts/release/ -llibopenchartplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../openCharts/debug/ -llibopenchartplugin
else:unix: LIBS += -L$$OUT_PWD/../openCharts/ -lopenchartplugin

INCLUDEPATH += $$PWD/../openCharts
DEPENDPATH += $$PWD/../openCharts

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../BlinkingLabel/release/ -lblinkinkplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../BlinkingLabel/debug/ -lblinkinkplugin
else:unix: LIBS += -L$$OUT_PWD/../BlinkingLabel/ -lblinkinkplugin

INCLUDEPATH += $$PWD/../BlinkingLabel
DEPENDPATH += $$PWD/../BlinkingLabel

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ODSlib/release/ -lODSlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ODSlib/debug/ -lODSlib
else:unix: LIBS += -L$$OUT_PWD/../ODSlib/ -lODSlib

INCLUDEPATH += $$PWD/../ODSlib
DEPENDPATH += $$PWD/../ODSlib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../quazip/release/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../quazip/debug/ -lquazip
else:unix: LIBS += -L$$OUT_PWD/../quazip/ -lquazip

INCLUDEPATH += $$PWD/../quazip
DEPENDPATH += $$PWD/../quazip
