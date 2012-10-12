#ifndef SQLCALLS_H
#define SQLCALLS_H

#include <QtSql>

class SqlCalls
{
public:
    SqlCalls();
    ~SqlCalls();
    QSqlQuery RecuperarPaciente(int idCliente);
};

#endif // SQLCALLS_H
