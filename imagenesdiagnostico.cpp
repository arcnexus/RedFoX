#include "imagenesdiagnostico.h"
#include <QtSql>
#include <QSqlRecord>
#include <QMessageBox>

ImagenesDiagnostico::ImagenesDiagnostico()
{
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
    } else {
        QMessageBox::warning(NULL,QObject::tr("Imagenes Diagnóstico"),QObject::tr("No se ha podido recuperar el registro de imagen"),
                             QObject::tr("Aceptar"));
    }
}

void ImagenesDiagnostico::guardarDatosDB()
{

}
