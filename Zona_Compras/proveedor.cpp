#include "proveedor.h"



Proveedor::Proveedor(QObject *parent) :
    QObject(parent)
{
    this->id = 0;
    id_pais = 1;
    pais = "España";
    id_divisa = 1;
}

void Proveedor::cargaracumulados(int id_proveedor)
{
    QSqlQuery query_acumulados(Configuracion_global->empresaDB);
    if(query_acumulados.exec(QString("select * from acum_proveedores where id_proveedor =%1 and id_empresa=%2").arg(id_proveedor).arg(Configuracion_global->idEmpresa)))
    {
        query_acumulados.next();
        this->enero = query_acumulados.record().value("acum_enero").toDouble();
        this->febrero = query_acumulados.record().value("acum_febrero").toDouble();
        this->marzo = query_acumulados.record().value("acum_marzo").toDouble();
        this->abril = query_acumulados.record().value("acum_abril").toDouble();
        this->mayo = query_acumulados.record().value("acum_mayo").toDouble();
        this->junio = query_acumulados.record().value("acum_junio").toDouble();
        this->julio = query_acumulados.record().value("acum_julio").toDouble();
        this->agosto = query_acumulados.record().value("acum_agosto").toDouble();
        this->septiembre = query_acumulados.record().value("acum_septiembre").toDouble();
        this->octubre = query_acumulados.record().value("acum_octubre").toDouble();
        this->noviembre = query_acumulados.record().value("acum_noviembre").toDouble();
        this->diciembre = query_acumulados.record().value("acum_diciembre").toDouble();
    }


}

bool Proveedor::acumular(int id_proveedor, int mes, double importe)
{
return true;
}

void Proveedor::anadir_persona_contacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                        QString desc_telefono2, QString Telefono2, QString desc_telefono3, QString Telefono3,
                                        QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2,QString cargo,
                                        QString email)
    {
        QSqlQuery qContactos(Configuracion_global->groupDB);
        qContactos.prepare("INSERT INTO personascontactoproveedor "
                           "(nombre,"
                           "telefono1,"
                           "telefono2,"
                           "telefono3,"
                           "movil1,"
                           "movil2,"
                           "id_proveedor,"
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
                          ":movil1,"
                          ":movil2,"
                          ":id_proveedor,"
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
                           qContactos.bindValue(":id_proveedor",id);
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

void Proveedor::guardar_persona_contacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                         QString desc_telefono2, QString Telefono2, QString desc_telefono3,
                                         QString Telefono3, QString desc_movil1, QString Movil1, QString desc_movil2,
                                         QString Movil2, QString cargo, QString email)
{
    QSqlQuery qContactos(Configuracion_global->groupDB);
    qContactos.prepare("UPDATE personascontactoproveedor SET "
    "nombre =:nombre,"
    "telefono1 =:telefono1,"
    "telefono2 =:telefono2,"
    "telefono3 =:telefono3,"
    "movil1 =:movil1,"
    "movil2 =:movil2,"
    "desc_telefono1 =:desc_telefono1,"
    "desc_telefono2 =:desc_telefono2,"
    "desc_telefono3 =:desc_telefono3,"
    "desc_movil1 =:desc_movil1,"
    "desc_movil2 =:desc_movil2,"
    "cargo_empresa =:cargo_empresa,"
    "email =:email"
    " where id = :id");


    qContactos.bindValue(":nombre",Nombre);
    qContactos.bindValue(":telefono1",Telefono1);
    qContactos.bindValue( ":telefono2",Telefono2);
    qContactos.bindValue(":telefono3",Telefono3);
    qContactos.bindValue(":movil1",Movil1);
    qContactos.bindValue(":movil2",Movil2);
    qContactos.bindValue(":id_proveedor",id);
    qContactos.bindValue(":desc_telefono1",desc_telefono1);
    qContactos.bindValue(":desc_telefono2",desc_telefono2);
    qContactos.bindValue(":desc_telefono3",desc_telefono3);
    qContactos.bindValue(":desc_movil1",desc_movil1);
    qContactos.bindValue(":desc_movil2",desc_movil2);
    qContactos.bindValue(":cargo_empresa",cargo);
    qContactos.bindValue(":email",email);
    qContactos.bindValue(":id",id);
    if(!qContactos.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Editar personas de contacto"),
                             tr("Falló el guardar una persona de contacto: %1").arg(qContactos.lastError().text()));
}

void Proveedor::Anadir()
{
    QString error;
    QHash<QString,QVariant> _data;
    _data["codigo"] = Nuevocodigo_proveedor();
    int nid  = SqlCalls::SqlInsert(_data,"proveedores",Configuracion_global->groupDB,error);
    if(nid != -1)
    {
        this->codigo = _data.value("codigo").toString();
        this->cuenta_aplicacion = this->codigo;
        this->id = nid;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),
                             QObject::tr("Gestión de proveedores"),
                             QObject::tr("No se ha podido crear una nueva ficha de proveedor:\n")+
                             error,QObject::tr("Aceptar"));
    }
}

bool Proveedor::Recuperar(int id)
{
    QString error;
    QMap<int, QSqlRecord> _map = SqlCalls::SelectRecord("proveedores",QString("id = %1").arg(id),Configuracion_global->groupDB,error);
    if(!_map.isEmpty())
    {
        Cargar(_map.first());
        cargaracumulados(this->id);
        return true;
    }
    else
    {
        if(error.isEmpty())
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("No se encuentra el proveedor"));
        else
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),
                                 QObject::tr("Problema en la BD no se puede recuperar el proveedor:\n")+
                                        error ,QObject::tr("Ok"));
    }
    return false;
}

bool Proveedor::Next()
{
    QString error;
    QMap<int, QSqlRecord> _map = SqlCalls::SelectRecord("proveedores",QString("id > %1").arg(id),Configuracion_global->groupDB,error);
    if(!_map.isEmpty())
    {
        Cargar(_map.first());
        cargaracumulados(this->id);
        return true;
    }
    else
    {
        if(error.isEmpty())
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("Se alcanzó el final del archivo"));
        else
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),
                                 QObject::tr("Problema en la BD no se puede recuperar el proveedor:\n")+
                                        error ,QObject::tr("Ok"));
    }
    return false;
}

bool Proveedor::Prev()
{
    QString error;
    QMap<int, QSqlRecord> _map = SqlCalls::SelectRecord("proveedores",QString("id < %1").arg(id),Configuracion_global->groupDB,error);
    if(!_map.isEmpty())
    {
        Cargar(_map.first());
        cargaracumulados(this->id);
        return true;
    }
    else
    {
        if(error.isEmpty())
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("Se alcanzó el final del archivo"));
        else
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),
                                 QObject::tr("Problema en la BD no se puede recuperar el proveedor:\n")+
                                        error ,QObject::tr("Ok"));
    }
    return false;
}

void Proveedor::Cargar(QSqlRecord &rProveedor)
{
    this->id = rProveedor.field("id").value().toInt();
    this->codigo = rProveedor.field("codigo").value().toString();
    this->proveedor = rProveedor.field("proveedor").value().toString();
    this->cif = rProveedor.field("cif").value().toString();
    this->direccion1 = rProveedor.field("direccion1").value().toString();
    this->direccion2 = rProveedor.field("direccion2").value().toString();
    this->cp = rProveedor.field("cp").value().toString();
    this->poblacion = rProveedor.field("poblacion").value().toString();
    this->provincia = rProveedor.field("provincia").value().toString();
    this->id_pais = rProveedor.field("id_pais").value().toInt();
    this->pais = Configuracion_global->Devolver_pais(this->id_pais);
    this->telefono1 = rProveedor.field("telefono1").value().toString();
    this->telefono2 = rProveedor.field("telefono2").value().toString();
    this->telefono3 = rProveedor.field("telefono3").value().toString();
    this->fax = rProveedor.field("fax").value().toString();
    this->movil = rProveedor.field("movil").value().toString();
    this->email = rProveedor.field("email").value().toString();
    this->web = rProveedor.field("web").value().toString();
    this->tipo_iva = rProveedor.field("tipo_iva").value().toInt();
    this->persona_contacto = rProveedor.field("persona_contacto").value().toString();
    this->dia_cobro = rProveedor.field("dia_cobro").value().toInt();
    this->direccion_almacen = rProveedor.field("direccion_almacen").value().toString();
    this->cp_almacen = rProveedor.field("cp_almacen").value().toString();
    this->poblacion_almacen = rProveedor.field("poblacion_almacen").value().toString();
    this->provincia_almacen = rProveedor.field("provincia_almacen").value().toString();
    this->id_pais_almacen = rProveedor.field("id_pais_almacen").value().toInt();
    this->paisAlmacen = Configuracion_global->Devolver_pais(this->id_pais_almacen);
    this->telefono_almacen = rProveedor.field("telefono_almacen").value().toString();
    this->fax_almacen = rProveedor.field("fax_almacen").value().toString();
    this->idFormadePago = rProveedor.field("id_forma_pago").value().toInt();
    this->codigoFormaPago = Configuracion_global->Devolver_codigo_forma_pago(this->idFormadePago);
    this->fecha_ultima_compra = rProveedor.field("fecha_ultima_compra").value().toDate();
    this->importe_acumulado_compras = rProveedor.field("importe_acumulado_compras").value().toDouble();
    this->entidad_bancaria_proveedor = rProveedor.field("entidad_bancaria_proveedor").value().toString();
    this->oficina_bancaria_proveedor = rProveedor.field("oficina_bancaria_proveedor").value().toString();
    this->dc_proveedor = rProveedor.field("dc_proveedor").value().toString();
    this->cc_proveedor = rProveedor.field("cc_proveedor").value().toString();
    this->entidad_pago_proveedor = rProveedor.field("entidad_pago_proveedor").value().toString();
    this->oficina_pago_proveedor = rProveedor.field("oficina_pago_proveedor").value().toString();
    this->dc_pago_proveedor = rProveedor.field("dc_pago_proveedor").value().toString();
    this->retencion_irpf = rProveedor.field("retencion_irpf").value().toDouble();
    this->tipo_retencion = rProveedor.field("tipo_retencion").value().toInt();
    this->cuenta_aplicacion = rProveedor.field("cuenta_aplicacion").value().toString();
    this->comentarios = rProveedor.field("comentarios").value().toString();
    this->dto = rProveedor.field("dto").value().toDouble();
    this->fecha_alta = rProveedor.field("fecha_alta").value().toDate();
    this->deuda_maxima = rProveedor.field("deuda_maxima").value().toDouble();
    this->deuda_actual = rProveedor.field("deuda_actual").value().toDouble();
    this->recargo_equivalencia = rProveedor.field("recargo_equivalencia").value().toInt();
    this->texto_para_pedidos = rProveedor.field("texto_para_pedidos").value().toString();
    this->entregado_a_cuenta = rProveedor.field("entregado_a_cuenta").value().toDouble();
}


void Proveedor::Guardar()
{
   // QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(Configuracion_global->groupDB));
    QSqlQuery queryProveedor(Configuracion_global->groupDB);

    queryProveedor.prepare("UPDATE proveedores SET "
                        " codigo =:codigo,"
                        "proveedor =:proveedor,"
                        "cif =:cif,"
                        "direccion1 =:direccion1,"
                        "direccion2 =:direccion2,"
                        "cp =:cp,"
                        "poblacion =:poblacion,"
                        "provincia =:provincia, "
                        "id_pais =:id_pais, "
                        "telefono1 =:telefono1,"
                        "telefono2 =:telefono2,"
                        "telefono3 =:telefono3,"
                        "fax =:fax,"
                        "movil =:movil,"
                        "email =:email,"
                        "web =:web,"
                        "tipo_iva =:tipo_iva,"
                        "persona_contacto =:persona_contacto,"
                        "dia_cobro =:dia_cobro,"
                        "direccion_almacen =:direccion_almacen,"
                        "cp_almacen =:cp_almacen,"
                        "poblacion_almacen =:poblacion_almacen,"
                        "provincia_almacen =:provincia_almacen,"
                        "id_pais_almacen =:id_pais_almacen,"
                        "telefono_almacen =:telefono_almacen,"
                        "fax_almacen =:fax_almacen,"
                        "id_forma_pago =:id_forma_pago,"
                        "fecha_ultima_compra =:fecha_ultima_compra,"
                        "importe_acumulado_compras =:importe_acumulado_compras,"
                        "entidad_bancaria_proveedor =:entidad_bancaria_proveedor,"
                        "oficina_bancaria_proveedor =:oficina_bancaria_proveedor,"
                        "dc_proveedor =:dc_proveedor,"
                        "cc_proveedor =:cc_proveedor,"
                        "entidad_pago_proveedor =:entidad_pago_proveedor,"
                        "oficina_pago_proveedor =:oficina_pago_proveedor,"
                        "dc_pago_proveedor =:dc_pago_proveedor,"
                        "retencion_irpf =:retencion_irpf,"
                        "tipo_retencion =:tipo_retencion,"
                        "cuenta_aplicacion =:cuenta_aplicacion,"
                        "comentarios =:comentarios,"
                        "dto =:dto,"
                        "fecha_alta =:fecha_alta,"
                        "deuda_maxima =:deuda_maxima,"
                        "deuda_actual =:deuda_actual,"
                        "recargo_equivalencia =:recargo_equivalencia,"
                        "texto_para_pedidos =:texto_para_pedidos "
                        " WHERE id = :id");


    queryProveedor.bindValue(":codigo",this->codigo);
    queryProveedor.bindValue(":proveedor",this->proveedor);
    queryProveedor.bindValue(":cif",this->cif);
    queryProveedor.bindValue(":direccion1",this->direccion1);
    queryProveedor.bindValue(":direccion2",this->direccion2);
    queryProveedor.bindValue(":cp",this->cp);
    queryProveedor.bindValue(":poblacion",this->poblacion);
    queryProveedor.bindValue(":provincia",this->provincia);
    queryProveedor.bindValue(":id_pais",this->id_pais);
    queryProveedor.bindValue(":telefono1",this->telefono1);
    queryProveedor.bindValue(":telefono2",this->telefono2);
    queryProveedor.bindValue(":telefono3",this->telefono3);
    queryProveedor.bindValue(":fax",this->fax);
    queryProveedor.bindValue(":movil",this->movil);
    queryProveedor.bindValue(":email",this->email);
    queryProveedor.bindValue(":web",this->web);
    queryProveedor.bindValue(":tipo_iva",this->tipo_iva);
    queryProveedor.bindValue(":persona_contacto",this->persona_contacto);
    queryProveedor.bindValue(":dia_cobro",this->dia_cobro);
    queryProveedor.bindValue(":direccion_almacen",this->direccion_almacen);
    queryProveedor.bindValue(":cp_almacen",this->cp_almacen);
    queryProveedor.bindValue(":poblacion_almacen",this->poblacion_almacen);
    queryProveedor.bindValue(":provincia_almacen",this->provincia_almacen);
    queryProveedor.bindValue(":id_pais_almacen",this->id_pais_almacen);
    queryProveedor.bindValue(":telefono_almacen",this->telefono_almacen);
    queryProveedor.bindValue(":fax_almacen",this->fax_almacen);
    queryProveedor.bindValue(":id_forma_pago",this->idFormadePago);
    queryProveedor.bindValue(":fecha_ultima_compra",this->fecha_ultima_compra);
    queryProveedor.bindValue(":importe_acumulado_compras",this->importe_acumulado_compras);
    queryProveedor.bindValue(":entidad_bancaria_proveedor",this->entidad_bancaria_proveedor);
    queryProveedor.bindValue(":oficina_bancaria_proveedor",this->oficina_bancaria_proveedor);
    queryProveedor.bindValue(":dc_proveedor",this->dc_proveedor);
    queryProveedor.bindValue(":cc_proveedor",this->cc_proveedor);
    queryProveedor.bindValue(":entidad_pago_proveedor",this->entidad_pago_proveedor);
    queryProveedor.bindValue(":oficina_pago_proveedor",this->oficina_pago_proveedor);
    queryProveedor.bindValue(":dc_pago_proveedor",this->dc_pago_proveedor);
    queryProveedor.bindValue(":retencion_irpf",this->retencion_irpf);
    queryProveedor.bindValue(":tipo_retencion",this->tipo_retencion);
    queryProveedor.bindValue(":cuenta_aplicacion",this->cuenta_aplicacion);
    queryProveedor.bindValue(":comentarios",this->comentarios);
    queryProveedor.bindValue(":dto",this->dto);
    queryProveedor.bindValue(":fecha_alta",this->fecha_alta);
    queryProveedor.bindValue(":deuda_maxima",this->deuda_maxima);
    queryProveedor.bindValue(":deuda_actual",this->deuda_actual);
    queryProveedor.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
    queryProveedor.bindValue(":texto_para_pedidos",this->texto_para_pedidos);
    queryProveedor.bindValue(":id",this->id);

    if(!queryProveedor.exec()){
        qDebug() << queryProveedor.lastQuery();

        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión proveedores/Acreedores"),
           QObject::tr("No se ha podido insertar el proveedor en la BD: %1").arg(queryProveedor.lastError().text(),
           QObject::tr("Ok")));
    } else {
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),QObject::tr("Los datos se han guardado corectamente"));
    }
}

void Proveedor::Vaciar()
{
    this->id = 0;
    this->codigo = "";
    this->proveedor = "";
    this->cif = "";
    this->direccion1 = "";
    this->direccion2 = "";
    this->cp = "";
    this->poblacion = "";
    this->provincia = "";
    this->pais = "España";
    this->id_pais = 1;
    this->telefono1 = "";
    this->telefono2 = "";
    this->telefono3 = "";
    this->fax = "";
    this->movil = "";
    this->email ="";
    this->web = "";
    this->tipo_iva = 1;
    this->persona_contacto = "";
    this->dia_cobro = 0;
    this->direccion_almacen = "";
    this->cp_almacen = "";
    this->poblacion_almacen = "";
    this->provincia_almacen = "";
    this->paisAlmacen = "";
    this->telefono_almacen = "";
    this->fax_almacen = "";
    this->codigoFormaPago = "";
    this->fecha_ultima_compra = QDate::currentDate();
    this->importe_acumulado_compras = 0;
    this->entidad_bancaria_proveedor ="";
    this->oficina_bancaria_proveedor = "";
    this->dc_proveedor = "";
    this->cc_proveedor = "";
    this->entidad_pago_proveedor = "";
    this->oficina_pago_proveedor = "";
    this->dc_pago_proveedor = "";
    this->retencion_irpf = 0;
    this->tipo_retencion = 0;
    this->cuenta_aplicacion = "";
    this->comentarios = "";
    this->dto = 0;
    this->fecha_alta = QDate::currentDate();
    this->deuda_maxima = 0;
    this->deuda_actual = 0;
    this->recargo_equivalencia = 0;
    this->texto_para_pedidos = "";
    this->entregado_a_cuenta = 0;
    this->idFormadePago =1;
    this->id_pais_almacen =1;
    id_divisa = 1;
}

void Proveedor::Borrar(int nid)
{        
    QString error;
    if(SqlCalls::SqlDelete("proveedores",Configuracion_global->groupDB,QString("id = %1").arg(nid),error))
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Proveedor borrado con éxito"));
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Proveedores"),
                             tr("No Se ha borrado la ficha del proveedor:\n%1").arg(error)
                             ,tr("Aceptar"));
}

void Proveedor::clear()
{
    this->id = 0;
    this->codigo = "";
    this->proveedor = "";
    this->cif = "";
    this->direccion1 = "";
    this->direccion2 = "";
    this->cp = "";
    this->poblacion = "";
    this->provincia = "";
    this->id_pais = 0;
    this->pais =  "";
    this->telefono1 = "";
    this->telefono2 = "";
    this->telefono3 = "";
    this->fax = "";
    this->movil = "";
    this->email = "";
    this->web = "";
    this->persona_contacto = "";
    this->dia_cobro = 0;
    this->direccion_almacen = "";
    this->cp_almacen = "";
    this->poblacion_almacen = "";
    this->provincia_almacen = "";
    this->id_pais_almacen = 0;
    this->paisAlmacen = "";
    this->telefono_almacen = "";
    this->fax_almacen = "";
    this->idFormadePago = 0;
    this->codigoFormaPago = "";
    this->fecha_ultima_compra = QDate::currentDate();
    this->importe_acumulado_compras = 0;
    this->entidad_bancaria_proveedor = "";
    this->oficina_bancaria_proveedor = "";
    this->dc_proveedor = "";
    this->cc_proveedor = "";
    this->entidad_pago_proveedor ="";
    this->oficina_pago_proveedor = "";
    this->dc_pago_proveedor = "";
    this->retencion_irpf = 0;
    this->tipo_retencion = 0;
    this->cuenta_aplicacion = "";
    this->comentarios = "";
    this->dto = 0;
    this->fecha_alta = QDate::currentDate();
    this->deuda_maxima = 0;
    this->deuda_actual = 0;
    this->recargo_equivalencia = false;
    this->texto_para_pedidos = "";
    this->entregado_a_cuenta =0;
}

QString Proveedor::Nuevocodigo_proveedor()
{   
   QString codigo;
   double nCodigo = 1;
   QSqlQuery qProveedores(Configuracion_global->groupDB);

   if(qProveedores.exec("select codigo from proveedores  order by codigo desc limit 1"))
   {
       if (qProveedores.next())
       {
           QSqlRecord registro = qProveedores.record();
           codigo = registro.field("codigo").value().toString();
           nCodigo = codigo.mid(Configuracion_global->cuenta_clientes.length()).toDouble();
           nCodigo ++;
           codigo = QString::number(nCodigo);
       }
  }

   QString codigo_nuevo;
   QString formato = QString("%1.0f").arg(Configuracion_global->digitos_cuentas_contables - Configuracion_global->cuenta_proveedores.length());
   formato.prepend("%0");
   std::string _x = formato.toStdString();
   codigo_nuevo.sprintf(_x.c_str(),nCodigo);

   codigo_nuevo.prepend(Configuracion_global->cuenta_proveedores);
   return codigo_nuevo;
}


