#include "analitica.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

Analitica::Analitica(QObject *parent) :
    QObject(parent)
{

}

void Analitica::AnadirAnalitica()
{
    if(this->analisis.isEmpty())
        this->setAnalisis(tr("pendiente determinar"));
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("Insert INTO Analitica (idepisodio, analisis, comentarios)"
                  " VALUES (:idepisodio, :analisis, :comentarios)");
    qAna->bindValue(":idepisodio",this->idepisodio);
    qAna->bindValue(":analisis", this->analisis);
    qAna->bindValue(":comentarios",this->comentarios);
    if(!qAna->exec()){
        QMessageBox::warning(NULL,tr("Nueva Analítica"),tr("Falló al insertar una nueva analítica: ")+
                                                           qAna->lastError().text(),tr("Aceptar"));

        qDebug() << qAna->lastQuery();
    } else
        this->setId(qAna->lastInsertId().toInt());
}

void Analitica::AnadirLineas(int idanalitica, QString descripcion, QString valor, QString referencia,
                             QString comentarios,QString tipo)
{
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("INSERT INTO analitica2 (idanalitica, descripcion, valor, referencia, comentarios,"
                  "tipo) VALUES (:idanalitica, :descripcion, :valor, :referencia, :comentarios,:tipo)");
    qAna->bindValue(":idanalitica",idanalitica);
    qAna->bindValue(":descripcion",descripcion);
    qAna->bindValue(":valor",valor);
    qAna->bindValue(":referencia",referencia);
    qAna->bindValue(":comentarios",comentarios);
    qAna->bindValue(":tipo",tipo);
    if (!qAna->exec()) {
        QMessageBox::warning(NULL,tr("Añadir línea analitica"),tr("Fallo al insertar línea analitica")+
                             qAna->lastError().text(),tr("Aceptar"));
         qDebug() << qAna->lastQuery();
}


}

void Analitica::recuperarDatos(int nId)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("select * from analitica were id = :id");
    qAnalitica->bindValue(":id",nId);
    if (qAnalitica->exec()) {
        qAnalitica->next();
        QSqlRecord qRec = qAnalitica->record();
        this->analisis = qRec.field("analisis").value().toString();
        this->fecha = qRec.field("fechaanalisis").value().toDateTime();
        this->comentarios = qRec.field("comentarios").value().toString();
    }
    delete qAnalitica;

}
