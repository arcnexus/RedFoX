#include "imagenesdiagnostico.h"
#include <QtSql>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>



ImagenesDiagnostico::ImagenesDiagnostico(QObject *parent) :
    QObject(parent)
{
}

void ImagenesDiagnostico::guardarDatosDB()
{
        //emit ui_ponerDatosEnObjetoImagen();
        QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        qImagen->prepare("UPDATE imagenes SET "
                         "localizacionimagen =:localizacionimagen,"
                         "comentarios = :comentarios,"
                         "evaluada =:evaluada,"
                         "descripcion = :descripcion,"
                         "fechaimagen = :fechaimagen,"
                         "tipoimagen = :tipoimagen "
                         "WHERE id =:id");

        qImagen->bindValue(":localizacionimagen",this->LocalizacionImagen);
        qImagen->bindValue(":comentarios",this->Comentarios);
        qImagen->bindValue(":evaluada",this->Evaluada);
        qImagen->bindValue(":descripcion",this->Descripcion);
        qImagen->bindValue(":fechaimagen",this->FechaImagen);
        qImagen->bindValue(":tipoimagen",this->TipoImagen);
        qImagen->bindValue(":id",this->id);

        if(qImagen->exec()){
            QMessageBox::information(NULL,QObject::tr("Modificar Imagenes"),QObject::tr("La imagen ha sido modificada correctamente"),
                                     QObject::tr("Aceptar"));
            emit refrescarlista();
        } else
            QMessageBox::information(NULL,QObject::tr("Modificar Imagenes"),QObject::tr("Ocurrió un error al modificar la imagen en el episodio:")+
                                     qImagen->lastError().text(),
                                     QObject::tr("Aceptar"));
}


void ImagenesDiagnostico::llenarObjetoconDatosDB()
{
    QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "Select * from imagenes where id =:id";

    qImagen->prepare(cSQL);
    qImagen->bindValue(":id",this->id);
    if (qImagen->exec()) {
        qImagen->next();
        QSqlRecord rImagen = qImagen->record();
        this->idEpisodio = rImagen.field("idepisodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->FechaImagen = rImagen.field("fechaimagen").value().toDate();
        this->LocalizacionImagen = rImagen.field("localizacionimagen").value().toString();
        this->TipoImagen = rImagen.field("tipoimagen").value().toString();
        this->Evaluada =rImagen.field("evaluada").value().toInt();
    } else {
        QMessageBox::warning(NULL,QObject::tr("Imagenes Diagnóstico"),QObject::tr("No se ha podido recuperar el registro de imagen"),
                             QObject::tr("Aceptar"));
    }
}

void ImagenesDiagnostico::llenarObjetoconDatosDB(int nid)
{
    QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "Select * from imagenes where id =:id";

    qImagen->prepare(cSQL);
    qImagen->bindValue(":id",nid);
    if (qImagen->exec()) {
        qImagen->next();
        QSqlRecord rImagen = qImagen->record();
        this->idEpisodio = rImagen.field("idepisodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->FechaImagen = rImagen.field("fechaimagen").value().toDate();
        this->LocalizacionImagen = rImagen.field("localizacionimagen").value().toString();
        this->TipoImagen = rImagen.field("tipoimagen").value().toString();
        this->Evaluada =rImagen.field("evaluada").value().toInt();
    } else {
        QMessageBox::warning(NULL,QObject::tr("Imagenes Diagnóstico"),QObject::tr("No se ha podido recuperar el registro de imagen"),
                             QObject::tr("Aceptar"));
    }
}

void ImagenesDiagnostico::BorrarImagen(int nid)
{
    if(QMessageBox::question(NULL,QObject::tr("Borrar imagen del historial"),
                             QObject::tr("Va a proceder a borrar una imagen del historial, ¿Desea continuar?"),
                             QObject::tr("Cancelar"),QObject::tr("Borrar")) ==QMessageBox::Accepted) {
        QSqlQuery *qImagenes = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        qImagenes->prepare("delete from imagenes where id = :nid");
        qImagenes->bindValue(":nid",nid);
        if(!qImagenes->exec())
            QMessageBox::warning(NULL,QObject::tr("Borrar Imagen"),tr("Atención::No se puede borrar la imagen :")+
                                 qImagenes->lastError().text(),tr("Aceptar"));


    }
}

void ImagenesDiagnostico::anadirDatosDB()
{
    emit ui_ponerDatosEnObjetoImagen();
    QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qImagen->prepare("INSERT INTO imagenes "
                    "(idepisodio,localizacionimagen,comentarios,evaluada,descripcion,fechaimagen,"
                    "tipoimagen) VALUES (:idepisodio,:localizacionimagen,:comentarios,:evaluada,"
                    ":descripcion,:fechaimagen,:tipoimagen)");
    qImagen->bindValue(":idepisodio",this->idEpisodio);
    qImagen->bindValue(":localizacionimagen",this->LocalizacionImagen);
    qImagen->bindValue(":comentarios",this->Comentarios);
    qImagen->bindValue(":evaluada",this->Evaluada);
    qImagen->bindValue(":descripcion",this->Descripcion);
    qImagen->bindValue(":fechaimagen",this->FechaImagen);
    qImagen->bindValue(":tipoimagen",this->TipoImagen);
    if(qImagen->exec()){
        QMessageBox::information(NULL,QObject::tr("Insertar Imagenes"),QObject::tr("La imagen ha sido insertada correctamente"),
                                 QObject::tr("Aceptar"));
        emit cerrarventana();
    } else
        QMessageBox::information(NULL,QObject::tr("Insertar Imagenes"),QObject::tr("Ocurrió un error al insertar la imagen en el episodio:")+
                                 qImagen->lastError().text(),
                                 QObject::tr("Aceptar"));


}
