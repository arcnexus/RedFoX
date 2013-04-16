#ifndef IMAGENESDIAGNOSTICO_H
#define IMAGENESDIAGNOSTICO_H

#include "Auxiliares/Globlal_Include.h"

class ImagenesDiagnostico : public QObject
{
    Q_OBJECT


public:
    int id;
    int idEpisodio;
    QString Descripcion;
    QDate FechaImagen;
    int idTipoImagen;
    QString Comentarios;
    QString LocalizacionImagen;
    int Evaluada;

public:
    explicit ImagenesDiagnostico(QObject *parent =0);

public slots:
    void guardarDatosDB();
    void llenarObjetoconDatosDB();
    void llenarObjetoconDatosDB(int nid);
    void BorrarImagen(int nid);
    int DevolverId_tipo_imagen(QString TipoImagen);
    QString DevolverTexto_tipo_imagen(int id);

private slots:


signals:
    void ui_ponerDatosEnObjetoImagen();
    void cerrarventana();
    void refrescarlista();
};

#endif // IMAGENESDIAGNOSTICO_H
