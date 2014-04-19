#ifndef ODS_H
#define ODS_H

#include "odslib_global.h"
#include <QObject>
class QSqlDatabase;
class QStringList;
class QAbstractItemModel;

class ODSLIBSHARED_EXPORT ODS : public QObject
{
    Q_OBJECT
public:
    ODS();
    static bool SqlToODS(QString fileName, QString query, QSqlDatabase db, QStringList headers);
    static bool ODStoTable(QString fileName, QAbstractItemModel *model);
    static QString lastError;
};

#endif // ODS_H
