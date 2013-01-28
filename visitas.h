#ifndef VISITAS_H
#define VISITAS_H

#include "Auxiliares/Globlal_Include.h"

class Visitas : public QObject
{
    Q_OBJECT

signals:
    
public slots:
    int AnadirVisita(int nIdEpisodio, QString cMedico);
    void RecuperarVisita(int nIdVisita);
    void GuardarDatos();

private:
int id;
int idepisodio;
QDateTime fechahora;
QString medico;
QString exploracion;
QString tratamiento;
QString lengua;
QString pulso;

public:
    explicit Visitas(QObject *parent = 0);
    void setId(int id) {this->id =id;}
    void setIdEpisodio(int id) {this->idepisodio = id;}
    void setFechaHora(QDateTime dFechaH) {this->fechahora = dFechaH;}
    void setMedico(QString cMed) {this->medico = cMed;}
    void setExploracion(QString cExplo) {this->exploracion = cExplo;}
    void setTratamiento(QString cTrat) {this->tratamiento = cTrat;}
    void setLengua(QString cLengua) {this->lengua =cLengua;}
    void setPulso(QString cPulso) {this->pulso = cPulso;}

    int getId() {return this->id;}
    int getIdEpisodio() {return this->idepisodio;}
    QDateTime getFechaHora() {return this->fechahora;}
    QString getMedico() {return this->medico;}
    QString getExploracion() {return this->exploracion;}
    QString getTratamiento() {return this->tratamiento;}
    QString getLengua() {return this->lengua;}
    QString getPulso() {return this->pulso;}

    
};

#endif // VISITAS_H
