#include "pedidos.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlRecord>
#include <QErrorMessage>
#include <QMessageBox>
#include <frmdecision.h>
#include <QDebug>
#include "configuracion.h"
#include <QString>


Pedidos::Pedidos()
{
    this->id =0;
}
void Pedidos::AnadirPedido() {
    Configuracion *oConf =  new Configuracion();
    oConf->CargarDatos();
    this->nPorcentajeIVA1 = oConf->nIVA1;
    this->nPorcentajeIVA2 = oConf->nIVA2;
    this->nPorcentajeIVA3 = oConf->nIVA3;
    this->nPorcentajeIVA4 = oConf->nIVA4 ;
    this->nRec1 = oConf->nRE1;
    this->nRec2 = oConf->nRE2;
    this->nRec3 = oConf->nRE3;
    this->nRec4 = oConf->nRE4;
    this->lRecargoEquivalencia = 0;
    delete oConf;
    QSqlQuery cab_ped(QSqlDatabase::database("empresa"));
     cab_ped.prepare("INSERT INTO ped_cli (nPedido,nPorcentajeIva1,nPorcentajeIva2,nPorcentajeIva3,nPorcentajeIva4,"
                     "nPorcentajeRecargoEq1,nPorcentajeRecargoEq2,nPorcentajeRecargoEq3,nPorcentajeRecargoEq4)"
                   " VALUES (:nPedido,:nPorcentajeIva1,:nPorcentajeIva2,:nPorcentajeIva3,:nPorcentajeIva4,"
                     ":nPorcentajeRecargoEq1,:nPorcentajeRecargoEq2,:nPorcentajeRecargoEq3,:nPorcentajeRecargoEq4)");

     cab_ped.bindValue(":nPedido",0);
     cab_ped.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
     cab_ped.bindValue(":nPorcentajeIva2",this->nPorcentajeIVA2);
     cab_ped.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
     cab_ped.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
     cab_ped.bindValue(":nPorcentajeRecargoEq1",this->nRec1);
     cab_ped.bindValue(":nPorcentajeRecargoEq2",this->nRec2);
     cab_ped.bindValue(":nPorcentajeRecargoEq3",this->nRec3);
     cab_ped.bindValue(":nPorcentajeRecargoEq4",this->nRec4);
     if(!cab_ped.exec()){
         QMessageBox::critical(NULL,"error al guardar datos Pedido:", cab_ped.lastError().text());
     } else {
         this->id = cab_ped.lastInsertId().toInt();
         QString cSQL = "Select * from ped_cli where id ="+QString::number(this->id);
         RecuperarPedido(cSQL);
     }

}
// Guardar el Pedido
void Pedidos::GuardarPedido(int nId_Pedido) {
QSqlQuery cab_ped(QSqlDatabase::database("empresa"));

if (this->nPedido == 0)
    this->nPedido = NuevoNumeroPedido();



cab_ped.prepare( "update ped_cli set "
                 "cCif = :cCif,"
                 "cCliente = :cCliente,"
                 "cCodigoCliente =:cCodigoCliente,"
                 "cCP = :cCP,"
                 "cDireccion = :cDireccion,"
                 "cDireccion2 = :cDireccion2,"
                 "cPais = :cPais,"
                 "cPedido = :cPedido,"
                 "nPedido = :nPedido,"
                 "cPoblacion = :cPoblacion,"
                 "cProvincia = :cProvincia,"
                 "dFecha = :dFecha,"
                 "dFechaFactura = :dFechaFactura,"
                 "id_Cliente = :id_Cliente,"
                 "lFacturado = :lFacturado,"
                 "lImpreso = :lImpreso,"
                 "lRecargoEquivalencia =:lRecargoEquivalencia,"
                 "cFactura =:cFactura,"
                   "nDto = :nDto,"
                   "rDto = :rDto,"
                "nPorcentajeIva1 = :nPorcentajeIva1,"
                 "nPorcentajeIva2 = :nPorcentajeIva2,"
                 "nPorcentajeIva3 = :nPorcentajeIva3,"
                 "nPorcentajeIva4 = :nPorcentajeIva4,"
                 "nPorcentajeRecargoEq1 = :nPorcentajeRecargoEq1,"
                "nPorcentajeRecargoEq2 = :nPorcentajeRecargoEq2,"
                 "nPorcentajeRecargoEq3 = :nPorcentajeRecargoEq3,"
                 "nPorcentajeRecargoEq4 = :nPorcentajeRecargoEq4,"
                 "rACuenta = :rACuenta,"
                 "rBase1 = :rBase1,"
                 "rBase2 = :rBase2,"
                "rBase3 = :rBase3,"
                 "rBase4 = :rBase4,"
                 "rBaseTotal = :rBaseTotal,"
                 "rImporteIva1 = :rImporteIva1,"
                 "rImporteIva2 = :rImporteIva2,"
                 "rImporteIva3 = :rImporteIva3,"
                 "rImporteIva4 = :rImporteIva4,"
                 "rImporteRecargoEq1 = :rImporteRecargoEq1,"
                 "rImporteRecargoEq2 = :rImporteRecargoEq2,"
                 "rImporteRecargoEq3 = :rImporteRecargoEq3,"
                 "rImporteRecargoEq4 = :rImporteRecargoEq4,"
                 "rIvaTotal = :rIvaTotal,"
                 "rRecargoEqTotal = :rRecargoEqTotal,"
                 "rSubtotal = :rSubtotal,"
                 "rTotal1 = :rTotal1,"
                 "rTotal2 = :rTotal2,"
                 "rTotal3 = :rTotal3,"
                 "rTotal4 = :rTotal4,"
                 "rTotalPedido = :rTotalPedido,"
                 "tComentario = :tComentario,"
                 "lTraspasadoAlb = :lTraspasadoAlb,"
                 "lTraspasadoFac =:lTraspasadoFac,"
                 "cDireccionEntrega =:cDireccionEntrega,"
                 "cDireccionEntrega2 =:cDireccionEntrega2,"
                   "cCPEntrega =:cCPEntrega,"
                   "cPoblacionEntrega =:cPoblacionEntrega,"
                   "cProvinciaEntrega =:cProvinciaEntrega,"
                   "cPaisEntrega =:cPaisEntrega,"
                   "lEnviado =:lEnviado,"
                   "lCompleto =:lCompleto,"
                   "lEntregado =:lEntregado,"
                   "dFechaLimiteEntrega =:dFechaLimiteEntrega"
                 " where id =:Id");




// Pasamos valores reales a la Select

cab_ped.bindValue(":cCif",this->cCif);
cab_ped.bindValue(":cCliente", this->cCliente);
cab_ped.bindValue(":cCodigoCliente",this->cCodigoCliente);
cab_ped.bindValue(":cCP", this->cCp);
cab_ped.bindValue(":cDireccion",this->cDireccion);
cab_ped.bindValue(":cDireccion2",this->cDireccion2);
cab_ped.bindValue(":cPais", this->cPais);
cab_ped.bindValue(":cPedido", this->cPedido);
cab_ped.bindValue(":nPedido", this->nPedido);
cab_ped.bindValue(":cPoblacion", this->cPoblacion);
cab_ped.bindValue(":cProvincia", this->cProvincia);
cab_ped.bindValue(":dFecha", this->dFecha);
cab_ped.bindValue(":dFechaFactura", this->dFechaFactura);
cab_ped.bindValue(":id_Cliente", this->iId_Cliente);
cab_ped.bindValue(":lFacturado", this->lFacturado);
cab_ped.bindValue(":lImpreso",this->lImpreso);
cab_ped.bindValue(":lRecargoEquivalencia", this->lRecargoEquivalencia);
cab_ped.bindValue(":cFactura", this->cFactura);
cab_ped.bindValue(":nDto", this->nDto);
cab_ped.bindValue(":rDto", this->rImporteDescuento);
cab_ped.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
cab_ped.bindValue(":nPorcentajeIva2",this->nPorcentajeIVA2);
cab_ped.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
cab_ped.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
cab_ped.bindValue(":nPorcentajeRecargoEq1", this->nRec1);
cab_ped.bindValue(":nPorcentajeRecargoEq2", this->nRec2);
cab_ped.bindValue(":nPorcentajeRecargoEq3", this->nRec3);
cab_ped.bindValue(":nPorcentajeRecargoEq4", this->nRec4);
cab_ped.bindValue(":rACuenta", this->rACuenta);
cab_ped.bindValue(":rBase1",this->rBase1);
cab_ped.bindValue(":rBase2",this->rBase2);
cab_ped.bindValue(":rBase3",this->rBase3);
cab_ped.bindValue(":rBase4",this->rBase4);
cab_ped.bindValue(":rBaseTotal",this->rBase);
cab_ped.bindValue(":rImporteIva1",this->rIVA1);
cab_ped.bindValue(":rImporteIva2",this->rIVA2);
cab_ped.bindValue(":rImporteIva3",this->rIVA3);
cab_ped.bindValue(":rImporteIva4", this->rIVA4);
cab_ped.bindValue(":rImporteRecargoEq1", this->rRecargoEq1);
cab_ped.bindValue(":rImporteRecargoEq2", this->rRecargoEq2);
cab_ped.bindValue(":rImporteRecargoEq3", this->rRecargoEq3);
cab_ped.bindValue(":rImporteRecargoEq4", this->rRecargoEq4);
cab_ped.bindValue(":rIvaTotal",this->rImporteIva);
cab_ped.bindValue(":rRecargoEqTotal",this->rRecargoEqTotal);
cab_ped.bindValue(":rSubtotal",this->rSubtotal);
cab_ped.bindValue(":rTotal1",this->rTotal1);
cab_ped.bindValue(":rTotal2",this->rTotal2);
cab_ped.bindValue(":rTotal3",this->rTotal3);
cab_ped.bindValue(":rTotal4",this->rTotal4);
cab_ped.bindValue(":rTotalPedido",this->rTotal);
cab_ped.bindValue(":tComentario",this->tComentario);
cab_ped.bindValue(":lTraspasadoAlb",this->lTraspasadoAlb);
cab_ped.bindValue(":lTraspasadoFac",this->lTraspasadoFac);
cab_ped.bindValue(":cDireccionEntrega", this->cDireccionEntrega);
cab_ped.bindValue(":cDireccionEntrega2", this->cDireccionEntrega2);
cab_ped.bindValue(":cCPEntrega", this->cCPEntrega);
cab_ped.bindValue(":cPoblacionEntrega",this->cPoblacionEntrega);
cab_ped.bindValue(":cProvinciaEntrega", this->cProvinciaEntrega);
cab_ped.bindValue(":cPaisEntrega",this->cPaisEntrega);
cab_ped.bindValue(":lEnviado",this->lEnviado);
cab_ped.bindValue(":lCompleto", this->lCompleto);
cab_ped.bindValue(":lEntregado", this->lEntregado);
cab_ped.bindValue(":dFechaLimiteEntrega", this->dFechaLimiteEntrega);
cab_ped.bindValue(":Id",this->id);

if(!cab_ped.exec()){
    QMessageBox::critical(NULL,QObject::QObject::tr("error al guardar datos Pedido:"), cab_ped.lastError().text());
    qDebug() << cab_ped.lastQuery();
} else {
    QMessageBox::information(NULL,QObject::QObject::tr("Guardar datos"),QObject::QObject::tr("El Pedido se ha guardado correctamente:"),
                             QObject::QObject::tr("Ok"));
    QString cSQL = "Select * from ped_cli where id ="+QString::number(nId_Pedido);
    RecuperarPedido(cSQL);
}


}

void Pedidos::RecuperarPedido(QString cSQL){
    cab_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
    cab_ped->prepare(cSQL);
    if( !cab_ped->exec() ) {
        QMessageBox::critical(NULL, "error:", cab_ped->lastError().text());
    } else {
        if (cab_ped->next()) {
            QSqlRecord registro = cab_ped->record();
            this->id = registro.field("id").value().toInt();
            this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
            this->cPedido = registro.field("cPedido").value().toString();
            this->nPedido = registro.field("nPedido").value().toInt();
            this->dFecha = registro.field("dFecha").value().toDate();
            this->dFechaFactura = registro.field("dFechaFactura").value().toDate();
            this->iId_Cliente = registro.field("iId_Cliente").value().toInt();
            this->cCliente = registro.field("cCliente").value().toString();
            this->cDireccion = registro.field("cDireccion").value().toString();
            this->cDireccion2 = registro.field("cDireccion2").value().toString();
            this->cCp = registro.field("cCp").value().toString();
            this->cPoblacion = registro.field("cPoblacion").value().toString();
            this->cProvincia = registro.field("cProvincia").value().toString();
            this->cPais = registro.field("cPais").value().toString();
            this->cCif =registro.field("cCif").value().toString();
            this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toInt();
            this->rSubtotal = registro.field("rSubtotal").value().toDouble();
            this->nDto = registro.field("nDto").value().toInt();
            this->rImporteDescuento = registro.field("rDto").value().toDouble();
            this->rBase = registro.field("rBaseTotal").value().toDouble();
            this->nIva = registro.field("nIva").value().toInt();
            this->rImporteIva = registro.field("rIvaTotal").value().toDouble();
            this->rTotal = registro.field("rTotalPedido").value().toDouble();
            this->lImpreso = registro.field("lImpreso").value().toInt();
            this->lFacturado = registro.field("lFacturado").value().toInt();
            this->tComentario = registro.field("tComentario").value().toString();
            this->rBase1 = registro.field("rBase1").value().toDouble();
            this->rBase2 = registro.field("rBase2").value().toDouble();
            this->rBase3 = registro.field("rBase3").value().toDouble();
            this->rBase4 = registro.field("rBase4").value().toDouble();
            this->nPorcentajeIVA1 = registro.field("nPorcentajeIVA1").value().toInt();
            this->nPorcentajeIVA2 = registro.field("nPorcentajeIVA2").value().toInt();
            this->nPorcentajeIVA3 = registro.field("nPorcentajeIVA3").value().toInt();
            this->nPorcentajeIVA4 = registro.field("nPorcentajeIVA4").value().toInt();
            this->rIVA1 = registro.field("rImporteIva1").value().toDouble();
            this->rIVA2 = registro.field("rImporteIva2").value().toDouble();
            this->rIVA3 = registro.field("rImporteIva3").value().toDouble();
            this->rIVA4 = registro.field("rImporteIva4").value().toDouble();
            this->rTotal1 = registro.field("rTotal1").value().toDouble();
            this->rTotal2 = registro.field("rTotal2").value().toDouble();
            this->rTotal3 = registro.field("rTotal3").value().toDouble();
            this->rTotal4 = registro.field("rTotal4").value().toDouble();
            this->nRec1 = registro.field("nPorcentajeRecargoEq1").value().toDouble();
            this->nRec2 = registro.field("nPorcentajeRecargoEq2").value().toDouble();
            this->nRec3 = registro.field("nPorcentajeRecargoEq3").value().toDouble();
            this->nRec4 = registro.field("nPorcentajeRecargoEq4").value().toDouble();
            this->rRecargoEq1 = registro.field("rImporteRecargoEq1").value().toDouble();
            this->rRecargoEq2 = registro.field("rImporteRecargoEq2").value().toDouble();
            this->rRecargoEq3 = registro.field("nImporteRecargoEq3").value().toDouble();
            this->rRecargoEq4 = registro.field("rImporteRecargoEq4").value().toDouble();
            this->rRecargoEqTotal = registro.field("rRecargoEqTotal").value().toDouble();
            this->rACuenta = registro.field("rACuenta").value().toDouble();
            this->lTraspasadoAlb = registro.field("lTraspasadoAlb").value().toInt();
            this->lTraspasadoFac = registro.field("lTraspasadoFac").value().toInt();
            this->cDireccionEntrega = registro.field("cDireccionEntrega").value().toString();
            this->cDireccionEntrega2 = registro.field("cDireccionEntrega2").value().toString();
            this->cCPEntrega = registro.field("cCPEntrega").value().toString();
            this->cPoblacionEntrega = registro.field("cPoblacionEntrega").value().toString();
            this->cProvinciaEntrega = registro.field("cProvinciaEntrega").value().toString();
            this->cPaisEntrega = registro.field("cPaisEntrega").value().toString();
            this->lEnviado = registro.field("lEnviado").value().toInt();
            this->lCompleto = registro.field("lCompleto").value().toInt();
            this->lEntregado = registro.field("lEntregado").value().toInt();
            this->dFechaLimiteEntrega = registro.field("dFechaLimiteEntrega").value().toDate();


           }
    }
}

int Pedidos::NuevoNumeroPedido() {
QSqlQuery cab_ped(QSqlDatabase::database("empresa"));
int nPedido;
cab_ped.prepare("Select nPedido from ped_cli order by nPedido desc limit 0,1");
if(cab_ped.exec()) {
    cab_ped.next();
    nPedido= cab_ped.value(0).toInt();
    nPedido ++;
} else {
     QMessageBox::critical(NULL, "error:", cab_ped.lastError().text());
}
return nPedido;


}

void Pedidos::AnadirLineaPedido(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                             double porcdto, double dto, double total, double nPorcIva)
{
QSqlDatabase::database("empresa").transaction();
bool lCorrecto;
lCorrecto = true;
QSqlQuery *Qlin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
Qlin_ped->prepare("INSERT INTO lin_ped (Id_Cab,cCodigo,nCantidad,cDescripcion,rPvp,nDto,rDto,rSubTotal,rTotal,nPorcIva)"
                  " VALUES(:id_cab,:cCodigo,:nCantidad,:cDescripcion,:rPvp,:nDto,:rDto,:rSubTotal,:rTotal,:nPorcIva)");
Qlin_ped->bindValue(":id_cab",id_cab);
Qlin_ped->bindValue(":cCodigo",cCodigo);
Qlin_ped->bindValue(":nCantidad",nCantidad);
Qlin_ped->bindValue(":cDescripcion",cDescripcion);
Qlin_ped->bindValue(":rPvp",pvp);
Qlin_ped->bindValue(":nDto",porcdto);
Qlin_ped->bindValue(":rDto",dto);
Qlin_ped->bindValue(":rSubTotal",subtotal);
Qlin_ped->bindValue(":rTotal",total);
Qlin_ped->bindValue(":nPorcIva",nPorcIva);
if (!Qlin_ped->exec()){
   QMessageBox::critical(NULL,"error al guardar datos línea Pedido:", Qlin_ped->lastError().text());
   lCorrecto = false;
}
delete Qlin_ped;
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
    QMessageBox::warning(NULL,QObject::tr("Gestión de Pedidoes"),QObject::tr("No se puede actualizar la ficha del artículo")+
                         QArticulos->lastError().text(),QObject::tr("Aceptar"));
    lCorrecto = false;
}
if(lCorrecto) {
    QSqlDatabase::database("empresa").commit();
} else {
    QMessageBox::warning(NULL,QObject::tr("Gestión de Pedidoes"),QObject::tr("Se desharán los últimos cambios"));
    QSqlDatabase::database("empresa").rollback();
}
}

void Pedidos::ModificarLineaPedido(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
// Borro valores anteriores registro Artículo (Los valores se sacan del registro de lin_ped ya que aun no se ha modificado)uhjmkk,kl,,
QSqlQuery *Lin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
Lin_ped->prepare("Select * from lin_ped where id =:nId");
Lin_ped->bindValue(":nId",id_lin);
if(Lin_ped->exec()) {
    Lin_ped->next();
    QSqlRecord record = Lin_ped->record();

    QSqlQuery *QArticulos = new QSqlQuery(QSqlDatabase::database("empresa"));
    QArticulos->prepare("update articulos set "
                        "nUnidadesVendidas = nUnidadesVendidas -:nCantidad,"
                        "nStockReal = nStockReal - :nCantidad2, "
                        "rAcumuladoVentas = rAcumuladoVentas + :rTotal "
                        "where cCodigo= :cCodigo");
    QArticulos->bindValue(":dUltimaVenta",QDate::currentDate());
    QArticulos->bindValue(":nCantidad",record.field("nCantidad").value().toDouble());
    QArticulos->bindValue(":nCantidad2",record.field("nCantidad").value().toDouble());
    QArticulos->bindValue(":rTotal",record.field("rTotal").value().toDouble());
    QArticulos->bindValue(":cCodigo",record.field("cCodigo").value().toString());
    QArticulos->exec();
    delete QArticulos;
    delete Lin_ped;

}
// Actualizo Línea Pedido
QSqlQuery *Qlin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
Qlin_ped->prepare("update lin_ped  set "
                  "cCodigo =:cCodigo,"
                  "nCantidad =:nCantidad,"
                  "cDescripcion =:cDescripcion,"
                  "rPvp =:rPvp,"
                  "nDto =:nDto,"
                  "rDto =:rDto,"
                  "rSubTotal =:rSubTotal,"
                  "rTotal =:rTotal,"
                  "nPorcIva =:nPorcIva where id = :id_lin");
Qlin_ped->bindValue(":id_lin",id_lin);
Qlin_ped->bindValue(":cCodigo",cCodigo);
Qlin_ped->bindValue(":nCantidad",nCantidad);
Qlin_ped->bindValue(":cDescripcion",cDescripcion);
Qlin_ped->bindValue(":rPvp",pvp);
Qlin_ped->bindValue(":nDto",porcdto);
Qlin_ped->bindValue(":rDto",dto);
Qlin_ped->bindValue(":rSubTotal",subtotal);
Qlin_ped->bindValue(":rTotal",total);
Qlin_ped->bindValue(":nPorcIva",nPorcIva);
if (!Qlin_ped->exec()){
   QMessageBox::critical(NULL,"error al modificar datos línea Pedido:", Qlin_ped->lastError().text());
}
delete Qlin_ped;
// Actualizo ficha artículo
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
delete QArticulos;

}

void Pedidos::BorrarLineaPedido(int id_lin)
{
    if (id_lin !=0) {
        QSqlQuery *qrylin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
        frmDecision msgBox;
        msgBox.Inicializar(QObject::tr("Borrar línea"),QObject::tr("Está a punto de borrar la línea de la Pedido"),
                           QObject::tr("¿Desea continuar?"),"",QObject::tr("Sí"),QObject::tr("No"));
        int elegido = msgBox.exec();
       if(elegido == 1) {
           qrylin_ped->prepare("Select * from lin_ped where id = :id_lin");
           qrylin_ped->bindValue(":id_lin",id_lin);
           if (qrylin_ped->exec()) {
               QSqlRecord record = qrylin_ped->record();
               // Reponer Artículo
               QSqlQuery *QArticulos = new QSqlQuery(QSqlDatabase::database("empresa"));
               QArticulos->prepare("update articulos set "
                                   "nUnidadesVendidas = nUnidadesVendidas -:nCantidad,"
                                   "nStockReal = nStockReal - :nCantidad2, "
                                   "rAcumuladoVentas = rAcumuladoVentas + :rTotal "
                                   "where cCodigo= :cCodigo");
               QArticulos->bindValue(":dUltimaVenta",QDate::currentDate());
               QArticulos->bindValue(":nCantidad",record.field("nCantidad").value().toDouble());
               QArticulos->bindValue(":nCantidad2",record.field("nCantidad").value().toDouble());
               QArticulos->bindValue(":rTotal",record.field("rTotal").value().toDouble());
               QArticulos->bindValue(":cCodigo",record.field("cCodigo").value().toString());
               QArticulos->exec();
               delete QArticulos;

           }
            qrylin_ped->prepare("Delete from lin_ped where id = :id_lin");
            qrylin_ped->bindValue(":id_lin",id_lin);
            if(!qrylin_ped->exec()){
               QMessageBox::critical(NULL,QObject::tr("Borrar línea"),QObject::tr("Falló el borrado de la línea de Pedido"),
                                     QObject::tr("&Aceptar"));
            }
            delete qrylin_ped;
            calcularPedido();
         }
    } else {
        QMessageBox::critical(NULL,QObject::tr("Borrar Línea Pedido"),QObject::tr("Debe seleccionar una línea para poder borrar"),
                              QObject::tr("OK"));
    }
}

void Pedidos::calcularPedido()
{
Configuracion *o_config = new Configuracion();
o_config->CargarDatos();
// Reseteo valores
this->rSubtotal = 0;
this->rImporteDescuento =0;
this->rBase = 0;
this->rImporteIva = 0;
this->rRecargoEqTotal = 0;
this->rTotal = 0;
this->rBase1 =0;
this->rBase2 =0;
this->rBase3 = 0;
this->rBase4 = 0;
this->rIVA1 = 0;
this->rIVA2 = 0;
this->rIVA3 = 0;
this->rIVA4 = 0;
this->rRecargoEq1 = 0;
this->rRecargoEq2 = 0;
this->rRecargoEq3 = 0;
this->rRecargoEq4 = 0;
this->rTotal1 =0;
this->rTotal2 =0;
this->rTotal3 =0;
this->rTotal4 = 0;

QSqlQuery *Qlin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));
Qlin_ped->prepare("Select * from lin_ped where id_cab = :nId");
Qlin_ped->bindValue(":nId",this->id);
if (Qlin_ped->exec()) {
    QSqlRecord record = Qlin_ped->record();
    while (Qlin_ped->next()) {
        record = Qlin_ped->record();

        this->rSubtotal = this->rSubtotal + record.field("rSubtotal").value().toDouble();
        this->rImporteDescuento = this->rImporteDescuento + record.field("rDto").value().toDouble();
        this->rBase = (this->rSubtotal - this->rImporteDescuento);


        // IVA 1
        if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA1) {
            this->rBase1 = this->rBase1 + record.field("rTotal").value().toDouble();
            // RE 1
            if(this->lRecargoEquivalencia == 1)
                this->rRecargoEq1 = (this->rBase1 * this->nRec1)/100;

            this->rIVA1 = (this->rBase1 * this->nPorcentajeIVA1)/100;
            this->rTotal1 = this->rBase1 + this->rIVA1 + this->rRecargoEq1;
        }
        // IVA 2
        if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA2) {
            this->rBase2 = this->rBase2 + record.field("rTotal").value().toDouble();
            // RE 2
            if(this->lRecargoEquivalencia == 1)
                this->rRecargoEq2 = (this->rBase2 * this->nRec2)/100;
            this->rIVA2 = (this->rBase2 * this->nPorcentajeIVA2)/100;
            this->rTotal2 = this->rBase2 + this->rIVA2 + this->rRecargoEq2;
        }
        // IVA 3
        if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA3) {
            this->rBase3 = this->rBase3 + record.field("rTotal").value().toDouble();
            // RE 3
            if(this->lRecargoEquivalencia == 1)
                this->rRecargoEq3 = (this->rBase3 * this->nRec3)/100;

            this->rIVA3 = (this->rBase3 * this->nPorcentajeIVA3)/100;
            this->rTotal3 = this->rBase3 + this->rIVA3 + this->rRecargoEq3;
        }
        // IVA 4
        if (record.field("nPorcIva").value().toDouble() == this->nPorcentajeIVA4) {
            this->rBase4 = this->rBase4 + record.field("rTotal").value().toDouble();

            // RE 4
            if(this->lRecargoEquivalencia == 1)
                this->rRecargoEq4 = (this->rBase4 * this->nRec4)/100;

            this->rIVA4 = (this->rBase4 * this->nPorcentajeIVA4)/100;
            this->rTotal4 = this->rBase4 + this->rIVA4 + this->rRecargoEq4;

        }
        this->rImporteIva =  (this->rIVA1 +  this->rIVA2 + this->rIVA3 + this->rIVA4);
        this->rRecargoEqTotal = (this->rRecargoEq1 + this->rRecargoEq2 + this->rRecargoEq3 + this->rRecargoEq4);
        this->rTotal = this->rBase + this->rImporteIva + this->rRecargoEqTotal;
    }
}
}

void Pedidos::FacturarPedido()
{

}

// getters
int  Pedidos::Getid() {
    return this->id;
}

QString Pedidos::getcCodigoCliente() {
    return this->cCodigoCliente;
}

QString Pedidos::getcPedido()
{
    return this->cPedido;
}
int Pedidos::getnPedido()
{
    return this->nPedido;
}


QDate Pedidos::getdFecha() {
return this->dFecha;
}
QDate Pedidos::getdFechaFactura() {
return this->dFechaFactura;
}

QString Pedidos::getcFactura()
{
return this->cFactura;
}
int Pedidos::getiId_Cliente() {
return this->iId_Cliente;
}
QString Pedidos::getcCliente() {
return this->cCliente;
}
QString Pedidos::getcDireccion() {
return this->cDireccion;
}
QString Pedidos::getcDireccion2() {
return this->cDireccion2;
}
QString Pedidos::getcCp() {
return this->cCp;
}
QString Pedidos::getcPoblacion() {
return this->cPoblacion;
}
QString Pedidos::getcProvincia() {
return this->cProvincia;
}
QString Pedidos::getcPais() {
return this->cPais;
}
QString Pedidos::getcCif() {
return this->cCif;
}
int Pedidos::getlRecargoEquivalencia() {
return this->lRecargoEquivalencia;
}
double Pedidos::getrSubtotal() {
return this->rSubtotal;
}
int Pedidos::getnDto() {
return this->nDto;
}

double Pedidos::getrImporteDescuento() {
return this->rImporteDescuento;
}
double Pedidos::getrBase() {
return this->rBase;
}
int Pedidos::getnIva() {
return this->nIva;
}
double Pedidos::getrImporteIva() {
return this->rImporteIva;
}
double Pedidos::getrTotal() {
return this->rTotal;
}
int Pedidos::getlImpreso() {
return this->lImpreso;
}
int Pedidos::getlFacturado() {
return this->lFacturado;
}
QString Pedidos::gettComentario() {
return this->tComentario;
}
double Pedidos::getrBase1() {
return this->rBase1;
}
double Pedidos::getrBase2() {
return this->rBase2;
}
double Pedidos::getrBase3() {
return this->rBase3;
}
double Pedidos::getrBase4() {
return this->rBase4;
}
int Pedidos::getnPorcentajeIVA1() {
return this->nPorcentajeIVA1;
}
int Pedidos::getnPorcentajeIVA2() {
return this->nPorcentajeIVA2;
}
int Pedidos::getnPorcentajeIVA3() {
return this->nPorcentajeIVA3;
}
int Pedidos::getnPorcentajeIVA4() {
return this->nPorcentajeIVA4;
}
double Pedidos::getrIVA1() {
return this->rIVA1;
}
double Pedidos::getrIVA2() {
return this->rIVA2;
}
double Pedidos::getrIVA3() {
return this->rIVA3;
}
double Pedidos::getrIVA4() {
return this->rIVA4;
}
double Pedidos::getrTotal1() {
return this->rTotal1;
}
double Pedidos::getrTotal2() {
return this->rTotal2;
}
double Pedidos::getrTotal3() {
return this->rTotal3;
}
double Pedidos::getrTotal4() {
return this->rTotal4;
}
double Pedidos::getnRec1() {
return this->nRec1;
}
double Pedidos::getnRec2() {
return this->nRec2;
}
double Pedidos::getnRec3() {
return this->nRec3;
}
double Pedidos::getnRec4() {
return this->nRec4;
}
double Pedidos::getrRecargoEq1() {
return this->rRecargoEq1;
}
double Pedidos::getrRecargoEq2() {
return this->rRecargoEq2;
}
double Pedidos::getrRecargoEq3() {
return this->rRecargoEq3;
}
double Pedidos::getrRecargoEq4() {
return this->rRecargoEq4;
}
double Pedidos::getrRecargoEqTotal() {
return this->rRecargoEqTotal;
}
double Pedidos::getrACuenta() {
return this->rACuenta;
}
double Pedidos::getrImportePendiente() {
return this->rImportePendiente;
}


int Pedidos::getlTraspasadoAlb()
{
    return this->lTraspasadoAlb;
}

int Pedidos::getlTraspasadoFac()
{
    return this->lTraspasadoFac;
}

QString Pedidos::getcDireccionEntrega()
{
    return this->cDireccionEntrega;
}

QString Pedidos::getcDireccionEntrega2()
{
    return this->cDireccionEntrega2;
}

QString Pedidos::getcCPEntrega()
{
    return this->cCPEntrega;
}

QString Pedidos::getcPoblacionEntrega()
{
    return this->cPoblacionEntrega;
}

QString Pedidos::getcProvinciaEntrega()
{
    return this->cProvinciaEntrega;
}

QString Pedidos::getcPaisEntrega()
{
    return this->cPaisEntrega;
}

int Pedidos::getlEnviado()
{
    return this->lEnviado;
}

int Pedidos::getlCompleto()
{
    return this->lCompleto;
}

int Pedidos::getlEntregado()
{
    return this->lEntregado;
}

QDate Pedidos::getdFechaLimiteEntrega()
{
    return this->dFechaLimiteEntrega;
}


// setters
void Pedidos::setid( int iID_Pedido) {
this->id = iID_Pedido;
}
void Pedidos::setcCodigoCliente(QString cCodigoCliente) {
this->cCodigoCliente = cCodigoCliente;
}

void Pedidos::setcPedido(QString cPedido)
{
    this->cPedido = cPedido;
}

void Pedidos::setnPedido(int nPedido)
{
    this->nPedido = nPedido;
}

void Pedidos::setdFecha(QDate dFecha) {
this->dFecha = dFecha;
}
void Pedidos::setdFechaFactura(QDate dFechaFactura) {
this->dFechaFactura = dFechaFactura;
}

void Pedidos::setcFactura(QString cFactura)
{
this->cFactura = cFactura;
}
void Pedidos::setiId_Cliente(int iId_Cliente) {
this->iId_Cliente = iId_Cliente;
}
void Pedidos::setcCliente(QString cCliente) {
this->cCliente = cCliente;
}
void Pedidos::setcDireccion(QString cDireccion) {
this->cDireccion = cDireccion;
}
void Pedidos::setcDireccion2(QString cDireccion2) {
this->cDireccion2 = cDireccion2;
}
void Pedidos::setcCp(QString cCp) {
this->cCp = cCp;
}
void Pedidos::setcPoblacion(QString cPoblacion) {
this->cPoblacion = cPoblacion;
}
void Pedidos::setcProvincia(QString cProvincia) {
this->cProvincia = cProvincia;}
void Pedidos::setcPais(QString cPais) {
this->cPais = cPais;
}
void Pedidos::setcCif(QString cCif) {
this->cCif = cCif;
}

void Pedidos::setlRecargoEquivalencia(int lRecargoEquivalencia)
{
this->lRecargoEquivalencia = lRecargoEquivalencia;
}

void Pedidos::setrSubtotal(double rSubtotal) {
this->rSubtotal = rSubtotal;
}
void Pedidos::setnDto(int nDto) {
this->nDto = nDto;
}
void Pedidos::setrImporteDescuento(double rImporteDescuento) {
this->rImporteDescuento = rImporteDescuento;
}

void Pedidos::setrBase(double rBase) {
this->rBase = rBase;
}
void Pedidos::setnIva(int nIva) {
this->nIva = nIva;
}
void Pedidos::setrImporteIva(double rImporteIva) {
this->rImporteIva = rImporteIva;
}
void Pedidos::setrTotal(double rTotal) {
this->rTotal = rTotal;
}
void Pedidos::setlImpreso(int lImpreso) {
this->lImpreso = lImpreso;
}
void Pedidos::setlFacturado(int lFacturado) {
this->lFacturado = lFacturado;
}
void Pedidos::settComentario(QString tComentario) {
this->tComentario = tComentario;
}
void Pedidos::setrBase1(double rBase1) {
this->rBase1 = rBase1;
}
void Pedidos::setrBase2(double rBase2) {
this->rBase2 = rBase2;
}
void Pedidos::setrBase3(double rBase3) {
this->rBase3 = rBase3;
}
void Pedidos::setrBase4(double rBase4) {
this->rBase4 = rBase4;
}
void Pedidos::setnPorcentajeIVA1(int nPorcentajeIVA1) {
this->nPorcentajeIVA1 = nPorcentajeIVA1;
}
void Pedidos::setnPorcentajeIVA2(int nPorcentajeIVA2) {
this->nPorcentajeIVA2 = nPorcentajeIVA2;
}
void Pedidos::setnPorcentajeIVA3(int nPorcentajeIVA3) {
this->nPorcentajeIVA3 = nPorcentajeIVA3;
}
void Pedidos::setnPorcentajeIVA4(int nPorcentajeIVA4) {
this->nPorcentajeIVA4 = nPorcentajeIVA4;
}
void Pedidos::setrIVA1( double rIVA1) {
this->rIVA1 = rIVA1;
}
void Pedidos::setrIVA2( double rIVA2) {
this->rIVA2 = rIVA2;
}
void Pedidos::setrIVA3( double rIVA3) {
this->rIVA3 = rIVA3;
}
void Pedidos::setrIVA4( double rIVA4) {
this->rIVA4 = rIVA4;
}
void Pedidos::setrTotal1(double rTotal1) {
this->rTotal1 = rTotal1;
}
void Pedidos::setrTotal2(double rTotal2) {
this->rTotal2 = rTotal2;
}
void Pedidos::setrTotal3(double rTotal3) {
this->rTotal3 = rTotal3;
}
void Pedidos::setrTotal4(double rTotal4) {
this->rTotal4 = rTotal4;
}
void Pedidos::setnRec1(int nRec1) {
this->nRec1 = nRec1;
}
void Pedidos::setnRec2(int nRec2) {
this->nRec2 = nRec2;
}
void Pedidos::setnRec3(int nRec3) {
this->nRec3 = nRec3;
}
void Pedidos::setnRec4(int nRec4) {
this->nRec4 = nRec4;
}
void Pedidos::setrRecargoEq1(double rRecargoEq1) {
this->rRecargoEq1 = rRecargoEq1;
}
void Pedidos::setrRecargoEq2(double rRecargoEq2) {
this->rRecargoEq2 = rRecargoEq2;
}
void Pedidos::setrRecargoEq3(double rRecargoEq3) {
this->rRecargoEq3 = rRecargoEq3;
}
void Pedidos::setrRecargoEq4(double rRecargoEq4) {
this->rRecargoEq4 = rRecargoEq4;
}

void Pedidos::setrRecargoEqTotal(double rRecargoEqTotal)
{
this->rRecargoEqTotal = rRecargoEqTotal;
}

void Pedidos::setrACuenta(double rACuenta) {
    this->rACuenta = rACuenta;
}

void Pedidos::setlTraspasadoAlb(int lTraspasadoAlb)
{
    this->lTraspasadoAlb = lTraspasadoAlb;
}

void Pedidos::setlTraspasadoFac(int lTraspasadoFac)
{
    this->lTraspasadoFac = lTraspasadoFac;
}

void Pedidos::setcDireccionEntrega(QString cDireccionEntrega)
{
    this->cDireccionEntrega = cDireccionEntrega;
}

void Pedidos::setcDireccionEntrega2(QString cDireccionEntrega2)
{
    this->cDireccionEntrega2 = cDireccionEntrega2;
}

void Pedidos::setcCPEntrega(QString cCPEntrega)
{
    this->cCPEntrega = cCPEntrega;
}

void Pedidos::setcPoblacionEntrega(QString cPoblacionEntrega)
{
    this->cPoblacionEntrega = cPoblacionEntrega;
}

void Pedidos::setcProvinciaEntrega(QString cProvinciaEntrega)
{
    this->cProvinciaEntrega = cProvinciaEntrega;
}

void Pedidos::setcPaisEntrega(QString cPaisEntrega)
{
    this->cPaisEntrega = cPaisEntrega;
}

void Pedidos::setlEnviado(int lEnviado)
{
    this->lEnviado = lEnviado;
}

void Pedidos::setlCompleto(int lCompleto)
{
    this->lCompleto = lCompleto;
}

void Pedidos::setlEntregado(int lEntregado)
{
    this->lEntregado = lEntregado;
}

void Pedidos::setdFechaLimiteEntrega(QDate dFechaLimiteEntrega)
{
    this->dFechaLimiteEntrega = dFechaLimiteEntrega;
}


