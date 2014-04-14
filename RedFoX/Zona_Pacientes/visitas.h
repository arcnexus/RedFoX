#ifndef VISITAS_H
#define VISITAS_H



#include "Auxiliares/Globlal_Include.h"
class Visitas : public QObject
{
    Q_OBJECT

signals:
    
public slots:
    int AnadirVisita(int nid_episodio, QString cMedico);
    void RecuperarVisita(int nidVisita);
    void GuardarDatos();

public:
    int id;
    int id_episodio;
    QDateTime fecha_hora;
    QString medico;
    QString exploracion;
    QString tratamiento;
    QString lengua;
    QString pulso;
    QString diagnostico;
    explicit Visitas(QObject *parent = 0);

    
};

#endif // VISITAS_H
