#include "episodio.h"
#include "sqlcalls.h"

Episodio::Episodio()
{
}

int Episodio::NuevoEpisodio(int idPaciente)
{
    SqlCalls *llamadas = new SqlCalls();
    QStringList parametros;
    QString id =QString::number(this->id);
    parametros.append(QString::number(idPaciente));
    int nid = llamadas->addRec("insert into episodios (idpaciente) values(?)",parametros,"dbmedica");
    return nid;

}

QSqlQuery Episodio::RecuperarEpisodio(int idEpisodio)
{
    SqlCalls *llamadas = new SqlCalls();
    QStringList parametros;
    parametros.append(QString::number(idEpisodio));
    QSqlQuery qEpisodio = llamadas->query("select * from episodios where id =?",parametros,"dbmedica");
    if (qEpisodio.next())

        return qEpisodio;

}

int Episodio::getid()
{
    return this->id;
}

int Episodio::getidPaciente()
{
    return this->idPaciente;
}

int Episodio::getcerrado()
{
    return this->cerrado;
}

int Episodio::getprivado()
{
    return this->privado;
}

QString Episodio::getdoctor()
{
    return this->doctor;
}

QDate Episodio::getfecha()
{
    return this->fecha;
}

QString Episodio::getCIE()
{
    return this->cie;
}

QString Episodio::getdescripcion()
{
    return this->descripcion;
}

QString Episodio::gethistorial()
{
    return this->historial;
}

void Episodio::setid(int id)
{
    this->id = id;
}

void Episodio::setidPaciente(int idPaciente)
{
    this->idPaciente = idPaciente;
}

void Episodio::setcerrado(int cerrado)
{
    this->cerrado = cerrado;
}

void Episodio::setprivado(int privado)
{
    this->privado = privado;
}

void Episodio::setdoctor(QString doctor)
{
    this->doctor = doctor;
}

void Episodio::setfecha(QDate fecha)
{
    this->fecha = fecha;
}

void Episodio::setCIE(QString CIE)
{
    this->cie = CIE;
}

void Episodio::setdescripcion(QString descripcion)
{
    this->descripcion = descripcion;
}

void Episodio::sethistorial(QString historial)
{
    this->historial = historial;
}

