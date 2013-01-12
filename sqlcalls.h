#ifndef SQLCALLS_H
#define SQLCALLS_H

#include <QtSql>

class SqlCalls : public QObject
{
    Q_OBJECT

public:
    SqlCalls(QObject *parent = 0);

    ~SqlCalls();

    bool addConnection(const QString &host,
                       const QString &user,
                       const QString &database,
                       const QString &password,
                       const QString &connectionName);

    void setDefaultConnection(const QString &name);

    QStringList connections() const;

    // Generic Querys
    QSqlQuery query(const QString &stament) const;
    QSqlQuery query(const QString &stament,
                    const QString &connection) const;
    QSqlQuery query(const QString &stament,
                    const QStringList &arguments,
                    const QString &connection) const;

    // Generic QueryModels
 //   QSqlQueryModel queryModel(const QString statment,
 //                             const QString &connection) const;

    QStringList queryList(const QString statment,
                              const QString &connection) const;

    // generic Inserts
    int addRec(const QString &statment,const QString &connection);
    int addRec(const QString &statment,const QStringList &parameters,const QString &connection);
    QSqlQuery RecuperarPaciente(int idCliente);

  //  int CrearPaciente(int idCliente);
private:
    QStringList m_connections;
    QString m_defaultConnection;

};

#endif // SQLCALLS_H
