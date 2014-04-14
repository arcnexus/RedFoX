#include "farmacologia.h"

//#include "QtSql/QSqlDatabase"


Farmacologia::Farmacologia()
{
}

int Farmacologia::getidFarmacologia()
{
    return this->id;
}

int Farmacologia::getid_medicamento()
{
    return this->id_medicamento;
}

QString Farmacologia::getmedicamento()
{
    return this->medicamento;
}

QDate Farmacologia::getinicio_tratamiento()
{
    return this->inicio_tratamiento;
}

QString Farmacologia::getindicacion_posologia()
{
    return this->indicacion_posologia;
}

QString Farmacologia::gecomentarios()
{
    return this->comentarios;
}

int Farmacologia::getid_episodio()
{
    return this->id_episodio;
}

int Farmacologia::getactivo()
{
    return this->activo;
}

QString Farmacologia::getcodigo_nacional()
{
    return this->codigo_nacional;
}

void Farmacologia::setidFarmacologia(int id)
{
    this->id = id;
}

void Farmacologia::setid_medicamento(int id_medicamento)
{
    this->id_medicamento = id_medicamento;
}

void Farmacologia::setmedicamento(QString medicamento)
{
    this->medicamento = medicamento;
}

void Farmacologia::setinicio_tratamiento(QDate inicio_tratamiento)
{
    this->inicio_tratamiento = inicio_tratamiento;
}

void Farmacologia::setindicacion_posologia(QString indicacion_posologia)
{
    this->indicacion_posologia = indicacion_posologia;
}

void Farmacologia::secomentarios(QString comentarios)
{
    this->comentarios = comentarios;
}

void Farmacologia::setid_episodio(int id_episodio)
{
    this->id_episodio = id_episodio;
}

void Farmacologia::setactivo(int activo)
{
    this->activo = activo;
}

void Farmacologia::setcodigo_nacional(QString codigo_nacional)
{
    this->codigo_nacional = codigo_nacional;
}

void Farmacologia::AnadirFarmaco()
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL ="INSERT INTO histofarma (id_medicamento,id_paciente,medicamento,inicio_tratamiento,"
    "indicacion_posologia,comentarios,id_episodio,activo,codigo_nacional) VALUES "
    "(:id_medicamento,:id_paciente,:medicamento,:inicio_tratamiento,:indicacion,:posologia,"
    ":comentarios,id_episodio,:activo,:codigo_nacional)";

    qFarma->prepare(cSQL);
    qFarma->bindValue(":id_medicamento",this->id_medicamento);
    qFarma->bindValue(":id_paciente",this->id_paciente);
    qFarma->bindValue(":medicamento",this->medicamento);
    qFarma->bindValue(":inicio_tratamiento",this->inicio_tratamiento);
    qFarma->bindValue(":indicacion_posologia",this->indicacion_posologia);
    qFarma->bindValue(":comentarios",this->comentarios);
    qFarma->bindValue(":id_episodio",this->id_episodio);
    qFarma->bindValue(":activo",this->activo);
    qFarma->bindValue(":codigo_nacional",this->codigo_nacional);
    qFarma->bindValue(":id",this->id);

    if(!qFarma->exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Historial Farmacología"),
                      QObject::tr("No se ha podido guardar el registro del historial;ERROR: ")+qFarma->lastError().text(),
                             QObject::tr("Aceptar"));
    delete qFarma;
}

void Farmacologia::modificarFarmaco(int id)
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL;
    cSQL = "UPDATE histofarma SET "
    "inicio_tratamiento = :inicio_tratamiento,"
    "id_paciente =:id_paciente,"
    "indicacion_posologia = :indicacion_posologia,"
    "comentarios = :comentarios,"
    "activo = :activo"
    " WHERE id = :id";

    qFarma->prepare(cSQL);
    qFarma->bindValue(":inicio_tratamiento",this->inicio_tratamiento);
    qFarma->bindValue(":id_paciente",this->id_paciente);
    qFarma->bindValue(":indicacion_posologia",this->indicacion_posologia);
    qFarma->bindValue(":comentarios",this->comentarios);
    qFarma->bindValue(":activo",this->activo);
    qFarma->bindValue(":id",id);
    if(!qFarma->exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Historial Farmacología"),
                      QObject::tr("No se ha podido guardar el registro del historial;ERROR: ")+qFarma->lastError().text(),
                             QObject::tr("Aceptar"));
    delete qFarma;
}

void Farmacologia::borrarFarmaco(int id)
{
    QSqlQuery *qFarma = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL;
    cSQL = "DELETE from histofarma  WHERE id = :id";
    qFarma->prepare(cSQL);
    qFarma->bindValue(":id",id);
    if(!qFarma->exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Historial Farmacología"),
                      QObject::tr("No se ha podido borrar el registro del historial;ERROR: ")+qFarma->lastError().text(),
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
        this->id_paciente = rFarma.field("id_paciente").value().toInt();
        this->id_medicamento = rFarma.field("id_medicamento").value().toInt();
        this->medicamento = rFarma.field("medicamento").value().toString();
        this->inicio_tratamiento = rFarma.field("inicio_tratamiento").value().toDate();
        this->indicacion_posologia = rFarma.field("indicacion_posologia").value().toString();
        this->comentarios = rFarma.field("comentarios").value().toString();
        this->id_episodio = rFarma.field("id_episodio").value().toInt();
        this->activo = rFarma.field("activo").value().toInt();
        this->codigo_nacional = rFarma.field("codigo_nacional").value().toString();
    } else
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR"),QObject::tr("No se ha podido recuperar el registro de farmacología"),
                             QObject::tr("Aceptar"));
    delete qFarma;
}


