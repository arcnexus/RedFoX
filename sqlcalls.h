#ifndef SQLCALLS_H
#define SQLCALLS_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QObject>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include <QMap>

class SqlCalls : public QObject
{
    Q_OBJECT

public:
    SqlCalls(QObject *parent = 0);
    ~SqlCalls();


    static QStringList SelectList(QString table, QString column,QStringList clausulas, QSqlDatabase database, QString &error);
    static QStringList SelectList(QString table, QString column,QString clausulas, QSqlDatabase database, QString &error);


    template <class Key, class T> static QMap<Key,T> SelectMap(QString table,QString KeyColumn,QString dataColumn,QStringList clausulas, QSqlDatabase database, QString &error)
    {
        QMap<Key,T> r;
        QSqlQuery q(database);
        QString query;
        QTextStream s(&query);

        s << "SELECT " << KeyColumn << "," << dataColumn << " FROM " << table;
        if(!clausulas.isEmpty())
        {
            s << " WHERE ";
            QStringListIterator it(clausulas);
            while(it.hasNext())
            {
                s << it.next();
                if(it.hasNext())
                    s<<" AND ";
            }
        }
        s<<";";
        q.prepare(query);
        if(q.exec())
        {
            while(q.next())
            {
                QSqlRecord rec = q.record();
                r.insert(qvariant_cast<Key>(rec.value(KeyColumn)),qvariant_cast<T>(rec.value(dataColumn)));
            }
        }
        error = q.lastError().databaseText();
        return r;
    }
    template <class Key, class T> static QMap<Key,T> SelectMap(QString table,QString KeyColumn,QString dataColumn,QString clausulas, QSqlDatabase database, QString &error)
    {
        QStringList l;
        l << clausulas;
        return SelectMap<Key,T>(table, KeyColumn, dataColumn,l,database,error);
    }

    static QMap<int,QSqlRecord> SelectRecord(QString table,QStringList clausulas, QSqlDatabase database, QString &error);
    static QMap<int,QSqlRecord> SelectRecord(QString table,QString clausulas, QSqlDatabase database, QString &error);
    static QMap<int,QSqlRecord> SelectRecord(QString table, QStringList clausulas,QStringList extras, QSqlDatabase database, QString &error);



    static int SqlInsert(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QString& error);

    static bool SqlUpdate(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QStringList clausulas, QString& error);
    static bool SqlUpdate(QHash<QString,QVariant> values, QString table, QSqlDatabase database, QString clausula, QString& error);

    static bool SqlDelete(QString table, QSqlDatabase database, QStringList clausulas, QString& error);
    static bool SqlDelete(QString table, QSqlDatabase database, QString clausula, QString& error);
    static QVariant SelectOneField(QString table, QString field, QStringList clausulas, QSqlDatabase database, QString &error);
    static QVariant SelectOneField(QString table, QString field, QString clausulas, QSqlDatabase database, QString &error);
};

#endif // SQLCALLS_H
