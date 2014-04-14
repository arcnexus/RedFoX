#include "analitica.h"

Analitica::Analitica(QObject *parent) :
    QObject(parent)
{

}

void Analitica::AnadirAnalitica()
{
    if(this->analisis.isEmpty())
        this->setAnalisis(tr("pendiente determinar"));
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("Insert INTO Analitica (id_episodio,id_paciente analisis, comentarios)"
                  " VALUES (:id_episodio,:id_paciente :analisis, :comentarios)");
    qAna->bindValue(":id_episodio",this->id_episodio);
    qAna->bindValue(":analisis", this->analisis);
    qAna->bindValue(":id_paciente",this->id_paciente);
    qAna->bindValue(":comentarios",this->comentarios);
    if(!qAna->exec()){
        QMessageBox::warning(qApp->activeWindow(),tr("Nueva Analítica"),tr("Falló al insertar una nueva analítica: ")+
                                                           qAna->lastError().text(),tr("Aceptar"));

        qDebug() << qAna->lastQuery();
    } else
        this->setid(qAna->lastInsertId().toInt());
}

void Analitica::AnadirLineas(int id_analitica, QString descripcion, QString valor, QString referencia,
                             QString comentarios,QString tipo)
{
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("INSERT INTO analitica2 (id_analitica, descripcion, valor, referencia, comentarios,"
                  "tipo) VALUES (:id_analitica, :descripcion, :valor, :referencia, :comentarios,:tipo)");
    qAna->bindValue(":id_analitica",id_analitica);
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

void Analitica::recuperarDatos(int nid)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("select * from Analitica where id = :id");
    qAnalitica->bindValue(":id",nid);
    if (qAnalitica->exec()) {
        qAnalitica->next();
        QSqlRecord qRec = qAnalitica->record();
        this->id_paciente = qRec.field("id_paciente").value().toInt();
        this->analisis = qRec.field("analisis").value().toString();
        this->fecha = qRec.field("fecha_analisis").value().toDate();
        this->comentarios = qRec.field("comentarios").value().toString();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Analítica"),tr("Error en la carga de datos de analitica: ")+
                             qAnalitica->lastError().text(), tr("Aceptar"));
        qDebug() << qAnalitica->lastQuery();
    }
    delete qAnalitica;

}

void Analitica::GuardarDatos(int nid)
{
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAnalitica->prepare("UPDATE Analitica SET "
                        "analisis = :analisis,"
                        "id_paciente = :id_paciente,"
                        "comentarios = :comentarios,"
                        "fecha_analisis = :fecha_analisis"
                        " WHERE id = :id");
    qAnalitica->bindValue(":analisis",this->analisis);
    qAnalitica->bindValue(":id_paciente",this->id_paciente);
    qAnalitica->bindValue(":comentarios",this->comentarios);
    qAnalitica->bindValue(":fecha_analisis",this->fecha);
    qAnalitica->bindValue(":id",nid);
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
            qlineas.prepare("delete from analitica2 where id_analitica = :id_analitica");
            qlineas.bindValue(":id_analitica",id);
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

