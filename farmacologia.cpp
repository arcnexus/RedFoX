#include "farmacologia.h"
#include <QString>
#include <QtSql>
#include <QtSql/QSqlQuery>
#include "QtSql/QSqlDatabase"
#include <QSqlRecord>
#include <QMessageBox>


Farmacologia::Farmacologia()
{
}

int Farmacologia::getidFarmacologia()
{
    return this->id;
}

int Farmacologia::getidmedicamento()
{
    return this->idmedicamento;
}

QString Farmacologia::getmedicamento()
{
    return this->medicamento;
}

QDate Farmacologia::getiniciotratamiento()
{
    return this->iniciotratamiento;
}

QString Farmacologia::getindicacionposologia()
{
    return this->indicacionposologia;
}

QString Farmacologia::getcomentarios()
{
    return this->comentarios;
}

int Farmacologia::getidepisodio()
{
    return this->idepisodio;
}

int Farmacologia::getactivo()
{
    return this->activo;
}

void Farmacologia::setidFarmacologia(int id)
{
    this->id = id;
}

void Farmacologia::setidmedicamento(int idmedicamento)
{
    this->idmedicamento = idmedicamento;
}

void Farmacologia::setmedicamento(QString medicamento)
{
    this->medicamento = medicamento;
}

void Farmacologia::setiniciotratamiento(QDate iniciotratamiento)
{
    this->iniciotratamiento = iniciotratamiento;
}

void Farmacologia::setindicacionposologia(QString indicacionposologia)
{
    this->indicacionposologia = indicacionposologia;
}

void Farmacologia::setcomentarios(QString comentarios)
{
    this->comentarios = comentarios;
}

void Farmacologia::setidepisodio(int idepisodio)
{
    this->idepisodio = idepisodio;
}

void Farmacologia::setactivo(int activo)
{
    this->activo = activo;
}

void Farmacologia::AnadirFarmaco()
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL ="INSERT INTO histofarma (idmedicamento,medicamento,iniciotratamiento,"
    "indicacionposologia,comentarios,idepisodio,activo) VALUES "
    "(:idmedicamento,:medicamento,:iniciotratamiento,:indicacion,:posologia,"
    ":comentarios,idepisodio,:activo)";

    qFarma->prepare(cSQL);
    qFarma->bindValue(":idmedicamento",this->idmedicamento);
    qFarma->bindValue(":medicamento",this->medicamento);
    qFarma->bindValue(":iniciotratamiento",this->iniciotratamiento);
    qFarma->bindValue(":indicacionposologia",this->indicacionposologia);
    qFarma->bindValue(":comentarios",this->comentarios);
    qFarma->bindValue(":idepisodio",this->idepisodio);
    qFarma->bindValue(":activo",this->activo);
    qFarma->bindValue(":id",this->id);
    if(!qFarma->exec())
        QMessageBox::warning(NULL,QObject::tr("Historial Farmacología"),
                      QObject::tr("No se ha podido guardar el registro del historial;ERROR: ")+qFarma->lastError().text(),
                             QObject::tr("Aceptar"));

}

void Farmacologia::modificarFarmaco(int id)
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL;
    cSQL = "UPDATE histofarma SET "
    "idmedicamento = :idmedicamento,"
    "medicamento = :medicamento,"
    "iniciotratamiento = :iniciotratamiento,"
    "indicacionposologia = :indicacionposologia,"
    "comentarios = :comentarios,"
    "idepisodio = :idepisodio,"
    "activo = :activo"
    " WHERE id = :id";

    qFarma->prepare(cSQL);
    qFarma->bindValue(":idmedicamento",this->idmedicamento);
    qFarma->bindValue(":medicamento",this->medicamento);
    qFarma->bindValue(":iniciotratamiento",this->iniciotratamiento);
    qFarma->bindValue(":indicacionposologia",this->indicacionposologia);
    qFarma->bindValue(":comentarios",this->comentarios);
    qFarma->bindValue(":idepisodio",this->idepisodio);
    qFarma->bindValue(":activo",this->activo);
    qFarma->bindValue(":id",id);
    if(!qFarma->exec())
        QMessageBox::warning(NULL,QObject::tr("Historial Farmacología"),
                      QObject::tr("No se ha podido guardar el registro del historial;ERROR: ")+qFarma->lastError().text(),
                             QObject::tr("Aceptar"));
   delete qFarma;
}

void Farmacologia::cargarDatos(QString cSQL)
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    if (qFarma->exec(cSQL)) {
        qFarma->next();
        QSqlRecord rFarma = qFarma->record();
        this->id = rFarma.field("id").value().toInt();
        this->idmedicamento = rFarma.field("idmedicamento").value().toInt();
        this->medicamento = rFarma.field("medicamento").value().toString();
        this->iniciotratamiento = rFarma.field("iniciotratamiento").value().toDate();
        this->indicacionposologia = rFarma.field("indicacionposologia").value().toString();
        this->comentarios = rFarma.field("comentarios").value().toString();
        this->idepisodio = rFarma.field("idepisodio").value().toInt();
        this->activo = rFarma.field("activo").value().toInt();
    } else
        QMessageBox::warning(NULL,QObject::tr("ERROR"),QObject::tr("No se ha podido recuperar el registro de farmacología"),
                             QObject::tr("Aceptar"));
    delete qFarma;
}


