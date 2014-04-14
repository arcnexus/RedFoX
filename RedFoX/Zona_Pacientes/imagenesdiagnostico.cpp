#include "imagenesdiagnostico.h"

ImagenesDiagnostico::ImagenesDiagnostico(QObject *parent) :
    QObject(parent)
{
}

void ImagenesDiagnostico::guardarDatosDB()
{
        //emit ui_ponerDatosEnObjetoImagen();
        QScopedPointer<QSqlQuery>qImagen (new QSqlQuery(QSqlDatabase::database("dbmedica")));
        //QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        qImagen->prepare("UPDATE imagenes SET "
                         "comentarios = :comentarios,"
                         "evaluada =:evaluada,"
                         "descripcion = :descripcion,"
                         "fecha_imagen = :fecha_imagen,"
                         "id_tipo_imagen = :id_tipo_imagen "
                         "WHERE id =:id");


        qImagen->bindValue(":comentarios",this->Comentarios);
        qImagen->bindValue(":evaluada",this->Evaluada);
        qImagen->bindValue(":descripcion",this->Descripcion);
        qImagen->bindValue(":fecha_imagen",this->fecha_imagen);
        qImagen->bindValue(":id_tipo_imagen",this->id_tipo_imagen);
        qImagen->bindValue(":id",this->id);

        if(qImagen->exec()){
            TimedMessageBox::Box(qApp->activeWindow(),QObject::tr("La imagen ha sido modificada corectamente"));
            emit refrescarlista();
        } else
            TimedMessageBox::Box(qApp->activeWindow(),QObject::tr("Ocurrió un error al modificar la imagen en el episodio:\n")+
                                     qImagen->lastError().text());
}


void ImagenesDiagnostico::llenarObjetoconDatosDB()
{
    QScopedPointer<QSqlQuery>qImagen (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "Select * from imagenes where id =:id";

    qImagen->prepare(cSQL);
    qImagen->bindValue(":id",this->id);
    if (qImagen->exec()) {
        qImagen->next();
        QSqlRecord rImagen = qImagen->record();
        this->id_episodio = rImagen.field("id_episodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->fecha_imagen = rImagen.field("fecha_imagen").value().toDate();
        this->localizacion_imagen = rImagen.field("localizacion_imagen").value().toString();
        this->id_tipo_imagen = rImagen.field("id_tipo_imagen").value().toInt();
        this->Evaluada =rImagen.field("evaluada").value().toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Imagenes Diagnóstico"),QObject::tr("No se ha podido recuperar el registro de imagen"),
                             QObject::tr("Aceptar"));
    }
}

void ImagenesDiagnostico::llenarObjetoconDatosDB(int nid)
{
    QScopedPointer<QSqlQuery>qImagen (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qImagen = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = "Select * from imagenes where id =:id";

    qImagen->prepare(cSQL);
    qImagen->bindValue(":id",nid);
    if (qImagen->exec()) {
        qImagen->next();
        QSqlRecord rImagen = qImagen->record();
        this->id_episodio = rImagen.field("id_episodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->fecha_imagen = rImagen.field("fecha_imagen").value().toDate();
        this->localizacion_imagen = rImagen.field("localizacion_imagen").value().toString();
        this->id_tipo_imagen = rImagen.field("id_tipo_imagen").value().toInt();
        this->Evaluada =rImagen.field("evaluada").value().toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Imagenes Diagnóstico"),QObject::tr("No se ha podido recuperar el registro de imagen"),
                             QObject::tr("Aceptar"));
    }
}

void ImagenesDiagnostico::BorrarImagen(int nid)
{
    if(QMessageBox::question(NULL,QObject::tr("Borrar imagen del historial"),
                             QObject::tr("Va a proceder a borrar una imagen del historial, ¿Desea continuar?"),
                             QObject::tr("Cancelar"),QObject::tr("Borrar")) ==QMessageBox::Accepted) {
        QScopedPointer<QSqlQuery>qImagenes (new QSqlQuery(QSqlDatabase::database("dbmedica")));
        //QSqlQuery *qImagenes = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        qImagenes->prepare("delete from imagenes where id = :nid");
        qImagenes->bindValue(":nid",nid);
        if(!qImagenes->exec())
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Borrar Imagen"),tr("Atención::No se puede borrar la imagen :")+
                                 qImagenes->lastError().text(),tr("Aceptar"));


    }
}

int ImagenesDiagnostico::Devolverid_tipo_imagen(QString tipo_imagen)
{
    QSqlQuery queryTiposImagen(QSqlDatabase::database("dbmedica"));
    queryTiposImagen.prepare("Select id from tiposimagen where descripcion = :tipo_imagen");
    queryTiposImagen.bindValue(":tipo_imagen",tipo_imagen);
    if(queryTiposImagen.exec()){
        queryTiposImagen.next();
        return queryTiposImagen.record().value("id").toInt();
    }
    return -1;
}

QString ImagenesDiagnostico::DevolverTexto_tipo_imagen(int id)
{
    QSqlQuery queryTiposImagen(QSqlDatabase::database("dbmedica"));
    queryTiposImagen.prepare("Select descripcion from tiposimagen where id = :id");
    queryTiposImagen.bindValue(":id",id);
    if(queryTiposImagen.exec()){
        queryTiposImagen.next();
        return queryTiposImagen.record().value("descripcion").toString();
    }
    return "";
}



