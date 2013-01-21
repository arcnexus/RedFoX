#include "analitica.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
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
        QMessageBox::warning(qApp->activeWindow(),tr("Nueva Analítica"),tr("Falló al insertar una nueva analítica: ")+
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
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir línea analitica"),tr("Fallo al insertar línea analitica")+
                             qAna->lastError().text(),tr("Aceptar"));
         qDebug() << qAna->lastQuery();
}


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
        QMessageBox::warning(qApp->activeWindow(),tr("Analítica"),tr("Error en la carga de datos de analitica: ")+
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
        QMessageBox::warning(qApp->activeWindow(),tr("Analítica"),tr("Error al guardar los datos de analitica: ")+
                                                     qAnalitica->lastError().text(),tr("Aceptar"));

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
        QMessageBox::warning(qApp->activeWindow(),tr("Analítica"),tr("No se puede actualizar el registro de analítica. Error:")+
                             qAnalitica->lastError().text(),
                             tr("Aceptar"));
}


void Analitica::EliminarAnalitica(int id)
{
    QInputDialog d;
    d.setLabelText("Introduzca la contraseña de borrado.\n Este paso quedará registrado a su nombre");
    d.setOkButtonText("Aceptar");
    d.setCancelButtonText("Cancelar");
    d.setTextEchoMode(QLineEdit::Password);
    if(d.exec() == QInputDialog::Accepted) {
        QString Contra = d.textValue();
        // TODO - Añadir a configuración y hacer parametrizable
        // TODO - Registrar paso en registro movimientos traumaticos
        if(Contra == "AAAA"){
            QSqlQuery qlineas = QSqlQuery(QSqlDatabase::database("dbmedica"));
            qlineas.prepare("delete from analitica2 where idanalitica = :idAnalitica");
            qlineas.bindValue(":idAnalitica",id);
            if(!qlineas.exec())
                QMessageBox::warning(qApp->activeWindow(),tr("Analitica"),tr("Ocurrió un error al borrar la línea. No se puede Borrar: ")+
                                     qlineas.lastError().text(),tr("Aceptar"));

            qlineas.prepare("delete from Analitica where id = :id");
            qlineas.bindValue(":id",id);
            if(!qlineas.exec())
                QMessageBox::warning(qApp->activeWindow(),tr("Analitica"),tr("Ocurrió un error al borrar la analítica. No se puede Borrar: ")+
                                     qlineas.lastError().text(),tr("Aceptar"));
        }
    }
}


void Analitica::EliminarLinea(int id)
{
    QInputDialog d;
    d.setLabelText("Introduzca la contraseña de borrado.\n Este paso quedará registrado a su nombre");
    d.setOkButtonText("Aceptar");
    d.setCancelButtonText("Cancelar");
    d.setTextEchoMode(QLineEdit::Password);
    if(d.exec() == QInputDialog::Accepted) {
        QString Contra = d.textValue();
        // TODO - Añadir a configuración y hacer parametrizable
        // TODO - Registrar paso en registro movimientos traumaticos
        if(Contra == "AAAA"){

            QSqlQuery qlineas = QSqlQuery(QSqlDatabase::database("dbmedica"));
            qlineas.prepare("delete from analitica2 where id = :id");
            qlineas.bindValue(":id",id);
            if(!qlineas.exec())
                QMessageBox::warning(qApp->activeWindow(),tr("Analitica"),tr("Ocurrió un error al borrar la línea. No se puede Borrar: ")+
                                     qlineas.lastError().text(),tr("Aceptar"));
        }
    }
}

