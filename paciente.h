#ifndef PACIENTE_H
#define PACIENTE_H
#include <QtSql>
#include <QString>
#include <QDate>

class Paciente
{
public:
    Paciente();
public slots:
    // Funciones varias
    void nuevoEpisodio();
    Paciente RecuperarPaciente(int idCliente);


    // Getters
    int getalcohol();
    bool getalcoholbool();
    int getnumHistoria();
    QString getalergiasConocidas();
    QString getantecedentesFamiliares();
    QString getcirugiasPrevias();
    double getdiastole();
    QString getenfermedadesConocidas();
    QString getfamilia();
    QDate getfechaAlta();
    QString getfiliacion();
    QString gethabitosDrogas();
    int gethijos();
    QString gethistorial();
    int getid();
    int getidCliente();
    int getidMutua();
    double getIMC();
    QDateTime getnacimiento();
    QString getnivelEstudios();
    QString getnumSS();
    QString getotrasDrogas();
    double getperimetroCraneal();
    double getpeso();
    QString getprofesion();
    QString getsexo();
    double getsistole();
    int gettabaco();
    bool gettabacobool();
    double gettalla();
    int gettrabaja();
    bool gettrabajabool();

    // setters
    void setalcohol(int alcohol);
    void setnumHistoria(int numhistoria);
    void setalergiasConocidas(QString alergiasConocidas);
    void setantecedentesFamiliares(QString antecedentesFamiliares);
    void setcirugiasPrevias(QString cirugiasPrevias);
    void setdiastole(double diastole);
    void setenfermedadesConocidas(QString enfermedadesConocidas);
    void setfamilia(QString familia);
    void setfechaAlta(QDate fechaAlta);
    void setfiliacion(QString filiacion);
    void sethabitosDrogas(QString habitosDrogas);
    void sethijos(int hijos);
    void sethistorial(QString historial);
    void setid(int id);
    void setidCliente(int idCliente);
    void setidMutua(int idMutua);
    void setIMC(double IMC);
    void setnacimiento(QDateTime nacimiento);
    void setnivelEstudios(QString nivelEstudios);
    void setnumSS(QString numSS);
    void setotrasDrogas(QString otrasDrogas);
    void setperimetroCraneal(double perimetroCraneal);
    void setpeso(double peso);
    void setprofesion(QString profesion);
    void setsexo(QString sexo);
    void setsistole(double sistole );
    void settabaco(int tabaco);
    void settalla(double talla);
    void settrabaja(int trabaja);

private:

    int alcohol;
    int numhistoria;
    QString alergiasConocidas;
    QString antecedentesFamiliares;
    QString cirugiasPrevias;
    double diastole;
    QString enfermedadesConocidas;
    QString familia;
    QDate fechaAlta;
    QString filiacion;
    QString habitosDrogas;
    int hijos;
    QString historial;
    int id;
        int idCliente;
    int idMutua;
    double IMC;
    QDateTime nacimiento;
    QString nivelEstudios;
    QString numSS;
    QString otrasDrogas;
    double perimetroCraneal;
    double peso;
    QString profesion;
    QString sexo;
    double sistole;
    int tabaco;
    double talla;
    int trabaja;

};

#endif // PACIENTE_H
