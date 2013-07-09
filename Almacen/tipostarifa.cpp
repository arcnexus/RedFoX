#include "tipostarifa.h"

TiposTarifa::TiposTarifa(QObject *parent) :
    QObject(parent)
{
}

void TiposTarifa::clear()
{
    id = 0;
    descripcion = "";
    codigo_tarifa = "";
    dto = 0;
    desc_promo  = "";
    cant_promo = 0;
    cant_base = 0;
    importe_dto = 0;
    id_pais = 0;
    id_monedas = 0;
    margen = 0;
    margen_min = 0;
}

int TiposTarifa::anadir()
{
    QSqlQuery query_tarifas(QSqlDatabase::database("Maya"));
    query_tarifas.prepare("INSERT INTO codigotarifa (margen,margen_min)"
                          " VALUES (:margen,:margen_min)");
    query_tarifas.bindValue(":margen",Configuracion_global->margen);
    query_tarifas.bindValue(":margen_min",Configuracion_global->margen_minimo);
    if(query_tarifas.exec())
    {
        int nId = query_tarifas.lastInsertId().toInt();
        return nId;
    }

    else {
        QMessageBox::warning(qApp->activeWindow(),tr("Tipos de tarifa"),
                             tr("Falló la inserción de tipos de tarifa: %1").arg(query_tarifas.lastError().text()),
                             tr("Aceptar"));
        return -1;
    }
}

bool TiposTarifa::guardar()
{
    QSqlQuery query_tarifas(QSqlDatabase::database("Maya"));
    query_tarifas.prepare("UPDATE codigotarifa SET "

                          "descripcion=:descripcion,"
                          "codigo_tarifa=:codigo_tarifa,"
                          "id_pais=:id_pais,"
                          "id_monedas=:id_monedas,"
                          "margen=:margen,"
                          "margen_min=:margen_min "
                            " WHERE id=:id");
    query_tarifas.bindValue(":descripcion", this->descripcion);
    query_tarifas.bindValue(":codigo_tarifa",this->codigo_tarifa);
    query_tarifas.bindValue(":id_pais",this->id_pais);
    query_tarifas.bindValue(":id_monedas",this->id_monedas);
    query_tarifas.bindValue(":margen",this->margen);
    query_tarifas.bindValue(":margen_min",this->margen_min);
    query_tarifas.bindValue(":id",this->id);
    if(query_tarifas.exec())
        return true;
    else {
        QMessageBox::warning(qApp->activeWindow(),tr("Tipos de tarifa"),
                             tr("Falló la modificación de tipos de tarifa: %1").arg(query_tarifas.lastError().text()),
                             tr("Aceptar"));
        return false;
    }
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
