#include "cuentas_contables.h"

Cuentas_contables::Cuentas_contables(QObject *parent) :
    QObject(parent)
{

}

void Cuentas_contables::anadir_cuenta()
{
    QSqlQuery query_cuentas(QSqlDatabase::database("dbconta"));
    query_cuentas.prepare("INSERT INTO plan_general "
                          "(codigo_cta,descripcion,activo,codigo_balance,desglose_balance,afecta_a, saldo) "
                          "VALUES (:codigo_cta,:descripcion,:activo,:codigo_balance,:desglose_balance,:afecta_a,"
                          ":saldo);");
    query_cuentas.bindValue(":codigo_cta",this->codigo_cta);
    query_cuentas.bindValue(":descripcion",this->descripcion);
    query_cuentas.bindValue(":activo",this->activo);
    query_cuentas.bindValue(":codigo_balance",this->codigo_balance);
    query_cuentas.bindValue(":desglose_balance",this->desglose_balance);
    query_cuentas.bindValue(":afecta_a",this->afecta_a);
    query_cuentas.bindValue(":saldo",this->saldo);
    if(!query_cuentas.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir cuentas contables"),
                             tr("Falló la inserción de la cuenta contable: %1").arg(query_cuentas.lastError().text()));
    else
        this->id = query_cuentas.lastInsertId().toInt();
}

void Cuentas_contables::guardar_cuenta()
{
    QSqlQuery query_cuentas(QSqlDatabase::database("dbconta"));
    query_cuentas.prepare("UPDATE plan_general SET "
                          "codigo_cta = :codigo_cta,"
                          "descripcion =:descripcion,"
                          "activo = :activo,"
                          "codigo_balance = :codigo_balance,"
                          "desglose_balance = :desglose_balance,"
                          "afecta_a =:afecta_a,"
                          "saldo = :saldo "
                          " WHERE id = :id;");
    query_cuentas.bindValue(":codigo_cta",this->codigo_cta);
    query_cuentas.bindValue(":descripcion",this->descripcion);
    query_cuentas.bindValue(":activo",this->activo);
    query_cuentas.bindValue(":codigo_balance",this->codigo_balance);
    query_cuentas.bindValue(":desglose_balance",this->desglose_balance);
    query_cuentas.bindValue(":afecta_a",this->afecta_a);
    query_cuentas.bindValue(":saldo",this->saldo);
    query_cuentas.bindValue(":id",this->id);
    if(!query_cuentas.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Editar cuentas contables"),
                             tr("Falló la edición de la cuenta contable: %1").arg(query_cuentas.lastError().text()));
}

void Cuentas_contables::recuperar_cuenta(QString cuenta)
{
    QSqlQuery query_cuenta(QSqlDatabase::database("dbconta"));
    query_cuenta.prepare("select * from plan_general where codigo_cta = :cuenta ");
    query_cuenta.bindValue(":cuenta",cuenta);
    if(query_cuenta.exec()){
        query_cuenta.next();
        this->id = query_cuenta.record().value("id").toInt();
        this->activo = query_cuenta.record().value("activo").toBool();
        this->afecta_a = query_cuenta.record().value("afecta_a").toString();
        this->codigo_balance = query_cuenta.record().value("codigo_balance").toString();
        this->descripcion = query_cuenta.record().value("descripcion").toString();
        this->desglose_balance = query_cuenta.record().value("desglose_balance").toString();
        this->saldo = query_cuenta.record().value("saldo").toDouble();
    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de cuentas de PGC"),
                             tr("Ocurrió un error al recuperar la cuenta: %1").arg(query_cuenta.lastError().text()));
}

void Cuentas_contables::recuperar_cuenta(int id)
{
    QSqlQuery query_cuenta(QSqlDatabase::database("dbconta"));
    query_cuenta.prepare("select * from plan_general where id = :id ");
    query_cuenta.bindValue(":id",id);
    if(query_cuenta.exec()){
        query_cuenta.next();
        this->id = query_cuenta.record().value("id").toInt();
        this->activo = query_cuenta.record().value("activo").toBool();
        this->afecta_a = query_cuenta.record().value("afecta_a").toString();
        this->codigo_balance = query_cuenta.record().value("codigo_balance").toString();
        this->descripcion = query_cuenta.record().value("descripcion").toString();
        this->desglose_balance = query_cuenta.record().value("desglose_balance").toString();
        this->saldo = query_cuenta.record().value("saldo").toDouble();
    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de cuentas de PGC"),
                             tr("Ocurrió un error al recuperar la cuenta: %1").arg(query_cuenta.lastError().text()));
}

void Cuentas_contables::clear()
{
    this->id = 0;
    this->activo = true;
    this->afecta_a ="";
    this->codigo_balance = "";
    this->codigo_cta = "";
    this->descripcion = "";
    this->desglose_balance = "";
    this->saldo = 0;


}

