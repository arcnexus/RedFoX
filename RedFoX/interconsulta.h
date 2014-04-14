#ifndef INTERCONSULTA_H
#define INTERCONSULTA_H

#include "Auxiliares/Globlal_Include.h"

class Interconsulta : public QObject
{
    Q_OBJECT
private:
    int id;
    int id_paciente;
    int id_episodio;
    QString referidode;
    QString motivoReferido;
    QString deivara;
    QString motivoDeivacion;
    QString peticion;
    QString respuesta;
    int recibido;
    QDate fechaPeticion;
    QDate fechaRespuesta;


public:
    explicit Interconsulta(QObject *parent = 0);
    void setid(int id) {this->id = id;}
    void setid_paciente(int id_paciente) {this->id_paciente = id_paciente;}
    void setid_episodio(int id_episodio) {this->id_episodio = id_episodio;}
    void setreferidode(QString referidode) {this->referidode = referidode;}
    void setmotivoReferido(QString motivoReferido) {this->motivoReferido = motivoReferido;}
    void setdeivara(QString deivara) {this->deivara = deivara; }
    void setmotivoDeivacion(QString motivoDeivacion) {this->motivoDeivacion = motivoDeivacion;}
    void setpeticion(QString peticion) {this->peticion = peticion;}
    void setrespuesta(QString respuesta) {this->respuesta = respuesta;}
    void setrecibido(int recibido) {this->recibido = recibido;}
    void setfechaPeticion(QDate fechaPeticion) {this->fechaPeticion = fechaPeticion;}
    void setfechaRespuesta(QDate fechaRespuesta) {this->fechaRespuesta = fechaRespuesta;}

    int getid() {return this->id;}
    int getid_paciente() {return this->id_paciente;}
    int getid_episodio() {return this->id_episodio;}
    QString getreferidode() {return this->referidode;}
    QString getmotivoReferido() {return this->motivoReferido;}
    QString getdeivara() {return this->deivara;}
    QString getmotivoDeivacion() {return this->motivoDeivacion;}
    QString getpeticion() {return this->peticion;}
    QString getrespuesta() {return this->respuesta;}
    int getrecibido() {return this->recibido;}
    QDate getfechaPeticion() {return this->fechaPeticion;}
    QDate getfechaRespuesta() {return this->fechaRespuesta;}
    
signals:
    
public slots:
    void AnadirInterconsulta(int id_episodio,int id_paciente);

    



};

#endif // INTERCONSULTA_H
