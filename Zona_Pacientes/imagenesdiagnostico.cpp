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
                         "fechaimagen = :fechaimagen,"
                         "idtipoimagen = :idtipoimagen "
                         "WHERE id =:id");


        qImagen->bindValue(":comentarios",this->Comentarios);
        qImagen->bindValue(":evaluada",this->Evaluada);
        qImagen->bindValue(":descripcion",this->Descripcion);
        qImagen->bindValue(":fechaimagen",this->FechaImagen);
        qImagen->bindValue(":idtipoimagen",this->idTipoImagen);
        qImagen->bindValue(":id",this->id);

        if(qImagen->exec()){
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("La imagen ha sido modificada correctamente"));
            emit refrescarlista();
        } else
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("Ocurrió un error al modificar la imagen en el episodio:\n")+
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
        this->idEpisodio = rImagen.field("idepisodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->FechaImagen = rImagen.field("fechaimagen").value().toDate();
        this->LocalizacionImagen = rImagen.field("localizacionimagen").value().toString();
        this->idTipoImagen = rImagen.field("idtipoimagen").value().toInt();
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
        this->idEpisodio = rImagen.field("idepisodio").value().toInt();
        this->Comentarios = rImagen.field("comentarios").value().toString();
        this->Descripcion = rImagen.field("descripcion").value().toString();
        this->FechaImagen = rImagen.field("fechaimagen").value().toDate();
        this->LocalizacionImagen = rImagen.field("localizacionimagen").value().toString();
        this->idTipoImagen = rImagen.field("idtipoimagen").value().toInt();
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

int ImagenesDiagnostico::DevolverId_tipo_imagen(QString TipoImagen)
{
    QSqlQuery queryTiposImagen(QSqlDatabase::database("dbmedica"));
    queryTiposImagen.prepare("Select id from tiposimagen where descripcion = :TipoImagen");
    queryTiposImagen.bindValue(":TipoImagen",TipoImagen);
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



