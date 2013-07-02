#ifndef PACIENTE_H
#define PACIENTE_H

#include "../Auxiliares/Globlal_Include.h"

#include "episodio.h"

class Paciente
{
public:
    Paciente();
    ~Paciente();
    QSqlQuery *qepisodio;
public slots:
    // Funciones varias
    void RecuperarPaciente(int id_cliente);
    void GuardarPaciente();
    int AnadirPaciente(int id_cliente);

    // Getters
    int getalcohol();
    bool getalcoholbool();
    int getnum_historia();
    QString getalergias_conocidas();
    QString getantecedentes_familiares();
    QString getcirugias_previas();
    double getdiastole();
    QString getenfermedadesConocidas();
    QString getfamilia();
    QDate getfecha_alta();
    QString getfiliacion();
    QString gethabitos_drogas();
    int gethijos();
    QString gethistorial();
    int getid();
    int getid_cliente();
    int getid_mutua();
    double getimc();
    QDateTime getnacimiento();
    QString getnivel_estudios();
    QString getnum_ss();
    int getotras_drogas();
    bool getotras_drogasbool();
    double getperimetro_craneal();
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
    void setnum_historia(int num_historia);
    void setalergias_conocidas(QString alergias_conocidas);
    void setantecedentes_familiares(QString antecedentes_familiares);
    void setcirugias_previas(QString cirugias_previas);
    void setdiastole(double diastole);
    void setenfermedadesConocidas(QString enfermedadesConocidas);
    void setfamilia(QString familia);
    void setfecha_alta(QDate fecha_alta);
    void setfiliacion(QString filiacion);
    void sethabitos_drogas(QString habitos_drogas);
    void sethijos(int hijos);
    void sethistorial(QString historial);
    void setid(int id);
    void setid_cliente(int id_cliente);
    void setid_mutua(int id_mutua);
    void setimc(double imc);
    void setnacimiento(QDateTime nacimiento);
    void setnivel_estudios(QString nivel_estudios);
    void setnum_ss(QString num_ss);
    void setotras_drogas(int otras_drogas);
    void setperimetro_craneal(double perimetro_craneal);
    void setpeso(double peso);
    void setprofesion(QString profesion);
    void setsexo(QString sexo);
    void setsistole(double sistole );
    void settabaco(int tabaco);
    void settalla(double talla);
    void settrabaja(int trabaja);

public:

    int alcohol;
    int num_historia;
    QString alergias_conocidas;
    QString antecedentes_familiares;
    QString cirugias_previas;
    double diastole;
    QString enfermedadesConocidas;
    QString familia;
    QDate fecha_alta;
    QString filiacion;
    QString habitos_drogas;
    int hijos;
    QString historial;
    int id;
        int id_cliente;
    int id_mutua;
    double imc;
    QDateTime nacimiento;
    QString nivel_estudios;
    QString num_ss;
    int otras_drogas;
    double perimetro_craneal;
    double peso;
    QString profesion;
    QString sexo;
    double sistole;
    int tabaco;
    double talla;
    int trabaja;

};

#endif // PACIENTE_H
