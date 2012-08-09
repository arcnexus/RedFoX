#include "articulo.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
Articulo::Articulo()
{

}

void Articulo::Recuperar(QString cSQL)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("empresa"));
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec()||qryArticulo.next()){
        this->id = qryArticulo.value(0).toInt();
        this->cCodigo = qryArticulo.value(1).toString();
        this->cCodigoBarras = qryArticulo.value(2).toString();
        this->cCodigoFabricante = qryArticulo.value(3).toString();
        this->cDescripcion = qryArticulo.value(4).toString();

        this->rTarifa1 = qryArticulo.value(17).toDouble();

    }
}

void Articulo::Vender(int id, int cantidad, double rPVP)
{
}

void Articulo::Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion)
{
}
