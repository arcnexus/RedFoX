#include "facturasproveedor.h"

FacturasProveedor::FacturasProveedor(QObject *parent) :
    QObject(parent)
{
}
int FacturasProveedor::anadir_factura()
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    if(queryFactura.exec("insert into fac_pro (Id_Proveedor) values (0)"))
    {
        this->id = queryFactura.lastInsertId().toInt();
        return this->id;
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Facturas a proveedor"),
                             tr("No se pudo crear la nueva factura : %1").arg(queryFactura.lastError().text()),
                             tr("Aceptar"));
        return -1;
    }
}

void FacturasProveedor::guardar_factura()
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    queryFactura.prepare("UPDATE fac_pro SET "
                         "cFactura =:cFactura,"
                         "dFecha =:dFecha,"
                         "dRecepcion =:dRecepcion,"
                         "cPedido =:cPedido,"
                         "Id_Proveedor =:Id_Proveedor,"
                         "cProveedor =:cProveedor,"
                         "cCifProveedor =:cCifProveedor,"
                         "lRetencionIRPF =:lRetencionIRPF,"
                         "lRecargoEquivalencia =:lRecargoEquivalencia,"
                         "rBase1 =:rBase1,"
                         "rBase2 =:rBase2,"
                         "rBase3 =:rBase3,"
                         "rBase4 =:rBase4,"
                         "nPorcIva1 =:nPorcIva1,"
                         "nPorcIva2 =:nPorcIva2,"
                         "nPorcIva3 =:nPorcIva3,"
                         "nPorcIva4 =:nPorcIva4,"
                         "rIVA1 =:rIVA1,"
                         "rIVA2 =:rIVA2,"
                         "rIVA3 =:rIVA3,"
                         "rIVA4 =:rIVA4,"
                         "nREC1 =:nREC1,"
                         "nREC2 =:nREC2,"
                         "nREC3 =:nREC3,"
                         "nREC4 =:nREC4,"
                         "rREC1 =:rREC1,"
                         "rREC2 =:rREC2,"
                         "rREC3 =:rREC3,"
                         "rREC4 =:rREC4,"
                         "rTotal1 =:rTotal1,"
                         "rTotal2 =:rTotal2,"
                         "rTotal3 =:rTotal3,"
                         "rTotal4 =:rTotal4,"
                         "rTotalBase =:rTotalBase,"
                         "rTotalIVA =:rTotalIVA,"
                         "rTotalRetencion =:rTotalRetencion,"
                         "rTotalRecargoEq =:rTotalRecargoEq,"
                         "rTotal =:rTotal,"
                         "cFormaPago =:cFormaPago,"
                         "rImportePagadoTotal =:rImportePagadoTotal,"
                         "Id_TipoGasto =:Id_TipoGasto,"
                         "tComentario =:tComentario,"
                         "lPagado =:lPagado,"
                         "rImporteDeudaPendiente =:rImporteDeudaPendiente,"
                         "cCAlbaran =:cCAlbaran "
                         "WHERE id =:id");

    queryFactura.bindValue(":cFactura",this->cFactura);
    queryFactura.bindValue(":dFecha",this->dFecha);
    queryFactura.bindValue(":dRecepcion",this->dRecepcion);
    queryFactura.bindValue(":cPedido",this->cPedido);
    queryFactura.bindValue(":Id_Proveedor",this->Id_Proveedor);
    queryFactura.bindValue(":cProveedor",this->cProveedor);
    queryFactura.bindValue(":cCifProveedor",this->cCifProveedor);
    queryFactura.bindValue(":lRetencionIRPF",this->lRetencionIRPF);
    queryFactura.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
    queryFactura.bindValue(":rBase1",this->rBase1);
    queryFactura.bindValue(":rBase2",this->rBase2);
    queryFactura.bindValue(":rBase3",this->rBase3);
    queryFactura.bindValue(":rBase4",this->rBase4);
    queryFactura.bindValue(":nPorcIva1",this->nPorcIva1);
    queryFactura.bindValue(":nPorcIva2",this->nPorcIva2);
    queryFactura.bindValue(":nPorcIva3",this->nPorcIva3);
    queryFactura.bindValue(":nPorcIva4",this->nPorcIva4);
    queryFactura.bindValue(":rIVA1",this->rIVA1);
    queryFactura.bindValue(":rIVA2",this->rIVA2);
    queryFactura.bindValue(":rIVA3",this->rIVA3);
    queryFactura.bindValue(":rIVA4",this->rIVA4);
    queryFactura.bindValue(":nREC1",this->nREC1);
    queryFactura.bindValue(":nREC2",this->nREC2);
    queryFactura.bindValue(":nREC3",this->nREC3);
    queryFactura.bindValue(":nREC4",this->nREC4);
    queryFactura.bindValue(":rREC1",this->rREC1);
    queryFactura.bindValue(":rREC2",this->rREC2);
    queryFactura.bindValue(":rREC3",this->rREC3);
    queryFactura.bindValue(":rREC4",this->rREC4);
    queryFactura.bindValue(":rTotal1",this->rTotal1);
    queryFactura.bindValue(":rTotal2",this->rTotal2);
    queryFactura.bindValue(":rTotal3",this->rTotal3);
    queryFactura.bindValue(":rTotal4",this->rTotal4);
    queryFactura.bindValue(":rTotalBase",this->rTotalBase);
    queryFactura.bindValue(":rTotalIVA",this->rTotalIVA);
    queryFactura.bindValue(":rTotalRetencion",this->rTotalRetencion);
    queryFactura.bindValue(":rTotalRecargoEq",this->rTotalRecargoEq);
    queryFactura.bindValue(":rTotal",this->rTotal);
    queryFactura.bindValue(":cFormaPago",this->cFormaPago);
    queryFactura.bindValue(":rImportePagadoTotal",this->rImportePagadoTotal);
    queryFactura.bindValue(":Id_TipoGasto",this->Id_TipoGasto);
    queryFactura.bindValue(":tComentario",this->tComentario);
    queryFactura.bindValue(":lPagado",this->lPagado);
    queryFactura.bindValue(":rImporteDeudaPendiente",this->rImporteDeudaPendiente);
    queryFactura.bindValue(":cCAlbaran",this->cCAlbaran);
    queryFactura.bindValue(":id",this->id);

    if(queryFactura.exec())
    {
        // TODO - acumulados + contabilidad
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Facturas a proveedores"),
                             tr("Falló la modificación de la factura: %1").arg(queryFactura.lastError().text()));
    }

}

QSqlQuery FacturasProveedor::recuperar_factura(QString cSQL, int accion)
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    if(queryFactura.exec(cSQL))
        cargar_factura(queryFactura,accion);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
}

QSqlQuery FacturasProveedor::recuperar_factura(int id_factura)
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    queryFactura.prepare("select * from fac_pro where id = :id");
    queryFactura.bindValue(":id",id_factura);
    if(queryFactura.exec())
            cargar_factura(queryFactura,0);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
}

void FacturasProveedor::cargar_factura(QSqlQuery queryFact,int accion)
{
  // accion == 0 - nada : 1 - Anterior : 2 - Siguiente
    if(queryFact.next())
    {
        this->cFactura = queryFact.record().value("cFactura").toString();
        this->dFecha = queryFact.record().value("dFecha").toDate();
        this->dRecepcion = queryFact.record().value("dRecepcion").toDate();
        this->cPedido = queryFact.record().value("cPedido").toString();
        this->Id_Proveedor = queryFact.record().value("Id_Provedor").toInt();
        this->cProveedor = queryFact.record().value("cProveedor").toString();
        this->cCifProveedor = queryFact.record().value("cCifProveedor").toString();
        this->lRetencionIRPF = queryFact.record().value("lRetencionIRPF").toBool();
        this->lRecargoEquivalencia = queryFact.record().value("lRecargoEquivalencia").toBool();
        this->rBase1 = queryFact.record().value("rBase1").toDouble();
        this->rBase2 = queryFact.record().value("rBase2").toDouble();
        this->rBase3 = queryFact.record().value("rBase3").toDouble();
        this->rBase4 = queryFact.record().value("rBase4").toDouble();
        this->nPorcIva1 = queryFact.record().value("nPorcIva1").toDouble();
        this->nPorcIva2 = queryFact.record().value("nPorcIva2").toDouble();
        this->nPorcIva3 = queryFact.record().value("nPorcIva3").toDouble();
        this->nPorcIva4 = queryFact.record().value("nPorcIva4").toDouble();
        this->rIVA1 = queryFact.record().value("rIVA1").toDouble();
        this->rIVA2 = queryFact.record().value("rIVA2").toDouble();
        this->rIVA3 = queryFact.record().value("rIVA3").toDouble();
        this->rIVA4 = queryFact.record().value("rIVA4").toDouble();
        this->nREC1 = queryFact.record().value("nREC1").toDouble();
        this->nREC2 = queryFact.record().value("nREC2").toDouble();
        this->nREC3 = queryFact.record().value("nREC3").toDouble();
        this->nREC4 = queryFact.record().value("nREC4").toDouble();
        this->rREC1 = queryFact.record().value("rREC1").toDouble();
        this->rREC2 = queryFact.record().value("rREC2").toDouble();
        this->rREC3 = queryFact.record().value("rREC3").toDouble();
        this->rREC4 = queryFact.record().value("rREC4").toDouble();
        this->rTotal1 = queryFact.record().value("rTotal1").toDouble();
        this->rTotal2 = queryFact.record().value("rTotal2").toDouble();
        this->rTotal3 = queryFact.record().value("rTotal3").toDouble();
        this->rTotal4 = queryFact.record().value("rTotal4").toDouble();
        this->rTotalBase = queryFact.record().value("rTotalBase").toDouble();
        this->rTotalIVA = queryFact.record().value("rTotalIVA").toDouble();
        this->rTotalRetencion = queryFact.record().value("rTotalRetencion").toDouble();
        this->rTotalRecargoEq = queryFact.record().value("rTotalRecargoEq").toDouble();
        this->rTotal = queryFact.record().value("rTotal").toDouble();
        this->cFormaPago = queryFact.record().value("cFormaPago").toDouble();
        this->rImportePagadoTotal = queryFact.record().value("rImportePagadoTotal").toDouble();
        this->Id_TipoGasto = queryFact.record().value("Id_TipoGasto").toDouble();
        this->tComentario = queryFact.record().value("tComentario").toString();
        this->lPagado = queryFact.record().value("lPagado").toBool();
        this->rImporteDeudaPendiente = queryFact.record().value("rImporteDeudaPendiente").toDouble();
        this->cCAlbaran = queryFact.record().value("cCAlbaran").toString();
        this->id = queryFact.record().value("id").toInt();

    } else
    {
        if(accion == 1)
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al inicio del fichero");
        if(accion == 2)
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al final del fichero");
    }
}

