#include "sqlcalls.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql>

SqlCalls::SqlCalls()
{
}

SqlCalls::~SqlCalls()
{
}

QSqlQuery SqlCalls::RecuperarPaciente(int idCliente)
{
    QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbpacientes"));
    paciente->prepare("Select * from pacientes where idCliente =:idCliente");
    paciente->bindValue(":idCliente",idCliente);
    if (paciente->exec())
        return *paciente;
    else
        QMessageBox::warning(NULL,QObject::tr("Error Pacientes"),QObject::tr("No se puede recuperar el paciente"),
                             QObject::tr("Aceptar"));

}
