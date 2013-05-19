#include "empresa.h"

Empresa::Empresa(QObject *parent) :
    QObject(parent)
{
    this->id=0;
}

void Empresa::Anadir(QString id)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("insert into empresas (id) values (:id)");
    qEmpresa.bindValue(":id",id);
    if (!qEmpresa.exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("No se puede crear la empresa\n")+
                              qEmpresa.lastError().text());
    else
        this->id = id.toInt();
}

void Empresa::Recuperar(QString cSQL)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    if (!qEmpresa.exec(cSQL))
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("No se puede recuperar la ficha de empresa"),
                             qEmpresa.lastError().text());
    else 
	{
        if(qEmpresa.next()) 
		{
            QSqlRecord registro = qEmpresa.record();
            this->id = registro.field("id").value().toInt();
            this->id_divisa = registro.field("id_divisa").value().toInt();
            this->cCodigo = registro.field("codigo").value().toString();
            this->cNombre = registro.field("nombre").value().toString();
            this->nDigitosFactura = registro.field("digitosfactura").value().toInt();
            this->cSerie = registro.field("serie").value().toString();
            this->cRutaBDSqLite = registro.field("RutaBDSqLite").value().toString();
            this->cHost = registro.field("host").value().toString();
            this->cUser = registro.field("user").value().toString();
            this->cContrasena = registro.field("contrasena").value().toString();
            this->cPuerto = registro.field("puerto").value().toString();
            this->cNombreBD = registro.field("nombreBD").value().toString();
            this->cHostMed = registro.field("hostBDMedica").value().toString();
            this->cUserMed = registro.field("userBDMedica").value().toString();
            this->cContrasenaMed = registro.field("contrasenaBDMedica").value().toString();
            this->cPuertoMed = registro.field("puertoBDMedica").value().toString();
            this->cNombreBDMed = registro.field("nombreBDMedica").value().toString();
            this->cDriverBDMed = registro.field("driverBDMedica").value().toString();
            this->cHostMed = registro.field("hostBDMedica").value().toString();
            this->cUserMed = registro.field("userBDMedica").value().toString();
            this->cContrasenaMed = registro.field("contrasenaBDMedica").value().toString();
            this->cPuertoMed = registro.field("puertoBDMedica").value().toString();
            this->cNombreBDMed = registro.field("nombreBDMedica").value().toString();
            this->cDriverBDMed = registro.field("driverBDMedica").value().toString();
            this->cDireccion = registro.field("Direccion").value().toString();
            this->cCp = registro.field("cp").value().toString();
            this->cPoblacion = registro.field("poblacion").value().toString();
            this->cProvincia = registro.field("provincia").value().toString();
            this->cPais = registro.field("pais").value().toString();
            this->cTelefono1 = registro.field("telefono1").value().toString();
            this->cTelefono2 = registro.field("telefono2").value().toString();
            this->cFax = registro.field("fax").value().toString();
            this->cMail = registro.field("mail").value().toString();
            this->cWeb =registro.field("web").value().toString();
            this->cCif = registro.field("cif").value().toString();
            this->cInscripcion = registro.field("inscripcion").value().toString();
            this->cComentarioAlbaran = registro.field("comentarioalbaran").value().toString();
            this->cComentarioFactura = registro.field("comentariofactura").value().toString();
            this->cComentarioTicket = registro.field("comentarioticket").value().toString();
            this->nEjercicio = registro.field("ejercicio").value().toInt();
            this->nDigitosCuentas = registro.field("ndigitoscuenta").value().toInt();
            this->cCuentaClientes = registro.field("codigocuentaclientes").value().toString();
            this->cCuentaAcreeedores = registro.field("codigocuentaacreedores").value().toString();
            this->cCuentaProveedores = registro.field("codigocuentaproveedores").value().toString();
            this->Autocodificar = registro.field("autocodigo").value().toBool();
            this->Tamanocodigo = registro.field("tamanocodigo").value().toInt();
            this->cCuentaCobros = registro.field("cCuentaCobros").value().toString();
            this->cCuentaPagos = registro.field("cCuentaPagos").value().toString();
            this->contabilidad = registro.field("contabilidad").value().toBool();
            this->consultas = registro.field("consultas").value().toInt();
            this->primer_dia_laborable = registro.field("primer_dia_laborable").value().toString();
            this->ultimo_dia_laborable = registro.field("ultimo_dia_laborable").value().toString();
            this->horario_primer_dia = registro.field("horario_primer_dia").value().toString();
            this->horario_dia_normal = registro.field("horario_dia_normal").value().toString();
            this->horario_ultimo_dia = registro.field("horario_ultimo_dia").value().toString();
            this->HostBD_contabilidad = registro.field("hostBDConta").value().toString();
            this->NombreBD_contabilidad = registro.field("nombreBDConta").value().toString();
            this->UsuarioBD_contabilidad = registro.field("userBDConta").value().toString();
            this->ContrasenaBD_contabilidad = registro.field("passwordBDConta").value().toString();
            this->puertoDB_contabilidad = registro.field("puertoDBConta").value().toString();
            this->RutaBD_Contabilidad_sqlite = registro.field("rutaBDConta").value().toString();
            this->DriverDB_contabilidad = registro.field("driverBDConta").value().toString();
            this->ticket_factura = registro.field("ticket_factura").value().toBool();
		} 
		else 
		{
            QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión Empresas"),QObject::tr("No se ha podido encontrar la ficha de empresa"));
        }
    }
}

void Empresa::Recuperar(QString cSQL, int nProcede)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    if (!qEmpresa.exec(cSQL))
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("No se puede recuperar la ficha de empresa"),
                             qEmpresa.lastError().text());
    else {
        if(qEmpresa.next()) {
            QSqlRecord registro = qEmpresa.record();
            this->id = registro.field("id").value().toInt();
            this->id_divisa = registro.field("id_divisa").value().toInt();
            this->cCodigo = registro.field("codigo").value().toString();
            this->cNombre = registro.field("nombre").value().toString();
            this->nDigitosFactura = registro.field("digitosfactura").value().toInt();
            this->cSerie = registro.field("serie").value().toString();
            this->cRutaBDSqLite = registro.field("RutaBDSqLite").value().toString();
            this->cHost = registro.field("host").value().toString();
            this->cUser = registro.field("user").value().toString();
            this->cContrasena = registro.field("contrasena").value().toString();
            this->cPuerto = registro.field("puerto").value().toString();
            this->cNombreBD = registro.field("nombreBD").value().toString();
            this->cDriverBD = registro.field("driverBD").value().toString();

            this->cHostMed = registro.field("hostBDMedica").value().toString();
            this->cUserMed = registro.field("userBDMedica").value().toString();
            this->cContrasenaMed = registro.field("contrasenaBDMedica").value().toString();
            this->cPuertoMed = registro.field("puertoBDMedica").value().toString();
            this->cNombreBDMed = registro.field("nombreBDMedica").value().toString();
            this->cDriverBDMed = registro.field("driverBDMedica").value().toString();

            this->cDireccion = registro.field("Direccion").value().toString();
            this->cCp = registro.field("cp").value().toString();
            this->cPoblacion = registro.field("poblacion").value().toString();
            this->cProvincia = registro.field("provincia").value().toString();
            this->cPais = registro.field("pais").value().toString();
            this->cTelefono1 = registro.field("telefono1").value().toString();
            this->cTelefono2 = registro.field("telefono2").value().toString();
            this->cFax = registro.field("fax").value().toString();
            this->cMail = registro.field("mail").value().toString();
            this->cWeb =registro.field("web").value().toString();
            this->cCif = registro.field("cif").value().toString();
            this->cInscripcion = registro.field("inscripcion").value().toString();
            this->cComentarioAlbaran = registro.field("comentarioalbaran").value().toString();
            this->cComentarioFactura = registro.field("comentariofactura").value().toString();
            this->cComentarioTicket = registro.field("comentarioticket").value().toString();
            this->nEjercicio = registro.field("ejercicio").value().toInt();
            this->nDigitosCuentas = registro.field("ndigitoscuenta").value().toInt();
            this->cCuentaClientes = registro.field("codigocuentaclientes").value().toString();
            this->cCuentaAcreeedores = registro.field("codigocuentaacreedores").value().toString();
            this->cCuentaProveedores = registro.field("codigocuentaproveedores").value().toString();
            this->Autocodificar = registro.field("autocodigo").value().toBool();
            this->Tamanocodigo = registro.field("tamanocodigo").value().toInt();
            this->cCuentaCobros = registro.field("cCuentaCobros").value().toString();
            this->cCuentaPagos = registro.field("cCuentaPagos").value().toString();
            this->contabilidad = registro.field("contabilidad").value().toBool();
            this->consultas = registro.field("consultas").value().toInt();
            this->primer_dia_laborable = registro.field("primer_dia_laborable").value().toString();
            this->ultimo_dia_laborable = registro.field("ultimo_dia_laborable").value().toString();
            this->horario_primer_dia = registro.field("horario_primer_dia").value().toString();
            this->horario_dia_normal = registro.field("horario_dia_normal").value().toString();
            this->horario_ultimo_dia = registro.field("horario_ultimo_dia").value().toString();
            this->HostBD_contabilidad = registro.field("hostDBConta").value().toString();
            this->NombreBD_contabilidad = registro.field("nombreDBConta").value().toString();
            this->UsuarioBD_contabilidad = registro.field("userDBConta").value().toString();
            this->ContrasenaBD_contabilidad = registro.field("passwordDBConta").value().toString();
            this->puertoDB_contabilidad = registro.field("puertoDBConta").value().toString();
            this->RutaBD_Contabilidad_sqlite = registro.field("rutaBDConta").value().toString();
            this->DriverDB_contabilidad = registro.field("driverBDConta").value().toString();
            this->ticket_factura = registro.field("ticket_factura").value().toBool();
        } else {
            if (nProcede == 1)
                QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión Empresas"),QObject::tr("No hay más empresas: Se ha llegado al final del fichero"));
            else
                QMessageBox::information(qApp->activeWindow(), QObject::tr("Gestión Empresas"),QObject::tr("No hay más empresas: se ha llegado al inicio del fichero"));

        }
    }
}

void Empresa::Guardar()
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("update empresas set "
                     "id_divisa =:id_divisa,"
                     "codigo =:cCodigo,"
                     "nombre =:cNombre,"
                     "digitosfactura =:nDigitosFactura,"
                     "serie =:cSerie,"
                     "RutaBDSqLite =:cRutaBDSqLite,"
                     "host =:cHost,"
                     "user =:cUser,"
                     "contrasena =:cContrasena,"
                     "puerto=:cPuerto,"
                     "nombreBD=:cNombreBD,"
                     "driverBD=:cDriverBD, "
                     "hostBDMedica =:cHostDBMedica,"
                     "userBDMedica =:cUserDBMedica,"
                     "contrasenaBDMedica =:cContrasenaDBMedica,"
                     "puertoBDMedica=:cPuertoDBMedica,"
                     "nombreBDMedica=:cNombreBDMedica,"
                     "driverBDMedica=:cDriverBDMedica,"
                     "Direccion=:cDireccion,"
                     "cp=:cCP,"
                     "poblacion=:cPoblacion,"
                     "provincia=:cProvincia,"
                     "pais=:cPais,"
                     "telefono1=:cTelefono1,"
                     "telefono2=:cTelefono2,"
                     "fax=:cFax,"
                     "mail=:cMail,"
                     "web=:cWeb,"
                     "cif=:cCif,"
                     "inscripcion =:cInscripcion,"
                     "comentarioalbaran=:cComentarioAlbaran,"
                     "comentariofactura=:cComentarioFactura,"
                     "comentarioticket=:cComentarioTicket,"
                     "ejercicio=:ejercicio, "
                     "ndigitoscuenta =:ndigitoscuentas,"
                     "codigocuentaclientes =:codigocuentaclientes,"
                     "codigocuentaproveedores =:codigocuentaproveedores,"
                     "codigocuentaacreedores =:codigocuentaacreedores, "
                     "autocodigo=:autocodigo,"
                     "tamanocodigo =:tamanocodigo,"
                     "cCuentaCobros =:cCuentaCobros,"
                     "contabilidad =:contabilidad,"
                     "consultas = :consultas,"
                     "primer_dia_laborable =:primer_dia_laborable,"
                     "ultimo_dia_laborable =:ultimo_dia_laborable,"
                     "horario_primer_dia =:horario_primer_dia,"
                     "horario_dia_normal =:horario_dia_normal,"
                     "horario_ultimo_dia =:horario_ultimo_dia,"
                     "hostDBConta =:hostDBConta,"
                     "nombreDBConta =:nombreBDConta,"
                     "userDBConta =:userBDConta,"
                     "passwordDBConta =:passwordBDConta,"
                     "puertoDBConta = :puertoDBConta,"
                     "rutaDBConta =:rutaBDConta,"
                     "driverDBConta =:driverBDConta,"
                     "ticket_factura =:ticket_factura,"
                     "cCuentaPagos =:cCuentaPagos"
                     " where id=:nID");

    qEmpresa.bindValue(":id_divisa",this->id_divisa);
    qEmpresa.bindValue(":cCodigo",this->cCodigo);
    qEmpresa.bindValue(":cNombre",this->cNombre);
    qEmpresa.bindValue(":nDigitosFactura", this->nDigitosFactura);
    qEmpresa.bindValue(":cSerie",this->cSerie);
    qEmpresa.bindValue(":cRutaBDSqLite",this->cRutaBDSqLite);
    qEmpresa.bindValue(":cHost",this->cHost);
    qEmpresa.bindValue(":cUser",this->cUser);
    qEmpresa.bindValue(":cContrasena",this->cContrasena);
    qEmpresa.bindValue(":cPuerto",this->cPuerto);
    qEmpresa.bindValue(":cNombreBD",this->cNombreBD);
    qEmpresa.bindValue(":cDriverBD ", this->cDriverBD);
    qEmpresa.bindValue(":cHostDBMedica",this->cHostMed);
    qEmpresa.bindValue(":cUserDBMedica",this->cUserMed);
    qEmpresa.bindValue(":cContrasenaDBMedica",this->cContrasenaMed);
    qEmpresa.bindValue(":cPuertoDBMedica",this->cPuertoMed);
    qEmpresa.bindValue(":cNombreBDMedica",this->cNombreBDMed);
    qEmpresa.bindValue(":cDriverBDMedica",this->cDriverBDMed);
    qEmpresa.bindValue(":cDireccion",this->cDireccion);
    qEmpresa.bindValue(":cCP",this->cCp);
    qEmpresa.bindValue(":cPoblacion",this->cPoblacion);
    qEmpresa.bindValue(":cProvincia",this->cProvincia);
    qEmpresa.bindValue(":cPais",this->cPais);
    qEmpresa.bindValue(":cTelefono1",this->cTelefono1);
    qEmpresa.bindValue(":cTelefono2",this->cTelefono2);
    qEmpresa.bindValue(":cFax",this->cFax);
    qEmpresa.bindValue(":cMail",this->cMail);
    qEmpresa.bindValue(":cWeb", this->cWeb);
    qEmpresa.bindValue(":cCif",this->cCif);
    qEmpresa.bindValue(":cInscripcion", this->cInscripcion);
    qEmpresa.bindValue(":cComentarioAlbaran",this->cComentarioAlbaran);
    qEmpresa.bindValue(":cComentarioFactura", this->cComentarioFactura);
    qEmpresa.bindValue(":cComentarioTicket",this->cComentarioTicket);
    qEmpresa.bindValue(":ejercicio",this->nEjercicio);
    qEmpresa.bindValue(":ndigitoscuentas",this->nDigitosCuentas);
    qEmpresa.bindValue(":codigocuentaclientes", this->cCuentaClientes);
    qEmpresa.bindValue(":codigocuentaproveedores", this->cCuentaProveedores);
    qEmpresa.bindValue(":codigocuentaacreedores", this->cCuentaAcreeedores);
    qEmpresa.bindValue(":autocodigo",this->Autocodificar);
    qEmpresa.bindValue(":tamanocodigo",this->Tamanocodigo);
    qEmpresa.bindValue(":cCuentaCobros",this->cCuentaCobros);
    qEmpresa.bindValue(":cCuentaPagos",this->cCuentaPagos);
    qEmpresa.bindValue(":contabilidad",this->contabilidad);
    qEmpresa.bindValue(":consultas",this->consultas);
    qEmpresa.bindValue(":primer_dia_laborable",this->primer_dia_laborable);
    qEmpresa.bindValue(":ultimo_dia_laborable",this->ultimo_dia_laborable);
    qEmpresa.bindValue(":horario_primer_dia",this->horario_primer_dia);
    qEmpresa.bindValue(":horario_dia_normal",this->horario_dia_normal);
    qEmpresa.bindValue(":horario_ultimo_dia",this->horario_ultimo_dia);
    qEmpresa.bindValue(":hostDBConta",this->HostBD_contabilidad);
    qEmpresa.bindValue(":nombreBDConta",this->NombreBD_contabilidad);
    qEmpresa.bindValue(":userBDConta",this->UsuarioBD_contabilidad);
    qEmpresa.bindValue(":passwordBDConta",this->ContrasenaBD_contabilidad);
    qEmpresa.bindValue(":puertoDBConta",this->puertoDB_contabilidad);
    qEmpresa.bindValue(":rutaBDConta",this->RutaBD_Contabilidad_sqlite);
    qEmpresa.bindValue(":driverBDConta",this->DriverDB_contabilidad);
    qEmpresa.bindValue(":ticket_factura",this->ticket_factura);
    qEmpresa.bindValue(":nID",this->id);


    if (!qEmpresa.exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("No se ha podido modificar la ficha de la empresa")+
                             qEmpresa.lastError().text(), QObject::tr("Ok"));
    else {
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("La ficha de la empresa ha sido modificada\n"
                                                                                     "ADMINISTRADOR: Verifique funcionamiento correcto\n"
                                                                                     "antes de dar acceso a usuarios"),
                                 QObject::tr("OK"));
    }


}

void Empresa::Vaciar()
{
    this->id_divisa = 0;
    this->cCodigo = "";
    this->cNombre = "";
    this->nDigitosFactura = 0;
    this->cSerie = "";
    this->cRutaBDSqLite = "";
    this->cHost = "";
    this->cUser = "";
    this->cContrasena = "";
    this->cPuerto = "";
    this->cNombreBD = "";
    this->cDriverBD = "";
    this->cCodEmpresa = "";
    this->cComentarioAlbaran = "";
    this->cComentarioFactura = "";
    this->cComentarioTicket = "";
    this->cCp = "";
    this->cDireccion = "";
    this->cPoblacion = "";
    this->cFax = "";
    this->cInscripcion = "";
    this->cMail = "";
    this->cPais = "";
    this->cTelefono1 = "";
    this->cTelefono2 = "";
    this->cWeb  = "";
    this->cCif = "";
    this->nEjercicio = 2000;
    this->nDigitosCuentas = 0;
    this->cCuentaAcreeedores = "410";
    this->cCuentaClientes = "430";
    this->cCuentaProveedores= "400";
    this->Autocodificar = true;
    this->Tamanocodigo = 13;
    this->cCuentaCobros = "";
    this->cCuentaPagos = "";
    this->contabilidad = false;
    this->consultas = 1;
    this->primer_dia_laborable = "";
    this->ultimo_dia_laborable = "";
    this->horario_dia_normal = "";
    this->horario_primer_dia = "";
    this->horario_ultimo_dia = "";
    this->HostBD_contabilidad = "";
    this->NombreBD_contabilidad = "";
    this->ContrasenaBD_contabilidad = "";
    this->puertoDB_contabilidad = "";
    this->RutaBD_Contabilidad_sqlite = "";
    this->DriverDB_contabilidad = "";
    this->ticket_factura = false;

}

bool Empresa::Borrar(int nId)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    //    frmDecision *Decision = new frmDecision();
    //    Decision->Inicializar(QObject::tr("Borrar Empresa"),QObject::tr("¿Desea realmente Borrar esta ficha de empresa"),QObject::tr("Esta opción no se puede deshacer"),
    //                          QObject::tr("Se borrarán todos los datos de la empresa"),QObject::tr("Borrar"),QObject::tr("Cancelar"));
    //    int elegido = Decision->exec();
    //   if(elegido == 1) {
    qEmpresa.prepare("Delete from empresas where id = "+QString::number(nId));
    //qEmpresa.bindValue(":id",nId);
    if(!qEmpresa.exec())
    {
        qDebug() << qEmpresa.lastQuery();
        qDebug() << qEmpresa.lastError();
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("Borrar Empresa"),QObject::tr("Falló el borrado de la Empresa"),QObject::tr("&Aceptar"));
        return false;
    }
    else
    {
        // Busco el id más proximo
        qEmpresa.prepare("select * from articulos where id <:nId");
        qEmpresa.bindValue(":nId",this->id);
        qEmpresa.exec();
        QSqlRecord registro = qEmpresa.record();
        this->id = registro.field("id").value().toInt();
        return true;
        //         }
    }
   //Decision->deleteLater();
}

int Empresa::getid()
{
    return this->id;
}

QString Empresa::getcCodigo()
{
    return this->cCodigo;
}

QString Empresa::getcNombre()
{
    return this->cNombre;
}

int Empresa::getnDigitosFactura()
{
    return this->nDigitosFactura;
}

QString Empresa::getcSerie()
{
    return this->cSerie;
}

QString Empresa::getcRutaBDSqLite()
{
    return this->cRutaBDSqLite;
}

QString Empresa::getcHost()
{
    return this->cHost;
}

QString Empresa::getcUser()
{
    return this->cUser;
}

QString Empresa::getcContrasena()
{
    return this->cContrasena;
}

QString Empresa::getcNombreBD()
{
    return this->cNombreBD;
}

QString Empresa::getcPuerto()
{
    return this->cPuerto;
}

QString Empresa::getcDriverBD()
{
    return this->cDriverBD;
}

QString Empresa::getcRutaBDSqLiteMed()
{
    return this->cRutaBDSqLiteMed;
}

QString Empresa::getcHostMed()
{
    return this->cHostMed;
}

QString Empresa::getcUserMed()
{
    return this->cUserMed;
}

QString Empresa::getcContrasenaMed()
{
    return this->cContrasenaMed;
}

QString Empresa::getcNombreBDMed()
{
    return this->cNombreBDMed;
}

QString Empresa::getcPuertoMed()
{
    return this->cPuertoMed;
}

QString Empresa::getcDriverBDMed()
{
    return this->cDriverBDMed;
}

QString Empresa::getcDireccion()
{
    return this->cDireccion;
}

QString Empresa::getcCp()
{
    return this->cCp;
}

QString Empresa::getcPoblacion()
{
    return this->cPoblacion;
}

QString Empresa::getcProvincia()
{
    return this->cProvincia;
}

QString Empresa::getcPais()
{
    return this->cPais;
}

QString Empresa::getcTelefono1()
{
    return this->cTelefono1;
}

QString Empresa::getcTelefono2()
{
    return this->cTelefono2;
}

QString Empresa::getcFax()
{
    return this->cFax;
}

QString Empresa::getcMail()
{
    return this->cMail;
}

QString Empresa::getcWeb()
{
    return this->cWeb;
}

int Empresa::getnEjercicio()
{
    return this->nEjercicio;
}

QString Empresa::getcCif()
{
    return this->cCif;
}

QString Empresa::getcInscripcion()
{
    return this->cInscripcion;
}

QString Empresa::getcComentarioFactura()
{
    return this->cComentarioFactura;
}

QString Empresa::getcComentarioAlbaran()
{
    return this->cComentarioAlbaran;
}

QString Empresa::getcComentarioTicket()
{
    return this->cComentarioTicket;
}

int Empresa::getnDigitosCuentas()
{
    return this->nDigitosCuentas;
}

QString Empresa::getcCuentaClientes()
{
    return this->cCuentaClientes;
}

QString Empresa::getcCuentaProveedores()
{
    return this->cCuentaProveedores;
}

QString Empresa::getcCuentaAcreedores()
{
    return this->cCuentaAcreeedores;
}


void Empresa::setid(int id)
{
    this->id = id;
}

void Empresa::setcCodigo(QString cCodigo)
{
    this->cCodigo = cCodigo;
}

void Empresa::setcNombre(QString cNombre)
{
    this->cNombre = cNombre;
}

void Empresa::setnDigitosFactura(int nDigitosFactura)
{
    this->nDigitosFactura = nDigitosFactura;
}

void Empresa::setcSerie(QString cSerie)
{
    this->cSerie = cSerie;
}

void Empresa::setcRutaBDSqLite(QString cRutaBDSqLite)
{
    this->cRutaBDSqLite = cRutaBDSqLite;
}

void Empresa::setcHost(QString cHost)
{
    this->cHost = cHost;
}

void Empresa::setcUser(QString cUser)
{
    this->cUser = cUser;
}

void Empresa::setcContrasena(QString cContrasena)
{
    this->cContrasena = cContrasena;
}

void Empresa::setcNombreBD(QString cNombreBD)
{
    this->cNombreBD = cNombreBD;
}

void Empresa::setcPuerto(QString cPuerto)
{
    this->cPuerto = cPuerto;
}

void Empresa::setcDriverBD(QString cDriverBD)
{
    this->cDriverBD = cDriverBD;
}

void Empresa::setcRutaBDSqLiteMed(QString cRutaBDSqLiteMed)
{
    this->cRutaBDSqLiteMed = cRutaBDSqLiteMed;
}

void Empresa::setcHostMed(QString cHostMed)
{
    this->cHostMed = cHostMed;
}

void Empresa::setcUserMed(QString cUserMed)
{
    this->cUserMed = cUserMed;
}

void Empresa::setcContrasenaMed(QString cContrasenaMed)
{
    this->cContrasenaMed = cContrasenaMed;
}

void Empresa::setcNombreBDMed(QString cNombreBDMed)
{
    this->cNombreBDMed = cNombreBDMed;
}

void Empresa::setcPuertoMed(QString cPuertoMed)
{
    this->cPuertoMed = cPuertoMed;
}

void Empresa::setcDriverBDMed(QString cDriverBDMed)
{
    this->cDriverBDMed = cDriverBDMed;
}

void Empresa::setcDireccion(QString cDireccion)
{
    this->cDireccion = cDireccion;
}

void Empresa::setcCP(QString cCp)
{
    this->cCp = cCp;
}

void Empresa::setcPoblacion(QString cPoblacion)
{
    this->cPoblacion = cPoblacion;
}

void Empresa::setcProvincia(QString cProvincia)
{
    this->cProvincia = cProvincia;
}

void Empresa::setcPais(QString cPais)
{
    this->cPais;
}

void Empresa::setcTelefono1(QString cTelefono1)
{
    this->cTelefono1 = cTelefono1;
}

void Empresa::setcTelefono2(QString cTelefono2)
{
    this->cTelefono2 = cTelefono2;
}

void Empresa::setcFax(QString cFax)
{
    this->cFax = cFax;
}

void Empresa::setcMail(QString cMail)
{
    this->cMail = cMail;
}

void Empresa::setcWeb(QString cWeb)
{
    this->cWeb = cWeb;
}

void Empresa::setcCif(QString cCif)
{
    this->cCif = cCif;
}

void Empresa::setnEjercicio(int nEjercicio)
{
    this->nEjercicio = nEjercicio;

}

void Empresa::setcInscripcion(QString cInscripcion)
{
    this->cInscripcion = cInscripcion;
}

void Empresa::setcComentarioFactura(QString cComentarioFactura)
{
    this->cComentarioFactura = cComentarioFactura;
}

void Empresa::setcComemtarioAlbaran(QString cComentarioAlbaran)
{
    this->cComentarioAlbaran = cComentarioAlbaran;
}

void Empresa::setcComentarioTicket(QString cComentarioTicket)
{
    this->cComentarioTicket = cComentarioTicket;
}

void Empresa::setnDigitosCuentas(int nDigitosCuentas)
{
    this->nDigitosCuentas = nDigitosCuentas;
}

void Empresa::setcCodigoCuentaClientes(QString cCuentaClientes)
{
    this->cCuentaClientes = cCuentaClientes;
}

void Empresa::setcCodigoCuentaProveedor(QString cCuentaProveedores)
{
    this->cCuentaProveedores = cCuentaProveedores;
}

void Empresa::setcCodigoCuentaAcreedores(QString cCuentaAcreeedores)
{
    this->cCuentaAcreeedores = cCuentaAcreeedores;
}






