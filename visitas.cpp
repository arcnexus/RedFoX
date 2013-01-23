#include "visitas.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QApplication>

Visitas::Visitas(QObject *parent) :
    QObject(parent)
{
}

int Visitas::AnadirVisita(int nIdEpisodio,QString cMedico)
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    int nIdVisita;
    qVisita.prepare("INSERT INTO visitas (idepisodio,fechahora,medico) "
                    "VALUES (:idepisodio,:fechahora,:medico)");
    qVisita.bindValue(":idepisodio",nIdEpisodio);
    qVisita.bindValue(":fechahora",QDate::currentDate());
    qVisita.bindValue(":medico",cMedico);
    if(!qVisita.exec()){
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrio un error al crear una nueva visita")+
                             qVisita.lastError().text(),tr("Aceptar"));
        nIdVisita = 0;
    }
    else {
        QMessageBox::information(qApp->activeWindow(),tr("Visitas"),tr("Se ha creado una nueva visita"), tr("Aceptar"));
        nIdVisita = qVisita.lastInsertId().toInt();
    }
    return nIdVisita;
}

void Visitas::RecuperarVisita(int nIdVisita)
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    qVisita.prepare("select * from visitas where id = :nId");
    qVisita.bindValue(":nId",nIdVisita);
    if(qVisita.exec()) {
        qVisita.next();
        QSqlRecord rVisita = qVisita.record();
        this->id = nIdVisita;
        this->idepisodio = rVisita.field("idepisodio").value().toInt();
        this->fechahora = rVisita.field("fechahora").value().toDateTime();
        this->medico = rVisita.field("medico").value().toString();
        this->exploracion = rVisita.field("exploracion").value().toString();
        this->tratamiento = rVisita.field("tratamiento").value().toString();
        this->lengua = rVisita.field("lengua").value().toString();
        this->pulso = rVisita.field("pulso").value().toString();


    } else{
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrió un error al recuperar la visita: ")+
                             qVisita.lastError().text(),tr("Aceptar"));
    }

}

void Visitas::GuardarDatos()
{
    QSqlQuery qVisita(QSqlDatabase::database("dbmedica"));
    qVisita.prepare("UPDATE visitas SET "
                    "idepisodio =:idEpisodio,"
                    "fechahora  = :fechahora,"
                    "medico  =  :medico,"
                    "exploracion  =  :exploracion,"
                    "tratamiento  =  :tratamiento,"
                    "lengua  = :lengua,"
                    "pulso  = :pulso"
                    " WHERE id = :nId");
    qVisita.bindValue(":idEpisodio",this->idepisodio);
    qVisita.bindValue(":fechahora",this->fechahora);
    qVisita.bindValue(":medico",this->medico);
    qVisita.bindValue(":exploracion",this->exploracion);
    qVisita.bindValue(":tratamiento",this->tratamiento);
    qVisita.bindValue(":lengua",this->lengua);
    qVisita.bindValue(":pulso",this->pulso);
    qVisita.bindValue(":nId", this->id);
    if(!qVisita.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Visitas"),tr("Ocurrió un error al modificar los datos de la visita: ")+
                             qVisita.lastError().text(),tr("Aceptar"));
    }

}
