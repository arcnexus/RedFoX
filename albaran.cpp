#include "albaran.h"
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


Albaran::Albaran(QObject *parent) :
    QObject(parent)
{
    this->id =0;
}

// Metodos utilidad Clase
void Albaran::AnadirAlbaran() {
    Configuracion *oConf =  new Configuracion();
    oConf->CargarDatos();
    this->nPorcentajeIVA1 = oConf->nIVA1;
    this->nPorcentajeIVA2 = oConf->nIVA2;
    this->nPorcentajeIVA3 = oConf->nIVA3;
    this->nPorcentajeIVA4 = oConf->nIVA4 ;
    this->lRecargoEquivalencia = 0;
    delete oConf;
    QSqlQuery cab_alb(QSqlDatabase::database("empresa"));
     cab_alb.prepare("INSERT INTO cab_alb (nAlbaran)"
                   " VALUES (:nAlbaran)");

     cab_alb.bindValue(":nAlbaran",this->nAlbaran);

     if(!cab_alb.exec()){
         QMessageBox::critical(NULL,"error al guardar datos Albaran:", cab_alb.lastError().text());
     } else {
         this->id = cab_alb.lastInsertId().toInt();
         QString cSQL = "Select * from cab_alb where id ="+QString::number(this->id);
         RecuperarAlbaran(cSQL);
     }

}
// Guardar el Albaran
void Albaran::GuardarAlbaran(int nId_Albaran) {
    QSqlQuery cab_alb(QSqlDatabase::database("empresa"));
    cab_alb.prepare( "update cab_alb set "
                     "cCif = :cCif,"
                     "cCliente = :cCliente,"
                     "cCodigoCliente =:cCodigoCliente,"
                     "cCP = :cCP,"
                     "cDireccion = :cDireccion,"
                     "cDireccion2 = :cDireccion2,"
                     "cPais = :cPais,"
                     "cPedidoCli = :cPedidoCli,"
                     "cPoblacion = :cPoblacion,"
                     "cProvincia = :cProvincia,"
                     "dFecha = :dFecha,"
                     "dFechaFactura = :dFechaFactura,"
                     "id_Cliente = :id_Cliente,"
                     "lFacturado = :lFacturado,"
                     "lImpreso = :lImpreso,"
                     "lRecargoEquivalencia =:lRecargoEquivalencia,"
                    "nAlbaran = :nAlbaran,"
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
                     "rDto = :rDto,"
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
                     "rTotalAlbaran = :rTotalAlbaran,"
                     "tComentario = :tComentario"
                     " where id =:Id");




    // Pasamos valores reales a la Select
    cab_alb.bindValue(":cCif",this->cCif);
    cab_alb.bindValue(":cCliente", this->cCliente);
    cab_alb.bindValue(":cCodigoCliente",this->cCodigoCliente);
    cab_alb.bindValue(":cCP",this->cCp);
    cab_alb.bindValue(":cDireccion", this->cDireccion);
    cab_alb.bindValue(":cDireccion2", this->cDireccion2);
    cab_alb.bindValue(":cPais",this->cPais);
    cab_alb.bindValue(":cPedidoCli", this->cPedidoCli);
    cab_alb.bindValue(":cPoblacion", this->cPoblacion);
    cab_alb.bindValue(":cProvincia",this->cProvincia);
    cab_alb.bindValue(":dFecha",this->dFecha);
    cab_alb.bindValue(":dFechaFactura",this->dFechaFactura);
    cab_alb.bindValue(":lFacturado",this->lFacturado);
    cab_alb.bindValue(":lImpreso", this->lImpreso);
    cab_alb.bindValue(":lRecargoEquivalencia", this->lRecargoEquivalencia);
    cab_alb.bindValue(":nAlbaran",this->nAlbaran);
    cab_alb.bindValue(":cFactura",this->cFactura);
    cab_alb.bindValue(":nDto",this->nDto);
    cab_alb.bindValue(":rDto",this->rImporteDescuento);
    cab_alb.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
    cab_alb.bindValue(":nPorcentajeIva2", this->nPorcentajeIVA2);
    cab_alb.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
    cab_alb.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
   cab_alb.bindValue(":nPorcentajeRecargoEq1",this->nRec1);
    cab_alb.bindValue(":nPorcentajeRecargoEq2",this->nRec2);
    cab_alb.bindValue(":nPorcentajeRecargoEq3",this->nRec3);
    cab_alb.bindValue(":nPorcentajeRecargoEq4",this->nRec4);
    cab_alb.bindValue(":rACuenta",this->rACuenta);
    cab_alb.bindValue(":rBase1",this->rBase1);
    cab_alb.bindValue(":rBase2", this->rBase2);
    cab_alb.bindValue(":rBase3",this->rBase3);
    cab_alb.bindValue(":rBase4",this->rBase4);
    cab_alb.bindValue(":rBaseTotal",this->rBase);
    cab_alb.bindValue(":rDto",this->rImporteDescuento);
    cab_alb.bindValue(":rImporteIva1",this->rIVA1);
    cab_alb.bindValue(":rImporteIva2",this->rIVA2);
    cab_alb.bindValue(":rImporteIva3",this->rIVA3);
   cab_alb.bindValue(":rImporteIva4",this->rIVA4);
    cab_alb.bindValue(":rImporteRecargoEq1",this->rRecargoEq1);
   cab_alb.bindValue(":rImporteRecargoEq2",this->rRecargoEq2);
    cab_alb.bindValue(":rImporteRecargoEq3",this->rRecargoEq3);
    cab_alb.bindValue(":rImporteRecargoEq4",this->rRecargoEq4);
    cab_alb.bindValue(":rIvaTotal", this->rImporteIva);
    cab_alb.bindValue(":rRecargoEqTotal",this->rRecargoEqTotal);
    cab_alb.bindValue(":rSubtotal",this->rSubtotal);
   cab_alb.bindValue(":rTotal1",this->rTotal1);
    cab_alb.bindValue(":rTotal2",this->rTotal2);
    cab_alb.bindValue(":rTotal3",this->rTotal3);
    cab_alb.bindValue(":rTotal4",this->rTotal4);
    cab_alb.bindValue(":rTotalAlbaran",this->rTotal);
    cab_alb.bindValue(":tComentario", this->tComentario);
    cab_alb.bindValue(":Id", this->id);

    if(!cab_alb.exec()){
        QMessageBox::critical(NULL,tr("error al guardar datos Albaran:"), cab_alb.lastError().text());
        qDebug() << cab_alb.lastQuery();
    } else {
        QMessageBox::information(NULL,tr("Guardar datos"),tr("El Albaran se ha guardado correctamente:"),tr("Ok"));
        QString cSQL = "Select * from cab_alb where id ="+QString::number(nId_Albaran);
        RecuperarAlbaran(cSQL);
    }


}

void Albaran::RecuperarAlbaran(QString cSQL){
        cab_alb = new QSqlQuery(QSqlDatabase::database("empresa"));
        cab_alb->prepare(cSQL);
        if( !cab_alb->exec() ) {
            QMessageBox::critical(NULL, "error:", cab_alb->lastError().text());
        } else {
            if (cab_alb->next()) {
                QSqlRecord registro = cab_alb->record();
                this->id = registro.field("id").value().toInt();
                this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
                this->nAlbaran = registro.field("nAlbaran").value().toInt();
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
                this->rImporteDescuento = registro.field("rImporteDescuento").value().toDouble();
                this->rBase = registro.field("rBase").value().toDouble();
                this->nIva = registro.field("nIva").value().toInt();
                this->rImporteIva = registro.field("rImporteIva").value().toDouble();
                this->rTotal = registro.field("rTotal").value().toDouble();
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
                this->rRecargoEqTotal = registro.field("rTotalRecargoEq").value().toDouble();
                this->rACuenta = registro.field("rACuenta").value().toDouble();
                this->rImportePendiente = registro.field("rImportePendiente").value().toDouble();
                this->cPedidoCli = registro.field("cPedidoCli").value().toString();


               }
        }
}

int Albaran::NuevoNumeroAlbaran() {
    QSqlQuery cab_alb(QSqlDatabase::database("empresa"));
    int nAlbaran;
    cab_alb.prepare("Select nAlbaran from cab_alb order by nAlbaran desc limit 0,1");
    if(cab_alb.exec()) {
        cab_alb.next();
        nAlbaran= cab_alb.value(0).toInt();
        nAlbaran ++;
    } else {
         QMessageBox::critical(NULL, "error:", cab_alb.lastError().text());
    }
    return nAlbaran;


}

void Albaran::AnadirLineaAlbaran(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
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
       QMessageBox::critical(NULL,"error al guardar datos línea Albaran:", Qlin_fac->lastError().text());
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

void Albaran::ModificarLineaAlbaran(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
    // Borro valores anteriores registro Artículo (Los valores se sacan del registro de lin_fac ya que aun no se ha modificado)uhjmkk,kl,,
    QSqlQuery *Lin_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
    Lin_fac->prepare("Select * from lin_fac where id =:nId");
    Lin_fac->bindValue(":nId",id_lin);
    if(Lin_fac->exec()) {
        Lin_fac->next();
        QSqlRecord record = Lin_fac->record();

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
        delete Lin_fac;

    }
    // Actualizo Línea Albaran
    QSqlQuery *Qlin_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
    Qlin_fac->prepare("update lin_fac  set "
                      "cCodigo =:cCodigo,"
                      "nCantidad =:nCantidad,"
                      "cDescripcion =:cDescripcion,"
                      "rPvp =:rPvp,"
                      "nDto =:nDto,"
                      "rDto =:rDto,"
                      "rSubTotal =:rSubTotal,"
                      "rTotal =:rTotal,"
                      "nPorcIva =:nPorcIva where id = :id_lin");
    Qlin_fac->bindValue(":id_lin",id_lin);
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
       QMessageBox::critical(NULL,"error al modificar datos línea Albaran:", Qlin_fac->lastError().text());
    }
    delete Qlin_fac;
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

void Albaran::BorrarLineaAlbaran(int id_lin)
{
    if (id_lin !=0) {
        QSqlQuery *qrylin_fac = new QSqlQuery(QSqlDatabase::database("empresa"));
        frmDecision msgBox;
        msgBox.Inicializar(tr("Borrar línea"),tr("Está a punto de borrar la línea de la Albaran"),
                           tr("¿Desea continuar?"),"",tr("Sí"),tr("No"));
        int elegido = msgBox.exec();
       if(elegido == 1) {
           qrylin_fac->prepare("Select * from lin_fac where id = :id_lin");
           qrylin_fac->bindValue(":id_lin",id_lin);
           if (qrylin_fac->exec()) {
               QSqlRecord record = qrylin_fac->record();
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
            qrylin_fac->prepare("Delete from lin_fac where id = :id_lin");
            qrylin_fac->bindValue(":id_lin",id_lin);
            if(!qrylin_fac->exec()){
               QMessageBox::critical(NULL,tr("Borrar línea"),tr("Falló el borrado de la línea de Albaran"),tr("&Aceptar"));
            }
            delete qrylin_fac;
            calcularAlbaran();
         }
    } else {
        QMessageBox::critical(NULL,tr("Borrar Línea Albaran"),tr("Debe seleccionar una línea para poder borrar"),tr("OK"));
    }
}

void Albaran::calcularAlbaran()
{
    Configuracion *o_config = new Configuracion();
    o_config->CargarDatos();
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
            this->rBase = (this->rSubtotal - this->rImporteDescuento);


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

void Albaran::CobrarAlbaran()
{
    // marcar Albaran como cobrada
    this->lFacturado = true;
    QSqlQuery *Cliente = new QSqlQuery(QSqlDatabase::database("empresa"));

    // Añadimos acumulados ficha cliente.
    Cliente->prepare("Update clientes set dFechaUltimaCompra = :dFechaUltimaCompra, "
                             "rAcumuladoVentas = rAcumuladoVentas + :rAcumuladoVentas,"
                             "rVentasEjercicio = rVentasEjercicio + :rVentasEjercicio "
                             " where cCodigoCliente = :cCodigoCliente");
    Cliente->bindValue(":dFechaUltimaCompra",QDate::currentDate());
    Cliente->bindValue(":rAcumuladoVentas",this->rTotal);
    Cliente->bindValue(":rVentasEjercicio",this->rTotal);
    Cliente->bindValue(":cCodigoCliente",this->cCodigoCliente);
    if (!Cliente->exec()){
        QMessageBox::warning(NULL,tr("Añadir Acumulados"),
                             tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                             tr("OK"));
    }
    QSqlQuery *cab_alb = new QSqlQuery(QSqlDatabase::database("empresa"));
    cab_alb->prepare("update cab_alb set lCobrada = 1 where Id =:id_cab");
    cab_alb->bindValue(":id_cab",this->id);
    if(!cab_alb->exec()) {
        QMessageBox::warning(NULL,tr("Guardar Albaran"),tr("No se ha podido marcar la Albaran como cobrada"),tr("OK"));
    }
    delete Cliente;
    delete cab_alb;
 }

// getters
int  Albaran::Getid() {
   return this->id;
}

QString Albaran::getcCodigoCliente() {
    return this->cCodigoCliente;
}

int Albaran::getnAlbaran()
{
    return this->nAlbaran;
}


QDate Albaran::getdFecha() {
    return this->dFecha;
}
QDate Albaran::getdFechaFactura() {
    return this->dFechaFactura;
}

QString Albaran::getcFactura()
{
    return this->cFactura;
}
int Albaran::getiId_Cliente() {
    return this->iId_Cliente;
}
QString Albaran::getcCliente() {
    return this->cCliente;
}
QString Albaran::getcDireccion() {
    return this->cDireccion;
}
QString Albaran::getcDireccion2() {
    return this->cDireccion2;
}
QString Albaran::getcCp() {
    return this->cCp;
}
QString Albaran::getcPoblacion() {
    return this->cPoblacion;
}
QString Albaran::getcProvincia() {
    return this->cProvincia;
}
QString Albaran::getcPais() {
    return this->cPais;
}
QString Albaran::getcCif() {
    return this->cCif;
}
int Albaran::getlRecargoEquivalencia() {
    return this->lRecargoEquivalencia;
}
double Albaran::getrSubtotal() {
    return this->rSubtotal;
}
int Albaran::getnDto() {
    return this->nDto;
}

double Albaran::getrImporteDescuento() {
    return this->rImporteDescuento;
}
double Albaran::getrBase() {
    return this->rBase;
}
int Albaran::getnIva() {
    return this->nIva;
}
double Albaran::getrImporteIva() {
    return this->rImporteIva;
}
double Albaran::getrTotal() {
    return this->rTotal;
}
int Albaran::getlImpreso() {
    return this->lImpreso;
}
int Albaran::getlFacturado() {
    return this->lFacturado;
}
QString Albaran::gettComentario() {
    return this->tComentario;
}
double Albaran::getrBase1() {
    return this->rBase1;
}
double Albaran::getrBase2() {
    return this->rBase2;
}
double Albaran::getrBase3() {
    return this->rBase3;
}
double Albaran::getrBase4() {
    return this->rBase4;
}
int Albaran::getnPorcentajeIVA1() {
    return this->nPorcentajeIVA1;
}
int Albaran::getnPorcentajeIVA2() {
    return this->nPorcentajeIVA2;
}
int Albaran::getnPorcentajeIVA3() {
    return this->nPorcentajeIVA3;
}
int Albaran::getnPorcentajeIVA4() {
    return this->nPorcentajeIVA4;
}
double Albaran::getrIVA1() {
    return this->rIVA1;
}
double Albaran::getrIVA2() {
    return this->rIVA2;
}
double Albaran::getrIVA3() {
    return this->rIVA3;
}
double Albaran::getrIVA4() {
    return this->rIVA4;
}
double Albaran::getrTotal1() {
    return this->rTotal1;
}
double Albaran::getrTotal2() {
    return this->rTotal2;
}
double Albaran::getrTotal3() {
    return this->rTotal3;
}
double Albaran::getrTotal4() {
    return this->rTotal4;
}
double Albaran::getnRec1() {
    return this->nRec1;
}
double Albaran::getnRec2() {
    return this->nRec2;
}
double Albaran::getnRec3() {
    return this->nRec3;
}
double Albaran::getnRec4() {
    return this->nRec4;
}
double Albaran::getrRecargoEq1() {
    return this->rRecargoEq1;
}
double Albaran::getrRecargoEq2() {
    return this->rRecargoEq2;
}
double Albaran::getrRecargoEq3() {
    return this->rRecargoEq3;
}
double Albaran::getrRecargoEq4() {
    return this->rRecargoEq4;
}
double Albaran::getrRecargoEqTotal() {
    return this->rRecargoEqTotal;
}
double Albaran::getrACuenta() {
    return this->rACuenta;
}
double Albaran::getrImportePendiente() {
    return this->rImportePendiente;
}

QString Albaran::getcPedidoCli() {
    return this->cPedidoCli;
}


// setters
void Albaran::setid( int iID_Albaran) {
    this->id = iID_Albaran;
}
void Albaran::setcCodigoCliente(QString cCodigoCliente) {
    this->cCodigoCliente = cCodigoCliente;
}

void Albaran::setnAlbaran(int nAlbaran)
{
    this->nAlbaran = nAlbaran;
}

void Albaran::setdFecha(QDate dFecha) {
    this->dFecha = dFecha;
}
void Albaran::setdFechaFactura(QDate dFechaFactura) {
    this->dFechaFactura = dFechaFactura;
}

void Albaran::setcFactura(QString cFactura)
{
    this->cFactura = cFactura;
}
void Albaran::setiId_Cliente(int iId_Cliente) {
    this->iId_Cliente = iId_Cliente;
}
void Albaran::setcCliente(QString cCliente) {
    this->cCliente = cCliente;
}
void Albaran::setcDireccion(QString cDireccion) {
    this->cDireccion = cDireccion;
}
void Albaran::setcDireccion2(QString cDireccion2) {
    this->cDireccion2 = cDireccion2;
}
void Albaran::setcCp(QString cCp) {
    this->cCp = cCp;
}
void Albaran::setcPoblacion(QString cPoblacion) {
    this->cPoblacion = cPoblacion;
}
void Albaran::setcProvincia(QString cProvincia) {
    this->cProvincia = cProvincia;}
void Albaran::setcPais(QString cPais) {
    this->cPais = cPais;
}
void Albaran::setcCif(QString cCif) {
    this->cCif = cCif;
}

void Albaran::setrSubtotal(double rSubtotal) {
    this->rSubtotal = rSubtotal;
}
void Albaran::setnDto(int nDto) {
    this->nDto = nDto;
}
void Albaran::setrImporteDescuento(double rImporteDescuento) {
    this->rImporteDescuento = rImporteDescuento;
}

void Albaran::setrBase(double rBase) {
    this->rBase = rBase;
}
void Albaran::setnIva(int nIva) {
    this->nIva = nIva;
}
void Albaran::setrImporteIva(double rImporteIva) {
    this->rImporteIva = rImporteIva;
}
void Albaran::setrTotal(double rTotal) {
    this->rTotal = rTotal;
}
void Albaran::setlImpreso(int lImpreso) {
    this->lImpreso = lImpreso;
}
void Albaran::setlFacturado(int lFacturado) {
    this->lFacturado = lFacturado;
}
void Albaran::settComentario(QString tComentario) {
    this->tComentario = tComentario;
}
void Albaran::setrBase1(double rBase1) {
    this->rBase1 = rBase1;
}
void Albaran::setrBase2(double rBase2) {
    this->rBase2 = rBase2;
}
void Albaran::setrBase3(double rBase3) {
    this->rBase3 = rBase3;
}
void Albaran::setrBase4(double rBase4) {
    this->rBase4 = rBase4;
}
void Albaran::setnPorcentajeIVA1(int nPorcentajeIVA1) {
    this->nPorcentajeIVA1 = nPorcentajeIVA1;
}
void Albaran::setnPorcentajeIVA2(int nPorcentajeIVA2) {
    this->nPorcentajeIVA2 = nPorcentajeIVA2;
}
void Albaran::setnPorcentajeIVA3(int nPorcentajeIVA3) {
    this->nPorcentajeIVA3 = nPorcentajeIVA3;
}
void Albaran::setnPorcentajeIVA4(int nPorcentajeIVA4) {
    this->nPorcentajeIVA4 = nPorcentajeIVA4;
}
void Albaran::setrIVA1( double rIVA1) {
    this->rIVA1 = rIVA1;
}
void Albaran::setrIVA2( double rIVA2) {
    this->rIVA2 = rIVA2;
}
void Albaran::setrIVA3( double rIVA3) {
    this->rIVA3 = rIVA3;
}
void Albaran::setrIVA4( double rIVA4) {
    this->rIVA4 = rIVA4;
}
void Albaran::setrTotal1(double rTotal1) {
    this->rTotal1 = rTotal1;
}
void Albaran::setrTotal2(double rTotal2) {
    this->rTotal2 = rTotal2;
}
void Albaran::setrTotal3(double rTotal3) {
    this->rTotal3 = rTotal3;
}
void Albaran::setrTotal4(double rTotal4) {
    this->rTotal4 = rTotal4;
}
void Albaran::setnRec1(int nRec1) {
    this->nRec1 = nRec1;
}
void Albaran::setnRec2(int nRec2) {
    this->nRec2 = nRec2;
}
void Albaran::setnRec3(int nRec3) {
    this->nRec3 = nRec3;
}
void Albaran::setnRec4(int nRec4) {
    this->nRec4 = nRec4;
}
void Albaran::setrRecargoEq1(double rRecargoEq1) {
    this->rRecargoEq1 = rRecargoEq1;
}
void Albaran::setrRecargoEq2(double rRecargoEq2) {
    this->rRecargoEq2 = rRecargoEq2;
}
void Albaran::setrRecargoEq3(double rRecargoEq3) {
    this->rRecargoEq3 = rRecargoEq3;
}
void Albaran::setrRecargoEq4(double rRecargoEq4) {
    this->rRecargoEq4 = rRecargoEq4;
}

void Albaran::setrRecargoEqTotal(double rRecargoEqTotal)
{
    this->rRecargoEqTotal = rRecargoEqTotal;
}

void Albaran::setrACuenta(double rACuenta) {
    this->rACuenta = rACuenta;
}
void Albaran::setcPedidoCli(QString cPedidoCli) {
    this->cPedidoCli = cPedidoCli;
}

