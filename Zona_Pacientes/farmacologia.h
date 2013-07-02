#ifndef FARMACOLOGIA_H
#define FARMACOLOGIA_H

#include "../Auxiliares/Globlal_Include.h"

class Farmacologia
{
public:
    Farmacologia();

    int getidFarmacologia();
    int getid_medicamento();
    QString getmedicamento();
    QDate getinicio_tratamiento();
    QString getindicacion_posologia();
    QString gecomentarios();
    int getid_episodio();
    int getactivo();
    QString getcodigo_nacional();

    void setidFarmacologia(int id);
    void setid_medicamento(int id_medicamento);
    void setmedicamento(QString medicamento);
    void setinicio_tratamiento(QDate inicio_tratamiento);
    void setindicacion_posologia(QString indicacion_posologia);
    void secomentarios(QString comentarios);
    void setid_episodio(int id_episodio);
    void setactivo(int activo);
    void setcodigo_nacional(QString codigo_nacional);

    int id;
    int id_paciente;
    int id_medicamento;
    QString medicamento;
    QDate inicio_tratamiento;
    QString indicacion_posologia;
    QString comentarios;
    int id_episodio;
    int activo;
    QString codigo_nacional;
 public slots:
    void AnadirFarmaco();
    void modificarFarmaco(int id);
    void borrarFarmaco(int id);
    void cargarDatos(QString cSQL);
};

#endif // FARMACOLOGIA_H
