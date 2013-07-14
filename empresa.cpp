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
            this->codigo = registro.field("codigo").value().toString();
            this->nombre = registro.field("nombre").value().toString();
            this->ndigitos_factura = registro.field("digitos_factura").value().toInt();
            this->serie = registro.field("serie").value().toString();
            this->ruta_bd_sqlite = registro.field("ruta_bd_sqlite").value().toString();
            this->cHost = registro.field("host").value().toString();
            this->cUser = registro.field("user").value().toString();
            this->cContrasena = registro.field("contrasena").value().toString();
            this->cPuerto = registro.field("puerto").value().toString();
            this->nombre_bd = registro.field("nombre_bd").value().toString();
            this->cHostMed = registro.field("host_bd_medica").value().toString();
            this->cUserMed = registro.field("user_bd_medica").value().toString();
            this->cContrasenaMed = registro.field("contrasena_bd_medica").value().toString();
            this->cPuertoMed = registro.field("puerto_bd_medica").value().toString();
            this->nombre_bdMed = registro.field("nombre_bd_medica").value().toString();
            this->cDriverBDMed = registro.field("driver_bd_medica").value().toString();
            this->cHostMed = registro.field("host_bd_medica").value().toString();
            this->cUserMed = registro.field("user_bd_medica").value().toString();
            this->cContrasenaMed = registro.field("contrasena_bd_medica").value().toString();
            this->cPuertoMed = registro.field("puerto_bd_medica").value().toString();
            this->nombre_bdMed = registro.field("nombre_bd_medica").value().toString();
            this->cDriverBDMed = registro.field("driver_bd_medica").value().toString();
            this->direccion1 = registro.field("direccion").value().toString();
            this->cp = registro.field("cp").value().toString();
            this->poblacion = registro.field("poblacion").value().toString();
            this->provincia = registro.field("provincia").value().toString();
            this->pais = registro.field("pais").value().toString();
            this->telefono1 = registro.field("telefono1").value().toString();
            this->telefono2 = registro.field("telefono2").value().toString();
            this->fax = registro.field("fax").value().toString();
            this->cMail = registro.field("mail").value().toString();
            this->web =registro.field("web").value().toString();
            this->cif = registro.field("cif").value().toString();
            this->cInscripcion = registro.field("inscripcion").value().toString();
            this->ccomentario_albaran = registro.field("comentario_albaran").value().toString();
            this->ccomentario_factura = registro.field("comentario_factura").value().toString();
            this->ccomentario_ticket = registro.field("comentario_ticket").value().toString();
            this->ejercicio = registro.field("ejercicio").value().toInt();
            this->digitos_cuentas = registro.field("digitos_cuenta").value().toInt();
            this->cuenta_clientes = registro.field("codigo_cuenta_clientes").value().toString();
            this->cuentaAcreeedores = registro.field("codigo_cuenta_acreedores").value().toString();
            this->cuenta_proveedores = registro.field("codigo_cuenta_proveedores").value().toString();
            this->Autocodificar = registro.field("auto_codigo").value().toBool();
            this->tamano_codigo = registro.field("tamano_codigo").value().toInt();
            this->cuenta_cobros = registro.field("cuenta_cobros").value().toString();
            this->cuenta_pagos = registro.field("cuenta_pagos").value().toString();
            this->cuenta_venta_mercaderias = registro.field("cuenta_ventas_mercaderias").value().toString();
            this->cuenta_venta_servicios = registro.field("cuenta_ventas_servicios").value().toString();
            this->contabilidad = registro.field("contabilidad").value().toBool();
            this->consultas = registro.field("consultas").value().toInt();
            this->primer_dia_laborable = registro.field("primer_dia_laborable").value().toString();
            this->ultimo_dia_laborable = registro.field("ultimo_dia_laborable").value().toString();
            this->horario_primer_dia = registro.field("horario_primer_dia").value().toString();
            this->horario_dia_normal = registro.field("horario_dia_normal").value().toString();
            this->horario_ultimo_dia = registro.field("horario_ultimo_dia").value().toString();
            this->HostBD_contabilidad = registro.field("host_bd_conta").value().toString();
            this->nombre_bd_contabilidad = registro.field("nombre_bdConta").value().toString();
            this->UsuarioBD_contabilidad = registro.field("user_bd_conta").value().toString();
            this->ContrasenaBD_contabilidad = registro.field("password_bd_conta").value().toString();
            this->puertoDB_contabilidad = registro.field("puerto_db_conta").value().toString();
            this->RutaBD_Contabilidad_sqlite = registro.field("ruta_bd_conta").value().toString();
            this->driver_db_contabilidad = registro.field("driver_db_conta").value().toString();
            this->ticket_factura = registro.field("ticket_factura").value().toBool();
            this->id_tarifa_predeterminada = registro.field("id_tarifa_predeterminada").value().toInt();
            this->actualizar_divisas = registro.field("actualizar_divisas").value().toInt();
            this->empresa_internacional = registro.field("medica").value().toBool();
            this->empresa_medica = registro.field("internacional").value().toBool();
            this->cuenta_iva_repercutido1 = registro.field("cuenta_iva_repercutido1").value().toString();
            this->cuenta_iva_repercutido2 = registro.field("cuenta_iva_repercutido2").value().toString();
            this->cuenta_iva_repercutido3 = registro.field("cuenta_iva_repercutido3").value().toString();
            this->cuenta_iva_repercutido4 = registro.field("cuenta_iva_repercutido4").value().toString();
            this->cuenta_iva_soportado1 = registro.field("cuenta_iva_soportado1").value().toString();
            this->cuenta_iva_soportado2 = registro.field("cuenta_iva_soportado2").value().toString();
            this->cuenta_iva_soportado3 = registro.field("cuenta_iva_soportado3").value().toString();
            this->cuenta_iva_soportado4 = registro.field("cuenta_iva_soportado4").value().toString();
            this->cuenta_iva_repercutido_re1 = registro.field("cuenta_iva_repercutido1_re").value().toString();
            this->cuenta_iva_repercutido_re2 = registro.field("cuenta_iva_repercutido2_re").value().toString();
            this->cuenta_iva_repercutido_re3 = registro.field("cuenta_iva_repercutido3_re").value().toString();
            this->cuenta_iva_repercutido_re4 = registro.field("cuenta_iva_repercutido4_re").value().toString();
            this->cuenta_iva_soportado_re1 = registro.field("cuenta_iva_soportado1_re").value().toString();
            this->cuenta_iva_soportado_re2 = registro.field("cuenta_iva_soportado2_re").value().toString();
            this->cuenta_iva_soportado_re3 = registro.field("cuenta_iva_soportado3_re").value().toString();
            this->cuenta_iva_soportado_re4 = registro.field("cuenta_iva_soportado4_re").value().toString();
            this->enlace_web = registro.field("enlace_web").value().toBool();
        }
		else 
		{
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),
                                                      QObject::tr("No se ha podido encontrar la ficha de empresa"));
        }
    }
}

void Empresa::Recuperar(QString cSQL, int nProcede)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    if (!qEmpresa.exec(cSQL))
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),
                                                  QObject::tr("No se puede recuperar la ficha de empresa\n")+
                                                              qEmpresa.lastError().text());
    else {
        if(qEmpresa.next()) {
            QSqlRecord registro = qEmpresa.record();
            this->id = registro.field("id").value().toInt();
            this->id_divisa = registro.field("id_divisa").value().toInt();
            this->codigo = registro.field("codigo").value().toString();
            this->nombre = registro.field("nombre").value().toString();
            this->ndigitos_factura = registro.field("digitos_factura").value().toInt();
            this->serie = registro.field("serie").value().toString();
            this->ruta_bd_sqlite = registro.field("ruta_bd_sqlite").value().toString();
            this->cHost = registro.field("host").value().toString();
            this->cUser = registro.field("user").value().toString();
            this->cContrasena = registro.field("contrasena").value().toString();
            this->cPuerto = registro.field("puerto").value().toString();
            this->nombre_bd = registro.field("nombre_bd").value().toString();
            this->cDriverBD = registro.field("driverBD").value().toString();

            this->cHostMed = registro.field("host_bd_medica").value().toString();
            this->cUserMed = registro.field("user_bd_medica").value().toString();
            this->cContrasenaMed = registro.field("contrasena_bd_medica").value().toString();
            this->cPuertoMed = registro.field("puerto_bd_medica").value().toString();
            this->nombre_bdMed = registro.field("nombre_bd_medica").value().toString();
            this->cDriverBDMed = registro.field("driver_bd_medica").value().toString();

            this->direccion1 = registro.field("direccion").value().toString();
            this->cp = registro.field("cp").value().toString();
            this->poblacion = registro.field("poblacion").value().toString();
            this->provincia = registro.field("provincia").value().toString();
            this->pais = registro.field("pais").value().toString();
            this->telefono1 = registro.field("telefono1").value().toString();
            this->telefono2 = registro.field("telefono2").value().toString();
            this->fax = registro.field("fax").value().toString();
            this->cMail = registro.field("mail").value().toString();
            this->web =registro.field("web").value().toString();
            this->cif = registro.field("cif").value().toString();
            this->cInscripcion = registro.field("inscripcion").value().toString();
            this->ccomentario_albaran = registro.field("comentario_albaran").value().toString();
            this->ccomentario_factura = registro.field("comentario_factura").value().toString();
            this->ccomentario_ticket = registro.field("comentario_ticket").value().toString();
            this->ejercicio = registro.field("ejercicio").value().toInt();
            this->digitos_cuentas = registro.field("digitos_cuenta").value().toInt();
            this->cuenta_clientes = registro.field("codigo_cuenta_clientes").value().toString();
            this->cuentaAcreeedores = registro.field("codigo_cuenta_acreedores").value().toString();
            this->cuenta_proveedores = registro.field("codigo_cuenta_proveedores").value().toString();
            this->Autocodificar = registro.field("auto_codigo").value().toBool();
            this->tamano_codigo = registro.field("tamano_codigo").value().toInt();
            this->cuenta_cobros = registro.field("cuenta_cobros").value().toString();
            this->cuenta_pagos = registro.field("cuenta_pagos").value().toString();
            this->cuenta_venta_mercaderias = registro.field("cuenta_ventas_mercaderias").value().toString();
            this->cuenta_venta_servicios = registro.field("cuenta_ventas_servicios").value().toString();
            this->contabilidad = registro.field("contabilidad").value().toBool();
            this->consultas = registro.field("consultas").value().toInt();
            this->primer_dia_laborable = registro.field("primer_dia_laborable").value().toString();
            this->ultimo_dia_laborable = registro.field("ultimo_dia_laborable").value().toString();
            this->horario_primer_dia = registro.field("horario_primer_dia").value().toString();
            this->horario_dia_normal = registro.field("horario_dia_normal").value().toString();
            this->horario_ultimo_dia = registro.field("horario_ultimo_dia").value().toString();
            this->HostBD_contabilidad = registro.field("host_db_conta").value().toString();
            this->nombre_bd_contabilidad = registro.field("nombre_db_conta").value().toString();
            this->UsuarioBD_contabilidad = registro.field("user_db_conta").value().toString();
            this->ContrasenaBD_contabilidad = registro.field("password_db_conta").value().toString();
            this->puertoDB_contabilidad = registro.field("puerto_db_conta").value().toString();
            this->RutaBD_Contabilidad_sqlite = registro.field("ruta_bd_conta").value().toString();
            this->driver_db_contabilidad = registro.field("driver_db_conta").value().toString();
            this->ticket_factura = registro.field("ticket_factura").value().toBool();
            this->id_tarifa_predeterminada = registro.field("id_tarifa_predeterminada").value().toInt();
            this->actualizar_divisas = registro.field("actualizar_divisas").value().toInt();
            this->empresa_internacional = registro.field("medica").value().toBool();
            this->empresa_medica = registro.field("internacional").value().toBool();
            this->cuenta_iva_repercutido1 = registro.field("cuenta_iva_repercutido1").value().toString();
            this->cuenta_iva_repercutido2 = registro.field("cuenta_iva_repercutido2").value().toString();
            this->cuenta_iva_repercutido3 = registro.field("cuenta_iva_repercutido3").value().toString();
            this->cuenta_iva_repercutido4 = registro.field("cuenta_iva_repercutido4").value().toString();
            this->cuenta_iva_soportado1 = registro.field("cuenta_iva_soportado1").value().toString();
            this->cuenta_iva_soportado2 = registro.field("cuenta_iva_soportado2").value().toString();
            this->cuenta_iva_soportado3 = registro.field("cuenta_iva_soportado3").value().toString();
            this->cuenta_iva_soportado4 = registro.field("cuenta_iva_soportado4").value().toString();
            this->cuenta_iva_repercutido_re1 = registro.field("cuenta_iva_repercutido1_re").value().toString();
            this->cuenta_iva_repercutido_re2 = registro.field("cuenta_iva_repercutido2_re").value().toString();
            this->cuenta_iva_repercutido_re3 = registro.field("cuenta_iva_repercutido3_re").value().toString();
            this->cuenta_iva_repercutido_re4 = registro.field("cuenta_iva_repercutido4_re").value().toString();
            this->cuenta_iva_soportado_re1 = registro.field("cuenta_iva_soportado1_re").value().toString();
            this->cuenta_iva_soportado_re2 = registro.field("cuenta_iva_soportado2_re").value().toString();
            this->cuenta_iva_soportado_re3 = registro.field("cuenta_iva_soportado3_re").value().toString();
            this->cuenta_iva_soportado_re4 = registro.field("cuenta_iva_soportado4_re").value().toString();
            this->enlace_web = registro.field("enlace_web").value().toBool();
        } else {
            if (nProcede == 1)
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("No hay más empresas: Se ha llegado al final del fichero"));
            else
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("No hay más empresas: se ha llegado al inicio del fichero"));

        }
    }
}

void Empresa::Guardar()
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("update empresas set "
                     "id_divisa =:id_divisa,"
                     "codigo =:codigo,"
                     "nombre =:nombre,"
                     "digitos_factura =:ndigitos_factura,"
                     "serie =:serie,"
                     "ruta_bd_sqlite =:ruta_bd_sqlite,"
                     "nombre_bd =:nombre_bd,"
                     "nombre_bd_medica=:nombre_bd_medica,"
                     "direccion=:direccion1,"
                     "cp=:cp,"
                     "poblacion=:poblacion,"
                     "provincia=:provincia,"
                     "pais=:pais,"
                     "telefono1=:telefono1,"
                     "telefono2=:telefono2,"
                     "fax=:fax,"
                     "mail=:cMail,"
                     "web=:web,"
                     "cif=:cif,"
                     "inscripcion =:cInscripcion,"
                     "comentario_albaran=:ccomentario_albaran,"
                     "comentario_factura=:ccomentario_factura,"
                     "comentario_ticket=:ccomentario_ticket,"
                     "ejercicio=:ejercicio, "
                     "digitos_cuenta =:digitos_cuentas,"
                     "codigo_cuenta_clientes =:codigo_cuenta_clientes,"
                     "codigo_cuenta_proveedores =:codigo_cuenta_proveedores,"
                     "codigo_cuenta_acreedores =:codigo_cuenta_acreedores, "
                     "auto_codigo=:auto_codigo,"
                     "tamano_codigo =:tamano_codigo,"
                     "cuenta_cobros =:cuenta_cobros,"
                     "cuenta_ventas_mercaderias = :cuenta_ventas_mercaderias,"
                     "cuenta_ventas_servicios =:cuenta_ventas_servicios,"
                     "contabilidad =:contabilidad,"
                     "consultas = :consultas,"
                     "primer_dia_laborable =:primer_dia_laborable,"
                     "ultimo_dia_laborable =:ultimo_dia_laborable,"
                     "horario_primer_dia =:horario_primer_dia,"
                     "horario_dia_normal =:horario_dia_normal,"
                     "horario_ultimo_dia =:horario_ultimo_dia,"
                     "nombre_db_conta =:nombre_bdConta,"
                     "ticket_factura =:ticket_factura,"
                     "id_tarifa_predeterminada =:id_tarifa_predeterminada,"
                     "actualizar_divisas =:actualizar_divisas,"
                     "medica =:medica,"
                     "internacional =:internacional,"
                     "cuenta_iva_repercutido1 = :cuenta_iva_repercutido1,"
                     "cuenta_iva_repercutido2 = :cuenta_iva_repercutido2,"
                     "cuenta_iva_repercutido3 = :cuenta_iva_repercutido3,"
                     "cuenta_iva_repercutido4 = :cuenta_iva_repercutido4,"
                     "cuenta_iva_soportado1 =:cuenta_iva_soportado1,"
                     "cuenta_iva_soportado2 =:cuenta_iva_soportado2,"
                     "cuenta_iva_soportado3 =:cuenta_iva_soportado3,"
                     "cuenta_iva_soportado4 =:cuenta_iva_soportado4,"
                     "cuenta_iva_repercutido1_re = :cuenta_iva_repercutido1_re,"
                     "cuenta_iva_repercutido2_re = :cuenta_iva_repercutido2_re,"
                     "cuenta_iva_repercutido3_re = :cuenta_iva_repercutido3_re,"
                     "cuenta_iva_repercutido4_re = :cuenta_iva_repercutido4_re,"
                     "cuenta_iva_soportado1_re =:cuenta_iva_soportado1_re,"
                     "cuenta_iva_soportado2_re =:cuenta_iva_soportado2_re,"
                     "cuenta_iva_soportado3_re =:cuenta_iva_soportado3_re,"
                     "cuenta_iva_soportado4_re =:cuenta_iva_soportado4_re,"
                     "cuenta_pagos =:cuenta_pagos,"
                     "enlace_web =:enlace_web"
                     " where id=:nid");

    qEmpresa.bindValue(":id_divisa",this->id_divisa);
    qEmpresa.bindValue(":codigo",this->codigo);
    qEmpresa.bindValue(":nombre",this->nombre);
    qEmpresa.bindValue(":ndigitos_factura", this->ndigitos_factura);
    qEmpresa.bindValue(":serie",this->serie);
    qEmpresa.bindValue(":ruta_bd_sqlite",this->ruta_bd_sqlite);
    qEmpresa.bindValue(":nombre_bd",this->nombre_bd);
    qEmpresa.bindValue(":nombre_bd_medica",this->nombre_bdMed);
    qEmpresa.bindValue(":direccion1",this->direccion1);
    qEmpresa.bindValue(":cp",this->cp);
    qEmpresa.bindValue(":poblacion",this->poblacion);
    qEmpresa.bindValue(":provincia",this->provincia);
    qEmpresa.bindValue(":pais",this->pais);
    qEmpresa.bindValue(":telefono1",this->telefono1);
    qEmpresa.bindValue(":telefono2",this->telefono2);
    qEmpresa.bindValue(":fax",this->fax);
    qEmpresa.bindValue(":cMail",this->cMail);
    qEmpresa.bindValue(":web", this->web);
    qEmpresa.bindValue(":cif",this->cif);
    qEmpresa.bindValue(":cInscripcion", this->cInscripcion);
    qEmpresa.bindValue(":ccomentario_albaran",this->ccomentario_albaran);
    qEmpresa.bindValue(":ccomentario_factura", this->ccomentario_factura);
    qEmpresa.bindValue(":ccomentario_ticket",this->ccomentario_ticket);
    qEmpresa.bindValue(":ejercicio",this->ejercicio);
    qEmpresa.bindValue(":digitos_cuentas",this->digitos_cuentas);
    qEmpresa.bindValue(":codigo_cuenta_clientes", this->cuenta_clientes);
    qEmpresa.bindValue(":codigo_cuenta_proveedores", this->cuenta_proveedores);
    qEmpresa.bindValue(":codigo_cuenta_acreedores", this->cuentaAcreeedores);
    qEmpresa.bindValue(":cuenta_ventas_mercaderias",this->cuenta_venta_mercaderias);
    qEmpresa.bindValue(":cuenta_ventas_servicios",this->cuenta_venta_servicios);
    qEmpresa.bindValue(":auto_codigo",this->Autocodificar);
    qEmpresa.bindValue(":tamano_codigo",this->tamano_codigo);
    qEmpresa.bindValue(":cuenta_cobros",this->cuenta_cobros);
    qEmpresa.bindValue(":cuenta_pagos",this->cuenta_pagos);
    qEmpresa.bindValue(":contabilidad",this->contabilidad);
    qEmpresa.bindValue(":consultas",this->consultas);
    qEmpresa.bindValue(":primer_dia_laborable",this->primer_dia_laborable);
    qEmpresa.bindValue(":ultimo_dia_laborable",this->ultimo_dia_laborable);
    qEmpresa.bindValue(":horario_primer_dia",this->horario_primer_dia);
    qEmpresa.bindValue(":horario_dia_normal",this->horario_dia_normal);
    qEmpresa.bindValue(":horario_ultimo_dia",this->horario_ultimo_dia);
    qEmpresa.bindValue(":nombre_bdConta",this->nombre_bd_contabilidad);
    qEmpresa.bindValue(":ticket_factura",this->ticket_factura);
    qEmpresa.bindValue(":id_tarifa_predeterminada",this->id_tarifa_predeterminada);
    qEmpresa.bindValue(":actualizar_divisas",this->actualizar_divisas);
    qEmpresa.bindValue(":medica",this->empresa_medica);
    qEmpresa.bindValue(":internacional",this->empresa_internacional);
    qEmpresa.bindValue(":cuenta_iva_repercutido1",this->cuenta_iva_repercutido1);
    qEmpresa.bindValue(":cuenta_iva_repercutido2",this->cuenta_iva_repercutido2);
    qEmpresa.bindValue(":cuenta_iva_repercutido3",this->cuenta_iva_repercutido3);
    qEmpresa.bindValue(":cuenta_iva_repercutido4",this->cuenta_iva_repercutido4);
    qEmpresa.bindValue(":cuenta_iva_soportado1",this->cuenta_iva_soportado1);
    qEmpresa.bindValue(":cuenta_iva_soportado2",this->cuenta_iva_soportado2);
    qEmpresa.bindValue(":cuenta_iva_soportado3",this->cuenta_iva_soportado3);
    qEmpresa.bindValue(":cuenta_iva_soportado4",this->cuenta_iva_soportado4);
    qEmpresa.bindValue(":cuenta_iva_repercutido1_re",this->cuenta_iva_repercutido_re1);
    qEmpresa.bindValue(":cuenta_iva_repercutido2_re",this->cuenta_iva_repercutido_re2);
    qEmpresa.bindValue(":cuenta_iva_repercutido3_re",this->cuenta_iva_repercutido_re3);
    qEmpresa.bindValue(":cuenta_iva_repercutido4_re",this->cuenta_iva_repercutido_re4);
    qEmpresa.bindValue(":cuenta_iva_soportado1_re",this->cuenta_iva_soportado_re1);
    qEmpresa.bindValue(":cuenta_iva_soportado2_re",this->cuenta_iva_soportado_re2);
    qEmpresa.bindValue(":cuenta_iva_soportado3_re",this->cuenta_iva_soportado_re3);
    qEmpresa.bindValue(":cuenta_iva_soportado4_re",this->cuenta_iva_soportado_re4);
    qEmpresa.bindValue(":enlace_web",this->enlace_web);

    qEmpresa.bindValue(":nid",this->id);


    if (!qEmpresa.exec())
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Empresas"),QObject::tr("No se ha podido modificar la ficha de la empresa")+
                             qEmpresa.lastError().text(), QObject::tr("Ok"));
    else {
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Empresas")
                                 ,QObject::tr("La ficha de la empresa ha sido modificada\n"
                                              "ADMINISTRADOR: Verifique funcionamiento correcto\n"
                                              "antes de dar acceso a usuarios"),
                                 QObject::tr("OK"));
    }


}

void Empresa::Vaciar()
{
    this->id_divisa = 0;
    this->codigo = "";
    this->nombre = "";
    this->ndigitos_factura = 0;
    this->serie = "";
    this->ruta_bd_sqlite = "";
    this->cHost = "";
    this->cUser = "";
    this->cContrasena = "";
    this->cPuerto = "";
    this->nombre_bd = "";
    this->cDriverBD = "";
    this->cCodEmpresa = "";
    this->ccomentario_albaran = "";
    this->ccomentario_factura = "";
    this->ccomentario_ticket = "";
    this->cp = "";
    this->direccion1 = "";
    this->poblacion = "";
    this->fax = "";
    this->cInscripcion = "";
    this->cMail = "";
    this->pais = "";
    this->telefono1 = "";
    this->telefono2 = "";
    this->web  = "";
    this->cif = "";
    this->ejercicio = 2000;
    this->digitos_cuentas = 0;
    this->cuentaAcreeedores = "410";
    this->cuenta_clientes = "430";
    this->cuenta_proveedores= "400";
    this->cuenta_venta_mercaderias = "700";
    this->cuenta_venta_servicios = "";
    this->Autocodificar = true;
    this->tamano_codigo = 13;
    this->cuenta_cobros = "";
    this->cuenta_pagos = "";
    this->contabilidad = false;
    this->consultas = 1;
    this->primer_dia_laborable = "";
    this->ultimo_dia_laborable = "";
    this->horario_dia_normal = "";
    this->horario_primer_dia = "";
    this->horario_ultimo_dia = "";
    this->HostBD_contabilidad = "";
    this->nombre_bd_contabilidad = "";
    this->ContrasenaBD_contabilidad = "";
    this->puertoDB_contabilidad = "";
    this->RutaBD_Contabilidad_sqlite = "";
    this->driver_db_contabilidad = "";
    this->ticket_factura = false;
    this->id_tarifa_predeterminada = 0;
    this->actualizar_divisas = false;
    this->cuenta_iva_repercutido1 = "";
    this->cuenta_iva_repercutido2 = "";
    this->cuenta_iva_repercutido3 = "";
    this->cuenta_iva_repercutido4 = "";
    this->cuenta_iva_soportado1 = "";
    this->cuenta_iva_soportado2 = "";
    this->cuenta_iva_soportado3 = "";
    this->cuenta_iva_soportado4 = "";
    this->cuenta_iva_repercutido_re1 = "";
    this->cuenta_iva_repercutido_re2 = "";
    this->cuenta_iva_repercutido_re3 = "";
    this->cuenta_iva_repercutido_re4 = "";
    this->cuenta_iva_soportado_re1 = "";
    this->cuenta_iva_soportado_re2 = "";
    this->cuenta_iva_soportado_re3 = "";
    this->cuenta_iva_soportado_re4 = "";
    this->enlace_web = false;
}

bool Empresa::Borrar(int nid)
{
    qEmpresa = QSqlQuery(QSqlDatabase::database("Maya"));
    //    frmDecision *Decision = new frmDecision();
    //    Decision->Inicializar(QObject::tr("Borrar Empresa"),QObject::tr("¿Desea realmente Borrar esta ficha de empresa"),QObject::tr("Esta opción no se puede deshacer"),
    //                          QObject::tr("Se borrarán todos los datos de la empresa"),QObject::tr("Borrar"),QObject::tr("Cancelar"));
    //    int elegido = Decision->exec();
    //   if(elegido == 1) {
    qEmpresa.prepare("Delete from empresas where id = "+QString::number(nid));
    //qEmpresa.bindValue(":id",nid);
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
        qEmpresa.prepare("select * from articulos where id <:nid");
        qEmpresa.bindValue(":nid",this->id);
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

QString Empresa::getcodigo()
{
    return this->codigo;
}

QString Empresa::getnombre()
{
    return this->nombre;
}

int Empresa::getndigitos_factura()
{
    return this->ndigitos_factura;
}

QString Empresa::getserie()
{
    return this->serie;
}

QString Empresa::getruta_bd_sqlite()
{
    return this->ruta_bd_sqlite;
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

QString Empresa::getnombre_bd()
{
    return this->nombre_bd;
}

QString Empresa::getcPuerto()
{
    return this->cPuerto;
}

QString Empresa::getcDriverBD()
{
    return this->cDriverBD;
}

QString Empresa::getruta_bd_sqliteMed()
{
    return this->ruta_bd_sqliteMed;
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

QString Empresa::getnombre_bdMed()
{
    return this->nombre_bdMed;
}

QString Empresa::getcPuertoMed()
{
    return this->cPuertoMed;
}

QString Empresa::getcDriverBDMed()
{
    return this->cDriverBDMed;
}

QString Empresa::getdireccion1()
{
    return this->direccion1;
}

QString Empresa::getcp()
{
    return this->cp;
}

QString Empresa::getpoblacion()
{
    return this->poblacion;
}

QString Empresa::getprovincia()
{
    return this->provincia;
}

QString Empresa::getpais()
{
    return this->pais;
}

QString Empresa::gettelefono1()
{
    return this->telefono1;
}

QString Empresa::gettelefono2()
{
    return this->telefono2;
}

QString Empresa::getfax()
{
    return this->fax;
}

QString Empresa::getcMail()
{
    return this->cMail;
}

QString Empresa::getweb()
{
    return this->web;
}

int Empresa::getejercicio()
{
    return this->ejercicio;
}

QString Empresa::getcif()
{
    return this->cif;
}

QString Empresa::getcInscripcion()
{
    return this->cInscripcion;
}

QString Empresa::getccomentario_factura()
{
    return this->ccomentario_factura;
}

QString Empresa::getccomentario_albaran()
{
    return this->ccomentario_albaran;
}

QString Empresa::getccomentario_ticket()
{
    return this->ccomentario_ticket;
}

int Empresa::getdigitos_cuentas()
{
    return this->digitos_cuentas;
}

QString Empresa::getcuenta_clientes()
{
    return this->cuenta_clientes;
}

QString Empresa::getcuenta_proveedores()
{
    return this->cuenta_proveedores;
}

QString Empresa::getcuenta_acreedores()
{
    return this->cuentaAcreeedores;
}


void Empresa::setid(int id)
{
    this->id = id;
}

void Empresa::setcodigo(QString codigo)
{
    this->codigo = codigo;
}

void Empresa::setnombre(QString nombre)
{
    this->nombre = nombre;
}

void Empresa::setndigitos_factura(int ndigitos_factura)
{
    this->ndigitos_factura = ndigitos_factura;
}

void Empresa::setserie(QString serie)
{
    this->serie = serie;
}

void Empresa::setruta_bd_sqlite(QString ruta_bd_sqlite)
{
    this->ruta_bd_sqlite = ruta_bd_sqlite;
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

void Empresa::setnombre_bd(QString nombre_bd)
{
    this->nombre_bd = nombre_bd;
}

void Empresa::setcPuerto(QString cPuerto)
{
    this->cPuerto = cPuerto;
}

void Empresa::setcDriverBD(QString cDriverBD)
{
    this->cDriverBD = cDriverBD;
}

void Empresa::setruta_bd_sqliteMed(QString ruta_bd_sqliteMed)
{
    this->ruta_bd_sqliteMed = ruta_bd_sqliteMed;
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

void Empresa::setnombre_bdMed(QString nombre_bdMed)
{
    this->nombre_bdMed = nombre_bdMed;
}

void Empresa::setcPuertoMed(QString cPuertoMed)
{
    this->cPuertoMed = cPuertoMed;
}

void Empresa::setcDriverBDMed(QString cDriverBDMed)
{
    this->cDriverBDMed = cDriverBDMed;
}

void Empresa::setdireccion1(QString direccion1)
{
    this->direccion1 = direccion1;
}

void Empresa::setcp(QString cp)
{
    this->cp = cp;
}

void Empresa::setpoblacion(QString poblacion)
{
    this->poblacion = poblacion;
}

void Empresa::setprovincia(QString provincia)
{
    this->provincia = provincia;
}

void Empresa::setpais(QString pais)
{
    this->pais;
}

void Empresa::settelefono1(QString telefono1)
{
    this->telefono1 = telefono1;
}

void Empresa::settelefono2(QString telefono2)
{
    this->telefono2 = telefono2;
}

void Empresa::setfax(QString fax)
{
    this->fax = fax;
}

void Empresa::setcMail(QString cMail)
{
    this->cMail = cMail;
}

void Empresa::setweb(QString web)
{
    this->web = web;
}

void Empresa::setcif(QString cif)
{
    this->cif = cif;
}

void Empresa::setejercicio(int ejercicio)
{
    this->ejercicio = ejercicio;

}

void Empresa::setcInscripcion(QString cInscripcion)
{
    this->cInscripcion = cInscripcion;
}

void Empresa::setccomentario_factura(QString ccomentario_factura)
{
    this->ccomentario_factura = ccomentario_factura;
}

void Empresa::setcComemtarioAlbaran(QString ccomentario_albaran)
{
    this->ccomentario_albaran = ccomentario_albaran;
}

void Empresa::setccomentario_ticket(QString ccomentario_ticket)
{
    this->ccomentario_ticket = ccomentario_ticket;
}

void Empresa::setdigitos_cuentas(int digitos_cuentas)
{
    this->digitos_cuentas = digitos_cuentas;
}

void Empresa::setcodigo_cuenta_clientes(QString cuenta_clientes)
{
    this->cuenta_clientes = cuenta_clientes;
}

void Empresa::setcodigoCuentaProveedor(QString cuenta_proveedores)
{
    this->cuenta_proveedores = cuenta_proveedores;
}

void Empresa::setcodigo_cuenta_acreedores(QString cuentaAcreeedores)
{
    this->cuentaAcreeedores = cuentaAcreeedores;
}






