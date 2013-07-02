#ifndef EPISODIO_H
#define EPISODIO_H

#include "../Auxiliares/Globlal_Include.h"

class Episodio : public QObject
{
    Q_OBJECT
public:
    int id;
    int id_paciente;
    int cerrado;
    int privado;
    int idcie;
    int id_doctor;
    QString doctor;
    QDate fecha;
    QString cie;
    QString descripcion;
    QString historial;
    QString codigocie;
    bool Altas;
    Episodio(QObject* parent = 0);
    void NuevoEpisodio(int id_paciente);
    void RecuperarEpisodio(int id_episodio);
    QString RecuperarDoctor(int id_doctor);
    int Recuperarid_doctor(QString doctor);
    void setAltas(bool state) {this->Altas = state;}
    bool getAltas() {return this->Altas;}

    void GuardarEpisodio();
    int getid();
    int getid_paciente();
    int getcerrado();
    int getprivado();
    int getid_doctor() {return this->id_doctor;}
    QString getdoctor();
    QDate getfecha();
    QString getCIE();
    QString getdescripcion();
    QString gethistorial();
    int getidcie();
    QString getcodigocie();

    void setid(int id);
    void setid_paciente(int id_paciente);
    void setcerrado(int cerrado);
    void setprivado(int privado);
    void setid_doctor(int id_doctor) {this->id_doctor = id_doctor;}
    void setdoctor(QString doctor);
    void setfecha(QDate fecha);
    void setCIE(QString CIE);
    void setdescripcion(QString descripcion);
    void sethistorial(QString historial);
    void setidcie(int id);
    void setcodigocie(QString codigo);

};

#endif // EPISODIO_H
