#ifndef FARMACOLOGIA_H
#define FARMACOLOGIA_H

#include <QString>
#include <QDate>

class Farmacologia
{
public:
    Farmacologia();

    int getidFarmacologia();
    int getidmedicamento();
    QString getmedicamento();
    QDate getiniciotratamiento();
    QString getindicacionposologia();
    QString getcomentarios();
    int getidepisodio();
    int getactivo();

    void setidFarmacologia(int id);
    void setidmedicamento(int idmedicamento);
    void setmedicamento(QString medicamento);
    void setiniciotratamiento(QDate iniciotratamiento);
    void setindicacionposologia(QString indicacionposologia);
    void setcomentarios(QString comentarios);
    void setidepisodio(int idepisodio);
    void setactivo(int activo);

private:
    int id;
    int idmedicamento;
    QString medicamento;
    QDate iniciotratamiento;
    QString indicacionposologia;
    QString comentarios;
    int idepisodio;
    int activo;
 public slots:
    void AnadirFarmaco();
    void modificarFarmaco();
    void cargarDatos(QString cSQL);
};

#endif // FARMACOLOGIA_H
