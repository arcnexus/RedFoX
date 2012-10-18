#include "paciente.h"

Paciente::Paciente()
{
}

int Paciente::getalcohol()
{
    return this->alcohol;
}

QString Paciente::getalergiasConocidas()
{
    return this->alergiasConocidas;
}

QString Paciente::getantecedentesFamiliares()
{
    return this->antecedentesFamiliares;

}

QString Paciente::getcirugiasPrevias()
{
    return this->cirugiasPrevias;
}

double Paciente::getdiastole()
{
    return this->diastole;
}

QString Paciente::getenfermedadesConocidas()
{
    return this->enfermedadesConocidas;
}

QString Paciente::getfamilia()
{
    return this->familia;
}

QDate Paciente::getfechaAlta()
{
    return this->fechaAlta;
}

QString Paciente::getfiliacion()
{
    return this->filiacion;
}

QString Paciente::gethabitosDrogas()
{
    return this->habitosDrogas;
}

int Paciente::gethijos()
{
    return this->hijos;
}

QString Paciente::gethistorial()
{
    return this->historial;
}

int Paciente::getid()
{
    return this->id;
}

int Paciente::getidCliente()
{
    return this->idCliente;
}

int Paciente::getidMutua()
{
    return this->idMutua;
}

double Paciente::getIMC()
{
    return this->IMC;

}

QDate Paciente::getnacimiento()
{
    return this->nacimiento;
}

QString Paciente::getnivelEstudios()
{
    return nivelEstudios;
}

QString Paciente::getnumSS()
{
    return this->numSS;
}

QString Paciente::getotrasDrogas()
{
    return this->otrasDrogas;


}

double Paciente::getperimetroCraneal()
{
    this->perimetroCraneal;
}

double Paciente::getpeso()
{
    return this->peso;
}

QString Paciente::getprofesion()
{
    return this->profesion;
}

QString Paciente::getsexo()
{
    return this->sexo;
}

double Paciente::getsistole()
{
    return this->sistole;
}

int Paciente::gettabaco()
{
    return this->tabaco;
}

double Paciente::gettalla()
{
    return this->tabaco;
}

int Paciente::gettrabaja()
{
    return this->trabaja;
}

void Paciente::setalcohol(int alcohol)
{
    this->alcohol = alcohol;
}

void Paciente::setalergiasConocidas(QString alergiasConocidas)
{
    this->alergiasConocidas = alergiasConocidas;
}

void Paciente::setantecedentesFamiliares(QString antecedentesFamiliares)
{
    this->antecedentesFamiliares = antecedentesFamiliares;
}

void Paciente::setcirugiasPrevias(QString cirugiasPrevias)
{
    this->cirugiasPrevias = cirugiasPrevias;
}

void Paciente::setdiastole(double diastole)
{
    this->diastole = diastole;
}

void Paciente::setenfermedadesConocidas(QString enfermedadesConocidas)
{
    this->enfermedadesConocidas = enfermedadesConocidas;
}

void Paciente::setfamilia(QString familia)
{
    this->familia = familia;
}

void Paciente::setfechaAlta(QDate fechaAlta)
{
    this->fechaAlta = fechaAlta;
}

void Paciente::setfiliacion(QString filiacion)
{
    this->filiacion = filiacion;
}

void Paciente::sethabitosDrogas(QString habitosDrogas)
{
    this->habitosDrogas = habitosDrogas;
}

void Paciente::sethijos(int hijos)
{
    this->hijos = hijos;
}

void Paciente::sethistorial(QString historial)
{
    this->historial = historial;
}

void Paciente::setid(int id)
{
    this->id = id;
}

void Paciente::setidCliente(int idCliente)
{
    this->idCliente = idCliente;
}

void Paciente::setidMutua(int idMutua)
{
    this->idMutua = idMutua;
}

void Paciente::setIMC(double IMC)
{
    this->IMC = IMC;
}

void Paciente::setnacimiento(QDate nacimiento)
{
    this->nacimiento = nacimiento;
}

void Paciente::setnivelEstudios(QString nivelEstudios)
{
    this->nivelEstudios = nivelEstudios;
}

void Paciente::setnumSS(QString numSS)
{
    this->numSS = numSS;
}

void Paciente::setotrasDrogas(QString otrasDrogas)
{
    this->otrasDrogas = otrasDrogas;

}

void Paciente::setperimetroCraneal(double perimetroCraneal)
{
    this->perimetroCraneal = perimetroCraneal;
}

void Paciente::setpeso(double peso)
{
    this->peso = peso;
}

void Paciente::setprofesion(QString profesion)
{
    this->profesion = profesion;
}

void Paciente::setsexo(QString sexo)
{
    this->sexo = sexo;
}

void Paciente::setsistole(double sistole)
{
    this->sistole = sistole;
}

void Paciente::settabaco(int tabaco)
{
    this->tabaco = tabaco;
}

void Paciente::settalla(double talla)
{
    this->talla = talla;
}

void Paciente::settrabaja(int trabaja)
{
    this->trabaja = trabaja;
}


