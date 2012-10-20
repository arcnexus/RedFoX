#include "sqlcalls.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql>
#include <QStringList>

static const QString queryPaciente = "SELECT * FROM pacientes WHERE IdCliente = :idCliente";

SqlCalls::SqlCalls(QObject *parent) :
    QObject(parent)
{
    m_connections.clear();
    m_defaultConnection = QString();
}

SqlCalls::~SqlCalls()
{
}

bool SqlCalls::addConnection(const QString &host,
                             const QString &user,
                             const QString &database,
                             const QString &password,
                             const QString &connectionName)
{
    if (m_connections.contains(connectionName)) {
        qDebug() << "Connection name" << connectionName << "already exist.";
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName.toLatin1());
    db.setHostName(host);
    db.setUserName(user);
    db.setDatabaseName(database);
    db.setPassword(password);

    if (!db.open()) {
        qDebug() << db.lastError().text();
        return false;
    } else {
        //m_connections.append(connectionName);
    }

    return true;
}

void SqlCalls::setDefaultConnection(const QString &name)
{
    if (!m_connections.contains(name)) {
        qDebug() << "Connection name" << name << "does not exist.";
        return;
    } else {
        m_defaultConnection = name;
    }

}

QStringList SqlCalls::connections() const
{
        return m_connections;
}


QSqlQuery SqlCalls::query(const QString &stament) const
{
     QSqlQuery query(stament);
    return query;
}

QSqlQuery SqlCalls::query(const QString &stament,
                          const QString &connection) const
{
    QSqlQuery query(stament, QSqlDatabase::database(connection));

    if (!query.exec())
        qDebug() << query.lastError().text();

    return query;
}

QSqlQuery SqlCalls::query(const QString &stament,
                          const QStringList &arguments,
                          const QString &connection) const
{
    QSqlQuery query(stament, QSqlDatabase::database(connection));

    for (int i = 0; i < arguments.count(); i++)
        query.bindValue(i,arguments.at(i));

    if(!query.exec())
        qDebug() << query.lastError().text();

    return query;
}

QSqlQuery SqlCalls::RecuperarPaciente(int idCliente)
{
    QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    paciente->prepare(queryPaciente);

    paciente->bindValue(":idCliente",idCliente);
    if (paciente->exec())
        return *paciente;
    else
        QMessageBox::warning(NULL,QObject::tr("Error Pacientes"),
                             QObject::tr("No se puede recuperar el paciente"),
                             QObject::tr("Aceptar"));
    delete paciente;
}


int SqlCalls::CrearPaciente(int idCliente)
{
    QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    paciente->prepare("insert into pacientes (idCliente) values (:idCliente)");
    paciente->bindValue(":idCliente",idCliente);
    if (!paciente->exec())
        QMessageBox::warning(NULL,QObject::tr("Error Pacientes"),QObject::tr("No se ha podido insertar un nuevo paciente"),
                             QObject::tr("Aceptar"));
    else {
        return paciente->lastInsertId().toInt();
    }
    delete paciente;
}


