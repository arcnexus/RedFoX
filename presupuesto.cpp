#include "presupuesto.h"
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
Presupuesto::Presupuesto()
{
}

void Presupuesto::AnadirPresupuesto()
{
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
    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
     cab_pre.prepare("INSERT INTO  cab_pre(nPresupuesto,nIva1,nIva2,nIva3,nIva4,"
                     "nRecargoEquivalencia1,nRecargoEquivalencia2,nRecargoEquivalencia3,nRecargoEquivalencia4)"
                   " VALUES (:nPedido,:nPorcentajeIva1,:nPorcentajeIva2,:nPorcentajeIva3,:nPorcentajeIva4,"
                     ":nPorcentajeRecargoEq1,:nPorcentajeRecargoEq2,:nPorcentajeRecargoEq3,:nPorcentajeRecargoEq4)");

     cab_pre.bindValue(":nPedido",0);
     cab_pre.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
     cab_pre.bindValue(":nPorcentajeIva2",this->nPorcentajeIVA2);
     cab_pre.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
     cab_pre.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
     cab_pre.bindValue(":nPorcentajeRecargoEq1",this->nRec1);
     cab_pre.bindValue(":nPorcentajeRecargoEq2",this->nRec2);
     cab_pre.bindValue(":nPorcentajeRecargoEq3",this->nRec3);
     cab_pre.bindValue(":nPorcentajeRecargoEq4",this->nRec4);
     if(!cab_pre.exec()){
         QMessageBox::critical(NULL,"error al guardar datos Presupuesto:", cab_pre.lastError().text());
     } else {
         this->id = cab_pre.lastInsertId().toInt();
         QString cSQL = "Select * from cab_pre where id ="+QString::number(this->id);
         RecuperarPresupuesto(cSQL);
     }
}

void Presupuesto::RecuperarPresupuesto(QString cSQL)
{
}

void Presupuesto::RecuperarPresupuesto(QString cSQL, int nAccion)
{
}

void Presupuesto::GuardarPres(int nId_Presupuesto)
{
}

int Presupuesto::NuevoNumeroPresupuesto()
{
}

void Presupuesto::AnadirLineaPresupuesto(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
}

void Presupuesto::ModificarLineaPresupuesto(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal, double porcdto, double dto, double total, double nPorcIva)
{
}

void Presupuesto::BorrarLineaPresupuesto(int id_lin)
{
}

void Presupuesto::calcularPresupuesto()
{
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
