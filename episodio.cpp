#include "episodio.h"
#include "sqlcalls.h"
#include <QMessageBox>

Episodio::Episodio()
{
}

int Episodio::NuevoEpisodio(int idPaciente)
{
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("insert into episodios (idpaciente,cerrado) values(:idpaciente,0)");
    qEpisodio->bindValue(":idPaciente",idPaciente);
    if(qEpisodio->exec()) {
        int nid;
        nid =qEpisodio->lastInsertId().toInt();
        return nid;
    } else {
        QMessageBox::warning(NULL,QObject::tr("ERROR: Inserción de Episodios"),QObject::tr("Falló la inserción de episodios, error servidor:")+
                             qEpisodio->lastError().text());
        return 0;
    }
}

void Episodio::RecuperarEpisodio(int idEpisodio)
{
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("Select * from episodios where id = :idEpisodio");
    qEpisodio->bindValue(":idEpisodio",idEpisodio);
    if(qEpisodio->exec()) {
        qEpisodio->next();

        QSqlRecord rEpisodio = qEpisodio->record();
        this->id =rEpisodio.field("id").value().toInt();
        this->setidPaciente(rEpisodio.field("idpaciente").value().toInt());
        this->setcerrado(rEpisodio.field("cerrado").value().toInt());
        this->setCIE(rEpisodio.field("cie").value().toString());
        this->setdescripcion(rEpisodio.field("descripcion").value().toString());
        this->setdoctor(rEpisodio.field("doctor").value().toString());
        this->setfecha(rEpisodio.field("fecha").value().toDate());
        this->sethistorial(rEpisodio.field("historial").value().toString());
        this->setprivado(rEpisodio.field("privado").value().toInt());

    } else {
        QMessageBox::warning(NULL,QObject::tr("ERROR: Recuperar episodio"),QObject::tr("No se pudo recuperar el episodio. ERROR servidor:")+
                             qEpisodio->lastError().text(),QObject::tr("Aceptar"));
    }



}

void Episodio::GuardarEpisodio()
{
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("UPDATE episodios SET "
                       "idpaciente = :idpaciente,"
                       "cerrado = :cerrado,"
                       "privado = :privado,"
                       "doctor = :doctor,"
                       "fecha = :fecha,"
                       "historial = :historial,"
                       "cie = :cie,"
                       "descripcion = :descripcion "
                       "WHERE id =:id");
    qEpisodio->bindValue(":idpaciente",this->idPaciente);
    qEpisodio->bindValue(":cerrado",this->cerrado);
    qEpisodio->bindValue(":privado",this->privado);
    qEpisodio->bindValue(":doctor",this->doctor);
    qEpisodio->bindValue(":fecha",this->fecha);
    qEpisodio->bindValue(":historial",this->historial);
    qEpisodio->bindValue(":cie",this->cie);
    qEpisodio->bindValue(":descripcion",this->descripcion);
    qEpisodio->bindValue(":id",this->id);
    if (!qEpisodio->exec())
        QMessageBox::warning(NULL,QObject::tr("ERROR: Modificación Episodios"),QObject::tr("No se puede modificar el episodio:")+
                                                                                           qEpisodio->lastError().text(),
                             QObject::tr("Aceptar"));


}

void Episodio::BorrarEpisodio(int nID)
{


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

int Episodio::getidcie()
{
    return this->idcie;
}

QString Episodio::getcodigocie()
{
    return this->codigocie;
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

void Episodio::setidcie(int id)
{
    this->idcie = id;
}

void Episodio::setcodigocie(QString codigo)
{
    this->codigocie = codigo;
}

