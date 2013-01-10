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
    void BorrarEpisodio(int nID);
    int getid();
    int getidPaciente();
    int getcerrado();
    int getprivado();
    QString getdoctor();
    QDate getfecha();
    QString getCIE();
    QString getdescripcion();
    QString gethistorial();
    int getidcie();
    QString getcodigocie();

    void setid(int id);
    void setidPaciente(int idPaciente);
    void setcerrado(int cerrado);
    void setprivado(int privado);
    void setdoctor(QString doctor);
    void setfecha(QDate fecha);
    void setCIE(QString CIE);
    void setdescripcion(QString descripcion);
    void sethistorial(QString historial);
    void setidcie(int id);
    void setcodigocie(QString codigo);


private:
    int id;
    int idPaciente;
    int cerrado;
    int privado;
    int idcie;
    QString doctor;
    QDate fecha;
    QString cie;
    QString descripcion;
    QString historial;
    QString codigocie;







};

#endif // EPISODIO_H
