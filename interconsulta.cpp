#include "interconsulta.h"

Interconsulta::Interconsulta(QObject *parent) :
    QObject(parent)
{
}

void Interconsulta::AnadirInterconsulta(int id_episodio,int id_paciente)
{
    QScopedPointer<QSqlQuery>qInterconsulta (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qInterconsulta = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "INSERT INTO interconsulta (id_paciente, id_episodio,fechapeticion) VALUES "
            " (:id_paciente, :id_episodio, :fechapeticion)";
    qInterconsulta->prepare(cSQL);
    qInterconsulta->bindValue(":id_paciente",id_paciente);
    qInterconsulta->bindValue(":id_episodio",id_episodio);
    qInterconsulta->bindValue(":fechapeticion",QDate::currentDate());
    if(qInterconsulta->exec())
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Se ha añadido el registro corectamente"));
    else
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("No se ha podido añadir el registro corectamente"));


}
