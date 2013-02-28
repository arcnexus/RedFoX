#ifndef CREATECONNECTION_H
#define CREATECONNECTION_H

class createConnection
{
public:

    bool createConnection()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("mozart.konkordia.edu");
        db.setDatabaseName("musicdb");
        db.setUserName("gbatstone");
        db.setPassword("T17aV44");
        if (!db.open()) {
            QMessageBox::critical(0, QObject::tr("Database Error"),
            db.lastError().text());
            return false;
        }
        return true;

    }

#endif // CREATECONNECTION_H
