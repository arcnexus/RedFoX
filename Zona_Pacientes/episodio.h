#ifndef EPISODIO_H
#define EPISODIO_H
#include <QDate>
#include <QString>
#include <QtSql>
#include <QObject>
class Episodio : public QObject
{
    Q_OBJECT
private:
    int id;
    int idPaciente;
    int cerrado;
    int privado;
    int idcie;
    int iddoctor;
    QString doctor;
    QDate fecha;
    QString cie;
    QString descripcion;
    QString historial;
    QString codigocie;

public:
    Episodio(QObject* parent = 0);
    int NuevoEpisodio(int idPaciente);
    void RecuperarEpisodio(int idEpisodio);
    QString RecuperarDoctor(int iddoctor);
    int RecuperarIdDoctor(QString doctor);
    void GuardarEpisodio();
    int getid();
    int getidPaciente();
    int getcerrado();
    int getprivado();
    int getiddoctor() {return this->iddoctor;}
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
    void setiddoctor(int iddoctor) {this->iddoctor = iddoctor;}
    void setdoctor(QString doctor);
    void setfecha(QDate fecha);
    void setCIE(QString CIE);
    void setdescripcion(QString descripcion);
    void sethistorial(QString historial);
    void setidcie(int id);
    void setcodigocie(QString codigo);

};

#endif // EPISODIO_H
