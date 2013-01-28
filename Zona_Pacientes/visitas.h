#ifndef VISITAS_H
#define VISITAS_H

#include <QObject>
#include <QDateTime>
#include <QSql>
#include <QSqlQuery>
#include <QSqlRecord>

class Visitas : public QObject
{
    Q_OBJECT

private:
    int id;
    int idepisodio;
    QDateTime fechahora;
    QString medico;
    QString exploracion;
    QString tratamiento;
    QString lengua;
    QString pulso;
    QString diagnostico;

public:
    explicit Visitas(QObject *parent = 0);
    
signals:
    
public slots:


};

#endif // VISITAS_H
