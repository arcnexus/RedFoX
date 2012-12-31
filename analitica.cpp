#include "analitica.h"
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>

Analitica::Analitica(QObject *parent) :
    QObject(parent)
{

}

void Analitica::AnadirAnalitica()
{
    QSqlQuery *qAna = new QSqlQuery("dbmedica");
    qAna->prepare("Insert INTO Analitica (idepisodio, analisis, tipoanalisis, comentarios)"
                  " VALUES (:idepisodio, :analisis, :tipoanalisis, :comentarios)");
    qAna->bindValue(":idepisodio",this->idepisodio);
    qAna->bindValue(":analisis", this->analisis);
    qAna->bindValue(":tipoanalisis",this->tipoanalisis);
    qAna->bindValue(":comentarios",this->comentarios);
    if(!qAna->exec())
        QMessageBox::warning(NULL,tr("Nueva Analítica"),tr("Falló al insertar una nueva analítica: ")+
                                                           qAna->lastError().text(),tr("Aceptar"));

}

void Analitica::AnadirLineas(int idanalitica, QString descripcion, QString valor, QString referencia,
                             QString comentarios,QString tipo)
{
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("INSERT INTO analitica2 (idanalitica, descripcion, valor, referencia, comentarios,"
                  "tipo) VALUES :idanalitica, :descripcion, :valor, :referencia, :comentarios,:tipo)");
    qAna->bindValue(":idanalitica",idanalitica);
    qAna->bindValue(":descripcion",descripcion);
    qAna->bindValue(":valor",valor);
    qAna->bindValue(":referencia",referencia);
    qAna->bindValue(":comentarios",comentarios);
    qAna->bindValue(":tipo",tipo);
    if (!qAna->exec())
        QMessageBox::warning(NULL,tr("Añadir línea analitica"),tr("Fallo al insertar línea analitica")+
                             qAna->lastError().text(),tr("Aceptar"));


}
