#include "cliente.h"

//


Cliente::Cliente(QObject *parent) :
    QObject(parent)
{
}
void Cliente::Guardar() {
    bool transaccion = true;
    Configuracion_global->groupDB.transaction();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.transaction();

    QHash <QString,QVariant> h_cliente;
    QString error;
    QString condiciones = QString("id=%1").arg(this->id);

    h_cliente["codigo_cliente"] =  this->codigo_cliente;
    h_cliente["apellido1"] =  this->apellido1;
    h_cliente["apellido2"] = this->apellido2;
    h_cliente["nombre"] = this->nombre;
    h_cliente["nombre_fiscal"] = this->nombre_fiscal;
    h_cliente["nombre_comercial"] = this->nombre_comercial;
    h_cliente["persona_contacto"] = this->persona_contacto;
    h_cliente["cif_nif"] = this->cif_nif;
    h_cliente["direccion1"] = this->direccion1;
    h_cliente["direccion2"] = this->direccion2;
    h_cliente["cp"] = this->cp;
    h_cliente["poblacion"] = this->poblacion;
    h_cliente["provincia"] =  this->provincia;
    h_cliente["id_pais"] = this->id_pais;
    h_cliente["telefono1"] =  this->telefono1;
    h_cliente["telefono2"] = this->telefono2;
    h_cliente["fax"] = this->fax;
    h_cliente["movil"] = this->movil;
    h_cliente["email"] = this->email;
    h_cliente["web"] = this->web;
    h_cliente["fecha_alta"] = this->fecha_alta;
    h_cliente["fecha_ultima_compra"] = this->fechaCompra;
    h_cliente["acumulado_ventas"] = this->acumulado_ventas;
    h_cliente["ventas_ejercicio"] = this->ventas_ejercicio;
    h_cliente["riesgo_maximo"] = this->riesgo_maximo;
    h_cliente["deuda_actual"] = this->deuda_actual;
    h_cliente["comentarios"] = this->comentarios;
    h_cliente["bloqueado"] = this->bloqueado;
    h_cliente["comentario_bloqueo"] = this->comentario_bloqueo;
    h_cliente["porc_dto_cliente"] =  this->porc_dto_cliente;
    h_cliente["recargo_equivalencia"] = this->recargo_equivalencia;
    h_cliente["cuenta_contable"] = this->cuenta_contable;
    h_cliente["cuenta_iva_repercutido"] = this->cuenta_iva_repercutido;
    h_cliente["cuenta_deudas"] =  this->cuenta_deudas;
    h_cliente["cuenta_cobros"] =  this->cuenta_cobros;
    h_cliente["id_forma_pago"] = this->id_forma_pago;
    h_cliente["dia_pago1"] = this->dia_pago1;
    h_cliente["dia_pago2"] = this->dia_pago2;
    h_cliente["tarifa_cliente"] = this->tarifa_cliente;
    h_cliente["id_divisa"] = this->id_divisa;
    QString importe_a_cuenta;
    importe_a_cuenta = QString::number(this->importe_a_cuenta);
    importe_a_cuenta = Configuracion_global->MonedatoDouble(importe_a_cuenta);
    this->importe_a_cuenta = importe_a_cuenta.toDouble();
    h_cliente["importe_a_cuenta"] = this->importe_a_cuenta;
    h_cliente["vales"] = this->vales;
    h_cliente["entidad_bancaria"] =  this->entidad_bancaria;
    h_cliente["oficina_bancaria"] = this->oficina_bancaria;
    h_cliente["dc"] = this->dc;
    h_cliente["cuenta_corriente"] = this->cuenta_corriente;
    h_cliente["fecha_nacimiento"] = this->fecha_nacimiento;
    h_cliente["importe_pendiente"] = this->importe_pendiente;
    h_cliente["acceso_web"] = this->acceso_web;
    h_cliente["id_idioma_documentos"] = this->ididioma;
    h_cliente["cif_vies"] = this->cifVies;
    h_cliente["password_web"] = this->password_web;
    h_cliente["id_web"] = this->id_web;
    if (this->lIRPF)
        h_cliente["irpf"] = 1;
    else
        h_cliente["irpf"] = 0;
    h_cliente["observaciones"] = this->observaciones;
    h_cliente["visa_distancia1"] = this->visa_distancia1;
    h_cliente["visa_distancia2"] = this->visa_distancia2;
    h_cliente["id"] = this->id;
    h_cliente["visa1_caduca_mes"] = this->visa1_caduca_mes;
    h_cliente["visa2_caduca_mes"] = this->visa2_caduca_mes;
    h_cliente["visa1_caduca_ano"] = this->visa1_caduca_ano;
    h_cliente["visa2_caduca_ano"] = this->visa2_caduca_ano;
    h_cliente["visa1_cod_valid"] = this->visa1_cod_valid;
    h_cliente["visa2_cod_valid"] = this->visa2_cod_valid;
    h_cliente["id_agente"] = this->id_agente;
    h_cliente["id_transportista"] = this->id_transportista;
    h_cliente["grupo_iva"] = this->grupo_iva;
    h_cliente["tipo_dto_tarifa"] = this->tipo_dto_tarifa;

    bool updated = SqlCalls::SqlUpdate(h_cliente,"clientes",Configuracion_global->groupDB,condiciones,error);
    if(!updated){
        transaccion :false;
        QMessageBox::critical(qApp->activeWindow(),"error al guardar datos cliente. Descripción Error: ",error);
    } else {
        if (Configuracion_global->enlace_web ==true)
            GuardarWeb();
       t = new TimedMessageBox(qApp->activeWindow(),tr("Los datos se han guardado corectamente"));
    }
    // --------------------------
    // Cuenta contable
    //---------------------------
    if(Configuracion_global->contabilidad)
    {
        Cuentas_contables cuenta(this);
        cuenta.activo = true;
        cuenta.codigo_cta = this->codigo_cliente;
        cuenta.descripcion = this->nombre_fiscal;
        cuenta.saldo = 0;
        //-----------------------------------------------------
        // Busca la subcuenta y si no existe la crea como nueva
        //-----------------------------------------------------
        QVariant field;
        QString _error;
        QStringList clausulas;
        clausulas.append("codigo_cta = "+this->codigo_cliente);

        field = SqlCalls::SelectOneField("plan_general","codigo_cta",clausulas,
                                         Configuracion_global->contaDB,_error);

        if(field.toString().isEmpty()) {
            bool succes = cuenta.anadir_cuenta();
            if(!succes)
                transaccion = false;
        }

    }
    if(transaccion)
    {
        Configuracion_global->groupDB.commit();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.commit();

    } else
    {
        Configuracion_global->groupDB.rollback();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.rollback();
        QMessageBox::critical(qApp->activeWindow(),tr("error al guardar datos cliente. Descripción Error: "),
                              tr("No se pudo realizar la transacción, no se guardó la ficha"));
    }

}

void Cliente::GuardarWeb()
{
   Configuracion_global->AbrirDbWeb();
    QHash <QString,QVariant> h_web;
    QString error;
    QString condicion = QString("id = %1").arg(this->id);

   h_web["codigo_cliente"] =  this->codigo_cliente;
   h_web["apellido1"] =  this->apellido1;
   h_web["apellido2"] = this->apellido2;
   h_web["nombre"] = this->nombre;
   h_web["nombre_fiscal"] = this->nombre_fiscal;
   h_web["nombre_comercial"] = this->nombre_comercial;
   h_web["persona_contacto"] = this->persona_contacto;
   h_web["cif_nif"] = this->cif_nif;
   h_web["direccion1"] = this->direccion1;
   h_web["direccion2"] = this->direccion2;
   h_web["cp"] = this->cp;
   h_web["poblacion"] = this->poblacion;
   h_web["provincia"] =  this->provincia;
   h_web["id_pais"] = this->id_pais;
   h_web["telefono1"] =  this->telefono1;
   h_web["telefono2"] = this->telefono2;
   h_web["fax"] = this->fax;
   h_web["movil"] = this->movil;
   h_web["email"] = this->email;
   h_web["web"] = this->web;
   h_web["fecha_alta"] = this->fecha_alta;
   h_web["fecha_ultima_compra"] = this->fechaCompra;
   h_web["acumulado_ventas"] = this->acumulado_ventas;
   h_web["ventas_ejercicio"] = this->ventas_ejercicio;
   h_web["riesgo_maximo"] = this->riesgo_maximo;
   h_web["deuda_actual"] = this->deuda_actual;
   h_web["comentarios"] = this->comentarios;
   h_web["bloqueado"] = this->bloqueado;
   h_web["comentario_bloqueo"] = this->comentario_bloqueo;
   h_web["porc_dto_cliente"] =  this->porc_dto_cliente;
   h_web["recargo_equivalencia"] = this->recargo_equivalencia;
   h_web["cuenta_contable"] = this->cuenta_contable;
   h_web["cuenta_iva_repercutido"] = this->cuenta_iva_repercutido;
   h_web["cuenta_deudas"] =  this->cuenta_deudas;
   h_web["cuenta_cobros"] =  this->cuenta_cobros;
   h_web["dia_pago1"] = this->dia_pago1;
   h_web["dia_pago2"] = this->dia_pago2;
   h_web["tarifa_cliente"] = this->tarifa_cliente;
   QString importe_a_cuenta;
   importe_a_cuenta = QString::number(this->importe_a_cuenta);
   importe_a_cuenta = importe_a_cuenta.replace(".","");
   this->importe_a_cuenta = importe_a_cuenta.toDouble();
   h_web["importe_a_cuenta"] = this->importe_a_cuenta;
   h_web["vales"] = this->vales;
   h_web["entidad_bancaria"] =  this->entidad_bancaria;
   h_web["oficina_bancaria"] = this->oficina_bancaria;
   h_web["dc"] = this->dc;
   h_web["cuenta_corriente"] = this->cuenta_corriente;
   h_web["fecha_nacimiento"] = this->fecha_nacimiento;
   h_web["importe_pendiente"] = this->importe_pendiente;
   h_web["acceso_web"] = this->acceso_web;
   h_web["id_idioma"] = this->ididioma;
   h_web["cif_vies"] = this->cifVies;
   h_web["password_web"] = this->password_web;
   h_web["id_forma_pago"] = this->id_forma_pago;
   h_web["grupo_iva"] = this->grupo_iva;
   if (this->lIRPF)
       h_web["irpf"] = 1;
   else
       h_web["irpf"] = 0;
   h_web["id_web"] = this->id_web;
   h_web["observaciones"] = this->observaciones;
   h_web["id_local"] = this->id;
   h_web["visa1_caduca_mes"] = this->visa1_caduca_mes;
   h_web["visa2_caduca_mes"] = this->visa2_caduca_mes;
   h_web["visa1_caduca_ano"] = this->visa1_caduca_ano;
   h_web["visa2_caduca_ano"] = this->visa2_caduca_ano;
   h_web["visa1_cod_valid"] = this->visa1_cod_valid;
   h_web["visa2_cod_valid"] = this->visa2_cod_valid;
   h_web["id_agente"] = this->id_agente;
   h_web["id_transportista"] = this->id_transportista;
   h_web["tipo_dto_tarifa"] = this->tipo_dto_tarifa;
   bool updated = SqlCalls::SqlUpdate(h_web,"clientes",Configuracion_global->groupDB,condicion,error);

   if(!updated){
       QMessageBox::critical(qApp->activeWindow(),"error al guardar datos cliente en la web. Descripción Error: ",error);
   } else {
       t = new TimedMessageBox(qApp->activeWindow(),tr("Los datos se han guardado corectamente:"));
   }
       ;
   Configuracion_global->CerrarDbWeb();
}
void Cliente::Anadir() {
    QSqlQuery query(Configuracion_global->groupDB);
         query.prepare("INSERT INTO clientes (codigo_cliente,cuenta_contable,cuenta_iva_repercutido,"
                       "cuenta_deudas,cuenta_cobros,tarifa_cliente) "
                       "VALUES (:codigo_cliente,:cuenta_contable,:cuenta_iva_repercutido,"
                       ":cuenta_deudas,:cuenta_cobros,:tarifa_cliente)");
         query.bindValue(":codigo_cliente", this->codigo_cliente);
         query.bindValue(":cuenta_contable",this->cuenta_contable);
         query.bindValue(":cuenta_iva_repercutido",this->cuenta_iva_repercutido);
         query.bindValue(":cuenta_deudas", this->cuenta_deudas);
         query.bindValue(":cuenta_cobros", this->cuenta_cobros);
         query.bindValue(":tarifa_cliente",this->tarifa_cliente);

         if(!query.exec()){
             QMessageBox::critical(qApp->activeWindow(),"error al insertar:", query.executedQuery());
         } else{
             TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Cliente insertado Corectamente");
             int nid = query.lastInsertId().toInt();
             this->id = nid;
             if (Configuracion_global->enlace_web)
                 anadirWeb();
         }
}

void Cliente::anadirWeb()
{

    Configuracion_global->AbrirDbWeb();
    QSqlQuery queryClienteWeb(QSqlDatabase::database("dbweb"));
    queryClienteWeb.prepare("INSERT INTO clientes (codigo_cliente,id_local) VALUES (:codigo_cliente,:id_local)");
    queryClienteWeb.bindValue(":codigo_cliente",this->codigo_cliente);
    queryClienteWeb.bindValue(":id_local",this->id);

    if(!queryClienteWeb.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir en web"),
                             tr("No se ha podido crear el registro en la web: %1").arg(queryClienteWeb.lastError().text()),
                             tr("Aceptar"));
    else
        this->id_web = queryClienteWeb.lastInsertId().toInt();
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Se ha creado el cliente en la web"));

    Configuracion_global->CerrarDbWeb();
}

void Cliente::Recuperar(QString cSQL) {
    qryCliente = new QSqlQuery(Configuracion_global->groupDB);
    qryCliente->prepare(cSQL);
    if( !qryCliente->exec() ) {
        QMessageBox::critical(qApp->activeWindow(), "error:", qryCliente->lastError().text());
    } else
    {
       cargar(*qryCliente);
    }
    delete qryCliente;

}


void Cliente::Recuperar(int id)
{
    qryCliente = new QSqlQuery(Configuracion_global->groupDB);
    qryCliente->prepare("select * from clientes where id ="+QString::number(id));
    if( !qryCliente->exec() ) {
        QMessageBox::critical(qApp->activeWindow(), "error:", qryCliente->lastError().text());
    } else
    {
       cargar(*qryCliente);
    }
    delete qryCliente;

}


void Cliente::cargar(QSqlQuery &query)
{

   if(query.next())
   {
        QSqlRecord registro = query.record();
        this->id = registro.field("id").value().toInt();
        this->codigo_cliente= registro.field("codigo_cliente").value().toString();
        this->apellido1 = registro.field("apellido1").value().toString();
        this->apellido2 = registro.field("apellido2").value().toString();
        this->nombre = registro.field("nombre").value().toString();
        this->nombre_fiscal = registro.field("nombre_fiscal").value().toString();
        this->nombre_comercial = registro.field("nombre_comercial").value().toString();
        this->persona_contacto = registro.field("persona_contacto").value().toString();
        this->cif_nif = registro.field("cif_nif").value().toString();
        this->direccion1 = registro.field("direccion1").value().toString();
        this->direccion2 = registro.field("direccion2").value().toString();
        this->cp = registro.field("cp").value().toString();
        this->poblacion = registro.field("poblacion").value().toString();
        this->provincia = registro.field("provincia").value().toString();
        this->pais = registro.field("pais").value().toString();
        this->id_pais = registro.field("id_pais").value().toInt();
        this->telefono1 = registro.field("telefono1").value().toString();
        this->telefono2 = registro.field("telefono2").value().toString();
        this->fax =registro.field("fax").value().toString();
        this->movil = registro.field("movil").value().toString();
        this->email = registro.field("email").value().toString();
        this->web = registro.field("web").value().toString();
        this->fecha_alta = registro.field("fecha_alta").value().toDate();
        this->fechaCompra = registro.field("fecha_ultima_compra").value().toDate();
        this->acumulado_ventas = registro.field("acumulado_ventas").value().toDouble();
        this->ventas_ejercicio = registro.field("ventas_ejercicio").value().toDouble();
        this->riesgo_maximo = registro.field("riesgo_maximo").value().toDouble();
        this->deuda_actual = registro.field("deuda_actual").value().toDouble();
        this->comentarios = registro.field("comentarios").value().toString();
        this->bloqueado = registro.field("bloqueado").value().toInt();
        this->comentario_bloqueo = registro.field("comentario_bloqueo").value().toString();
        this->porc_dto_cliente = registro.field("porc_dto_cliente").value().toDouble();
        this->recargo_equivalencia = registro.field("recargo_equivalencia").value().toBool();
        this->cuenta_contable = registro.field("cuenta_contable").value().toString();
        this->cuenta_iva_repercutido = registro.field("cuenta_iva_repercutido").value().toString();
        this->cuenta_deudas = registro.field("cuenta_deudas").value().toString();
        this->cuenta_cobros = registro.field("cuenta_cobros").value().toString();
        this->id_forma_pago = registro.field("id_forma_pago").value().toInt();
        this->forma_pago = registro.field("forma_pago").value().toString();
        this->dia_pago1 = registro.field("dia_pago1").value().toInt();
        this->dia_pago2 = registro.field("dia_pago2").value().toInt();
        this->id_divisa = registro.field("id_divisa").value().toInt();
        this->tarifa_cliente = registro.field("tarifa_cliente").value().toInt();
        this->importe_a_cuenta = registro.field("importe_a_cuenta").value().toDouble();
        this->vales = registro.field("vales").value().toDouble();
        this->entidad_bancaria = registro.field("entidad_bancaria").value().toString();
        this->oficina_bancaria = registro.field("oficina_bancaria").value().toString();
        this->dc = registro.field("dc").value().toString();
        this->cuenta_corriente = registro.field("cuenta_corriente").value().toString();
        this->fecha_nacimiento = registro.field("fecha_nacimiento").value().toDate();
        this->importe_pendiente = registro.field("importe_pendiente").value().toDouble();
        this->acceso_web = registro.field("acceso_web").value().toString();
        this->password_web = registro.field("password_web").value().toString();
        this->ididioma = registro.field("id_idioma_documentos").value().toInt();
        this->idioma = Configuracion_global->Devolver_idioma(this->ididioma);
        this->cifVies = registro.field("cif_vies").value().toString();
        this->id_web = registro.value("id_web").toInt();
        this->idTarifa = registro.value("tarifa_cliente").toInt();
        this->observaciones = registro.value("observaciones").toString();
        this->visa1_caduca_mes =registro.field("visa1_caduca_mes").value().toInt();
        this->visa2_caduca_mes = registro.field("visa2_caduca_mes").value().toInt();
        this->visa1_caduca_ano = registro.field("visa1_caduca_ano").value().toInt();
        this->visa2_caduca_ano = registro.field("visa2_caduca_ano").value().toInt();
        this->visa1_cod_valid = registro.field("visa1_cod_valid").value().toInt();
        this->visa2_cod_valid = registro.field("visa2_cod_valid").value().toInt();
        this->visa_distancia1 = registro.field("visa_distancia1").value().toString();
        this->visa_distancia2 = registro.field("visa_distancia2").value().toString();
        this->id_agente = registro.field("id_agente").value().toInt();
        this->id_transportista = registro.field("id_transportista").value().toInt();
        this->tipo_dto_tarifa = registro.field("tipo_dto_tarifa").value().toInt();
        int irpf =registro.field("irpf").value().toInt();
        this->grupo_iva = registro.field("grupo_iva").value().toInt();
        if (irpf==1)
            this->lIRPF = true;
        else
            this->lIRPF = false;

   }
}

void Cliente::clear()
{
    this->id = 0;
    this->id_web = 0;
    this->codigo_cliente = "";
    this->apellido1 = "";
    this->apellido2= "";
    this->nombre = "";
    this->nombre_fiscal ="";
    this->nombre_comercial="";
    this->persona_contacto="";
    this->cif_nif="";
    this->tipo_dto_tarifa =0;
    this->cifVies="";
    this->direccion1="";
    this->direccion2="";
    this->cp="";
    this->poblacion="";
    this->provincia="";
    this->id_pais=0;
    this->pais ="";
    this->telefono1="";
    this->telefono2="";
    this->fax="";
    this->movil="";
    this->email="";
    this->web="";
    this->direccion1Factura1="";
    this->direccion1Factura2="";
    this->cpFactura="";
    this->poblacionFactura="";
    this->provinciaFactura="";
    this->id_paisFactura= 0;
    this->paisFactura="";
    this->direccion_almacen="";
    this->direccion_almacen2="";
    this->cp_almacen="";
    this->poblacion_almacen="";
    this->provincia_almacen="";
    this->id_pais_almacen=0;
    this->paisAlmacen="";
    this->fecha_alta= QDate::currentDate();
    this->fechaCompra = QDate::currentDate();
    this->acumulado_ventas=0;
    this->ventas_ejercicio=0;
    this->riesgo_maximo=0;
    this->deuda_actual=0;
    this->comentarios ="";
    this->bloqueado = 0;
    this->comentario_bloqueo = "";
    this->porc_dto_cliente= 0;
    this->recargo_equivalencia= 0;
    this->cuenta_contable = "";
    this->cuenta_iva_repercutido="";
    this->cuenta_deudas="";
    this->cuenta_cobros="";
    this->forma_pago = "";
    this->dia_pago1 = 0;
    this->dia_pago2 = 0;
    this->tarifa_cliente = 0;
    this->importe_a_cuenta=0;
    this->vales = 0;
    this->entidad_bancaria="";
    this->oficina_bancaria="";
    this->dc="";
    this->cuenta_corriente="";
    this->fecha_nacimiento = QDate::currentDate();
    this->importe_pendiente = 0;
    this->acceso_web = "";
    this->password_web = "";
    this->lIRPF= false;
    this->idTarifa = 0;
    this->id_divisa = -1;
    this->ididioma =0;
    this->idioma = "";
    this->observaciones = "";
    this->visa_distancia1 = "";
    this->visa_distancia2= "";
    this->visa1_caduca_mes = 0;
    this->visa2_caduca_mes = 0;
    this->visa1_caduca_ano = 0;
    this->visa2_caduca_ano = 0;
    this->visa1_cod_valid = 0;
    this->visa2_cod_valid = 0;
    this->id_agente = 0;
    this->id_transportista = 0;
    this->grupo_iva = 1;
}



void Cliente::AnadirDeuda(int id_cliente, QDate fechaDeuda, QDate fechaVto, QString documento, int id_Tiquet,
                          int id_factura, int tipo, double importe_deuda, double pagado = 0, double pendiente_cobro = 0,
                          QString entidad = "", QString oficina = "", QString dc="", QString cuenta = "") {
    QSqlQuery qCliente(Configuracion_global->groupDB);
    double importe;
    importe = this->deuda_actual + importe_deuda;
    qCliente.prepare("Update clientes set  deuda_actual = :importe where id =:id_cliente");
    qCliente.bindValue(":importe",importe);
    qCliente.bindValue(":id", id_cliente);
    if(!qCliente.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Añadir deuda cliente"),tr("Ha fallado la inserción de la deuda en la ficha del paciente"),tr("&Aceptar"));
    }

    QSqlQuery qClientes_Deuda(Configuracion_global->groupDB);
    qClientes_Deuda.prepare("Insert into clientes_deuda (id_cliente,fecha,vencimiento,documento,id_ticket,id_factura,tipo,"
                            "importe,pagado,pendiente_cobro,entidad,oficina,dc,cuenta Values (:id_cliente,:fecha,:vencimiento,"
                            ":documento,:id_ticket,:id_factura,:tipo,:importe,:pagado,:pendiente_cobro,:entidad,:oficina,"
                            ":dc,:cuenta)" );
    qClientes_Deuda.bindValue(":id_cliente",id_cliente);
    qClientes_Deuda.bindValue(":fecha",fechaDeuda);
    qClientes_Deuda.bindValue(":vencimiento",fechaVto);
    qClientes_Deuda.bindValue(":documento",documento);
    qClientes_Deuda.bindValue(":id_Tiquet",id_Tiquet);
    qClientes_Deuda.bindValue(":id_factura",id_factura);
    qClientes_Deuda.bindValue(":tipo",tipo);
    qClientes_Deuda.bindValue(":importe",importe_deuda);
    qClientes_Deuda.bindValue(":pagado",pagado);
    qClientes_Deuda.bindValue(":pendiente_cobro",pendiente_cobro);
    qClientes_Deuda.bindValue(":entidad",entidad);
    qClientes_Deuda.bindValue(":oficina",oficina);
    qClientes_Deuda.bindValue(":dc",dc);
    qClientes_Deuda.bindValue(":cuenta",cuenta);
    if(!qClientes_Deuda.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Añadir deuda cliente"),tr("Falló la inserción en la tabla de deudas"),tr("&Aceptar"));
    }

}

void Cliente::CobrarDeudaTotal(int id_deuda)
{

}

void Cliente::anadir_entrega_cuenta(int id_cliente, QDate fecha, double importe_a_cuenta, QString concepto)
{
    bool estado = true;
    Configuracion_global->groupDB.transaction();
    QSqlQuery q_entregas(Configuracion_global->groupDB);
    q_entregas.prepare("insert into clientes_entregas (id_cliente,fecha_entrega,importe,concepto,disponible)"
                       "VALUES(:id_cliente,:fecha_entrega,:importe_a_cuenta,:concepto,:disponible)");
    q_entregas.bindValue(":id_cliente",id_cliente);
    q_entregas.bindValue(":fecha_entrega",fecha);
    q_entregas.bindValue(":importe_a_acuenta",importe_a_cuenta);
    q_entregas.bindValue(":concepto",concepto);
    q_entregas.bindValue(":disponible",importe_a_cuenta);
    if(!q_entregas.exec())
        estado = false;
    QSqlQuery q_cliente(Configuracion_global->groupDB);

    q_cliente.prepare("update clientes set importe_a_cuenta = importe_a_cuenta + :importe_a_cuenta where id = :id");
    q_cliente.bindValue(":importe_a_cuenta",importe_a_cuenta);
    q_cliente.bindValue(":id",id_cliente);
    if(q_cliente.exec() && estado==true)
    {
        qDebug() << q_cliente.lastQuery();
        Configuracion_global->groupDB.commit();
    } else
    {
        Configuracion_global->groupDB.rollback();
        QMessageBox::warning(qApp->activeWindow(),tr("Entregas a cuenta"),tr("No se pudo guardar la entrega"));
    }
}

void Cliente::AnadirPersonaContacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                    QString desc_telefono2, QString Telefono2, QString desc_telefono3, QString Telefono3,
                                    QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2, QString cargo, QString email)
{
    QSqlQuery qContactos(Configuracion_global->groupDB);
    qContactos.prepare("INSERT INTO Personascontactocliente "
                       "(nombre,"
                       "telefono1,"
                       "telefono2,"
                       "telefono3,"
                       "movil,"
                       "movil2,"
                       "id_cliente,"
                       "desc_telefono1,"
                       "desc_telefono2,"
                       "desc_telefono3,"
                       "desc_movil1,"
                       "desc_movil2,"
                       "cargo_empresa,"
                       "email)"
                       " VALUES ("
                      ":nombre,"
                      ":telefono1,"
                      ":telefono2,"
                      ":telefono3,"
                      ":movil,"
                      ":movil2,"
                      ":id_cliente,"
                      ":desc_telefono1,"
                      ":desc_telefono2,"
                      ":desc_telefono3,"
                      ":desc_movil1,"
                      ":desc_movil2,"
                      ":cargo,"
                      ":email);");


                       qContactos.bindValue(":nombre",Nombre);
                       qContactos.bindValue(":telefono1",Telefono1);
                       qContactos.bindValue( ":telefono2",Telefono2);
                       qContactos.bindValue(":telefono3",Telefono3);
                       qContactos.bindValue(":movil",Movil1);
                       qContactos.bindValue(":movil2",Movil2);
                       qContactos.bindValue(":id_cliente",id);
                       qContactos.bindValue(":desc_telefono1",desc_telefono1);
                       qContactos.bindValue(":desc_telefono2",desc_telefono2);
                       qContactos.bindValue(":desc_telefono3",desc_telefono3);
                       qContactos.bindValue(":desc_movil1",desc_movil1);
                       qContactos.bindValue(":desc_movil2",desc_movil2);
                       qContactos.bindValue(":cargo",cargo);
                       qContactos.bindValue(":email",email);
                       if(!qContactos.exec())
                           QMessageBox::warning(qApp->activeWindow(),tr("Añadir personas de contacto"),
                                                tr("Falló el añadir una persona de contacto: %1").arg(qContactos.lastError().text()));
}

void Cliente::GuardarPersonaContacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                     QString desc_telefono2, QString Telefono2, QString desc_telefono3, QString Telefono3,
                                     QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2,
                                     QString cargo, QString email,int id_cliente)
{
    QSqlQuery qContactos(Configuracion_global->groupDB);
    qContactos.prepare("UPDATE Personascontactocliente set "
                       "nombre =:nombre,"
                       "telefono1 = :telefono1,"
                       "telefono2 = :telefono2,"
                       "telefono3 = :telefono3,"
                       "movil =:movil,"
                       "movil2 =:movil2,"
                       "id_cliente =:id_cliente,"
                       "desc_telefono1 =:desc_telefono1,"
                       "desc_telefono2 =:desc_telefono2,"
                       "desc_telefono3 =:desc_telefono3,"
                       "desc_movil1 =:desc_movil1,"
                       "desc_movil2 =:desc_movil2,"
                       "cargo_empresa =:cargo_empresa,"
                       "email =:email where id = :id");
                       qContactos.bindValue(":nombre",Nombre);
                       qContactos.bindValue(":telefono1",Telefono1);
                       qContactos.bindValue( ":telefono2",Telefono2);
                       qContactos.bindValue(":telefono3",Telefono3);
                       qContactos.bindValue(":movil",Movil1);
                       qContactos.bindValue(":movil2",Movil2);
                       qContactos.bindValue(":id_cliente",id_cliente);
                       qContactos.bindValue(":desc_telefono1",desc_telefono1);
                       qContactos.bindValue(":desc_telefono2",desc_telefono2);
                       qContactos.bindValue(":desc_telefono3",desc_telefono3);
                       qContactos.bindValue(":desc_movil1",desc_movil1);
                       qContactos.bindValue(":desc_movil2",desc_movil2);
                       qContactos.bindValue(":cargo",cargo);
                       qContactos.bindValue(":email",email);
                       qContactos.bindValue(":id",id);
                       if(!qContactos.exec())
                           QMessageBox::warning(qApp->activeWindow(),tr("editar personas de contacto"),
                                                tr("Falló el guardar una persona de contacto: %1").arg(qContactos.lastError().text()));
}

bool Cliente::BorrarPersona_contacto(int id_persona)
{
    QSqlQuery queryPersona(Configuracion_global->groupDB);
    if(queryPersona.exec("delete from Personascontactocliente where id ="+QString::number(id_persona)))
        return true;
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Personas contacto cliente"),
                             tr("Ocurrió un error al borrar: %1").arg(queryPersona.lastError().text()),
                             tr("Aceptar"));
        return false;
    }
}

void Cliente::Guardardireccion(bool Anadir, QString Descripcion, QString direccion1, QString direccion2, QString CP, QString Poblacion,
                               QString Provincia, QString Pais,int id_cliente, QString email, QString comentarios,int id)
{
    QSqlQuery qdirecciones(Configuracion_global->groupDB);
    QHash <QString, QVariant> d;
    QString error;
    int id_alternativa;
    d["descripcion"] = Descripcion;
    d["direccion1"] = direccion1;
    d["direccion2"] =direccion2;
    d["cp"] = CP;
    d["poblacion"] = Poblacion;
    d["provincia"] =Provincia;
    int id_pais = SqlCalls::SelectOneField("paises","id",QString("pais ='%1'").arg(Pais),Configuracion_global->groupDB,error).toInt();
    if (id_pais >0)
    d["id_pais"] =id_pais;
    d["email"] = email;
    d["comentarios"] = comentarios;
    d["id_cliente"] = id_cliente;

    if(Anadir){
        id_alternativa = SqlCalls::SqlInsert(d,"cliente_direcciones",Configuracion_global->groupDB,error);

    } else {
    if(!Anadir) // Guardar
        id_alternativa = SqlCalls::SqlUpdate(d,"cliente_direcciones",Configuracion_global->groupDB,
                                             QString("id=").append(QString::number(id)),error);

    if(id_alternativa == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir/Guardar dirección"),
                             tr("Ocurrió un error al guardar los datos de dirección: %1").arg(error),
                             tr("Aceptar"));
    }
}
void Cliente::DescontarDeuda(int id_deuda, double pagado){
    QSqlQuery qClientes_deuda(Configuracion_global->groupDB);
    qClientes_deuda.prepare("Select * from clientes_deuda where id =:id_deuda");
    qClientes_deuda.bindValue(":id_deuda",id_deuda);
    if (!qClientes_deuda.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Modificar deuda Cliente"),tr("Falló la lectura de la deuda del cliente"),tr("&Aceptar"));
    }
    qClientes_deuda.prepare("update clientes_deuda ");
    qClientes_deuda.clear();

}
void Cliente::Borrar(int id_cliente)
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Ficha"),
                          tr("Está apunto de borrar la ficha de un cliente\n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        bool borrado_ok = true;
        Configuracion_global->groupDB.transaction();

        QSqlQuery qTipos(Configuracion_global->groupDB);
        qTipos.prepare("delete from tipocliente where id_cliente =:id_cliente");
        qTipos.bindValue(":id_cliente",id_cliente);
        if (!qTipos.exec()) {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrar Cliente"),tr("Falló el borrado de tipos de cliente"),
                                 tr("Aceptar"));
            borrado_ok = false;
        }
        if (Configuracion_global->enlace_web)
            BorrarWeb(this->id_web);


        QSqlQuery qdirecciones(Configuracion_global->groupDB);
        qdirecciones.prepare("Delete from cliente_direcciones where id_cliente =:id_cliente");
        qdirecciones.bindValue(":id_cliente",id_cliente);
        if(!qdirecciones.exec()) {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrar cliente"),tr("Falló el borrado de direcciones alternativas"),
                                 tr("Aceptar"));
            borrado_ok = false;
        }


        QSqlQuery qryCliente(Configuracion_global->groupDB);
        qryCliente.prepare("Delete from clientes where id = :id_cliente");
        qryCliente.bindValue(":id_cliente",id_cliente);
        if(!qryCliente.exec()) {
            QMessageBox::critical(qApp->activeWindow(),tr("Borrar cliente"),
                                  tr("Falló el borrado de la deuda del cliente"),tr("&Aceptar"));
            borrado_ok = false;
        }
        if (borrado_ok ==true) {
            Configuracion_global->groupDB.commit();
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Borrado corectamente"));
        } else {
            Configuracion_global->groupDB.rollback();
            QMessageBox::critical(qApp->activeWindow(),tr("Borrar cliente"),
                                  tr("Falló el borrado del cliente \ndeberá contactar con el administrador para su borrado manual"),tr("&Aceptar"));
        }

    }
}

void Cliente::BorrarWeb(int id_web)
{

    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Ficha en la web"),
                          tr("Está apunto de borrar la ficha de un cliente en la web \n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        Configuracion_global->AbrirDbWeb();
        QSqlQuery queryClienteWeb(QSqlDatabase::database("dbweb"));
        queryClienteWeb.prepare("delete from clientes where id =:id");
        queryClienteWeb.bindValue(":id",id_web);
        if (!queryClienteWeb.exec()) {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrado web"),
                                 tr("Ocurrió un error al borrar: %1").arg(queryClienteWeb.lastError().text()),
                                 tr("Aceptar"));
        }
        Configuracion_global->CerrarDbWeb();
    }
}

void Cliente::Actualizar_de_web()
{
    Configuracion_global->AbrirDbWeb();
    QSqlQuery queryClienteWeb(QSqlDatabase::database("dbweb"));
    queryClienteWeb.prepare("select * from clientes where id_local = 0");
    if (!queryClienteWeb.exec()) {
        if (queryClienteWeb.next()) {
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Hay clientes nuevos en la web"));
            int id = queryClienteWeb.record().value("id").toInt();
            Recuperar("select * from clientes where id = "+QString::number(id));
            Guardar();

        }
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Insertar desde web"),
                             tr("ERROR: %1").arg(queryClienteWeb.lastError().text()));
    }

    Configuracion_global->CerrarDbWeb();
}

QString Cliente::Nuevocodigo_cliente()
{
    QString codigo;
    QString cNum;
    unsigned long nCodigo;
    QSqlQuery *qClientes = new QSqlQuery(Configuracion_global->groupDB);
    if(qClientes->exec("select codigo_cliente from clientes  order by codigo_cliente desc limit 1"))
    {
        if (qClientes->next())
        {
            QSqlRecord registro = qClientes->record();
            codigo = registro.field("codigo_cliente").value().toString();
            nCodigo = codigo.toULong();
            nCodigo ++;
            codigo = QString::number(nCodigo);
        }
   }
    qDebug() <<"ncodigo"<< nCodigo;
    if(codigo.length() == Configuracion_global->digitos_cuentas_contables)
        codigo = codigo.mid(Configuracion_global->cuenta_clientes.length());
    if (nCodigo == 0 || nCodigo == 1)
    {
        cNum = "1";
    }
    else
        cNum = codigo;

    while (cNum.length()< (Configuracion_global->digitos_cuentas_contables - Configuracion_global->cuenta_clientes.length()) )
    {
        cNum.prepend("0");
    }
    codigo = Configuracion_global->cuenta_clientes + cNum;
    cuenta_iva_repercutido = Configuracion_global->cuenta_iva_repercutido1 +cNum;

    return codigo;
}

int Cliente::Buscarid_pais(QString Pais)
{
    QSqlQuery qPais(Configuracion_global->groupDB);
    qPais.prepare("select id from paises where pais =:pais");
    qPais.bindValue(":pais",Pais);
    if(qPais.exec()) {
        qPais.next();
        QSqlRecord rPais = qPais.record();
        return rPais.field("id").value().toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Pacientes"),
                             tr("No se pudo enconontrar el identificador del pais: %1 ").arg(Pais),tr("Aceptar"));
    }
    return 0;
}

QString Cliente::RecuperarPais(int nid)
{
    QSqlQuery qPais(Configuracion_global->groupDB);
    qPais.prepare("select pais from paises where id =:id");
    qPais.bindValue(":id",nid);
    if(qPais.exec()) {
        qPais.next();
        QSqlRecord rPais = qPais.record();
        return rPais.field("pais").value().toString();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Pacientes"),
                             tr("No se pudo enconontrar el Pais: %1 ").arg(QString::number(nid)),tr("Aceptar"));
    }
    return "";
}



