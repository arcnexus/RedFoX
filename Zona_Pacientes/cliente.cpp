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

    QSqlQuery query(Configuracion_global->groupDB);
    query.prepare( "UPDATE clientes set "
                   "codigo_cliente = :codigo_cliente,"
                   "apellido1 = :apellido1,"
                   "apellido2 = :apellido2,"
                   "nombre= :nombre,"
                   "nombre_fiscal= :nombre_fiscal,"
                   "nombre_comercial= :nombre_comercial,"
                   "persona_contacto= :persona_contacto,"
                   "cif_nif =:cif_nif,"
                   "direccion1= :direccion1,"
                   "direccion2= :direccion2,"
                   "cp= :cp,"
                   "poblacion= :poblacion,"
                   "provincia= :provincia,"
                   "id_pais= :id_pais,"
                   "telefono1 = :telefono1,"
                   "telefono2 = :telefono2,"
                   "fax=:fax,"
                   "movil=:movil,"
                   "email=:email,"
                   "web=:web,"
                   "fecha_alta=:fecha_alta,"
                   "fecha_ultima_compra=:fecha_ultima_compra,"
                   "acumulado_ventas=:acumulado_ventas,"
                   "ventas_ejercicio=:ventas_ejercicio,"
                   "riesgo_maximo=:riesgo_maximo,"
                   "deuda_actual=:deuda_actual,"
                   "comentarios=:comentarios,"
                   "bloqueado=:bloqueado,"
                   "comentario_bloqueo =:comentario_bloqueo,"
                   "porc_dto_cliente=:porc_dto_cliente,"
                   "recargo_equivalencia=:recargo_equivalencia,"
                   "cuenta_contable=:cuenta_contable,"
                   "cuenta_iva_repercutido=:cuenta_iva_repercutido,"
                   "cuenta_deudas=:cuenta_deudas,"
                   "cuenta_cobros=:cuenta_cobros,"
                   "dia_pago1=:dia_pago1,"
                   "dia_pago2=:dia_pago2,"
                   "tarifa_cliente=:tarifa_cliente,"
                   "importe_a_cuenta=:importe_a_cuenta,"
                   "vales=:vales,"
                   "entidad_bancaria=:entidad_bancaria,"
                   "oficina_bancaria=:oficina_bancaria,"
                   "dc=:dc,"
                   "cuenta_corriente=:cuenta_corriente,"
                   "fecha_nacimiento=:fecha_nacimiento,"
                   "importe_pendiente=:importe_pendiente,"
                   "acceso_web =:acceso_web,"
                   "password_web=:password_web,"
                   "id_idioma_documentos=:id_idioma,"
                   "cif_vies=:cif_vies,"
                   "id_web =:id_web,"
                   "irpf =:irpf,"
                   "visa_distancia1 =:visa_distancia1,"
                   "visa_distancia2 =:visa_distancia2,"
                   "visa1_caduca_mes =:visa1_caduca_mes,"
                   "visa2_caduca_mes =:visa2_caduca_mes,"
                   "visa1_caduca_ano =:visa1_caduca_ano,"
                   "visa2_caduca_ano =:visa2_caduca_ano,"
                   "visa1_cod_valid =:visa1_cod_valid,"
                   "visa2_cod_valid =:visa2_cod_valid,"
                   "id_agente =:id_agente,"
                   "id_transportista =:id_transportista,"
                   "observaciones =:observaciones,"
                   "grupo_iva =:grupo_iva"
                   " WHERE id =:id" );

    query.bindValue(":codigo_cliente", this->codigo_cliente);
    query.bindValue(":apellido1", this->apellido1);
    query.bindValue(":apellido2",this->apellido2);
    query.bindValue(":nombre",this->nombre);
    query.bindValue(":nombre_fiscal",this->nombre_fiscal);
    query.bindValue(":nombre_comercial",this->nombre_comercial);
    query.bindValue(":persona_contacto",this->persona_contacto);
    query.bindValue(":cif_nif",this->cif_nif);
    query.bindValue(":direccion1",this->direccion1);
    query.bindValue(":direccion2",this->direccion2);
    query.bindValue(":cp",this->cp);
    query.bindValue(":poblacion",this->poblacion);
    query.bindValue(":provincia", this->provincia);
    query.bindValue(":id_pais",this->id_pais);
    query.bindValue(":telefono1", this->telefono1);
    query.bindValue(":telefono2",this->telefono2);
    query.bindValue(":fax",this->fax);
    query.bindValue(":movil",this->movil);
    query.bindValue(":email",this->email);
    query.bindValue(":web",this->web);
    query.bindValue(":fecha_alta",this->fecha_alta);
    query.bindValue(":fecha_ultima_compra",this->fechaCompra);
    query.bindValue(":acumulado_ventas",this->acumulado_ventas);
    query.bindValue(":ventas_ejercicio",this->ventas_ejercicio);
    query.bindValue(":riesgo_maximo",this->riesgo_maximo);
    query.bindValue(":deuda_actual",this->deuda_actual);
    query.bindValue(":comentarios",this->comentarios);
    query.bindValue(":bloqueado",this->bloqueado);
    query.bindValue(":comentario_bloqueo",this->comentario_bloqueo);
    query.bindValue(":porc_dto_cliente", this->porc_dto_cliente);
    query.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
    query.bindValue(":cuenta_contable",this->cuenta_contable);
    query.bindValue(":cuenta_iva_repercutido",this->cuenta_iva_repercutido);
    query.bindValue(":cuenta_deudas", this->cuenta_deudas);
    query.bindValue(":cuenta_cobros", this->cuenta_cobros);
    query.bindValue(":dia_pago1",this->dia_pago1);
    query.bindValue(":dia_pago2",this->dia_pago2);
    query.bindValue(":tarifa_cliente",this->tarifa_cliente);
    QString importe_a_cuenta;
    importe_a_cuenta = QString::number(this->importe_a_cuenta);
    importe_a_cuenta = importe_a_cuenta.replace(".","");
    this->importe_a_cuenta = importe_a_cuenta.toDouble();
    query.bindValue(":importe_a_cuenta",this->importe_a_cuenta);
    query.bindValue(":vales",this->vales);
    query.bindValue(":entidad_bancaria", this->entidad_bancaria);
    query.bindValue(":oficina_bancaria",this->oficina_bancaria);
    query.bindValue(":dc",this->dc);
    query.bindValue(":cuenta_corriente",this->cuenta_corriente);
    query.bindValue(":fecha_nacimiento",this->fecha_nacimiento);
    query.bindValue(":importe_pendiente",this->importe_pendiente);
    query.bindValue(":acceso_web",this->acceso_web);
    query.bindValue(":id_idioma",this->ididioma);
    query.bindValue(":cif_vies",this->cifVies);
    query.bindValue(":password_web",this->password_web);
    query.bindValue("id_web",this->id_web);
    if (this->lIRPF)
        query.bindValue(":irpf",1);
    else
        query.bindValue(":irpf",0);
    query.bindValue(":observaciones",this->observaciones);
    query.bindValue(":visa_distancia1",this->visa_distancia1);
    query.bindValue(":visa_distancia2",this->visa_distancia2);
    query.bindValue(":id",this->id);
    query.bindValue(":visa1_caduca_mes",this->visa1_caduca_mes);
    query.bindValue(":visa2_caduca_mes",this->visa2_caduca_mes);
    query.bindValue(":visa1_caduca_ano",this->visa1_caduca_ano);
    query.bindValue(":visa2_caduca_ano",this->visa2_caduca_ano);
    query.bindValue(":visa1_cod_valid",this->visa1_cod_valid);
    query.bindValue(":visa2_cod_valid",this->visa2_cod_valid);
    query.bindValue(":id_agente",this->id_agente);
    query.bindValue(":id_transportista",this->id_transportista);
    query.bindValue(":grupo_iva",this->grupo_iva);


    if(!query.exec()){
        transaccion :false;
        QMessageBox::critical(qApp->activeWindow(),"error al guardar datos cliente. Descripción Error: ", query.lastError().text());
    } else {
        if (Configuracion_global->enlace_web ==true)
            GuardarWeb();
       // QMessageBox::information(qApp->activeWindow(),"Guardar datos","Los datos se han guardado corectamente:","Ok");
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
        cuenta.codigo_balance = "ABIII1";
        cuenta.saldo = 0;
        bool succes = cuenta.anadir_cuenta();
        if(!succes)
            transaccion = false;

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
  // QSqlQuery query(Configuracion_global->dbWeb));
   QSqlQuery query(Configuracion_global->dbWeb);
   query.prepare( "UPDATE clientes set "
                  "codigo_cliente = :codigo_cliente,"
                  "apellido1 = :apellido1,"
                  "apellido2 = :apellido2,"
                  "nombre= :nombre,"
                  "nombre_fiscal= :nombre_fiscal,"
                  "nombre_comercial= :nombre_comercial,"
                  "persona_contacto= :persona_contacto,"
                  "cif_nif =:cif_nif,"
                  "direccion1= :direccion1,"
                  "direccion2= :direccion2,"
                  "cp= :cp,"
                  "poblacion= :poblacion,"
                  "provincia= :provincia,"
                  "id_pais= :id_pais,"
                  "telefono1 = :telefono1,"
                  "telefono2 = :telefono2,"
                  "fax=:fax,"
                  "movil=:movil,"
                  "email=:email,"
                  "web=:web,"
                  "fecha_alta=:fecha_alta,"
                  "fecha_ultima_compra=:fecha_ultima_compra,"
                  "acumulado_ventas=:acumulado_ventas,"
                  "ventas_ejercicio=:ventas_ejercicio,"
                  "riesgo_maximo=:riesgo_maximo,"
                  "deuda_actual=:deuda_actual,"
                  "comentarios=:comentarios,"
                  "bloqueado=:bloqueado,"
                  "comentario_bloqueo =:comentario_bloqueo,"
                  "porc_dto_cliente=:porc_dto_cliente,"
                  "recargo_equivalencia=:recargo_equivalencia,"
                  "cuenta_contable=:cuenta_contable,"
                  "cuenta_iva_repercutido=:cuenta_iva_repercutido,"
                  "cuenta_deudas=:cuenta_deudas,"
                  "cuenta_cobros=:cuenta_cobros,"
                  "dia_pago1=:dia_pago1,"
                  "dia_pago2=:dia_pago2,"
                  "tarifa_cliente=:tarifa_cliente,"
                  "importe_a_cuenta=:importe_a_cuenta,"
                  "vales=:vales,"
                  "entidad_bancaria=:entidad_bancaria,"
                  "oficina_bancaria=:oficina_bancaria,"
                  "dc=:dc,"
                  "cuenta_corriente=:cuenta_corriente,"
                  "fecha_nacimiento=:fecha_nacimiento,"
                  "importe_pendiente=:importe_pendiente,"
                  "acceso_web =:acceso_web,"
                  "password_web=:password_web,"
                  "id_idioma_documentos=:id_idioma,"
                  "cif_vies=:cif_vies,"
                  "id_local =:id_local,"
                  "irpf =:irpf,"
                  "observaciones = :observaciones,"
                  "visa_distancia1 =:visa_distancia1,"
                  "visa_distancia2 =:visa_distancia2,"
                  "visa1_caduca_mes =:visa1_caduca_mes,"
                  "visa2_caduca_mes =:visa2_caduca_mes,"
                  "visa1_caduca_ano =:visa1_caduca_ano,"
                  "visa2_caduca_ano =:visa2_caduca_ano,"
                  "visa1_cod_valid =:visa1_cod_valid,"
                  "visa2_cod_valid =:visa2_cod_valid,"
                  "id_agente =:id_agente,"
                  "id_transportista =:id_transportista,"
                  "grupo_iva =:grupo_iva"
                  " WHERE id =:id_web" );

   query.bindValue(":codigo_cliente", this->codigo_cliente);
   query.bindValue(":apellido1", this->apellido1);
   query.bindValue(":apellido2",this->apellido2);
   query.bindValue(":nombre",this->nombre);
   query.bindValue(":nombre_fiscal",this->nombre_fiscal);
   query.bindValue(":nombre_comercial",this->nombre_comercial);
   query.bindValue(":persona_contacto",this->persona_contacto);
   query.bindValue(":cif_nif",this->cif_nif);
   query.bindValue(":direccion1",this->direccion1);
   query.bindValue(":direccion2",this->direccion2);
   query.bindValue(":cp",this->cp);
   query.bindValue(":poblacion",this->poblacion);
   query.bindValue(":provincia", this->provincia);
   query.bindValue(":id_pais",this->id_pais);
   query.bindValue(":telefono1", this->telefono1);
   query.bindValue(":telefono2",this->telefono2);
   query.bindValue(":fax",this->fax);
   query.bindValue(":movil",this->movil);
   query.bindValue(":email",this->email);
   query.bindValue(":web",this->web);
   query.bindValue(":fecha_alta",this->fecha_alta);
   query.bindValue(":fecha_ultima_compra",this->fechaCompra);
   query.bindValue(":acumulado_ventas",this->acumulado_ventas);
   query.bindValue(":ventas_ejercicio",this->ventas_ejercicio);
   query.bindValue(":riesgo_maximo",this->riesgo_maximo);
   query.bindValue(":deuda_actual",this->deuda_actual);
   query.bindValue(":comentarios",this->comentarios);
   query.bindValue(":bloqueado",this->bloqueado);
   query.bindValue(":comentario_bloqueo",this->comentario_bloqueo);
   query.bindValue(":porc_dto_cliente", this->porc_dto_cliente);
   query.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
   query.bindValue(":cuenta_contable",this->cuenta_contable);
   query.bindValue(":cuenta_iva_repercutido",this->cuenta_iva_repercutido);
   query.bindValue(":cuenta_deudas", this->cuenta_deudas);
   query.bindValue(":cuenta_cobros", this->cuenta_cobros);
   query.bindValue(":dia_pago1",this->dia_pago1);
   query.bindValue(":dia_pago2",this->dia_pago2);
   query.bindValue(":tarifa_cliente",this->tarifa_cliente);
   QString importe_a_cuenta;
   importe_a_cuenta = QString::number(this->importe_a_cuenta);
   importe_a_cuenta = importe_a_cuenta.replace(".","");
   this->importe_a_cuenta = importe_a_cuenta.toDouble();
   query.bindValue(":importe_a_cuenta",this->importe_a_cuenta);
   query.bindValue(":vales",this->vales);
   query.bindValue(":entidad_bancaria", this->entidad_bancaria);
   query.bindValue(":oficina_bancaria",this->oficina_bancaria);
   query.bindValue(":dc",this->dc);
   query.bindValue(":cuenta_corriente",this->cuenta_corriente);
   query.bindValue(":fecha_nacimiento",this->fecha_nacimiento);
   query.bindValue(":importe_pendiente",this->importe_pendiente);
   query.bindValue(":acceso_web",this->acceso_web);
   query.bindValue(":id_idioma",this->ididioma);
   query.bindValue(":cif_vies",this->cifVies);
   query.bindValue(":password_web",this->password_web);
   query.bindValue(":grupo_iva",this->grupo_iva);
   if (this->lIRPF)
       query.bindValue(":irpf",1);
   else
       query.bindValue(":irpf",0);
   query.bindValue(":id_web",this->id_web);
   query.bindValue(":observaciones",this->observaciones);
   query.bindValue(":id_local",this->id);
   query.bindValue(":visa1_caduca_mes",this->visa1_caduca_mes);
   query.bindValue(":visa2_caduca_mes",this->visa2_caduca_mes);
   query.bindValue(":visa1_caduca_ano",this->visa1_caduca_ano);
   query.bindValue(":visa2_caduca_ano",this->visa2_caduca_ano);
   query.bindValue(":visa1_cod_valid",this->visa1_cod_valid);
   query.bindValue(":visa2_cod_valid",this->visa2_cod_valid);
   query.bindValue(":id_agente",this->id_agente);
   query.bindValue(":id_transportista",this->id_transportista);


   if(!query.exec()){
       QMessageBox::critical(qApp->activeWindow(),"error al guardar datos cliente en la web. Descripción Error: ", query.executedQuery());
   } else {
      // QMessageBox::information(qApp->activeWindow(),"Guardar datos","Los datos se han guardado corectamente:","Ok");
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
    } else {
        if (qryCliente->next()) {            
            QSqlRecord registro = qryCliente->record();
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
            this->fechaCompra = registro.field("fechaCompra").value().toDate();
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
            this->forma_pago = registro.field("forma_pago").value().toString();
            this->dia_pago1 = registro.field("dia_pago1").value().toInt();
            this->dia_pago2 = registro.field("dia_pago2").value().toInt();
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
            int irpf =registro.field("irpf").value().toInt();
            this->grupo_iva = registro.field("grupo_iva").value().toString();
            if (irpf==1)
                this->lIRPF = true;
            else
                this->lIRPF = false;

            }
            else
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Final de archivo"));
    delete qryCliente;

    }

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
                               QString Provincia, QString Pais,int id_cliente,int id)
{
    QSqlQuery qdirecciones(Configuracion_global->groupDB);
    if(Anadir){
        qdirecciones.prepare("INSERT INTO cliente_direcciones (`descripcion,` `direccion1`, `direccion2`, `cp`, `poblacion`,"
                         "`provincia`, `id_pais`, `id_cliente`) "
                         "VALUES (:descripcion,:direccion1,:direccion2,:cp, :poblacion,:provincia,:id_pais,:id_cliente)");

    } else {
        qdirecciones.prepare("UPDATE cliente_direcciones SET "
                             "descripcion = :descripcion,"
                             "direccion1 = :direccion1,"
                             "direccion2 = :direccion2,"
                             "cp = :cp,"
                             "poblacion = :poblacion,"
                             "provincia = :provincia,"
                             "id_pais = :id_pais,"
                             "id_cliente =:id_cliente "
                             "WHERE id =:id");
    }
    qdirecciones.bindValue(":descripcion",Descripcion);
    qdirecciones.bindValue(":direccion1",direccion1);
    qdirecciones.bindValue(":direccion2",direccion2);
    qdirecciones.bindValue(":cp",CP);
    qdirecciones.bindValue(":poblacion",Poblacion);
    qdirecciones.bindValue(":provincia",Provincia);
    qdirecciones.bindValue(":id_pais",Buscarid_pais(Pais));
    qdirecciones.bindValue(":id_cliente",id_cliente);
    if(!Anadir)
        qdirecciones.bindValue(":id",id);

    if(!qdirecciones.exec()){
        qDebug() << qdirecciones.lastQuery();
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir/Guardar dirección"),
                             tr("Ocurrió un error al guardar los datos de dirección: %1").arg(qdirecciones.lastError().text()),
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



