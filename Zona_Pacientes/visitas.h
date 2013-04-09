#ifndef VISITAS_H
#define VISITAS_H



#include "../Auxiliares/Globlal_Include.h"
class Visitas : public QObject
{
    Q_OBJECT

signals:
    
public slots:
    int AnadirVisita(int nIdEpisodio, QString cMedico);
    void RecuperarVisita(int nIdVisita);
    void GuardarDatos();

public:
    int id;
    int idepisodio;
    QDateTime fechahora;
    QString medico;
    QString exploracion;
    QString tratamiento;
    QString lengua;
    QString pulso;
    QString diagnostico;
    explicit Visitas(QObject *parent = 0);

    
};

#endif // VISITAS_H
