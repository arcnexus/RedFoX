#ifndef INTERCONSULTA_H
#define INTERCONSULTA_H

#include <QObject>
#include <QDate>

class Interconsulta : public QObject
{
    Q_OBJECT
private:
    int id;
    int idPaciente;
    int idEpisodio;
    QString referidode;
    QString motivoReferido;
    QString derivara;
    QString motivoDerivacion;
    QString peticion;
    QString respuesta;
    int recibido;
    QDate fechaPeticion;
    QDate fechaRespuesta;


public:
    explicit Interconsulta(QObject *parent = 0);
    void setid(int id) {this->id = id;}
    void setidPaciente(int idPaciente) {this->idPaciente = idPaciente;}
    void setidEpisodio(int idEpisodio) {this->idEpisodio = idEpisodio;}
    void setreferidode(QString referidode) {this->referidode = referidode;}
    void setmotivoReferido(QString motivoReferido) {this->motivoReferido = motivoReferido;}
    void setderivara(QString derivara) {this->derivara = derivara; }
    void setmotivoDerivacion(QString motivoDerivacion) {this->motivoDerivacion = motivoDerivacion;}
    void setpeticion(QString peticion) {this->peticion = peticion;}
    void setrespuesta(QString respuesta) {this->respuesta = respuesta;}
    void setrecibido(int recibido) {this->recibido = recibido;}
    void setfechaPeticion(QDate fechaPeticion) {this->fechaPeticion = fechaPeticion;}
    void setfechaRespuesta(QDate fechaRespuesta) {this->fechaRespuesta = fechaRespuesta;}

    int getid() {return this->id;}
    int getidPaciente() {return this->idPaciente;}
    int getidEpisodio() {return this->idEpisodio;}
    QString getreferidode() {return this->referidode;}
    QString getmotivoReferido() {return this->motivoReferido;}
    QString getderivara() {return this->derivara;}
    QString getmotivoDerivacion() {return this->motivoDerivacion;}
    QString getpeticion() {return this->peticion;}
    QString getrespuesta() {return this->respuesta;}
    int getrecibido() {return this->recibido;}
    QDate getfechaPeticion() {return this->fechaPeticion;}
    QDate getfechaRespuesta() {return this->fechaRespuesta;}
    
signals:
    
public slots:
    void AnadirInterconsulta(int idEpisodio,int idPaciente);

    



};

#endif // INTERCONSULTA_H
