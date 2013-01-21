#include "interconsulta.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QApplication>
Interconsulta::Interconsulta(QObject *parent) :
    QObject(parent)
{
}

void Interconsulta::AnadirInterconsulta(int idEpisodio,int idPaciente)
{
    QScopedPointer<QSqlQuery>qInterconsulta (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qInterconsulta = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "INSERT INTO interconsulta (idpaciente, idepisodio,fechapeticion) VALUES "
            " (:idpaciente, :idepisodio, :fechapeticion)";
    qInterconsulta->prepare(cSQL);
    qInterconsulta->bindValue(":idpaciente",idPaciente);
    qInterconsulta->bindValue(":idepisodio",idEpisodio);
    qInterconsulta->bindValue(":fechapeticion",QDate::currentDate());
    if(qInterconsulta->exec())
        QMessageBox::information(qApp->activeWindow(),tr("Interconsultas"),tr("Se ha añadido el registro correctamente"),
                                 tr("Aceptar"));
    else
        QMessageBox::information(qApp->activeWindow(),tr("Interconsultas"),tr("No se ha podido añadir el registro correctamente"),
                                 tr("Aceptar"));


}
