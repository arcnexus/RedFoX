#include "presupuesto.h"


Presupuesto::Presupuesto()
{
    this->id = 0;
}

Presupuesto::~Presupuesto()
{
}

void Presupuesto::AnadirPresupuesto()
{
    this->nIva1 = Configuracion_global->nIVA1;
    this->nIva2 = Configuracion_global->nIVA2;
    this->nIva3 = Configuracion_global->nIVA3;
    this->nIva4 = Configuracion_global->nIVA4 ;
    this->nRecargoEquivalencia1 = Configuracion_global->nRE1;
    this->nRecargoEquivalencia2 = Configuracion_global->nRE2;
    this->nRecargoEquivalencia3 = Configuracion_global->nRE3;
    this->nRecargoEquivalencia4 = Configuracion_global->nRE4;

    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    cab_pre.prepare("INSERT INTO cab_pre (nPresupuesto, dFecha, dValidoHasta,"
                    "id_Cliente, cCodigoCliente, cCliente, cCif, cDireccion, cDireccion2, cCP,"
                    "cPoblacion, cProvincia, idpais, cTelefono, cMovil, cFax, nDto, tComentarios,"
                    "rImporte, rSubtotal, rDescuento, rTotal, lImpreso, lAprobado, dFechaAprobacion,"
                    "rImporteFactura, rImportePendiente, cFactura, nAlbaran, nPedido, id_FormaPago,"
                    "tLugarEntrega, cAtencionde, rBase1, rBase2, rBase3, rBase4, nIva1, nIva2, nIva3,"
                    "nIva4, rIva1, rIva2, rIva3, rIva4, nRecargoEquivalencia1, nRecargoEquivalencia2,"
                    "nRecargoEquivalencia3, nRecargoEquivalencia4, rRec1, rRec2, rRec3, rRec4, rTotal1,"
                    "rTotal2, rTotal3, rTotal4, lRecargoEquivalencia, cEmail, rTotalIva, rTotalRec,"
                    "rImporte1, rImporte2, rImporte3, rImporte4) "
                    "VALUES"
                    "(:nPresupuesto, :dFecha, :dValidoHasta,"
                    ":id_Cliente, :cCodigoCliente, :cCliente, :cCif, :cDireccion, :cDireccion2, :cCP,"
                    ":cPoblacion, :cProvincia, :idpais, :cTelefono, :cMovil, :cFax, :nDto, :tComentarios,"
                    ":rImporte, :rSubtotal, :rDescuento, :rTotal, :lImpreso, :lAprobado, :dFechaAprobacion,"
                    ":rImporteFactura, :rImportePendiente, :cFactura, :nAlbaran, :nPedido, :id_FormaPago,"
                    ":tLugarEntrega, :cAtencionde, :rBase1, :rBase2, :rBase3, :rBase4, :nIva1, :nIva2, :nIva3,"
                    ":nIva4, :rIva1, :rIva2, :rIva3, :rIva4, :nRecargoEquivalencia1, :nRecargoEquivalencia2,"
                    ":nRecargoEquivalencia3, :nRecargoEquivalencia4, :rRec1, :rRec2, :rRec3, :rRec4, :rTotal1,"
                    ":rTotal2, :rTotal3, :rTotal4, :lRecargoEquivalencia, :cEmail, :rTotalIva, :rTotalRec,"
                    ":rImporte1, :rImporte2, :rImporte3, :rImporte4)");

    int x = 0;
    cab_pre.bindValue(":nPresupuesto",nPresupuesto);
    cab_pre.bindValue(":dFecha",dFecha);
    cab_pre.bindValue(":dValidoHasta",dValidoHasta);
    cab_pre.bindValue(":id_Cliente",id_cliente);
    cab_pre.bindValue(":cCodigoCliente",cCodigoCliente);
    cab_pre.bindValue(":cCliente",cCliente);
    cab_pre.bindValue(":cCif",cCif);
    cab_pre.bindValue(":cDireccion",cDireccion);
    cab_pre.bindValue(":cDireccion2",cDireccion2);
    cab_pre.bindValue(":cCP",cCP);
    cab_pre.bindValue(":cPoblacion",cPoblacion);
    cab_pre.bindValue(":cProvincia",cProvincia);
    cab_pre.bindValue(":idpais",idPais);
    cab_pre.bindValue(":cTelefono",x);
    cab_pre.bindValue(":cMovil",x);
    cab_pre.bindValue(":cFax",x);
    cab_pre.bindValue(":nDto",x);
    cab_pre.bindValue(":tComentarios",x);
    cab_pre.bindValue(":rImporte",x);
    cab_pre.bindValue(":rSubtotal",x);
    cab_pre.bindValue(":rDescuento",x);
    cab_pre.bindValue(":rTotal",x);
    cab_pre.bindValue(":lImpreso",x);
    cab_pre.bindValue(":lAprobado",x);
    cab_pre.bindValue(":dFechaAprobacion",x);
    cab_pre.bindValue(":rImporteFactura",x);
    cab_pre.bindValue(":rImportePendiente",x);
    cab_pre.bindValue(":cFactura",x);
    cab_pre.bindValue(":nAlbaran",x);
    cab_pre.bindValue(":nPedido",x);
    cab_pre.bindValue(":id_FormaPago",x);
    cab_pre.bindValue(":tLugarEntrega",x);
    cab_pre.bindValue(":cAtencionde",x);
    cab_pre.bindValue(":rBase1",x);
    cab_pre.bindValue(":rBase2",x);
    cab_pre.bindValue(":rBase3",x);
    cab_pre.bindValue(":rBase4",x);
    cab_pre.bindValue(":nIva1",x);
    cab_pre.bindValue(":nIva2",x);
    cab_pre.bindValue(":nIva3",x);
    cab_pre.bindValue(":nIva4",x);
    cab_pre.bindValue(":rIva1",x);
    cab_pre.bindValue(":rIva2",x);
    cab_pre.bindValue(":rIva3",x);
    cab_pre.bindValue(":rIva4",x);
    cab_pre.bindValue(":nRecargoEquivalencia1",x);
    cab_pre.bindValue(":nRecargoEquivalencia2",x);
    cab_pre.bindValue(":nRecargoEquivalencia3",x);
    cab_pre.bindValue(":nRecargoEquivalencia4",x);
    cab_pre.bindValue(":rRec1",x);
    cab_pre.bindValue(":rRec2",x);
    cab_pre.bindValue(":rRec3",x);
    cab_pre.bindValue(":rRec4",x);
    cab_pre.bindValue(":rTotal1",x);
    cab_pre.bindValue(":rTotal2",x);
    cab_pre.bindValue(":rTotal3",x);
    cab_pre.bindValue(":rTotal4",x);
    cab_pre.bindValue(":lRecargoEquivalencia",x);
    cab_pre.bindValue(":cEmail",x);
    cab_pre.bindValue(":rTotalIva",x);
    cab_pre.bindValue(":rTotalRec",x);
    cab_pre.bindValue(":rImporte1",x);
    cab_pre.bindValue(":rImporte2",x);
    cab_pre.bindValue(":rImporte3",x);
    cab_pre.bindValue(":rImporte4",x);

     if(!cab_pre.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Presupuesto:", cab_pre.lastError().text());
     }
     else
     {
         this->id = cab_pre.lastInsertId().toInt();
         QString cSQL = "Select * from cab_pre where id ="+QString::number(this->id);
         RecuperarPresupuesto(cSQL);
     }
}

void Presupuesto::RecuperarPresupuesto(QString cSQL)
{
    QSqlQuery qCab_pre(QSqlDatabase::database("empresa"));
    qCab_pre.prepare(cSQL);
    if( !qCab_pre.exec() ) {
        QMessageBox::critical(qApp->activeWindow(), "error:", qCab_pre.lastError().text());
    } else {
        if (qCab_pre.next()) {
            QSqlRecord registro = qCab_pre.record();
            this->id = registro.field("Id").value().toInt();
            this->nPresupuesto = registro.field("nPresupuesto").value().toInt();
            this->dFecha = registro.field("dFecha").value().toDate();
            this->dValidoHasta = registro.field("dValidoHasta").value().toDate();
            this->id_cliente = registro.field("id_cliente").value().toInt();
            this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
            this->cCliente = registro.field("cCliente").value().toString();
            this->cCif = registro.field("cCif").value().toString();
            this->cDireccion = registro.field("cDireccion").value().toString();
            this->cDireccion2 = registro.field("cDireccion2").value().toString();
            this->cCP = registro.field("cCP").value().toString();
            this->cPoblacion = registro.field("cPoblacion").value().toString();
            this->cProvincia = registro.field("cProvincia").value().toString();
            //TODO get pais id
            //this->cPais = registro.field("cPais").value().toString();
            this->cTelefono = registro.field("cTelefono").value().toString();
            this->cMovil = registro.field("cMovil").value().toString();
            this->cFax = registro.field("cFax").value().toString();
            this->cEmail = registro.field("cEmail").value().toString();
            this->nDto = registro.field("nDto").value().toDouble();
            this->tComentarios = registro.field("tComentarios").value().toString();
            this->rImporte = registro.field("rImporte").value().toDouble();
            this->rSubTotal = registro.field("rSubtotal").value().toDouble();
            this->rDescuento = registro.field("rDescuento").value().toDouble();
            this->rTotal = registro.field("rTotal").value().toDouble();
            this->lImpreso = registro.field("lImpreso").value().toBool();
            this->lAprobado = registro.field("lAprobado").value().toBool();
            this->dFechaAprobacion = registro.field("dFechaAprobacion").value().toDate();
            this->rImporteFactura = registro.field("rImporteFactura").value().toDouble();
            this->cFactura = registro.field("cFactura").value().toString();
            this->nAlbaran = registro.field("nAlbaran").value().toInt();
            this->nPedido = registro.field("nPedido").value().toInt();
            this->id_FormaPago = registro.field("id_FormaPago").value().toInt();
            this->cCodigoFormaPago = registro.field("cCodigoFormaPago").value().toString();
            this->cDescripcionFormaPago = registro.field("cDescripcionFormaPago").value().toString();
            this->tLugarEntrega = registro.field("tLugarEntrega").value().toString();
            this->cAtencionde = registro.field("cAtencionde").value().toString();
            this->rBase1 = registro.field("rImporte1").value().toDouble();
            this->rBase2 = registro.field("rImporte2").value().toDouble();
            this->rBase3 = registro.field("rImporte3").value().toDouble();
            this->rBase4 = registro.field("rImporte4").value().toDouble();
            this->nIva1 = registro.field("nIva1").value().toDouble();
            this->nIva2 = registro.field("nIva2").value().toDouble();
            this->nIva3 = registro.field("nIva3").value().toDouble();
            this->nIva4 = registro.field("nIva4").value().toDouble();
            this->rIva1 = registro.field("rIva1").value().toDouble();
            this->rIva2 = registro.field("rIva2").value().toDouble();
            this->rIva3 = registro.field("rIva3").value().toDouble();
            this->rIva4 = registro.field("rIva4").value().toDouble();
            this->rTotalIva = registro.field("rTotalIva").value().toDouble();
            this->nRecargoEquivalencia1 = registro.field("nRecargoEquivalencia1").value().toDouble();
            this->nRecargoEquivalencia2 = registro.field("nRecargoEquivalencia2").value().toDouble();
            this->nRecargoEquivalencia3 = registro.field("nRecargoEquivalencia3").value().toDouble();
            this->nRecargoEquivalencia4 = registro.field("nRecargoEquivalencia4").value().toDouble();
            this->rRec1 = registro.field("rRec1").value().toDouble();
            this->rRec2 = registro.field("rRec2").value().toDouble();
            this->rRec3 = registro.field("rRec3").value().toDouble();
            this->rRec4 = registro.field("rRec4").value().toDouble();
            this->rTotalRec = registro.field("rTotalRec").value().toDouble();
            this->rTotal1 = registro.field("rTotal1").value().toDouble();
            this->rTotal2 = registro.field("rTotal2").value().toDouble();
            this->rTotal3 = registro.field("rTotal3").value().toDouble();
            this->rTotal4 = registro.field("rTotal4").value().toDouble();
            this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toBool();
        } else {
            QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Presupuestos"),
                                     QObject::tr("No se ha podido recuperar el presupuesto. Error: ")+
                                                 qCab_pre.lastError().text(),QObject::tr("Aceptar"));
        }
   }

}

void Presupuesto::RecuperarPresupuesto(QString cSQL, int nAccion)
{
    QSqlQuery qCab_pre(QSqlDatabase::database("empresa"));
    qCab_pre.prepare(cSQL);
    if( !qCab_pre.exec() ) {
        QMessageBox::critical(qApp->activeWindow(), "error:", qCab_pre.lastError().text());
    } else {
        if (qCab_pre.next()) {
            QSqlRecord registro = qCab_pre.record();
            this->id = registro.field("Id").value().toInt();
            this->nPresupuesto = registro.field("nPresupuesto").value().toInt();
            this->dFecha = registro.field("dFecha").value().toDate();
            this->dValidoHasta = registro.field("dValidoHasta").value().toDate();
            this->id_cliente = registro.field("id_cliente").value().toInt();
            this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
            this->cCliente = registro.field("cCliente").value().toString();
            this->cCif = registro.field("cCif").value().toString();
            this->cDireccion = registro.field("cDireccion").value().toString();
            this->cDireccion2 = registro.field("cDireccion2").value().toString();
            this->cCP = registro.field("cCP").value().toString();
            this->cPoblacion = registro.field("cPoblacion").value().toString();

            this->cProvincia = registro.field("cProvincia").value().toString();
            //TODO get pais id
            //this->cPais = registro.field("cPais").value().toString();
            this->cTelefono = registro.field("cTelefono").value().toString();
            this->cMovil = registro.field("cMovil").value().toString();
            this->cFax = registro.field("cFax").value().toString();
            this->cEmail = registro.field("cEmail").value().toString();
            this->nDto = registro.field("nDto").value().toDouble();
            this->tComentarios = registro.field("tComentarios").value().toString();
            this->rImporte = registro.field("rImporte").value().toDouble();
            this->rSubTotal = registro.field("rSubtotal").value().toDouble();
            this->rDescuento = registro.field("rDescuento").value().toDouble();
            this->rTotal = registro.field("rTotal").value().toDouble();
            this->lImpreso = registro.field("lImpreso").value().toBool();
            this->lAprobado = registro.field("lAprobado").value().toBool();
            this->dFechaAprobacion = registro.field("dFechaAprobacion").value().toDate();
            this->rImporteFactura = registro.field("rImporteFactura").value().toDouble();
            this->cFactura = registro.field("cFactura").value().toString();
            this->nAlbaran = registro.field("nAlbaran").value().toInt();
            this->nPedido = registro.field("nPedido").value().toInt();
            this->id_FormaPago = registro.field("id_FormaPago").value().toInt();
            this->cCodigoFormaPago = registro.field("cCodigoFormaPago").value().toString();
            this->cDescripcionFormaPago = registro.field("cDescripcionFormaPago").value().toString();
            this->tLugarEntrega = registro.field("tLugarEntrega").value().toString();
            this->cAtencionde = registro.field("cAtencionde").value().toString();
            this->rBase1 = registro.field("rImporte1").value().toDouble();
            this->rBase2 = registro.field("rImporte2").value().toDouble();
            this->rBase3 = registro.field("rImporte3").value().toDouble();
            this->rBase4 = registro.field("rImporte4").value().toDouble();
            this->nIva1 = registro.field("nIva1").value().toDouble();
            this->nIva2 = registro.field("nIva2").value().toDouble();
            this->nIva3 = registro.field("nIva3").value().toDouble();
            this->nIva4 = registro.field("nIva4").value().toDouble();
            this->rIva1 = registro.field("rIva1").value().toDouble();
            this->rIva2 = registro.field("rIva2").value().toDouble();
            this->rIva3 = registro.field("rIva3").value().toDouble();
            this->rIva4 = registro.field("rIva4").value().toDouble();
            this->rTotalIva = registro.field("rTotalIva").value().toDouble();
            this->nRecargoEquivalencia1 = registro.field("nRecargoEquivalencia1").value().toDouble();
            this->nRecargoEquivalencia2 = registro.field("nRecargoEquivalencia2").value().toDouble();
            this->nRecargoEquivalencia3 = registro.field("nRecargoEquivalencia3").value().toDouble();
            this->nRecargoEquivalencia4 = registro.field("nRecargoEquivalencia4").value().toDouble();
            this->rRec1 = registro.field("rRec1").value().toDouble();
            this->rRec2 = registro.field("rRec2").value().toDouble();
            this->rRec3 = registro.field("rRec3").value().toDouble();
            this->rRec4 = registro.field("rRec4").value().toDouble();
            this->rTotalRec = registro.field("rTotalRec").value().toDouble();
            this->rTotal1 = registro.field("rTotal1").value().toDouble();
            this->rTotal2 = registro.field("rTotal2").value().toDouble();
            this->rTotal3 = registro.field("rTotal3").value().toDouble();
            this->rTotal4 = registro.field("rTotal4").value().toDouble();
            this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toBool();
        } else {
            if(nAccion == 0)
                QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Presupuestos"),
                                         QObject::tr("No hay más presupuestos, se encuentra al final del fichero"),QObject::tr("Aceptar"));
            else
                QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Presupuestos"),
                                         QObject::tr("No hay más presupuestos, se encuentra en el inicio del fichero"),QObject::tr("Aceptar"));
        }
   }

}

void Presupuesto::GuardarPres(int nId_Presupuesto)
{
    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));

    if (this->nPresupuesto == 0)
        this->nPresupuesto = NuevoNumeroPresupuesto();



    cab_pre.prepare( "UPDATE cab_pre SET "
                     "cAtencionde = :cAtencionde,"
                     "cCif = :cCif,"
                     "cCliente = :cCliente,"
                     "cCodigoCliente = :cCodigoCliente,"
                     "cCodigoFormaPago = :cCodigoFormaPago,"
                     "cCP = :cCP,"
                     "cDescripcionFormaPago = :cDescripcionFormaPago,"
                     "cDireccion = :cDireccion,"
                     "cDireccion2 = :cDireccion2,"
                     "cEmail = :cEmail,"
                     "cFactura = :cFactura,"
                     "cFax = :cFax,"
                     "cMovil = :cMovil,"
                     "cPais = :cPais,"
                     "cPoblacion = :cPoblacion,"
                     "cProvincia = :cProvincia,"
                     "cTelefono = :cTelefono,"
                     "dFecha = :dFecha,"
                     "dFechaAprobacion = :dFechaAprobacion,"
                     "dValidoHasta = :dValidoHasta,"
                     "id_Cliente = :id_Cliente,"
                     "id_FormaPago = :id_FormaPago,"
                     "lAprobado = :lAprobado,"
                     "lImpreso = :lImpreso,"
                     "lRecargoEquivalencia = :lRecargoEquivalencia,"
                     "nAlbaran = :nAlbaran,"
                     "nDto = :nDto,"
                     "nIva1 = :nIva1,"
                     "nIva2 = :nIva2,"
                     "nIva3 = :nIva3,"
                     "nIva4 = :nIva4,"
                     "nPedido = :nPedido,"
                     "nPresupuesto = :nPresupuesto,"
                     "nRecargoEquivalencia1 = :nRecargoEquivalencia1,"
                     "nRecargoEquivalencia2 = :nRecargoEquivalencia2,"
                     "nRecargoEquivalencia4 = :nRecargoEquivalencia4,"
                     "nRecargoEquivalencia3 = :nRecargoEquivalencia3,"
                     "rDescuento = :rDescuento,"
                     "rImporte = :rImporte,"
                     "rImporte1 = :rImporte1,"
                     "rImporte2 = :rImporte2,"
                     "rImporte3 = :rImporte3,"
                     "rImporte4 = :rImporte4,"
                     "rImporteFactura = :rImporteFactura,"
                     "rImportePendiente = :rImportePendiente,"
                     "rIva1 = :rIva1,"
                     "rIva2 = :rIva2,"
                     "rIva3 = :rIva3,"
                     "rIva4 = :rIva4,"
                     "rRec1 = :rRec1,"
                     "rRec2 = :rRec2,"
                     "rRec3 = :rRec3,"
                     "rRec4 = :rRec4,"
                     "rSubtotal = :rSubtotal,"
                     "rTotal = :rTotal,"
                     "rTotal1 = :rTotal1,"
                     "rTotal2 = :rTotal2,"
                     "rTotal3 = :rTotal3,"
                     "rTotal4 = :rTotal4,"
                     "rTotalIVA = :rTotalIVA,"
                     "rTotalRec = :rTotalRec,"
                     "tComentarios = :tComentarios,"
                     "tLugarEntrega = :tLugarEntrega "
                     "WHERE id = :nId");




    // Pasamos valores reales a la Select

         cab_pre.bindValue(":cAtencionde",this->cAtencionde);
         cab_pre.bindValue(":cCif",this->cCif);
         cab_pre.bindValue(":cCliente", this->cCliente);
         cab_pre.bindValue(":cCodigoCliente",this->cCodigoCliente);
         cab_pre.bindValue(":cCodigoFormaPago",this->cCodigoFormaPago);
         cab_pre.bindValue(":cCP",this->cCP);
         cab_pre.bindValue(":cDescripcionFormaPago",this->cDescripcionFormaPago);
         cab_pre.bindValue(":cDireccion",this->cDireccion);
         cab_pre.bindValue(":cDireccion2",this->cDireccion2);
         cab_pre.bindValue(":cEmail",this->cEmail);
         cab_pre.bindValue(":cFactura",this->cFactura);
         cab_pre.bindValue("c:cFax",this->cFax);
         cab_pre.bindValue(":cMovil",this->cMovil);
         //TODO fix pais
         //cab_pre.bindValue(":cPais",this->cPais);
         cab_pre.bindValue(":cPoblacion",this->cPoblacion);
         cab_pre.bindValue(":cProvincia",this->cProvincia);
         cab_pre.bindValue(":cTelefono",this->cTelefono);
         cab_pre.bindValue(":dFecha",this->dFecha);
         cab_pre.bindValue(":dFechaAprobacion",this->dFechaAprobacion);
         cab_pre.bindValue(":dValidoHasta",this->dValidoHasta);
         cab_pre.bindValue(":id_Cliente",this->id_cliente);
         cab_pre.bindValue(":id_FormaPago",this->id_FormaPago);
         if (this->lAprobado)
            cab_pre.bindValue(":lAprobado",1);
         else
             cab_pre.bindValue(":lAprovado",0);
         if(this->lImpreso)
            cab_pre.bindValue(":lImpreso",1);
         else
             cab_pre.bindValue(":lImpreso",0);
         if (this->lRecargoEquivalencia)
            cab_pre.bindValue(":lRecargoEquivalencia",1);
         else
             cab_pre.bindValue(":lRecargoEquivalencia",0);

         cab_pre.bindValue(":nAlbaran",this->nAlbaran);
         cab_pre.bindValue(":nDto",this->nDto);
         cab_pre.bindValue(":nIva1",this->nIva1);
         cab_pre.bindValue(":nIva2",this->nIva2);
         cab_pre.bindValue(":nIva3",this->nIva3);
         cab_pre.bindValue(":nIva4",this->nIva4);
         cab_pre.bindValue(":nPedido",this->nPedido);
         cab_pre.bindValue(":nPresupuesto",this->nPresupuesto);
         cab_pre.bindValue(":nRecargoEquivalencia1",this->nRecargoEquivalencia1);
         cab_pre.bindValue(":nRecargoEquivalencia2",this->nRecargoEquivalencia2);
         cab_pre.bindValue(":nRecargoEquivalencia4",this->nRecargoEquivalencia4);
         cab_pre.bindValue(":nRecargoEquivalencia3",this->nRecargoEquivalencia3);
         cab_pre.bindValue(":rDescuento",this->rDescuento);
         cab_pre.bindValue(":rImporte",this->rImporte);
         cab_pre.bindValue(":rImporte1",this->rBase1);
         cab_pre.bindValue(":rImporte2",this->rBase2);
         cab_pre.bindValue(":rImporte3",this->rBase3);
         cab_pre.bindValue(":rImporte4",this->rBase4);
         cab_pre.bindValue(":rImporteFactura",this->rImporteFactura);
         cab_pre.bindValue(":rImportePendiente",this->rImportePendiente);
         cab_pre.bindValue(":rIva1",this->rIva1);
         cab_pre.bindValue(":rIva2",this->rIva2);
         cab_pre.bindValue(":rIva3",this->rIva3);
         cab_pre.bindValue(":rIva4",this->rIva4);
         cab_pre.bindValue(":rRec1",this->rRec1);
         cab_pre.bindValue(":rRec2",this->rRec2);
         cab_pre.bindValue(":rRec3",this->rRec3);
         cab_pre.bindValue(":rRec4",this->rRec4);
         cab_pre.bindValue(":rSubtotal",this->rSubTotal);
         cab_pre.bindValue(":rTotal",this->rTotal);
         cab_pre.bindValue(":rTotal1",this->rTotal1);
         cab_pre.bindValue(":rTotal2",this->rTotal2);
         cab_pre.bindValue(":rTotal3",this->rTotal3);
         cab_pre.bindValue(":rTotal4",this->rTotal4);
         cab_pre.bindValue(":rTotalIVA",this->rTotalIva);
         cab_pre.bindValue(":rTotalRec",this->rTotalRec);
         cab_pre.bindValue(":tComentarios",this->tComentarios);
         cab_pre.bindValue(":tLugarEntrega", this->tLugarEntrega);
         cab_pre.bindValue(":nId",this->id);

    if(!cab_pre.exec()){
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("error al guardar datos Presupuesto:"), cab_pre.lastError().text());
        qDebug() << cab_pre.lastQuery();
    } else {
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Guardar datos"),QObject::tr("El Presupuesto se ha guardado correctamente:"),
                                 QObject::tr("Ok"));
        QString cSQL = "Select * from cab_pre where Id ="+QString::number(nId_Presupuesto);
        RecuperarPresupuesto(cSQL);
    }
}

int Presupuesto::NuevoNumeroPresupuesto()
{
    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    int nPresupuesto;
    cab_pre.prepare("Select nPresupuesto from cab_pre order by nPresupuesto desc limit 1");
    if(cab_pre.exec()) {
        cab_pre.next();
        nPresupuesto= cab_pre.value(0).toInt();
        nPresupuesto ++;
    } else {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_pre.lastError().text());
    }
    return nPresupuesto;

}


void Presupuesto::AnadirLineaPresupuesto(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
    QSqlDatabase::database("empresa").transaction();
    bool lCorrecto;
    lCorrecto = true;
    QSqlQuery *Qlin_pre = new QSqlQuery(QSqlDatabase::database("empresa"));
    Qlin_pre->prepare("INSERT INTO lin_pre (Id_Cab,cCodigo,nCantidad,cDescripcion,rPvp,nPorcDto,rDto,rSubTotal,rTotal,nPorcIVA)"
                      " VALUES(:id_cab,:cCodigo,:nCantidad,:cDescripcion,:rPvp,:nDto,:rDto,:rSubTotal,:rTotal,:nPorcIva)");


    Qlin_pre->bindValue(":id_cab",id_cab);
    Qlin_pre->bindValue(":cCodigo",cCodigo);
    Qlin_pre->bindValue(":nCantidad",nCantidad);
    Qlin_pre->bindValue(":cDescripcion",cDescripcion);
    Qlin_pre->bindValue(":rPvp",pvp);
    Qlin_pre->bindValue(":nDto",porcdto);
    Qlin_pre->bindValue(":rDto",dto);
    Qlin_pre->bindValue(":rSubTotal",subtotal);
    Qlin_pre->bindValue(":rTotal",total);
    Qlin_pre->bindValue(":nPorcIva",nPorcIva);
    if (!Qlin_pre->exec()){
       QMessageBox::critical(qApp->activeWindow(),QObject::tr("error al guardar datos línea Presupuesto:"), Qlin_pre->lastError().text());
       lCorrecto = false;
    }
    delete Qlin_pre;
    QSqlQuery *QArticulos = new QSqlQuery(QSqlDatabase::database("empresa"));
    QArticulos->prepare("update articulos set "
                        "dUltimaVenta = :dUltimaVenta,"
                        "nUnidadesVendidas = nUnidadesVendidas +:nCantidad,"
                        "nStockReal = nStockReal - :nCantidad2, "
                        "rAcumuladoVentas = rAcumuladoVentas + :rTotal "
                        "where cCodigo= :cCodigo");
    QArticulos->bindValue(":dUltimaVenta",QDate::currentDate());
    QArticulos->bindValue(":nCantidad",nCantidad);
    QArticulos->bindValue(":nCantidad2",nCantidad);
    QArticulos->bindValue(":rTotal",total);
    QArticulos->bindValue(":cCodigo",cCodigo);

    if (!QArticulos->exec()) {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Presupuestos"),QObject::tr("No se puede actualizar la ficha del artículo")+
                             QArticulos->lastError().text(),QObject::tr("Aceptar"));
        lCorrecto = false;
    }
    if(lCorrecto) {
        QSqlDatabase::database("empresa").commit();
    } else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Presupuestos"),QObject::tr("Se desharán los últimos cambios"));
        QSqlDatabase::database("empresa").rollback();
    }
}

void Presupuesto::ModificarLineaPresupuesto(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp,
                                            double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
    // Actualizo Línea Pedido
    QSqlQuery *Qlin_pre = new QSqlQuery(QSqlDatabase::database("empresa"));

    Qlin_pre->prepare("update lin_pre set "
                      "cCodigo = :cCodigo,"
                      "cDescripcion = :cDescripcion,"
                      "nCantidad = :nCantidad,"
                      "nPorcDto = :nPorcDto,"
                      "nPorcIVA = :nPorcIVA,"
                      "rDto =:rDto,"
                      "rPvp = :rPvp,"
                      "rSubTotal = :rSubTotal,"
                      "rTotal = :rTotal where id = :id_lin");
    Qlin_pre->bindValue(":cCodigo",cCodigo);
    Qlin_pre->bindValue(":cDescripcion",cDescripcion);
    Qlin_pre->bindValue(":nCantidad",nCantidad);
    Qlin_pre->bindValue(":nPorcDto",porcdto);
    Qlin_pre->bindValue(":nPorcIVA",nPorcIva);
    Qlin_pre->bindValue(":rDto",dto);
    Qlin_pre->bindValue(":rPvp",pvp);
    Qlin_pre->bindValue(":rSubTotal",subtotal);
    Qlin_pre->bindValue(":rTotal", total);
    Qlin_pre->bindValue(":id_lin",id_lin);
    if (!Qlin_pre->exec()){
       QMessageBox::critical(qApp->activeWindow(),"error al modificar datos línea Pedido:", Qlin_pre->lastError().text());
    }
    delete Qlin_pre;

}

void Presupuesto::BorrarLineaPresupuesto(int id_lin)
{
    if (id_lin !=0)
    {
        if(QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar línea"),
                                 qApp->tr("Está a punto de borrar la línea del Presupuesto\n¿Desea continuar?"),
                                 qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted)
        {
            QSqlQuery qrylin_pre(QSqlDatabase::database("empresa"));
            qrylin_pre.prepare("Delete from lin_pre where id = :id_lin");
            qrylin_pre.bindValue(":id_lin",id_lin);
            if(!qrylin_pre.exec())
            {
                QMessageBox::critical(qApp->activeWindow(),QObject::tr("Borrar línea"),QObject::tr("Falló el borrado de la línea de Presupuesto"),
                                      QObject::tr("&Aceptar"));
            }
            calcularPresupuesto();
        }
    }
    else
    {
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("Borrar línea"),QObject::tr("Debe seleccionar una línea para poder borrar"),
                              QObject::tr("OK"));
    }
}

void Presupuesto::calcularPresupuesto()
{
    // Reseteo valores
    this->rSubTotal = 0;
    this->rDescuento =0;
    this->rImporte = 0;
    this->rTotalIva = 0;
    this->rTotalRec = 0;
    this->rTotal = 0;
    this->rBase1 =0;
    this->rBase2 =0;
    this->rBase3 = 0;
    this->rBase4 = 0;
    this->rIva1 = 0;
    this->rIva2 = 0;
    this->rIva3 = 0;
    this->rIva4 = 0;
    this->rRec1 = 0;
    this->rRec2 = 0;
    this->rRec3 = 0;
    this->rRec4 = 0;
    this->rTotal1 =0;
    this->rTotal2 =0;
    this->rTotal3 =0;
    this->rTotal4 = 0;

    QSqlQuery *Qlin_pre = new QSqlQuery(QSqlDatabase::database("empresa"));
    Qlin_pre->prepare("Select * from lin_pre where id_cab = :nId");
    Qlin_pre->bindValue(":nId",this->id);
    if (Qlin_pre->exec()) {
        QSqlRecord record = Qlin_pre->record();
        while (Qlin_pre->next()) {
            record = Qlin_pre->record();

            this->rSubTotal = this->rSubTotal + record.field("rSubTotal").value().toDouble();
            this->rDescuento = this->rDescuento + record.field("rDto").value().toDouble();
            this->rImporte = (this->rSubTotal - this->rDescuento);


            // IVA 1
            if (record.field("nPorcIva").value().toDouble() == this->nIva1) {
                this->rBase1 = this->rBase1 + record.field("rTotal").value().toDouble();
                // RE 1
                if(this->lRecargoEquivalencia == 1)
                    this->rRec1 = (this->rBase1 * this->nRecargoEquivalencia1)/100;

                this->rIva1 = (this->rBase1 * this->nIva1)/100;
                this->rTotal1 = this->rBase1 + this->rIva1 + this->rRec1;
            }
            // IVA 2
            if (record.field("nPorcIva").value().toDouble() == this->nIva2) {
                this->rBase2 = this->rBase2 + record.field("rTotal").value().toDouble();
                // RE 2
                if(this->lRecargoEquivalencia == 1)
                    this->rRec2 = (this->rBase2 * this->nRecargoEquivalencia2)/100;
                this->rIva2 = (this->rBase2 * this->nIva2)/100;
                this->rTotal2 = this->rBase2 + this->rIva2 + this->rRec2;
            }
            // IVA 3
            if (record.field("nPorcIva").value().toDouble() == this->nIva3) {
                this->rBase3 = this->rBase3 + record.field("rTotal").value().toDouble();
                // RE 3
                if(this->lRecargoEquivalencia == 1)
                    this->rRec3 = (this->rBase3 * this->nRecargoEquivalencia3)/100;

                this->rIva3 = (this->rBase3 * this->nIva3)/100;
                this->rTotal3 = this->rBase3 + this->rIva3 + this->rRec3;
            }
            // IVA 4
            if (record.field("nPorcIva").value().toDouble() == this->nIva4) {
                this->rBase4 = this->rBase4 + record.field("rTotal").value().toDouble();

                // RE 4
                if(this->lRecargoEquivalencia == 1)
                    this->rRec4 = (this->rBase4 * this->nRecargoEquivalencia4)/100;

                this->rIva4 = (this->rBase4 * this->nIva4)/100;
                this->rTotal4 = this->rBase4 + this->rIva4 + this->rRec4;

            }
            this->rTotalIva =  (this->rIva1 +  this->rIva2 + this->rIva3 + this->rIva4);
            this->rTotalRec = (this->rRec1 + this->rRec2 + this->rRec3 + this->rRec4);
            this->rTotal = this->rImporte + this->rTotalIva + this->rTotalRec;
        }
    }
}

void Presupuesto::PresupuestoToPedido()
{
}

void Presupuesto::PresupuestoToAlbaran()
{
}

void Presupuesto::PresupuestoToFactura()
{
}
