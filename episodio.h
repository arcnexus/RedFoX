#ifndef EPISODIO_H
#define EPISODIO_H
#include <QDate>
#include <QString>
#include <QtSql>

class Episodio
{
public:
    Episodio();
    int NuevoEpisodio(int idPaciente);
    void RecuperarEpisodio(int idEpisodio);
    void GuardarEpisodio();
    int getid();
    int getidPaciente();
    int getcerrado();
    int getprivado();
    QString getdoctor();
    QDate getfecha();
    QString getCIE();
    QString getdescripcion();
    QString gethistorial();

    void setid(int id);
    void setidPaciente(int idPaciente);
    void setcerrado(int cerrado);
    void setprivado(int privado);
    void setdoctor(QString doctor);
    void setfecha(QDate fecha);
    void setCIE(QString CIE);
    void setdescripcion(QString descripcion);
    void sethistorial(QString historial);
private:
    int id;
    int idPaciente;
    int cerrado;
    int privado;
    QString doctor;
    QDate fecha;
    QString cie;
    QString descripcion;
    QString historial;




};

#endif // EPISODIO_H
