#include "factura.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlRecord>
#include <QErrorMessage>
#include <QMessageBox>
#include <frmdecision.h>
#include <QDebug>
#include "configuracion.h"


Factura::Factura(QObject *parent) :
    QObject(parent)
{
    this->id =0;
}

// Metodos utilidad Clase
void Factura::AnadirFactura() {
    Configuracion *oConf =  new Configuracion();
    oConf->CargarDatos();
    this->nPorcentajeIVA1 = oConf->nIVA1;
    this->nPorcentajeIVA2 = oConf->nIVA2;
    this->nPorcentajeIVA3 = oConf->nIVA3;
    this->nPorcentajeIVA4 = oConf->nIVA4 ;
    this->lRecargoEquivalencia = 0;
    delete oConf;
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
     cab_fac.prepare("INSERT INTO cab_fac (cCodigoCliente,cFactura,dFecha,dFechaCobro,iId_Cliente,cCliente,cDireccion,cDireccion2,"
                   "cCp,cPoblacion,cProvincia,cPais,cCif,lRecargoEquivalencia,rSubtotal,nDto,nDtoPP,rImporteDescuento,rImporteDescuentoPP,"
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
     cab_fac.bindValue(":cPais",this->cPais);
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
     if(!cab_fac.exec()){
         QMessageBox::critical(NULL,"error al guardar datos Factura:", cab_fac.lastError().text());
     }

}
// Guardar la factura
void Factura::GuardarFactura(int nId_Factura) {
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    cab_fac.prepare( "UPDATE cab_fac set "
                   "cCodigoCliente = :cCodigoCliente,"
                   "cFactura = :cFactura,"
                   "dFecha = :dFecha,"
                   "dFechaCobro = :dFechaCobro,"
                   "iId_Cliente = :iId_Cliente,"
                   "cCliente =:cCliente,"
                   "cDireccion =:cDireccion,"
                   "cDireccion2 =:cDireccion2,"
                   "cCp=:cCp,"
                   "cPoblacion =:cPoblacion,"
                   "cProvincia =:cProvincia,"
                   "cPais = :cPais,"
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
                   "cPedidoCliente =:cPedidoCliente"
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
    cab_fac.bindValue(":cPais",this->cPais);
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
    if(!cab_fac.exec()){
        QMessageBox::critical(NULL,tr("error al guardar datos Factura:"), cab_fac.lastError().text());
    } else {
        QMessageBox::information(NULL,tr("Guardar datos"),tr("La Factura se ha guardado correctamente:"),tr("Ok"));
        this->id = cab_fac.lastInsertId().toInt();
        QString cSQL = "Select * from cab_fac where id ="+QString::number(this->id);
        RecuperarFactura(cSQL);
    }


}

void Factura::RecuperarFactura(QString cSQL){
        cab_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
        cab_fac->prepare(cSQL);
        if( !cab_fac->exec() ) {
            QMessageBox::critical(NULL, "error:", cab_fac->lastError().text());
        } else {
            if (cab_fac->next()) {
                this->id = cab_fac->value(0).toInt();
                this->cCodigoCliente= cab_fac->value(1).toString();
                this->cFactura = cab_fac->value(2).toString();
                this->dFecha = cab_fac->value(3).toDate();
                this->dFechaCobro = cab_fac->value(4).toDate();
                this->iId_Cliente = cab_fac->value(5).toInt();
                this->cCliente = cab_fac->value(6).toString();
                this->cDireccion = cab_fac->value(7).toString();
                this->cDireccion2 = cab_fac->value(8).toString();
                this->cCp = cab_fac->value(9).toString();
                this->cPoblacion = cab_fac->value(10).toString();
                this->cProvincia = cab_fac->value(11).toString();
                this->cPais = cab_fac->value(12).toString();
                this->cCif = cab_fac->value(13).toString();
                this->lRecargoEquivalencia = cab_fac->value(14).toInt();
                this->rSubtotal = cab_fac->value(15).toDouble();
                this->nDto = cab_fac->value(16).toInt();
                this->nDtoPP = cab_fac->value(17).toInt();
                this->rImporteDescuento = cab_fac->value(18).toDouble();
                this->rImporteDescuentoPP = cab_fac->value(19).toDouble();
                this->rBase = cab_fac->value(20).toDouble();
                this->nIva = cab_fac->value(21).toInt();
                this->rImporteIva = cab_fac->value(22).toDouble();
                this->rTotal = cab_fac->value(23).toDouble();
                this->lImpresa = cab_fac->value(24).toInt();
                this->lCobrada = cab_fac->value(25).toInt();
                this->lContablilizada = cab_fac->value(26).toInt();
                this->id_FormaPago = cab_fac->value(27).toInt();
                this->cFormaPago = cab_fac->value(28).toString();
                this->tComentario = cab_fac->value(29).toString();
                this->rBase1 = cab_fac->value(30).toDouble();
                this->rBase2 = cab_fac->value(31).toDouble();
                this->rBase3 = cab_fac->value(32).toDouble();
                this->rBase4 = cab_fac->value(33).toDouble();
                this->nPorcentajeIVA1 = cab_fac->value(34).toInt();
                this->nPorcentajeIVA2 = cab_fac->value(35).toInt();
                this->nPorcentajeIVA3 = cab_fac->value(36).toInt();
                this->nPorcentajeIVA4 = cab_fac->value(37).toInt();
                this->rIVA1 = cab_fac->value(38).toDouble();
                this->rIVA2 = cab_fac->value(39).toDouble();
                this->rIVA3 = cab_fac->value(40).toDouble();
                this->rIVA4 = cab_fac->value(41).toDouble();
                this->rTotal1 = cab_fac->value(42).toDouble();
                this->rTotal2 = cab_fac->value(43).toDouble();
                this->rTotal3 = cab_fac->value(44).toDouble();
                this->rTotal4 = cab_fac->value(45).toDouble();
                this->nRec1 = cab_fac->value(46).toDouble();
                this->nRec2 = cab_fac->value(47).toDouble();
                this->nRec3 = cab_fac->value(48).toDouble();
                this->nRec4 = cab_fac->value(49).toDouble();
                this->rRecargoEq1 = cab_fac->value(50).toDouble();
                this->rRecargoEq2 = cab_fac->value(51).toDouble();
                this->rRecargoEq3 = cab_fac->value(52).toDouble();
                this->rRecargoEq4 = cab_fac->value(53).toDouble();
                this->rTotalRecargoEq = cab_fac->value(54).toDouble();
                this->rEntregadoaCuenta = cab_fac->value(55).toDouble();
                this->rImportePendiente = cab_fac->value(56).toDouble();
                this->cCodigoEntidad = cab_fac->value(57).toString();
                this->cOficinaEntidad = cab_fac->value(58).toString();
                this->cDCCuenta = cab_fac->value(59).toString();
                this->cNumeroCuenta = cab_fac->value(60).toString();
                this->cPedidoCliente = cab_fac->value(61).toString();


               }
        }
}

QString Factura::NuevoNumeroFactura() {
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    QString cNum,cSerie;
    QString cNumFac;
    int inum;
    Configuracion *oConfig = new Configuracion();
    oConfig->CargarDatos();
    cab_fac.prepare("Select cFactura from cab_fac  order by cFactura desc limit 0,1");
    if(cab_fac.exec()) {
        cab_fac.next();
        cNumFac = cab_fac.value(0).toString();
        cNum = cNumFac.right(oConfig->nDigitosFactura);
        cSerie = cNumFac.left(oConfig->nDigitosFactura);
        inum = cNum.toInt();
        inum++;
        cNum = cNum.number(inum);
        while (cNum.length()< oConfig->nDigitosFactura) {
            cNum.prepend("0");
        }
    } else {
         QMessageBox::critical(NULL, "error:", cab_fac.lastError().text());
    }
    delete oConfig;
    cNumFac = cSerie + cNum;
    return cNumFac;


}

void Factura::AnadirLineaFactura(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                                 double porcdto, double dto, double total, double nPorcIva)
{
    QSqlQuery *Qlin_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
    Qlin_fac->prepare("INSERT INTO lin_fac (id_Cab,cCodigo,nCantidad,cDescripcion,rPvp,nDto,rDto,rSubTotal,rTotal,nPorcIva)"
                      " VALUES(:id_cab,:cCodigo,:nCantidad,:cDescripcion,:rPvp,:nDto,:rDto,:rSubTotal,:rTotal,:nPorcIva)");
    Qlin_fac->bindValue(":id_cab",id_cab);
    Qlin_fac->bindValue(":cCodigo",cCodigo);
    Qlin_fac->bindValue(":nCantidad",nCantidad);
    Qlin_fac->bindValue(":cDescripcion",cDescripcion);
    Qlin_fac->bindValue(":rPvp",pvp);
    Qlin_fac->bindValue(":nDto",porcdto);
    Qlin_fac->bindValue(":rDto",dto);
    Qlin_fac->bindValue(":rSubTotal",subtotal);
    Qlin_fac->bindValue(":rTotal",total);
    Qlin_fac->bindValue(":nPorcIva",nPorcIva);
    if (!Qlin_fac->exec()){
       QMessageBox::critical(NULL,"error al guardar datos línea Factura:", Qlin_fac->lastError().text());
    }
    delete Qlin_fac;
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

    QArticulos->exec();

}

void Factura::calcularFactura()
{
    // Reseteo valores
    this->rSubtotal = 0;
    this->rImporteDescuento =0;
    this->rBase = 0;
    this->rTotal = 0;
    this->rBase1 =0;
    this->rBase2 =0;
    this->rBase3 = 0;
    this->rBase4 = 0;
    this->rIVA1 = 0;
    this->rIVA2 = 0;
    this->rIVA3 = 0;
    this->rIVA4 = 0;
    this->rTotal1 =0;
    this->rTotal2 =0;
    this->rTotal3 =0;
    this->rTotal4 = 0;

    QSqlQuery *Qlin_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
    Qlin_fac->prepare("Select * from lin_fac where id_cab = :nId");
    Qlin_fac->bindValue(":nId",this->id);
    if (Qlin_fac->exec()) {
        QSqlRecord record = Qlin_fac->record();
        while (Qlin_fac->next()) {
            record = Qlin_fac->record();

            this->rSubtotal = this->rSubtotal + record.field("rSubtotal").value().toDouble();
            this->rImporteDescuento = this->rImporteDescuento + record.field("rDto").value().toDouble();
            this->rBase = (this->rSubtotal - this->rImporteDescuento) - this->rImporteDescuentoPP;
            // IVA 1
            if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA1) {
                this->rBase1 = this->rBase1 + record.field("rTotal").value().toDouble();
                this->rIVA1 = (this->rBase1 * this->nPorcentajeIVA1)/100;
                this->rTotal1 = this->rBase1 + this->rIVA1;
            }
            // IVA 2
            if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA2) {
                this->rBase2 = this->rBase2 + record.field("rTotal").value().toDouble();
                this->rIVA2 = (this->rBase2 * this->nPorcentajeIVA2)/100;
                this->rTotal2 = this->rBase2 + this->rIVA2;
            }
            // IVA 3
            if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA3) {
                this->rBase3 = this->rBase3 + record.field("rTotal").value().toDouble();
                this->rIVA3 = (this->rBase3 * this->nPorcentajeIVA3)/100;
                this->rTotal3 = this->rBase3 + this->rIVA3;
            }
            // IVA 4
            if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA4) {
                this->rBase4 = this->rBase4 + record.field("rTotal").value().toDouble();
                this->rIVA4 = (this->rBase4 * this->nPorcentajeIVA4)/100;
                this->rTotal4 = this->rBase4 + this->rIVA4;

            }
            this->rImporteIva =  (this->rIVA1 +  this->rIVA2 + this->rIVA3 + this->rIVA4);
            this->rTotal = this->rBase + this->rImporteIva;

        }
    }
}

// getters
int  Factura::Getid() {
   return this->id;
}

QString Factura::getcCodigoCliente() {
   return this->cCodigoCliente;
}

QString Factura::getcFactura() {
    return this->cFactura;
}
QDate Factura::getdFecha() {
    return this->dFecha;
}
QDate Factura::getdFechaCobro() {
    return this->dFechaCobro;
}
int Factura::getiId_Cliente() {
    return this->iId_Cliente;
}
QString Factura::getcCliente() {
    return this->cCliente;
}
QString Factura::getcDireccion() {
    return this->cDireccion;
}
QString Factura::getcDireccion2() {
    return this->cDireccion2;
}
QString Factura::getcCp() {
    return this->cCp;
}
QString Factura::getcPoblacion() {
    return this->cPoblacion;
}
QString Factura::getcProvincia() {
    return this->cProvincia;
}
QString Factura::getcPais() {
    return this->cPais;
}
QString Factura::getcCif() {
    return this->cCif;
}
int Factura::getlRecargoEquivalencia() {
    return this->lRecargoEquivalencia;
}
double Factura::getrSubtotal() {
    return this->rSubtotal;
}
int Factura::getnDto() {
    return this->nDto;
}
int Factura::getnDtoPP() {
    return this->nDtoPP;
}
double Factura::getrImporteDescuento() {
    return this->rImporteDescuento;
}
double Factura::getrImporteDescuentoPP() {
    return this->rImporteDescuentoPP;
}
double Factura::getrBase() {
    return this->rBase;
}
int Factura::getnIva() {
    return this->nIva;
}
double Factura::getrImporteIva() {
    return this->rImporteIva;
}
double Factura::getrTotal() {
    return this->rTotal;
}
int Factura::getlImpresa() {
    return this->lImpresa;
}
int Factura::getlCobrada() {
    return this->lCobrada;
}
int Factura::getlContablilizada() {
    return this->lContablilizada;
}
int Factura::getid_FormaPago() {
    return this->id_FormaPago;
}
QString Factura::getcFormaPago() {
    return this->cFormaPago;
}
QString Factura::gettComentario() {
    return this->tComentario;
}
double Factura::getrBase1() {
    return this->rBase1;
}
double Factura::getrBase2() {
    return this->rBase2;
}
double Factura::getrBase3() {
    return this->rBase3;
}
double Factura::getrBase4() {
    return this->rBase4;
}
int Factura::getnPorcentajeIVA1() {
    return this->nPorcentajeIVA1;
}
int Factura::getnPorcentajeIVA2() {
    return this->nPorcentajeIVA2;
}
int Factura::getnPorcentajeIVA3() {
    return this->nPorcentajeIVA3;
}
int Factura::getnPorcentajeIVA4() {
    return this->nPorcentajeIVA4;
}
double Factura::getrIVA1() {
    return this->rIVA1;
}
double Factura::getrIVA2() {
    return this->rIVA2;
}
double Factura::getrIVA3() {
    return this->rIVA3;
}
double Factura::getrIVA4() {
    return this->rIVA4;
}
double Factura::getrTotal1() {
    return this->rTotal1;
}
double Factura::getrTotal2() {
    return this->rTotal2;
}
double Factura::getrTotal3() {
    return this->rTotal3;
}
double Factura::getrTotal4() {
    return this->rTotal4;
}
double Factura::getnRec1() {
    return this->nRec1;
}
double Factura::getnRec2() {
    return this->nRec2;
}
double Factura::getnRec3() {
    return this->nRec3;
}
double Factura::getnRec4() {
    return this->nRec4;
}
double Factura::getrRecargoEq1() {
    return this->rRecargoEq1;
}
double Factura::getrRecargoEq2() {
    return this->rRecargoEq2;
}
double Factura::getrRecargoEq3() {
    return this->rRecargoEq3;
}
double Factura::getrRecargoEq4() {
    return this->rRecargoEq4;
}
double Factura::getrTotalRecargoEq() {
    return this->rTotalRecargoEq;
}
double Factura::getrEntregadoaCuenta() {
    return this->rEntregadoaCuenta;
}
double Factura::getrImportePendiente() {
    return this->rImportePendiente;
}
QString Factura::getcCodigoEntidad() {
    return this->cCodigoEntidad;
}
QString Factura::getcOficinaEntidad() {
    return this->cOficinaEntidad;
}
QString Factura::getcDCCuenta() {
    return this->cDCCuenta;
}
QString Factura::getcNumeroCuenta() {
    return this->cNumeroCuenta;
}
QString Factura::getcPedidoCliente() {
    return this->cPedidoCliente;
}


// setters
void Factura::setid( int iID_Factura) {
    this->id = iID_Factura;
}
void Factura::setcCodigoCliente(QString cCodigoCliente) {
    this->cCodigoCliente = cCodigoCliente;
}
void Factura::setcFactura( QString cFactura) {
    this->cFactura = cFactura;
}
void Factura::setdFecha(QDate dFecha) {
    this->dFecha = dFecha;
}
void Factura::setdFechaCobro(QDate dFechaCobro) {
    this->dFechaCobro = dFechaCobro;
}
void Factura::setiId_Cliente(int iId_Cliente) {
    this->iId_Cliente = iId_Cliente;
}
void Factura::setcCliente(QString cCliente) {
    this->cCliente = cCliente;
}
void Factura::setcDireccion(QString cDireccion) {
    this->cDireccion = cDireccion;
}
void Factura::setcDireccion2(QString cDireccion2) {
    this->cDireccion2 = cDireccion2;
}
void Factura::setcCp(QString cCp) {
    this->cCp = cCp;
}
void Factura::setcPoblacion(QString cPoblacion) {
    this->cPoblacion = cPoblacion;
}
void Factura::setcProvincia(QString cProvincia) {
    this->cProvincia = cProvincia;}
void Factura::setcPais(QString cPais) {
    this->cPais = cPais;
}
void Factura::setcCif(QString cCif) {
    this->cCif = cCif;
}
void Factura::setlRecargoEquivalencia(int lRecargoEquivalencia) {
    this->lRecargoEquivalencia = lRecargoEquivalencia;
}
void Factura::setrSubtotal(double rSubtotal) {
    this->rSubtotal = rSubtotal;
}
void Factura::setnDto(int nDto) {
    this->nDto = nDto;
}
void Factura::setnDtoPP(int nDtoPP) {
    this->nDtoPP = nDtoPP;
}
void Factura::setrImporteDescuento(double rImporteDescuento) {
    this->rImporteDescuento = rImporteDescuento;
}
void Factura::setrImporteDescuentoPP(double rImporteDescuentoPP) {
    this->rImporteDescuentoPP = rImporteDescuentoPP;
}
void Factura::setrBase(double rBase) {
    this->rBase = rBase;
}
void Factura::setnIva(int nIva) {
    this->nIva = nIva;
}
void Factura::setrImporteIva(double rImporteIva) {
    this->rImporteIva = rImporteIva;
}
void Factura::setrTotal(double rTotal) {
    this->rTotal = rTotal;
}
void Factura::setlImpresa(int lImpresa) {
    this->lImpresa = lImpresa;
}
void Factura::setlCobrada(int lCobrada) {
    this->lCobrada = lCobrada;
}
void Factura::setlContablilizada(int lContablilizada) {
    this->lContablilizada = lContablilizada;
}
void Factura::setid_FormaPago(int id_FormaPago) {
    this->id_FormaPago = id_FormaPago;
}
void Factura::setcFormaPago(QString cFormaPago) {
    this->cFormaPago = cFormaPago;
}
void Factura::settComentario(QString tComentario) {
    this->tComentario = tComentario;
}
void Factura::setrBase1(double rBase1) {
    this->rBase1 = rBase1;
}
void Factura::setrBase2(double rBase2) {
    this->rBase2 = rBase2;
}
void Factura::setrBase3(double rBase3) {
    this->rBase3 = rBase3;
}
void Factura::setrBase4(double rBase4) {
    this->rBase4 = rBase4;
}
void Factura::setnPorcentajeIVA1(int nPorcentajeIVA1) {
    this->nPorcentajeIVA1 = nPorcentajeIVA1;
}
void Factura::setnPorcentajeIVA2(int nPorcentajeIVA2) {
    this->nPorcentajeIVA2 = nPorcentajeIVA2;
}
void Factura::setnPorcentajeIVA3(int nPorcentajeIVA3) {
    this->nPorcentajeIVA3 = nPorcentajeIVA3;
}
void Factura::setnPorcentajeIVA4(int nPorcentajeIVA4) {
    this->nPorcentajeIVA4 = nPorcentajeIVA4;
}
void Factura::setrIVA1( double rIVA1) {
    this->rIVA1 = rIVA1;
}
void Factura::setrIVA2( double rIVA2) {
    this->rIVA2 = rIVA2;
}
void Factura::setrIVA3( double rIVA3) {
    this->rIVA3 = rIVA3;
}
void Factura::setrIVA4( double rIVA4) {
    this->rIVA4 = rIVA4;
}
void Factura::setrTotal1(double rTotal1) {
    this->rTotal1 = rTotal1;
}
void Factura::setrTotal2(double rTotal2) {
    this->rTotal2 = rTotal2;
}
void Factura::setrTotal3(double rTotal3) {
    this->rTotal3 = rTotal3;
}
void Factura::setrTotal4(double rTotal4) {
    this->rTotal4 = rTotal4;
}
void Factura::setnRec1(int nRec1) {
    this->nRec1 = nRec1;
}
void Factura::setnRec2(int nRec2) {
    this->nRec2 = nRec2;
}
void Factura::setnRec3(int nRec3) {
    this->nRec3 = nRec3;
}
void Factura::setnRec4(int nRec4) {
    this->nRec4 = nRec4;
}
void Factura::setrRecargoEq1(double rRecargoEq1) {
    this->rRecargoEq1 = rRecargoEq1;
}
void Factura::setrRecargoEq2(double rRecargoEq2) {
    this->rRecargoEq2 = rRecargoEq2;
}
void Factura::setrRecargoEq3(double rRecargoEq3) {
    this->rRecargoEq3 = rRecargoEq3;
}
void Factura::setrRecargoEq4(double rRecargoEq4) {
    this->rRecargoEq4 = rRecargoEq4;
}
void Factura::setrTotalRecargoEq(double rTotalRecargoEq) {
    this->rTotalRecargoEq;
}
void Factura::setrEntregadoaCuenta(double rEntregadoaCuenta) {
    this->rEntregadoaCuenta = rEntregadoaCuenta;
}
void Factura::setrImportePendiente(double rImportePendiente) {
    this->rImportePendiente = rImportePendiente;
}
void Factura::setcCodigoEntidad(QString cCodigoEntidad) {
    this->cCodigoEntidad = cCodigoEntidad;
}
void Factura::setcOficinaEntidad(QString cOficinaEntidad) {
    this->cOficinaEntidad = cOficinaEntidad;
}
void Factura::setcDCCuenta(QString cDCCuenta) {
     this->cDCCuenta = cDCCuenta;
}
void Factura::setcNumeroCuenta(QString cNumeroCuenta) {
    this->cNumeroCuenta = cNumeroCuenta;
}
void Factura::setcPedidoCliente(QString cPedidoCliente) {
    this->cPedidoCliente = cPedidoCliente;
}
