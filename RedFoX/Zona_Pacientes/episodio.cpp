#include "episodio.h"


Episodio::Episodio(QObject *parent) : QObject(parent)
{
}

void Episodio::NuevoEpisodio(int id_paciente)
{
    Recuperarid_doctor(this->doctor);
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("INSERT INTO episodios (id_paciente,cerrado,privado,fecha,descripcion,cie,id_doctor,historial) "
                       "VALUES (:id_paciente,:cerrado,:privado,:fecha,:descripcion,:cie,:id_doctor,:historial)");

    qEpisodio->bindValue(":id_paciente",id_paciente);
    qEpisodio->bindValue(":cerrado",this->cerrado);
    qEpisodio->bindValue(":privado",this->privado);
    qEpisodio->bindValue(":id_doctor",this->id_doctor);
    qEpisodio->bindValue(":fecha",this->fecha);
    qEpisodio->bindValue(":historial",this->historial);
    qEpisodio->bindValue(":cie",this->cie);
    qEpisodio->bindValue(":descripcion",this->descripcion);
    if(qEpisodio->exec()) {
        int nid;
        nid =qEpisodio->lastInsertId().toInt();
        this->id = nid;
    } else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR: Inserción de Episodios"),QObject::tr("Falló la inserción de episodios, error servidor:")+
                             qEpisodio->lastError().text());
    }
    delete qEpisodio;
}

void Episodio::GuardarEpisodio()
{
    if(!this->doctor.isEmpty())
        Recuperarid_doctor(this->doctor);
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("UPDATE episodios SET "
                       "id_paciente = :id_paciente,"
                       "cerrado = :cerrado,"
                       "privado = :privado,"
                       "id_doctor = :id_doctor,"
                       "fecha = :fecha,"
                       "historial = :historial,"
                       "cie = :cie,"
                       "descripcion = :descripcion "
                       "WHERE id =:id");
    qEpisodio->bindValue(":id_paciente",this->id_paciente);
    qEpisodio->bindValue(":cerrado",this->cerrado);
    qEpisodio->bindValue(":privado",this->privado);
    qEpisodio->bindValue(":id_doctor",this->id_doctor);
    qEpisodio->bindValue(":fecha",this->fecha);
    qEpisodio->bindValue(":historial",this->historial);
    qEpisodio->bindValue(":cie",this->cie);
    qEpisodio->bindValue(":descripcion",this->descripcion);
    qEpisodio->bindValue(":id",this->id);
    if (!qEpisodio->exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR: Modificación Episodios"),QObject::tr("No se puede modificar el episodio:")+
                                                                                           qEpisodio->lastError().text(),
                             QObject::tr("Aceptar"));
    delete qEpisodio;

}

void Episodio::RecuperarEpisodio(int id_episodio)
{
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("Select * from episodios where id = :id_episodio");
    qEpisodio->bindValue(":id_episodio",id_episodio);
    if(qEpisodio->exec()) {
        qEpisodio->next();

        QSqlRecord rEpisodio = qEpisodio->record();
        this->id =rEpisodio.field("id").value().toInt();
        this->setid_paciente(rEpisodio.field("id_paciente").value().toInt());
        this->setcerrado(rEpisodio.field("cerrado").value().toInt());
        this->setCIE(rEpisodio.field("cie").value().toString());
        this->setdescripcion(rEpisodio.field("descripcion").value().toString());
        this->setid_doctor(rEpisodio.field("id_doctor").value().toInt());
        this->setfecha(rEpisodio.field("fecha").value().toDate());
        this->sethistorial(rEpisodio.field("historial").value().toString());
        this->setprivado(rEpisodio.field("privado").value().toInt());
        RecuperarDoctor(this->id_doctor);
    } else {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR: Recuperar episodio"),QObject::tr("No se pudo recuperar el episodio. ERROR servidor:")+
                             qEpisodio->lastError().text(),QObject::tr("Aceptar"));
    }
    delete qEpisodio;
}

QString Episodio::RecuperarDoctor(int id_doctor)
{
    QSqlQuery qDoctor(QSqlDatabase::database("dbmedica"));
    qDoctor.prepare("select nombre from doctores where id = :nid");
    qDoctor.bindValue(":nid",id_doctor);
    if(qDoctor.exec()) {
        qDoctor.next();
        QSqlRecord rDoctor = qDoctor.record();
        this->doctor = rDoctor.value("nombre").toString();
        return this->doctor;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Doctores"),tr("No se puede recuperar el Doctor"),tr("Aceptar"));
    }
    return "";
}

int Episodio::Recuperarid_doctor(QString doctor)
{
    QSqlQuery qDoctor(QSqlDatabase::database("dbmedica"));
    qDoctor.prepare("select id from doctores where nombre = :doctor");
    qDoctor.bindValue(":doctor",doctor);
    if(qDoctor.exec()) {
        qDoctor.next();
        QSqlRecord rDoctor = qDoctor.record();
        this->id_doctor = rDoctor.value("id").toInt();
        return this->id_doctor;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Doctores"),tr("No se puede recuperar el Doctor"),tr("Aceptar"));
    }
    return -1;
}



int Episodio::getid()
{
    return this->id;
}

int Episodio::getid_paciente()
{
    return this->id_paciente;
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

void Episodio::setid_paciente(int id_paciente)
{
    this->id_paciente = id_paciente;
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

