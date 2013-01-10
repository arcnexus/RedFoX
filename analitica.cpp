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
    delete qAna;
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
    delete qAna;

}

void Analitica::recuperarDatos(int nId)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("select * from Analitica where id = :id");
    qAnalitica->bindValue(":id",nId);
    if (qAnalitica->exec()) {
        qAnalitica->next();
        QSqlRecord qRec = qAnalitica->record();
        this->analisis = qRec.field("analisis").value().toString();
        this->fecha = qRec.field("fechaanalisis").value().toDate();
        this->comentarios = qRec.field("comentarios").value().toString();
    } else {
        QMessageBox::warning(NULL,tr("Analítica"),tr("Error en la carga de datos de analitica: ")+
                             qAnalitica->lastError().text(), tr("Aceptar"));
        qDebug() << qAnalitica->lastQuery();
    }
    delete qAnalitica;

}

void Analitica::GuardarDatos(int nId)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("UPDATE Analitica SET "
                        "analisis = :analisis,"
                        "comentarios = :comentarios,"
                        "fechaanalisis = :fechaanalisis"
                        " WHERE id = :id");
    qAnalitica->bindValue(":analisis",this->analisis);
    qAnalitica->bindValue(":comentarios",this->comentarios);
    qAnalitica->bindValue(":fechaanalisis",this->fecha);
    qAnalitica->bindValue(":id",nId);
    if(!qAnalitica->exec())
        QMessageBox::warning(NULL,tr("Analítica"),tr("Error al guardar los datos de analitica: ")+
                                                     qAnalitica->lastError().text(),tr("Aceptar"));
    delete qAnalitica;

}

void Analitica::GuardarLineas(int id, QString valor, QString referencia, QString comentarios)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("UPDATE analitica2 SET "
    "valor = :valor,"
    "referencia = :referencia,"
    "comentarios = :comentarios "
    "WHERE id =:id");
    qAnalitica->bindValue(":valor",valor);
    qAnalitica->bindValue(":referencia",referencia);
    qAnalitica->bindValue(":comentarios",comentarios);
    qAnalitica->bindValue(":id",id);
    if(!qAnalitica->exec())
        QMessageBox::warning(NULL,tr("Analítica"),tr("No se puede actualizar el registro de analítica. Error:")+
                             qAnalitica->lastError().text(),
                             tr("Aceptar"));
    delete qAnalitica;
}

void Analitica::BorrarAnalitica(int nId)
{
    // TODO : Abrir transacción:
    QSqlDatabase dbmedica = QSqlDatabase::database("dbmedica");
    dbmedica.transaction();

    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("delete from analitica2 where idanalitica = :nidAnalitica");
    qAna->bindValue(":nidAnalitica",nId);
    try {
        if(!qAna->exec())
            QMessageBox::warning(NULL,tr("Borrar Analítica"),tr("Ha ocurrido un error al tratar de borrar las líneas de analítica")+
                                 qAna->lastError().text(),tr("Aceptar"));
        qAna->prepare("delete from Analitica where id =:nId");
        qAna->bindValue(":nId",nId);
        if(!qAna->exec())
            QMessageBox::warning(NULL,tr("Borrar Analítica"),tr("Ha ocurrido un error al tratar de borrar la analítica")+
                                 qAna->lastError().text(),tr("Aceptar"));
        else
            QMessageBox::warning(NULL,tr("Borrar Analítica"),tr("La Analítica ha sido borrada satisfactoriamente"),tr("Aceptar"));
        dbmedica.commit();

    } catch(...) {
            dbmedica.rollback();
    }
        // TODO: Cerrar Transacción:
    delete qAna;
}

void Analitica::BorrarLineaAnalitica(int nId)
{
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("delete from analitica2 where id = :nid");
    qAna->bindValue(":nid",nId);
    if(!qAna->exec())
        QMessageBox::warning(NULL,tr("Borrar Analítica"),tr("Ha ocurrido un error al tratar de borrar la línea de analítica")+
                             qAna->lastError().text(),tr("Aceptar"));
    else
        QMessageBox::warning(NULL,tr("Borrar Analítica"),tr("La línea ha sido borrada satisfactoriamente"),tr("Aceptar"));
    delete qAna;
}
