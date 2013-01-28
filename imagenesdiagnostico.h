#ifndef IMAGENESDIAGNOSTICO_H
#define IMAGENESDIAGNOSTICO_H

#include "Auxiliares/Globlal_Include.h"

class ImagenesDiagnostico : public QObject
{
    Q_OBJECT


private:
    int id;
    int idEpisodio;
    QString Descripcion;
    QDate FechaImagen;
    QString TipoImagen;
    QString Comentarios;
    QString LocalizacionImagen;
    int Evaluada;

public:
    explicit ImagenesDiagnostico(QObject *parent =0);
    void setId(int id) {this->id = id;}
    void setIdEpisodio(int idEpisodio) {this->idEpisodio = idEpisodio;}
    void setDescripcion(QString Descripcion) {this->Descripcion = Descripcion;}
    void setFechaImagen(QDate FechaImagen) {this->FechaImagen = FechaImagen;}
    void setTipoImagen(QString TipoImagen) {this->TipoImagen = TipoImagen;}
    void setComentarios(QString Comentarios) {this->Comentarios = Comentarios;}
    void setLocalizacionImagen(QString LocalizacionImagen) {this->LocalizacionImagen = LocalizacionImagen;}
    void setEvaluada(int Evaluada) {this->Evaluada = Evaluada;}

    int getId() {return this->id;}
    int getIdEpisodio() {return this->idEpisodio;}
    QString getDescripcion() {return this->Descripcion;}
    QDate getFechaImagen() {return this->FechaImagen;}
    QString getTipoImagen() {return this->TipoImagen;}
    QString getComentarios() {return this->Comentarios;}
    QString getLocalizacionImagen() {return this->LocalizacionImagen;}
    int getEvaluada() {return this->Evaluada;}

public slots:
    void anadirDatosDB();
    void guardarDatosDB();
    void llenarObjetoconDatosDB();
    void llenarObjetoconDatosDB(int nid);
    void BorrarImagen(int nid);

private slots:


signals:
    void ui_ponerDatosEnObjetoImagen();
    void cerrarventana();
    void refrescarlista();
};

#endif // IMAGENESDIAGNOSTICO_H
