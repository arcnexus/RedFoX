#ifndef IMAGENESDIAGNOSTICO_H
#define IMAGENESDIAGNOSTICO_H

#include "Auxiliares/Globlal_Include.h"

class ImagenesDiagnostico : public QObject
{
    Q_OBJECT


public:
    int id;
    int id_episodio;
    QString Descripcion;
    QDate fecha_imagen;
    int id_tipo_imagen;
    QString Comentarios;
    QString localizacion_imagen;
    int Evaluada;

public:
    explicit ImagenesDiagnostico(QObject *parent =0);

public slots:
    void guardarDatosDB();
    void llenarObjetoconDatosDB();
    void llenarObjetoconDatosDB(int nid);
    void BorrarImagen(int nid);
    int Devolverid_tipo_imagen(QString tipo_imagen);
    QString DevolverTexto_tipo_imagen(int id);

private slots:


signals:
    void ui_ponerDatosEnObjetoImagen();
    void cerrarventana();
    void refrescarlista();
};

#endif // IMAGENESDIAGNOSTICO_H
