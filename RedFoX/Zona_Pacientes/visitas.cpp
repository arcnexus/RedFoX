#include "visitas.h"

Visitas::Visitas(QObject *parent) :
    QObject(parent)
{
}

int Visitas::AnadirVisita(int nid_episodio,QString cMedico)
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    int nidVisita;
    qVisita.prepare("INSERT INTO visitas (id_episodio,fecha_hora,medico) "
                    "VALUES (:id_episodio,:fecha_hora,:medico)");
    qVisita.bindValue(":id_episodio",nid_episodio);
    qVisita.bindValue(":fecha_hora",QDate::currentDate());
    qVisita.bindValue(":medico",cMedico);
    if(!qVisita.exec()){
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrio un error al crear una nueva visita")+
                             qVisita.lastError().text(),tr("Aceptar"));
        nidVisita = 0;
    }
    else {
        nidVisita = qVisita.lastInsertId().toInt();
    }
    return nidVisita;
}

void Visitas::RecuperarVisita(int nidVisita)
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    qVisita.prepare("select * from visitas where id = :nid");
    qVisita.bindValue(":nid",nidVisita);
    if(qVisita.exec()) {
        qVisita.next();
        QSqlRecord rVisita = qVisita.record();
        this->id = nidVisita;
        this->id_episodio = rVisita.field("id_episodio").value().toInt();
        this->fecha_hora = rVisita.field("fecha_hora").value().toDateTime();
        this->medico = rVisita.field("medico").value().toString();
        this->exploracion = rVisita.field("exploracion").value().toString();
        this->tratamiento = rVisita.field("tratamiento").value().toString();
        this->lengua = rVisita.field("lengua").value().toString();
        this->pulso = rVisita.field("pulso").value().toString();
        this->diagnostico = rVisita.field("diagnostico").value().toString();


    } else{
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrió un error al recuperar la visita: ")+
                             qVisita.lastError().text(),tr("Aceptar"));
    }

}

void Visitas::GuardarDatos()
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    qVisita.prepare("UPDATE visitas SET "
                    "id_episodio =:id_episodio,"
                    "fecha_hora  = :fecha_hora,"
                    "medico  =  :medico,"
                    "exploracion  =  :exploracion,"
                    "tratamiento  =  :tratamiento,"
                    "lengua  = :lengua,"
                    "pulso  = :pulso,"
                    "diagnostico =:diagnostico"
                    " WHERE id = :nid");
    qVisita.bindValue(":id_episodio",this->id_episodio);
    qVisita.bindValue(":fecha_hora",this->fecha_hora);
    qVisita.bindValue(":medico",this->medico);
    qVisita.bindValue(":exploracion",this->exploracion);
    qVisita.bindValue(":tratamiento",this->tratamiento);
    qVisita.bindValue(":lengua",this->lengua);
    qVisita.bindValue(":pulso",this->pulso);
    qVisita.bindValue(":diagnostico",this->diagnostico);
    qVisita.bindValue(":nid", this->id);
    if(!qVisita.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrió un error al modificar los datos de la visita: ")+
                             qVisita.lastError().text(),tr("Aceptar"));
    }

}
