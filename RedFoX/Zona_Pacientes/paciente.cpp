#include "paciente.h"

#include"episodio.h"

Paciente::Paciente()
{
    qepisodio=0;
}

Paciente::~Paciente()
{
    if(qepisodio)
        delete qepisodio;
}
void Paciente::RecuperarPaciente(int id_cliente)
{
//    SqlCalls *llamadas = new SqlCalls();
//    QStringList parametros;
//    parametros.append(QString::number(id_cliente));
//    QSqlQuery qpaciente = llamadas->RecuperarPaciente(id_cliente);
//    if (qpaciente.next()) {
//        QSqlRecord rPaciente = qpaciente.record();

//        Paciente *oPaciente = new Paciente();
        QScopedPointer<QSqlQuery>paciente (new QSqlQuery(QSqlDatabase::database("dbmedica")));
       // QSqlQuery *paciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
        QString queryPaciente = "select * from pacientes where id_cliente = :id_cliente";
        paciente->prepare(queryPaciente);

        paciente->bindValue(":id_cliente",id_cliente);
        if (paciente->exec()) {
            paciente->next();
            QSqlRecord rPaciente = paciente->record();
            this->id = rPaciente.field("id").value().toInt();
            this->id_cliente = rPaciente.field("id_cliente").value().toInt();
            this->num_historia = rPaciente.field("num_historia").value().toInt();
            this->alcohol = rPaciente.field("alcohol").value().toInt();
            this->alergias_conocidas = rPaciente.field("alergias_conocidas").value().toString();
            this->antecedentes_familiares = rPaciente.field("antecedentes_familiares").value().toString();
            this->cirugias_previas = rPaciente.field("cirugias_previas").value().toString();
            this->diastole = rPaciente.field("diastole").value().toDouble();
            this->enfermedadesConocidas = rPaciente.field("enfermedades_conocidas").value().toString();
            this->familia = rPaciente.field("familia").value().toString();
            this->fecha_alta = rPaciente.field("fecha_alta").value().toDate();
            this->filiacion = rPaciente.field("filiacion").value().toString();
            this->habitos_drogas = rPaciente.field("habitos_drogas").value().toString();
            this->hijos = rPaciente.field("hijos").value().toInt();
            this->historial = rPaciente.field("historial").value().toString();
            this->id_mutua = rPaciente.field("id_mutua").value().toInt();
            this->imc = rPaciente.field("imc").value().toDouble();
            this->nacimiento = rPaciente.field("nacimiento").value().toDateTime();
            this->nivel_estudios = rPaciente.field("nivel_estudios").value().toString();
            this->num_ss = rPaciente.field("num_ss").value().toString();
            this->otras_drogas = rPaciente.field("otras_drogas").value().toInt();
            this->perimetro_craneal = rPaciente.field("perimetro_craneal").value().toDouble();
            this->peso = rPaciente.field("peso").value().toDouble();
            this->profesion = rPaciente.field("profesion").value().toString();
            this->sexo = rPaciente.field("sexo").value().toString();
            this->sistole = rPaciente.field("sistole").value().toDouble();
            this->tabaco = rPaciente.field("tabaco").value().toInt();
            this->talla = rPaciente.field("talla").value().toDouble();
            this->trabaja = rPaciente.field("trabaja").value().toInt();
          }

        else {
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error Pacientes"),
                                 QObject::tr("No se puede recuperar el paciente"),
                                 QObject::tr("Aceptar"));
        }
}


void Paciente::GuardarPaciente()
{
    QString cSql;
    cSql = "UPDATE pacientes SET "
            "id_cliente = :id_cliente,"
            "alcohol = :alcohol,"
            "alergias_conocidas = :alergias_conocidas,"
            "antecedentes_familiares = :antecedentes_familiares,"
            "cirugias_previas =:cirugias_previas,"
            "diastole = :diastole,"
            "enfermedades_conocidas= :enfermedadesConocidas,"
            "familia =:familia,"
            "fecha_alta =:fecha_alta,"
            "filiacion = :filiacion,"
            "habitos_drogas = :habitos_drogas,"
            "hijos = :hijos,"
            "historial = :historial,"
            "id_mutua =:id_mutua,"
            "imc =:imc,"
            "nacimiento =:nacimiento,"
            "nivel_estudios =:nivel_estudios,"
            "num_ss = :num_ss,"
            "otras_drogas = :otras_drogas,"
            "perimetro_craneal = :perimetro_craneal,"
            "peso = :peso,"
            "profesion = :profesion,"
            "sexo = :sexo,"
            "sistole= :sistole,"
            "tabaco = :tabaco,"
            "talla = :talla,"
            "trabaja = :trabaja,"
            "num_historia =:num_historia"
            " WHERE id = :id";
    QScopedPointer<QSqlQuery>qPaciente (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qPaciente = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qPaciente->prepare(cSql);
    qPaciente->bindValue(":id_cliente",this->id_cliente);
    qPaciente->bindValue(":alcohol",this->alcohol);
    qPaciente->bindValue(":alergias_conocidas",this->alergias_conocidas);
    qPaciente->bindValue(":antecedentes_familiares",this->antecedentes_familiares);
    qPaciente->bindValue(":cirugias_previas",this->cirugias_previas);
    qPaciente->bindValue(":diastole",this->diastole);
    qPaciente->bindValue(":enfermedadesConocidas",this->enfermedadesConocidas);
    qPaciente->bindValue(":familia",this->familia);
    qPaciente->bindValue(":fecha_alta",this->fecha_alta);
    qPaciente->bindValue(":filiacion",this->filiacion);
    qPaciente->bindValue(":habitos_drogas",this->habitos_drogas);
    qPaciente->bindValue(":hijos",this->hijos);
    qPaciente->bindValue(":historial",this->historial);
    qPaciente->bindValue(":id_mutua", this->id_mutua);
    qPaciente->bindValue(":imc",this->imc);
    qPaciente->bindValue(":nacimiento",this->nacimiento);
    qPaciente->bindValue(":nivel_estudios",this->nivel_estudios);
    qPaciente->bindValue(":num_ss",this->num_ss);
    qPaciente->bindValue(":otras_drogas",this->otras_drogas);
    qPaciente->bindValue(":perimetro_craneal", this->perimetro_craneal);
    qPaciente->bindValue(":peso",this->peso);
    qPaciente->bindValue(":profesion",this->profesion);
    qPaciente->bindValue(":sexo",this->sexo);
    qPaciente->bindValue(":sistole",this->sistole);
    qPaciente->bindValue(":tabaco", this->tabaco);
    qPaciente->bindValue(":talla",this->talla);
    qPaciente->bindValue(":trabaja",this->trabaja);
    qPaciente->bindValue(":num_historia",this->num_historia);
    qPaciente->bindValue(":id",this->id);
    if(!qPaciente->exec()) {
        qDebug() << cSql;
        qDebug() <<qPaciente->lastQuery();
        qDebug()  << qPaciente->lastError().text();
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("ERROR PACIENTE"),QObject::tr("No se ha podido guardar el paciente"),
                             QObject::tr("Aceptar"));
    }
    else {
        TimedMessageBox::Box(qApp->activeWindow(),QObject::tr("Los datos del paciente han sido guardados"));
    }
}

int Paciente::AnadirPaciente(int id_cliente)
{
    QSqlQuery paciente(QSqlDatabase::database("dbmedica"));
    paciente.prepare("insert into pacientes (id_cliente,num_historia) values (:id_cliente,:num_historia)");
    paciente.bindValue(":id_cliente",id_cliente);
    paciente.bindValue(":num_historia",QString::number(id_cliente));
    if (!paciente.exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error Pacientes"),QObject::tr("No se ha podido insertar un nuevo paciente"),
                             QObject::tr("Aceptar"));
    else
        return paciente.lastInsertId().toInt();
    return 0;
}

int Paciente::getalcohol()
{
    return this->alcohol;
}

bool Paciente::getalcoholbool()
{
    if(this->alcohol == 0)
        return false;
    else
        return true;
}

int Paciente::getnum_historia()
{
    return this->num_historia;
}

QString Paciente::getalergias_conocidas()
{
    return this->alergias_conocidas;
}

QString Paciente::getantecedentes_familiares()
{
    return this->antecedentes_familiares;

}

QString Paciente::getcirugias_previas()
{
    return this->cirugias_previas;
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

QDate Paciente::getfecha_alta()
{
    return this->fecha_alta;
}

QString Paciente::getfiliacion()
{
    return this->filiacion;
}

QString Paciente::gethabitos_drogas()
{
    return this->habitos_drogas;
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

int Paciente::getid_cliente()
{
    return this->id_cliente;
}

int Paciente::getid_mutua()
{
    return this->id_mutua;
}

double Paciente::getimc()
{
    return this->imc;

}

QDateTime Paciente::getnacimiento()
{
    return this->nacimiento;
}

QString Paciente::getnivel_estudios()
{
    return nivel_estudios;
}

QString Paciente::getnum_ss()
{
    return this->num_ss;
}

int Paciente::getotras_drogas()
{
    return this->otras_drogas;


}

bool Paciente::getotras_drogasbool()
{
    if (this->otras_drogas == 0)
        return false;
    else
        return true;
}

double Paciente::getperimetro_craneal()
{
    return this->perimetro_craneal;
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
        return true;
    else
        return false;

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
        return true;
    else
        return false;
}

void Paciente::setalcohol(int alcohol)
{
    this->alcohol = alcohol;
}

void Paciente::setnum_historia(int num_historia)
{
    this->num_historia = num_historia;
}

void Paciente::setalergias_conocidas(QString alergias_conocidas)
{
    this->alergias_conocidas = alergias_conocidas;
}

void Paciente::setantecedentes_familiares(QString antecedentes_familiares)
{
    this->antecedentes_familiares = antecedentes_familiares;
}

void Paciente::setcirugias_previas(QString cirugias_previas)
{
    this->cirugias_previas = cirugias_previas;
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

void Paciente::setfecha_alta(QDate fecha_alta)
{
    this->fecha_alta = fecha_alta;
}

void Paciente::setfiliacion(QString filiacion)
{
    this->filiacion = filiacion;
}

void Paciente::sethabitos_drogas(QString habitos_drogas)
{
    this->habitos_drogas = habitos_drogas;
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

void Paciente::setid_cliente(int id_cliente)
{
    this->id_cliente = id_cliente;
}

void Paciente::setid_mutua(int id_mutua)
{
    this->id_mutua = id_mutua;
}

void Paciente::setimc(double imc)
{
    this->imc = imc;
}

void Paciente::setnacimiento(QDateTime nacimiento)
{
    this->nacimiento = nacimiento;
}

void Paciente::setnivel_estudios(QString nivel_estudios)
{
    this->nivel_estudios = nivel_estudios;
}

void Paciente::setnum_ss(QString num_ss)
{
    this->num_ss = num_ss;
}

void Paciente::setotras_drogas(int otras_drogas)
{
    this->otras_drogas = otras_drogas;

}

void Paciente::setperimetro_craneal(double perimetro_craneal)
{
    this->perimetro_craneal = perimetro_craneal;
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


