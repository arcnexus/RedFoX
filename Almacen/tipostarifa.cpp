#include "tipostarifa.h"

TiposTarifa::TiposTarifa(QObject *parent) :
    QObject(parent)
{
}

void TiposTarifa::recuperar(QString cSql)
{
    QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
    if(queryTarifas.exec(cSql))
     {
        cargar_datos(queryTarifas);
    }
}

void TiposTarifa::recuperar(int id)
{
    QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
    if(queryTarifas.exec("select * from codigotarifa where id ="+QString::number(id)))
    {
        cargar_datos(queryTarifas);
    }
}

void TiposTarifa::cargar_datos(QSqlQuery queryTarifa)
{
    queryTarifa.next();
    this->id = queryTarifa.record().value("id").toInt();
    this->cant_base = queryTarifa.record().value("cant_base").toInt();
    this->cant_promo = queryTarifa.record().value("cant_promo").toInt();
    this->codigo_tarifa = queryTarifa.record().value("codigo_tarifa").toString();
    this->descripcion = queryTarifa.record().value("descripcion").toString();
    this->desc_promo = queryTarifa.record().value("desc_promo").toString();
    this->dto = queryTarifa.record().value("dto").toDouble();
    this->id_monedas = queryTarifa.record().value("id_monedas").toInt();
    this->id_pais =queryTarifa.record().value("id_pais").toInt();
    this->importe_dto = queryTarifa.record().value("importe_dto").toDouble();
    this->margen = queryTarifa.record().value("margen").toFloat();
    this->margen_min = queryTarifa.record().value("margen_min").toFloat();

}
