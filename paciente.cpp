#include "paciente.h"
#include "sqlcalls.h"
#include <QtSql>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include"episodio.h"

Paciente::Paciente()
{

}
void Paciente::RecuperarPaciente(int idCliente)
{
//    SqlCalls *llamadas = new SqlCalls();
//    QStringList parametros;
//    parametros.append(QString::number(idCliente));
//    QSqlQuery qpaciente = llamadas->RecuperarPaciente(idCliente);
//    if (qpaciente.next()) {
//        QSqlRecord rPaciente = qpaciente.record();

//        Paciente *oPaciente = new Paciente();
        QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        QString queryPaciente = "select * from pacientes where idcliente = :idcliente";
        paciente->prepare(queryPaciente);

        paciente->bindValue(":idcliente",idCliente);
        if (paciente->exec()) {
            paciente->next();
            QSqlRecord rPaciente = paciente->record();
            this->id = rPaciente.field("id").value().toInt();
            this->idCliente = rPaciente.field("idcliente").value().toInt();
            this->numhistoria = rPaciente.field("numhistoria").value().toInt();
            this->alcohol = rPaciente.field("alcohol").value().toInt();
            this->alergiasConocidas = rPaciente.field("alergiasConocidas").value().toString();
            this->antecedentesFamiliares = rPaciente.field("antecedentesFamiliares").value().toString();
            this->cirugiasPrevias = rPaciente.field("cirugiasPrevias").value().toString();
            this->diastole = rPaciente.field("diastole").value().toDouble();
            this->enfermedadesConocidas = rPaciente.field("enfermedadesConocidas").value().toString();
            this->familia = rPaciente.field("familia").value().toString();
            this->fechaAlta = rPaciente.field("fechaAlta").value().toDate();
            this->filiacion = rPaciente.field("filiacion").value().toString();
            this->habitosDrogas = rPaciente.field("habitosDrogas").value().toString();
            this->hijos = rPaciente.field("hijos").value().toInt();
            this->historial = rPaciente.field("historial").value().toString();
            this->idMutua = rPaciente.field("idMutua").value().toInt();
            this->IMC = rPaciente.field("IMC").value().toDouble();
            this->nacimiento = rPaciente.field("nacimiento").value().toDateTime();
            this->nivelEstudios = rPaciente.field("nivelEstudios").value().toString();
            this->numSS = rPaciente.field("numSS").value().toString();
            this->otrasDrogas = rPaciente.field("otrasDrogas").value().toInt();
            this->perimetroCraneal = rPaciente.field("perimetroCraneal").value().toDouble();
            this->peso = rPaciente.field("peso").value().toDouble();
            this->profesion = rPaciente.field("profesion").value().toString();
            this->sexo = rPaciente.field("sexo").value().toString();
            this->sistole = rPaciente.field("sistole").value().toDouble();
            this->tabaco = rPaciente.field("tabaco").value().toInt();
            this->talla = rPaciente.field("talla").value().toDouble();
            this->trabaja = rPaciente.field("trabaja").value().toInt();
          }

        else {
            QMessageBox::warning(NULL,QObject::tr("Error Pacientes"),
                                 QObject::tr("No se puede recuperar el paciente"),
                                 QObject::tr("Aceptar"));
        }
}


void Paciente::GuardarPaciente()
{
    QString cSql;
    cSql = "UPDATE pacientes SET "
            "idcliente = :idcliente,"
            "alcohol = :alcohol,"
            "alergiasConocidas = :alergiasConocidas,"
            "antecedentesFamiliares = :antecedentesFamiliares,"
            "cirugiasPrevias =:cirugiasPrevias,"
            "diastole = :diastole,"
            "enfermedadesConocidas= :enfermedadesConocidas,"
            "familia =:familia,"
            "fechaAlta =:fechaAlta,"
            "filiacion = :filiacion,"
            "habitosDrogas = :habitosDrogas,"
            "hijos = :hijos,"
            "historial = :historial,"
            "idMutua =:idMutua,"
            "IMC =:IMC,"
            "nacimiento =:nacimiento,"
            "nivelEstudios =:nivelEstudios,"
            "numSS = :numSS,"
            "otrasDrogas = :otrasDrogas,"
            "perimetroCraneal = :perimetroCraneal,"
            "peso = :peso,"
            "profesion = :profesion,"
            "sexo = :sexo,"
            "sistole= :sistole,"
            "tabaco = :tabaco,"
            "talla = :talla,"
            "trabaja = :trabaja"
            " WHERE id = :id";
    QSqlQuery *qPaciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qPaciente->prepare(cSql);
    qPaciente->bindValue(":idcliente",this->idCliente);
    qPaciente->bindValue(":alcohol",this->alcohol);
    qPaciente->bindValue(":alergiasConocidas",this->alergiasConocidas);
    qPaciente->bindValue(":antecedentesFamiliares",this->antecedentesFamiliares);
    qPaciente->bindValue(":cirugiasPrevias",this->cirugiasPrevias);
    qPaciente->bindValue(":diastole",this->diastole);
    qPaciente->bindValue(":enfermedadesConocidas",this->enfermedadesConocidas);
    qPaciente->bindValue(":familia",this->familia);
    qPaciente->bindValue(":fechaAlta",this->fechaAlta);
    qPaciente->bindValue(":filiacion",this->filiacion);
    qPaciente->bindValue(":habitosDrogas",this->habitosDrogas);
    qPaciente->bindValue(":hijos",this->hijos);
    qPaciente->bindValue(":historial",this->historial);
    qPaciente->bindValue(":idMutua", this->idMutua);
    qPaciente->bindValue(":IMC",this->IMC);
    qPaciente->bindValue(":nacimiento",this->nacimiento);
    qPaciente->bindValue(":nivelEstudios",this->nivelEstudios);
    qPaciente->bindValue(":numSS",this->numSS);
    qPaciente->bindValue(":otrasDrogas",this->otrasDrogas);
    qPaciente->bindValue(":perimetroCraneal", this->perimetroCraneal);
    qPaciente->bindValue(":peso",this->peso);
    qPaciente->bindValue(":profesion",this->profesion);
    qPaciente->bindValue(":sexo",this->sexo);
    qPaciente->bindValue(":sistole",this->sistole);
    qPaciente->bindValue(":tabaco", this->tabaco);
    qPaciente->bindValue(":talla",this->talla);
    qPaciente->bindValue(":trabaja",this->trabaja);
    qPaciente->bindValue(":id",this->id);
    if(!qPaciente->exec()) {
        qDebug() << cSql;
        qDebug() <<qPaciente->lastQuery();
        qDebug()  << qPaciente->lastError().text();
        QMessageBox::warning(NULL,QObject::tr("ERROR PACIENTE"),QObject::tr("No se ha podido guardar el paciente"),
                             QObject::tr("Aceptar"));
    }
    else {
        QMessageBox::information(NULL,QObject::tr("PACIENTE"),QObject::tr("Los datos del paciente han sido guardados"),
                             QObject::tr("Aceptar"));
    }

}

int Paciente::getalcohol()
{
    return this->alcohol;
}

bool Paciente::getalcoholbool()
{
    if(this->alcohol == 0)
        return FALSE;
    else
        return TRUE;
}

int Paciente::getnumHistoria()
{
    return this->numhistoria;
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

QDateTime Paciente::getnacimiento()
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

int Paciente::getotrasDrogas()
{
    return this->otrasDrogas;


}

bool Paciente::getotrasDrogasbool()
{
    if (this->otrasDrogas == 0)
        return false;
    else
        return true;
}

double Paciente::getperimetroCraneal()
{
    return this->perimetroCraneal;
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

bool Paciente::gettabacobool()
{
    if (this->tabaco==1)
        return TRUE;
    else
        return FALSE;

}

double Paciente::gettalla()
{
    return this->talla;
}

int Paciente::gettrabaja()
{
    return this->trabaja;
}

bool Paciente::gettrabajabool()
{
    if(this->trabaja ==1)
        return TRUE;
    else
        return FALSE;
}

void Paciente::setalcohol(int alcohol)
{
    this->alcohol = alcohol;
}

void Paciente::setnumHistoria(int numhistoria)
{
    this->numhistoria = numhistoria;
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

void Paciente::setnacimiento(QDateTime nacimiento)
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

void Paciente::setotrasDrogas(int otrasDrogas)
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


