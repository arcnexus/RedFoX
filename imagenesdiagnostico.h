#ifndef IMAGENESDIAGNOSTICO_H
#define IMAGENESDIAGNOSTICO_H
#include <QString>
#include <QDate>

class ImagenesDiagnostico
{
private:
    int id;
    int idEpisodio;
    QString Descripcion;
    QDate FechaImagen;
    QString TipoImagen;
    QString Comentarios;
    QString LocalizacionImagen;

public:
    ImagenesDiagnostico();
    void setId(int id) {this->id = id;}
    void setIdEpisodio(int idEpisodio) {this->idEpisodio = idEpisodio;}
    void setDescripcion(QString Descripcion) {this->Descripcion = Descripcion;}
    void setFechaImagen(QDate FechaImagen) {this->FechaImagen = FechaImagen;}
    void setTipoImagen(QString TipoImagen) {this->TipoImagen = TipoImagen;}
    void setComentarios(QString Comentarios) {this->Comentarios = Comentarios;}
    void setLocalizacionImagen(QString LocalizacionImagen) {this->LocalizacionImagen = LocalizacionImagen;}

    int getId() {return this->id;}
    int getIdEpisodio() {return this->idEpisodio;}
    QString getDescripcion() {return this->Descripcion;}
    QDate getFechaImagen() {return this->FechaImagen;}
    QString getTipoImagen() {return this->TipoImagen;}
    QString getComentarios() {return this->Comentarios;}
    QString getLocalizacionImagen() {return this->LocalizacionImagen;}

    void llenarObjetoconDatosDB();
    void guardarDatosDB();

};

#endif // IMAGENESDIAGNOSTICO_H
