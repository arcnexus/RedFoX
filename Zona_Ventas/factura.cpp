#include "factura.h"

Factura::Factura(QObject *parent) :
    QObject(parent)
{
    this->id =0;
}

// Metodos utilidad Clase
bool Factura::AnadirFactura()
{
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    this->nPorcentajeIVA1 = Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble();
    this->nPorcentajeIVA2 = Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble();
    this->nPorcentajeIVA3 = Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble();
    this->nPorcentajeIVA4 = Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble();

    this->lRecargoEquivalencia = 0;
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
     cab_fac.prepare("INSERT INTO cab_fac (cCodigoCliente,cFactura,dFecha,dFechaCobro,iId_Cliente,cCliente,cDireccion,cDireccion2,"
                   "cCp,cPoblacion,cProvincia,idPais,cCif,lRecargoEquivalencia,rSubtotal,nDto,nDtoPP,rImporteDescuento,rImporteDescuentoPP,"
                   "rBase,nIva,rImporteIva,rTotal,lImpresa,lCobrada,lContabilizada,id_FormaPago,cFormaPago,tComentario,"
                   "rBase1,rBase2,rBase3,rBase4,nPorcentajeIVA1,nPorcentajeIVA2,nPorcentajeIVA3,nPorcentajeIVA4,rIVA1,rIVA2,rIVA3,rIVA4,"
                   "rTotal1,rTotal2,rTotal3,rTotal4,nRec1,nRec2,nRec3,nRec4,rRecargoEq1,rRecargoEq2,rRecargoEq3,rRecargoEq4,"
                   "rTotalRecargoEq,rEntregadoaCuenta,rImportePendiente,cCodigoEntidad,cOficinaEntidad,cDCCuenta,cNumeroCuenta,cPedidoCliente)"
                   " VALUES (:cCodigoCliente,:cFactura,:dFecha,:dFechaCobro,:iId_Cliente,:cCliente,:cDireccion,:cDireccion2,"
                   ":cCp,:cPoblacion,:cProvincia,:cPais,:cCif,:lRecargoEquivalencia,:rSubtotal,:nDto,:nDtoPP,:rImporteDescuento,:rImporteDescuentoPP,"
                   ":rBase,:nIva,:rImporteIva,:rTotal,:lImpresa,:lCobrada,:lContabilizada,:id_FormaPago,:cFormaPago,:tComentario,"
                   ":rBase1,:rBase2,:rBase3,:rBase4,:nPorcentajeIVA1,:nPorcentajeIVA2,:nPorcentajeIVA3,:nPorcentajeIVA4,:rIVA1,:rIVA2,:rIVA3,:rIVA4,"
                   ":rTotal1,:rTotal2,:rTotal3,:rTotal4,:nRec1,:nRec2,:nRec3,:nRec4,:rRecargoEq1,:rRecargoEq2,:rRecargoEq3,:rRecargoEq4,"
                   ":rTotalRecargoEq,:rEntregadoaCuenta,:rImportePendiente,:cCodigoEntidad,:cOficinaEntidad,:cDCCuenta,:cNumeroCuenta,:cPedidoCliente)");

     cab_fac.bindValue(":cCodigoCliente",this->cCodigoCliente);
     cab_fac.bindValue(":cFactura",this->cFactura);
     cab_fac.bindValue(":dFecha",this->dFecha);
     cab_fac.bindValue(":dFechaCobro",this->dFechaCobro);
     cab_fac.bindValue(":iId_Cliente", this->iId_Cliente);
     cab_fac.bindValue(":cCliente",this->cCliente);
     cab_fac.bindValue(":cDireccion",this->cDireccion);
     cab_fac.bindValue(":cDireccion2",this->cDireccion2);
     cab_fac.bindValue(":cCp",this->cCp);
     cab_fac.bindValue(":cPoblacion",this->cPoblacion);
     cab_fac.bindValue(":cProvincia",this->cProvincia);
     cab_fac.bindValue(":cPais",this->idPais);
     cab_fac.bindValue(":cCif",this->cCif);
     cab_fac.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
     cab_fac.bindValue(":rSubtotal",0);
     cab_fac.bindValue(":nDto",0);
     cab_fac.bindValue(":nDtoPP",0);
     cab_fac.bindValue(":rImporteDescuento",0);
     cab_fac.bindValue(":rImporteDescuentoPP",0);
     cab_fac.bindValue(":rBase",0);
     cab_fac.bindValue(":nIva",0);
     cab_fac.bindValue(":rImporteIva",0);
     cab_fac.bindValue(":rTotal",0);
     cab_fac.bindValue(":lImpresa",0);
     cab_fac.bindValue(":lCobrada",0);
     cab_fac.bindValue(":lContabilizada",0);
     cab_fac.bindValue(":id_FormaPago",this->id_FormaPago);
     cab_fac.bindValue(":cFormaPago",this->cFormaPago);
     cab_fac.bindValue(":tComentario",this->tComentario);
     cab_fac.bindValue(":rBase1",0);
     cab_fac.bindValue(":rBase2",0);
     cab_fac.bindValue(":rBase3",0);
     cab_fac.bindValue(":rBase4",0);
     cab_fac.bindValue(":nPorcentajeIVA1",this->nPorcentajeIVA1);
     cab_fac.bindValue(":nPorcentajeIVA2",this->nPorcentajeIVA2);
     cab_fac.bindValue(":nPorcentajeIVA3",this->nPorcentajeIVA3);
     cab_fac.bindValue(":nPorcentajeIVA4",this->nPorcentajeIVA4);
     cab_fac.bindValue(":rIVA1",0);
     cab_fac.bindValue(":rIVA2",0);
     cab_fac.bindValue(":rIVA3",0);
     cab_fac.bindValue(":rIVA4",0);
     cab_fac.bindValue(":rTotal1",0);
     cab_fac.bindValue(":rTotal2",0);
     cab_fac.bindValue(":rTotal3",0);
     cab_fac.bindValue(":rTotal4",0);
     cab_fac.bindValue(":nRec1",0);
     cab_fac.bindValue(":nRec2",0);
     cab_fac.bindValue(":nRec3",0);
     cab_fac.bindValue(":nRec4",0);
     cab_fac.bindValue(":rRecargoEq1",0);
     cab_fac.bindValue(":rRecargoEq2",0);
     cab_fac.bindValue(":rRecargoEq3",0);
     cab_fac.bindValue(":rRecargoEq4",0);
     cab_fac.bindValue(":rTotalRecargoEq",0);
     cab_fac.bindValue(":rEntregadoaCuenta",0);
     cab_fac.bindValue(":rImportePendiente",0);
     cab_fac.bindValue(":cCodigoEntidad",this->cCodigoEntidad);
     cab_fac.bindValue(":cOficinaEntidad",this->cOficinaEntidad);
     cab_fac.bindValue(":cDCCuenta",this->cDCCuenta);
     cab_fac.bindValue(":cNumeroCuenta",this->cNumeroCuenta);
     cab_fac.bindValue(":cPedidoCliente",this->cPedidoCliente);
     if(!cab_fac.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Factura:", cab_fac.lastError().text());
         return false;
     }
     else
     {
         this->id = cab_fac.lastInsertId().toInt();
         QString cSQL = "Select * from cab_fac where id ="+QString::number(this->id);
         RecuperarFactura(cSQL);
         return true;
     }

}
// Guardar la factura
bool Factura::GuardarFactura(int nId_Factura, bool FacturaLegal)
{
    bool succes = true;
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    cab_fac.prepare( "UPDATE cab_fac set "
                     "cCodigoCliente = :cCodigoCliente,"
                     "cFactura = :cFactura,"
                     "dFecha = :dFecha,"
                     "dFechaCobro = :dFechaCobro,"
                     "iId_Cliente = :iId_Cliente,"
                     "cCliente = :cCliente,"
                     "cDireccion = :cDireccion,"
                     "cDireccion2 = :cDireccion2,"
                     "cCp=:cCp,"
                     "cPoblacion =:cPoblacion,"
                     "cProvincia =:cProvincia,"
                     "idPais = :cPais,"
                     "cCif = :cCif,"
                     "lRecargoEquivalencia = :lRecargoEquivalencia,"
                     "rSubtotal =:rSubtotal,"
                     "nDto =:nDto,"
                     "nDtoPP =:nDtoPP,"
                     "rImporteDescuento =:rImporteDescuento,"
                     "rImporteDescuentoPP =:rImporteDescuentoPP,"
                     "rBase =:rBase,"
                     "nIva =:nIva,"
                     "rImporteIva = :rImporteIva,"
                     "rTotal = :rTotal,"
                     "lImpresa =:lImpresa,"
                     "lCobrada =:lCobrada,"
                     "lContabilizada =:lContabilizada,"
                     "id_FormaPago =:id_FormaPago,"
                     "cFormaPago = :cFormaPago,"
                     "tComentario =:tComentario,"
                     "rBase1 =:rBase1,"
                     "rBase2 =:rBase2,"
                     "rBase3 =:rBase3,"
                     "rBase4 =:rBase4,"
                     "nPorcentajeIVA1 =:nPorcentajeIVA1,"
                     "nPorcentajeIVA2 =:nPorcentajeIVA2,"
                     "nPorcentajeIVA3 =:nPorcentajeIVA3,"
                     "nPorcentajeIVA4 =:nPorcentajeIVA4,"
                     "rIVA1 =:rIVA1,"
                     "rIVA2 =:rIVA2,"
                     "rIVA3 =:rIVA3,"
                     "rIVA4 =:rIVA4,"
                     "rTotal1=:rTotal1,"
                     "rTotal2=:rTotal2,"
                     "rTotal3=:rTotal3,"
                     "rTotal4=:rTotal4,"
                     "nRec1 =:nRec1,"
                     "nRec2 =:nRec2,"
                     "nRec3 =:nRec3,"
                     "nRec4 =:nRec4,"
                     "rRecargoEq1 =:rRecargoEq1,"
                     "rRecargoEq2 =:rRecargoEq2,"
                     "rRecargoEq3 =:rRecargoEq3,"
                     "rRecargoEq4 =:rRecargoEq4,"
                     "rTotalRecargoEq =:rTotalRecargoEq,"
                     "rEntregadoaCuenta =:rEntregadoaCuenta,"
                     "rImportePendiente =:rImportePendiente,"
                     "cCodigoEntidad =:cCodigoEntidad,"
                     "cOficinaEntidad =:cOficinaEntidad,"
                     "cDCCuenta =:cDCCuenta,"
                     "cNumeroCuenta =:cNumeroCuenta,"
                     "cPedidoCliente =:cPedidoCliente,"
                     "nIRPF =:nIRPF,"
                     "rImporteIRPF =:rImporteIRPF"
                     " where Id=:Id");

    // Pasamos valores reales a la Select
    cab_fac.bindValue(":Id",nId_Factura);
    cab_fac.bindValue(":cCodigoCliente",this->cCodigoCliente);
    cab_fac.bindValue(":cFactura",this->cFactura);
    cab_fac.bindValue(":dFecha",this->dFecha);
    cab_fac.bindValue(":dFechaCobro",this->dFechaCobro);
    cab_fac.bindValue(":iId_Cliente", this->iId_Cliente);
    cab_fac.bindValue(":cCliente",this->cCliente);
    cab_fac.bindValue(":cDireccion",this->cDireccion);
    cab_fac.bindValue(":cDireccion2",this->cDireccion2);
    cab_fac.bindValue(":cCp",this->cCp);
    cab_fac.bindValue(":cPoblacion",this->cPoblacion);
    cab_fac.bindValue(":cProvincia",this->cProvincia);
    cab_fac.bindValue(":cPais",this->idPais);
    cab_fac.bindValue(":cCif",this->cCif);
    cab_fac.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
    cab_fac.bindValue(":rSubtotal",this->rSubtotal);
    cab_fac.bindValue(":nDto",this->nDto);
    cab_fac.bindValue(":nDtoPP",this->nDtoPP);
    cab_fac.bindValue(":rImporteDescuento",this->rImporteDescuento);
    cab_fac.bindValue(":rImporteDescuentoPP",this->rImporteDescuentoPP);
    cab_fac.bindValue(":rBase",this->rBase);
    cab_fac.bindValue(":nIva",this->nIva);
    cab_fac.bindValue(":rImporteIva",this->rImporteIva);
    cab_fac.bindValue(":rTotal",this->rTotal);
    cab_fac.bindValue(":lImpresa",this->lImpresa);
    cab_fac.bindValue(":lCobrada",this->lCobrada);
    cab_fac.bindValue(":lContabilizada",this->lContablilizada);
    cab_fac.bindValue(":id_FormaPago",this->id_FormaPago);
    cab_fac.bindValue(":cFormaPago",this->cFormaPago);
    cab_fac.bindValue(":tComentario",this->tComentario);
    cab_fac.bindValue(":rBase1",this->rBase1);
    cab_fac.bindValue(":rBase2",this->rBase2);
    cab_fac.bindValue(":rBase3",this->rBase3);
    cab_fac.bindValue(":rBase4",this->rBase4);
    cab_fac.bindValue(":nPorcentajeIVA1",this->nPorcentajeIVA1);
    cab_fac.bindValue(":nPorcentajeIVA2",this->nPorcentajeIVA2);
    cab_fac.bindValue(":nPorcentajeIVA3",this->nPorcentajeIVA3);
    cab_fac.bindValue(":nPorcentajeIVA4",this->nPorcentajeIVA4);
    cab_fac.bindValue(":rIVA1",this->rIVA1);
    cab_fac.bindValue(":rIVA2",this->rIVA2);
    cab_fac.bindValue(":rIVA3",this->rIVA3);
    cab_fac.bindValue(":rIVA4",this->rIVA4);
    cab_fac.bindValue(":rTotal1",this->rTotal1);
    cab_fac.bindValue(":rTotal2",this->rTotal2);
    cab_fac.bindValue(":rTotal3",this->rTotal3);
    cab_fac.bindValue(":rTotal4",this->rTotal4);
    cab_fac.bindValue(":nRec1",this->nRec1);
    cab_fac.bindValue(":nRec2",this->nRec2);
    cab_fac.bindValue(":nRec3",this->nRec3);
    cab_fac.bindValue(":nRec4",this->nRec4);
    cab_fac.bindValue(":rRecargoEq1",this->rRecargoEq1);
    cab_fac.bindValue(":rRecargoEq2",this->rRecargoEq2);
    cab_fac.bindValue(":rRecargoEq3",this->rRecargoEq3);
    cab_fac.bindValue(":rRecargoEq4",this->rRecargoEq4);
    cab_fac.bindValue(":rTotalRecargoEq",this->rTotalRecargoEq);
    cab_fac.bindValue(":rEntregadoaCuenta",this->rEntregadoaCuenta);
    cab_fac.bindValue(":rImportePendiente",this->rImportePendiente);
    cab_fac.bindValue(":cCodigoEntidad",this->cCodigoEntidad);
    cab_fac.bindValue(":cOficinaEntidad",this->cOficinaEntidad);
    cab_fac.bindValue(":cDCCuenta",this->cDCCuenta);
    cab_fac.bindValue(":cNumeroCuenta",this->cNumeroCuenta);
    cab_fac.bindValue(":cPedidoCliente",this->cPedidoCliente);
    cab_fac.bindValue(":nIRPF",this->nIRPF);
    cab_fac.bindValue(":rImporteIRPF",this->rImporteIRPF);
    if(!cab_fac.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),tr("error al guardar datos Factura:"), cab_fac.lastError().text());
        succes =  false;
    }
    else
    {
        QMessageBox::information(qApp->activeWindow(),tr("Guardar datos"),tr("La Factura se ha guardado correctamente:"),tr("Ok"));
        QString cSQL = "Select * from cab_fac where id ="+QString::number(nId_Factura);
        RecuperarFactura(cSQL);
        if (FacturaLegal)
        {
            if(QMessageBox::question(qApp->activeWindow(),tr("Guardar Factura"),
                                     tr("¿Desea cobrar la factura ahora o generar una deuda al cliente?"),
                                     tr("Cobrar"),tr("Generar deuda")) == QMessageBox::Accepted)
            {
                succes = CobrarFactura();
            }
            else
            {
                // Busco ficha cliente
                QSqlQuery Cliente(QSqlDatabase::database("empresa"));
                Cliente.prepare("Select * from clientes where cCodigoCliente = :cCodCli");
                Cliente.bindValue(":cCodCli",this->cCodigoCliente);
                if (Cliente.exec())
                {
                    Cliente.next();
                    QSqlRecord record = Cliente.record();

                    // Genero deuda cliente

                    QSqlQuery Deudacliente(QSqlDatabase::database("empresa"));
                    Deudacliente.prepare("Insert into clientes_deuda (id_Cliente,dFecha,dVencimiento,cDocumento,id_Ticket,id_Factura,nTipo,"
                                         "rImporte,rPagado,rPendienteCobro,cEntidad,cOficina,cDC,cCuenta)"
                                         " values (:id_cliente,:dFecha,:dVencimiento,:cDocumento,:id_tiquet,:id_factura,:nTipo,"
                                         ":rImporte,:rPagado,:rPendienteCobro,:cEntidad,:cOficina,:cDC,:cCuenta)");
                    Deudacliente.bindValue(":id_cliente",record.field("Id").value().toInt());
                    Deudacliente.bindValue(":dFecha",QDate::currentDate());
                    Deudacliente.bindValue(":dVencimiento",QDate::currentDate());
                    // TODO Deudacliente->bindValue(":dVencimiento",Configuracion_global->CalcularVencimiento());
                    Deudacliente.bindValue(":cDocumento",this->cFactura);
                    Deudacliente.bindValue(":id_tiquet",0);
                    Deudacliente.bindValue(":id_factura",nId_Factura);
                    Deudacliente.bindValue("nTipo",1);
                    Deudacliente.bindValue(":rImporte",this->rTotal);
                    Deudacliente.bindValue(":rPagado",0);
                    Deudacliente.bindValue(":rPendienteCobro",this->rTotal);
                    Deudacliente.bindValue(":cEntidad",record.field("cCEntidadBancaria").value().toString());
                    Deudacliente.bindValue(":cOficina",record.field("cOficinaBancaria").value().toString());
                    Deudacliente.bindValue(":cDC",record.field("cDC").value().toString());
                    Deudacliente.bindValue(":cCuenta",record.field("cCuentaCorriente").value().toString());
                    if(!Deudacliente.exec())
                    {
                        qDebug() << Deudacliente.lastQuery();
                        QMessageBox::warning(qApp->activeWindow(),tr("Añadir deuda"),tr("No se ha podido añadir la deuda ")+Deudacliente.lastError().text() ,tr("OK"));
                        succes = false;
                    }
                    else
                    {
                        // Añadimos acumulados ficha cliente.
                        Cliente.prepare("Update clientes set dFechaUltimaCompra = :dFechaUltimaCompra, "
                                        "rAcumuladoVentas = rAcumuladoVentas + :rAcumuladoVentas,"
                                        "rVentasEjercicio = rVentasEjercicio + :rVentasEjercicio,"
                                        "rDeudaActual = rDeudaActual + :rDeudaActual "
                                        " where Id = :Id_Cliente");
                        Cliente.bindValue(":dFechaUltimaCompra",QDate::currentDate());
                        Cliente.bindValue(":rAcumuladoVentas",this->rTotal);
                        Cliente.bindValue(":rVentasEjercicio",this->rTotal);
                        Cliente.bindValue(":rDeudaActual",this->rTotal);
                        Cliente.bindValue(":Id_Cliente",record.field("Id").value().toInt());
                        if (!Cliente.exec())
                        {
                            succes =  false;
                            QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                                                 tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                                                 tr("OK"));
                        }
                    }
                }
                else
                    succes = false;
            }
        }
    }
    return succes;
}

bool Factura::RecuperarFactura(QString cSQL){
        QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
        cab_fac.prepare(cSQL);
        if( !cab_fac.exec() )
        {
            QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
            return false;
        }
        else
        {
            if (cab_fac.next())
            {
                QSqlRecord registro = cab_fac.record();
                this->id = registro.field("id").value().toInt();
                this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
                this->cFactura = registro.field("cFactura").value().toString();
                this->dFecha = registro.field("dFecha").value().toDate();
                this->dFechaCobro = registro.field("dFechaCobro").value().toDate();
                this->iId_Cliente = registro.field("iId_Cliente").value().toInt();
                this->cCliente = registro.field("cCliente").value().toString();
                this->cDireccion = registro.field("cDireccion").value().toString();
                this->cDireccion2 = registro.field("cDireccion2").value().toString();
                this->cCp = registro.field("cCp").value().toString();
                this->cPoblacion = registro.field("cPoblacion").value().toString();
                this->cProvincia = registro.field("cProvincia").value().toString();
                this->idPais = registro.field("idPais").value().toInt();
                this->cCif =registro.field("cCif").value().toString();
                this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toInt();
                this->rSubtotal = registro.field("rSubtotal").value().toDouble();
                this->nDto = registro.field("nDto").value().toInt();
                this->nDtoPP = registro.field("nDtoPP").value().toInt();
                this->rImporteDescuento = registro.field("rImporteDescuento").value().toDouble();
                this->rImporteDescuentoPP = registro.field("rImporteDescuentoPP").value().toDouble();
                this->rBase = registro.field("rBase").value().toDouble();
                this->nIva = registro.field("nIva").value().toInt();
                this->rImporteIva = registro.field("rImporteIva").value().toDouble();
                this->rTotal = registro.field("rTotal").value().toDouble();
                this->lImpresa = registro.field("lImpresa").value().toInt();
                this->lCobrada = registro.field("lCobrada").value().toInt();
                this->lContablilizada = registro.field("lContabilizada").value().toInt();
                this->id_FormaPago = registro.field("id_FormaPago").value().toInt();
                this->cFormaPago = registro.field("cFormaPago").value().toString();
                this->tComentario = registro.field("tComentario").value().toString();
                this->rBase1 = registro.field("rBase1").value().toDouble();
                this->rBase2 = registro.field("rBase2").value().toDouble();
                this->rBase3 = registro.field("rBase3").value().toDouble();
                this->rBase4 = registro.field("rBase4").value().toDouble();
                this->nPorcentajeIVA1 = registro.field("nPorcentajeIVA1").value().toInt();
                this->nPorcentajeIVA2 = registro.field("nPorcentajeIVA2").value().toInt();
                this->nPorcentajeIVA3 = registro.field("nPorcentajeIVA3").value().toInt();
                this->nPorcentajeIVA4 = registro.field("nPorcentajeIVA4").value().toInt();
                this->rIVA1 = registro.field("rIVA1").value().toDouble();
                this->rIVA2 = registro.field("rIVA3").value().toDouble();
                this->rIVA3 = registro.field("rIVA3").value().toDouble();
                this->rIVA4 = registro.field("rIVA4").value().toDouble();
                this->rTotal1 = registro.field("rTotal1").value().toDouble();
                this->rTotal2 = registro.field("rTotal2").value().toDouble();
                this->rTotal3 = registro.field("rTotal3").value().toDouble();
                this->rTotal4 = registro.field("rTotal4").value().toDouble();
                this->nRec1 = registro.field("nRec1").value().toDouble();
                this->nRec2 = registro.field("nRec2").value().toDouble();
                this->nRec3 = registro.field("nRec3").value().toDouble();
                this->nRec4 = registro.field("nRec4").value().toDouble();
                this->rRecargoEq1 = registro.field("rRecargoEq1").value().toDouble();
                this->rRecargoEq2 = registro.field("rRecargoEq2").value().toDouble();
                this->rRecargoEq3 = registro.field("nRecargoEq3").value().toDouble();
                this->rRecargoEq4 = registro.field("rRecargoEq4").value().toDouble();
                this->rTotalRecargoEq = registro.field("rTotalRecargoEq").value().toDouble();
                this->rEntregadoaCuenta = registro.field("rEntregadoaCuenta").value().toDouble();
                this->rImportePendiente = registro.field("rImportePendiente").value().toDouble();
                this->cCodigoEntidad = registro.field("cCodigoEntidad").value().toString();
                this->cOficinaEntidad = registro.field("cOficinaEntidad").value().toString();
                this->cDCCuenta = registro.field("cDCCuenta").value().toString();
                this->cNumeroCuenta = registro.field("cNumeroCuenta").value().toString();
                this->cPedidoCliente = registro.field("cPedidoCliente").value().toString();
                this->nIRPF = registro.field("nIRPF").value().toInt();
                this->rImporteIRPF = registro.field("rImporteIRPF").value().toDouble();
                return true;
               }
            else
            {
                return false;
            }
        }
}

QString Factura::NuevoNumeroFactura() {
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    QString cNum,cSerie;
    QString cNumFac;
    int inum;

    cab_fac.prepare("Select cFactura from cab_fac  where cFactura <> '"+QObject::tr("BORRADOR")+"' order by cFactura desc limit 1");
    if(cab_fac.exec()) {
        cab_fac.next();
        cNumFac = cab_fac.value(0).toString();
        cNum = cNumFac.right(Configuracion_global->nDigitosFactura);
        cSerie = Configuracion_global->cSerie;
        inum = cNum.toInt();
        inum++;
        cNum = cNum.number(inum);
        while (cNum.length()< Configuracion_global->nDigitosFactura) {
            cNum.prepend("0");
        }
    } else {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
    }
    cNumFac = cSerie + cNum;
    return cNumFac;
}

bool Factura::BorrarLineasFactura(int id_lin)
{
    QSqlQuery query(QSqlDatabase::database("empresa"));
        QString sql = QString("DELETE FROM lin_fac WHERE id_Cab = %1").arg(id_lin);
        query.prepare(sql);
        if(query.exec())
            return true;
        else
        {
            QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
            return false;
        }
}

bool Factura::CobrarFactura()
{
    // marcar factura como cobrada
    this->lCobrada = true;
    QSqlQuery Cliente(QSqlDatabase::database("empresa"));

    // Añadimos acumulados ficha cliente.
    Cliente.prepare("Update clientes set dFechaUltimaCompra = :dFechaUltimaCompra, "
                             "rAcumuladoVentas = rAcumuladoVentas + :rAcumuladoVentas,"
                             "rVentasEjercicio = rVentasEjercicio + :rVentasEjercicio "
                             " where cCodigoCliente = :cCodigoCliente");
    Cliente.bindValue(":dFechaUltimaCompra",QDate::currentDate());
    Cliente.bindValue(":rAcumuladoVentas",this->rTotal);
    Cliente.bindValue(":rVentasEjercicio",this->rTotal);
    Cliente.bindValue(":cCodigoCliente",this->cCodigoCliente);
    if (!Cliente.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                             tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                             tr("OK"));
        return false;
    }
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    cab_fac.prepare("update cab_fac set lCobrada = 1 where Id =:id_cab");
    cab_fac.bindValue(":id_cab",this->id);
    if(!cab_fac.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Guardar Factura"),tr("No se ha podido marcar la factura como cobrada"),tr("OK"));
        return false;
    }
    return true;
 }
