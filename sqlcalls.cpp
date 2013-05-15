#include "sqlcalls.h"

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

//QSqlQueryModel SqlCalls::queryModel(const QString statment, const QString &connection) const
//{
//    QString cSql = statment;
//    QSqlQueryModel *qModel = new QSqlQueryModel();
//    qModel->setQuery(cSql,QSqlDatabase::database(connection));
    //    return &qModel;
//}

QStringList SqlCalls::queryList(const QString statment, const QString &connection) const
{
    QSqlQuery *query = new QSqlQuery(statment,QSqlDatabase::database(connection));
    QStringList lista;
    while (query->next())
        lista.append(query->value(0).toString());
    return lista;
}


int SqlCalls::addRec(const QString &statment, const QString &connection)
{
    //QString cSQL = statment;
    QSqlQuery tInsert = QSqlQuery(QSqlDatabase::database(connection));
    tInsert.prepare(statment);
    if (tInsert.exec()) {
        return tInsert.lastInsertId().toInt();
    }
    else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error"),QObject::tr("Ha ocurrido un fallo al insertar el registro"),
                   QObject::tr("Aceptar"));

        return NULL;
    }
}

int SqlCalls::addRec(const QString &statment, const QStringList &parameters, const QString &connection)
{
    //QString cSQL = statment;
    QSqlQuery tInsert = QSqlQuery(QSqlDatabase::database(connection));
    tInsert.prepare(statment);
    for (int i = 0; i < parameters.count(); i++)
        tInsert.bindValue(i,parameters.at(i));

    if(!tInsert.exec())
        qDebug() << tInsert.lastError().text();

    else
        return tInsert.lastInsertId().toInt();

}



QSqlQuery SqlCalls::RecuperarPaciente(int idCliente)
{
    QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    paciente->prepare(queryPaciente);

    paciente->bindValue(":idCliente",idCliente);
    if (paciente->exec())
        return *paciente;
    else
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error Pacientes"),
                             QObject::tr("No se puede recuperar el paciente"),
                             QObject::tr("Aceptar"));
    delete paciente;
}


//int SqlCalls::CrearPaciente(int idCliente)
//{
//    QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
//    paciente->prepare("insert into pacientes (idCliente,numhistoria) values (:idCliente,:numhistoria)");
//    paciente->bindValue(":idCliente",idCliente);
//    paciente->bindValue(":numhistoria",QString::number(idCliente));
//    if (!paciente->exec())
//        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error Pacientes"),QObject::tr("No se ha podido insertar un nuevo paciente"),
//                             QObject::tr("Aceptar"));
//    else {
//        return paciente->lastInsertId().toInt();
//    }
//    delete paciente;
//}
