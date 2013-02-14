#ifndef MEMDBLOADER_H
#define MEMDBLOADER_H

#include <QStringList>
#include <QDomElement>
#include <QSqlDatabase>

class MemDbLoader 
{
public:
    MemDbLoader();
    ~MemDbLoader();

public:

	static QString createMemoryDB(QString id="");

    // Load the DB from a file
    bool load(const QString &filename, QSqlDatabase db);
    // Load the DB from a DomElement
    bool load(const QDomElement & elemSource, QSqlDatabase db);
    QString lastError() {return _lastError;}
    
private:
    // Parse the table infos
    void parseTable(const QDomElement & elemSource);
    void parseColumns(const QDomElement & elemSource);
    void parseRecord(const QDomElement & elemSource);
    
	QSqlDatabase _db;
    QString 	_lastError;
    QString 	_tableName;
    QStringList _columnNames;
    
};

#endif // MEMDBLOADER_H
