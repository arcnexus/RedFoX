#include "sqlcalls.h"

SqlCalls::SqlCalls(QObject *parent)
{

}

SqlCalls::~SqlCalls()
{

}

QStringList SqlCalls::SelectList(QString table, QString column, QString clausula, QSqlDatabase database, QString &error)
{
    QStringList l;
    l << clausula;
    return SelectList(table, column, l, database, error);
}

QMap<int, QSqlRecord> SqlCalls::SelectRecord(QString table, QStringList clausulas, QSqlDatabase database, QString &error)
{
    QString query;
    QTextStream s(&query);

    s << "SELECT * FROM " << table;
    if(!clausulas.isEmpty())
    {
        s << " WHERE ";
        QStringListIterator i(clausulas);
        while (i.hasNext())
        {
            QString aux = i.next();
            s << aux;
            if(i.hasNext())
                s<< " AND ";
        }
    }
    s<<";";
    QSqlQuery q(database);
    q.prepare(query);
    QMap<int, QSqlRecord> lista;
    if(q.exec())
    {
        while (q.next())
            lista.insert(q.record().value("id").toInt(),q.record());
    }
    else
        error = q.lastError().text();
    return lista;
}

QMap<int, QSqlRecord> SqlCalls::SelectRecord(QString table, QString clausulas, QSqlDatabase database, QString &error)
{
    QStringList l;
    l << clausulas;
    return SelectRecord(table,l,database,error);
}

QVariant SqlCalls::SelectOneField(QString table, QString field, QStringList clausulas, QSqlDatabase database, QString &error)
{
    QString query;
    QTextStream s(&query);

    s << "SELECT "<< field << " FROM " << table;
    if(!clausulas.isEmpty())
    {
        s << " WHERE ";
        QStringListIterator i(clausulas);
        while (i.hasNext())
        {
            QString aux = i.next();
            s << aux;
            if(i.hasNext())
                s<< " AND ";
        }
    }
    s<<";";
    QSqlQuery q(database);
    q.prepare(query);
    QVariant campo;
    if(q.exec())
    {
        while (q.next())
            campo = q.record().value(field);
    }
    else
        error = q.lastError().text();
    return campo;
}


QStringList SqlCalls::SelectList(QString table, QString column,QStringList clausulas, QSqlDatabase database, QString &error)
{
    QString query;
    QTextStream s(&query);

    s << "SELECT " << column << " FROM " << table;
    if(!clausulas.isEmpty())
    {
        s << " WHERE ";
        QStringListIterator i(clausulas);
        while (i.hasNext())
        {
            QString aux = i.next();
            s << aux;
            if(i.hasNext())
                s<< " AND ";
        }
    }
    s<<";";
    QSqlQuery q(database);
    q.prepare(query);
    QStringList lista;

    if(q.exec())
    {
        while (q.next())
            lista.append(q.record().value(column).toString());
    }
    else
        error = q.lastError().text();
    return lista;
}

int SqlCalls::SqlInsert(QHash<QString, QVariant> values, QString table, QSqlDatabase database, QString &error)
{
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
/*    if(database.driverName() == "QMYSQL")
    {
        //qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }
TODO reactivar cuando haya soporte multi-driver
*/
    QString colums;
    QTextStream s(&colums);

    QString data;
    QTextStream s1(&data);

    //INSERT INTO `world`.`cab_fac` (`serie`) VALUES ('a');
    s << "INSERT INTO `" << table << "` (";
    s1 << "VALUES (";
    QHashIterator<QString,QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        s << "`" << i.key() << "`";
        s1 << ":" << i.key();
        if(i.hasNext())
        {
            s  << ",";
            s1 << ",";
        }
    }
    s1 << ");";
    s << ")" << data;

    QSqlQuery q(database);
    q.prepare(colums);

    QHashIterator<QString,QVariant> it(values);
    while (it.hasNext()) {
        it.next();
        QString aux = ":";
        aux.append(it.key());
        q.bindValue(aux,it.value());
    }


    bool b = q.exec();
    int r = -1;
    if (b)
        r = q.lastInsertId().toInt();
    else
        error = q.lastError().text();
    return r;
}

bool SqlCalls::SqlUpdate(QHash<QString, QVariant> values, QString table, QSqlDatabase database, QStringList clausulas, QString &error)
{
    //UPDATE `table` SET `column`=value WHERE `column`=value AND `otherColumn`=otherValue;
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
    /*
    if(database.driverName() == "QMYSQL")
    {
        //qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }
TODO reactivar cuando haya soporte multi-driver
*/
    QString colums;
    QTextStream s(&colums);

    s << "UPDATE `" << table << "` SET ";

    QHashIterator<QString,QVariant> i(values);
    while (i.hasNext()) {
        i.next();
        s << "`" << i.key() << "`=:" << i.key();
        if(i.hasNext())
            s  << ",";
    }
    s << " WHERE ";
    QStringListIterator li(clausulas);
    while (li.hasNext()) {
        QString a = li.next();
        s << a;
        if(li.hasNext())
            s<< " AND ";
    }
    s << ";";

    QSqlQuery q(database);
    q.prepare(colums);

    QHashIterator<QString,QVariant> it(values);
    while (it.hasNext()) {
        it.next();
        QString aux = ":";
        aux.append(it.key());
        q.bindValue(aux,it.value());
    }

    bool b = q.exec();
    if (!b)
        error = q.lastError().text();
    return b;
}

bool SqlCalls::SqlUpdate(QHash<QString, QVariant> values, QString table, QSqlDatabase database, QString clausula, QString &error)
{
    QStringList l;
    l << clausula;
    return SqlUpdate(values, table, database,l, error);
}

bool SqlCalls::SqlDelete(QString table, QSqlDatabase database, QStringList clausulas, QString &error)
{
    //DELETE FROM `table` WHERE `column`=value1 AND `column2`=otherValue;
    if(!database.isOpen())
    {
        error = QObject::tr("Base de datos cerrada");
        return false;
    }
    /*
    if(database.driverName() == "QMYSQL")
    {
        //qDebug()<< "Mysql";
    }
    else if (database.driverName() == "QSQLITE")
    {

    }
    TODO reactivar cuando haya soporte multi-driver
    */
    QString colums;
    QTextStream s(&colums);

    s << "DELETE FROM `" << table << "` WHERE ";

    QStringListIterator li(clausulas);
    while (li.hasNext()) {
        QString a = li.next();
        s << a;
        if(li.hasNext())
            s<< " AND ";
    }
    s << ";";

    QSqlQuery q(database);
    q.prepare(colums);
    bool b = q.exec();
    if (!b)
        error = q.lastError().text();
    return b;
}

bool SqlCalls::SqlDelete(QString table, QSqlDatabase database, QString clausula, QString &error)
{
    QStringList l;
    l<< clausula;
    return SqlDelete(table, database, l, error);
}
