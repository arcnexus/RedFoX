#include "presupuesto.h"


Presupuesto::Presupuesto()
{
    this->id = 0;
    this->editable = true;
}

Presupuesto::~Presupuesto()
{
}

bool Presupuesto::AnadirPresupuesto()
{
    this->iva1 = Configuracion_global->ivaList.at(0).toDouble();
    this->iva2 = Configuracion_global->ivaList.at(1).toDouble();
    this->iva3 = Configuracion_global->ivaList.at(2).toDouble();
    this->iva4 = Configuracion_global->ivaList.at(3).toDouble();

    this->porc_rec1 = 0;
    this->porc_rec2 = 0;
    this->porc_rec3 = 0;
    this->porc_rec4 = 0;

    QHash <QString, QVariant> cab_pre;
    cab_pre["presupuesto"] = presupuesto;
    cab_pre["fecha"] = fecha;
    cab_pre["valido_hasta"] = valido_hasta;
    cab_pre["id_cliente"] = id_cliente;
    cab_pre["codigo_cliente"] = codigo_cliente;
    cab_pre["cliente"] = cliente;
    cab_pre["cif"] = cif;
    cab_pre["direccion1"] = direccion1;
    cab_pre["direccion2"] = direccion2;
    cab_pre["cp"] = cp;
    cab_pre["poblacion"] = poblacion;
    cab_pre["provincia"] = provincia;
    cab_pre["id_pais"] = id_pais;
    cab_pre["direccion1_entrega"] = direccion1;
    cab_pre["direccion2_entrega"] = direccion2;
    cab_pre["cp_entrega"] = cp;
    cab_pre["poblacion_entrega"] = poblacion;
    cab_pre["provincia_entrega"] = provincia;
    cab_pre["id_pais_entrega"] = id_pais;
    cab_pre["email_entrega"] = email_entrega;
    cab_pre["comentarios_entrega"] = comentarios_entrega;
    cab_pre["telefono"] = telefono;
    cab_pre["movil"] = movil;
    cab_pre["fax"] = fax;
    cab_pre["porc_dto"] = 0;
    cab_pre["comentarios"] = comentarios;
    cab_pre["base"] = 0;
    cab_pre["dto"] = 0;
    cab_pre["total"] = 0;
    cab_pre["impreso"] = impreso;
    cab_pre["aprobado"] = aprobado;
    cab_pre["fecha_aprobacion"] = fecha_aprobacion;
    cab_pre["importe_factura"] = 0;
    cab_pre["importe_pendiente"] = 0;
    cab_pre["factura"] = factura;
    cab_pre["albaran"] = albaran;
    cab_pre["pedido"] = pedido;
    cab_pre["id_forma_pago"] = id_forma_pago;
    cab_pre["lugar_entrega"] = lugar_entrega;
    cab_pre["atencion_de"] = atencion_de;
    cab_pre["base1"] = 0;
    cab_pre["base2"] = 0;
    cab_pre["base3"] = 0;
    cab_pre["base4"] = 0;
    cab_pre["porc_iva1"] = iva1;
    cab_pre["porc_iva2"] = iva2;
    cab_pre["porc_iva3"] = iva3;
    cab_pre["porc_iva4"] = iva4;
    cab_pre["iva1"] = 0;
    cab_pre["iva2"] = 0;
    cab_pre["iva3"] = 0;
    cab_pre["iva4"] = 0;
    cab_pre["porc_rec1"] = porc_rec1;
    cab_pre["porc_rec2"] = porc_rec2;
    cab_pre["porc_rec3"] = porc_rec3;
    cab_pre["porc_rec4"] = porc_rec4;
    cab_pre["rec1"] = 0;
    cab_pre["rec2"] = 0;
    cab_pre["rec3"] = 0;
    cab_pre["rec4"] = 0;
    cab_pre["total1"] = 0;
    cab_pre["total2"] = 0;
    cab_pre["total3"] = 0;
    cab_pre["total4"] = 0;
    cab_pre["recargo_equivalencia"] = recargo_equivalencia;
    cab_pre["email"] = email;
    cab_pre["total_iva"] = 0;
    cab_pre["total_recargo"] = 0;
    cab_pre["editable"] = true;
    QString error;
    int new_id = SqlCalls::SqlInsert(cab_pre,"cab_pre",Configuracion_global->empresaDB,error);

     if(new_id == -1)
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Presupuesto:", error);
         return false;
     }
     else
     {         
         this->id = new_id;
         return true;
     }
}

bool Presupuesto::RecuperarPresupuesto(QString cSQL)
{
    QSqlQuery qCab_pre(Configuracion_global->empresaDB);
    qCab_pre.prepare(cSQL);
    if( qCab_pre.exec() )
    {
        if (qCab_pre.next())
        {
            QSqlRecord registro = qCab_pre.record();
            this->id = registro.field("id").value().toInt();
            this->presupuesto = registro.field("presupuesto").value().toInt();
            this->fecha = registro.field("fecha").value().toDate();
            this->ejercicio = registro.field("ejercicio").value().toInt();
            this->valido_hasta = registro.field("valido_hasta").value().toDate();
            this->id_cliente = registro.field("id_cliente").value().toInt();
            this->codigo_cliente= registro.field("codigo_cliente").value().toString();
            this->cliente = registro.field("cliente").value().toString();
            this->cif = registro.field("cif").value().toString();
            this->direccion1 = registro.field("direccion1").value().toString();
            this->direccion2 = registro.field("direccion2").value().toString();
            this->cp = registro.field("cp").value().toString();
            this->poblacion = registro.field("poblacion").value().toString();
            this->provincia = registro.field("provincia").value().toString();
            this->id_pais = registro.field("id_pais").value().toInt();
            this->direccion1_entrega = registro.field("direccion1").value().toString();
            this->direccion2_entrega = registro.field("direccion2").value().toString();
            this->cp_entrega = registro.field("cp").value().toString();
            this->poblacion_entrega = registro.field("poblacion").value().toString();
            this->provincia_entrega = registro.field("provincia").value().toString();
            this->id_pais_entrega = registro.field("id_pais").value().toInt();
            this->email_entrega = registro.field("email_entrega").value().toString();
            this->comentarios_entrega = registro.field("comentarios_entrega").value().toString();
            this->telefono = registro.field("telefono").value().toString();
            this->movil = registro.field("movil").value().toString();
            this->fax = registro.field("fax").value().toString();
            this->email = registro.field("email").value().toString();
            this->comentarios = registro.field("comentarios").value().toString();
            this->subtotal = registro.field("subtotal").value().toDouble();
            this->base = registro.field("base").value().toDouble();
            this->porc_dto = registro.field("porc_dto").value().toFloat();
            this->porc_dto_pp = registro.field("porc_dto_pp").value().toFloat();
            this->dto = registro.field("dto").value().toDouble();
            this->dto_pp = registro.field("dto_pp").value().toDouble();
            this->total = registro.field("total").value().toDouble();
            this->impreso = registro.value("impreso").toBool();
            this->aprobado = registro.field("aprobado").value().toBool();
            this->fecha_aprobacion = registro.field("fecha_aprobacion").value().toDate();
            this->importe_factura = registro.field("importe_factura").value().toDouble();
            this->factura = registro.field("factura").value().toString();
            this->albaran = registro.field("albaran").value().toInt();
            this->pedido = registro.field("pedido").value().toInt();
            this->desc_gasto1 = registro.field("gastos_distribuidos1").value().toString();
            this->desc_gasto2 = registro.field("gastos_distribuidos2").value().toString();
            this->desc_gasto3 = registro.field("gastos_distribuidos3").value().toString();
            this->importe_gasto1 = registro.field("importe_gasto1").value().toDouble();
            this->importe_gasto2 = registro.field("importe_gasto2").value().toDouble();
            this->importe_gasto3 = registro.field("importe_gasto3").value().toDouble();
            this->porc_iva_gasto1 = registro.field("porc_iva_gasto1").value().toFloat();
            this->porc_iva_gasto2 = registro.field("porc_iva_gasto2").value().toFloat();
            this->porc_iva_gasto3 = registro.field("porc_iva_gasto3").value().toFloat();
            this->iva_gasto1 = registro.field("iva_gasto1").value().toDouble();
            this->iva_gasto2 = registro.field("iva_gasto2").value().toDouble();
            this->iva_gasto3 = registro.field("iva_gasto3").value().toDouble();


            this->id_forma_pago = registro.field("id_forma_pago").value().toInt();

            QSqlQuery q(Configuracion_global->groupDB);
            if(q.exec("SELECT * FROM formpago WHERE id = "+QString::number(id_forma_pago)))
                if(q.next())
                {
                    this->codigoFormaPago = q.record().value("codigo").toString();
                    this->descripcionFormaPago = q.record().value("forma_pago").toString();
                }

            this->lugar_entrega = registro.field("lugar_entrega").value().toString();
            this->atencion_de = registro.field("atencion_de").value().toString();
            this->base1 = registro.field("base1").value().toDouble();
            this->base2 = registro.field("base2").value().toDouble();
            this->base3 = registro.field("base3").value().toDouble();
            this->base4 = registro.field("base4").value().toDouble();
            this->porc_iva1 = registro.field("porc_iva1").value().toDouble();
            this->porc_iva2 = registro.field("porc_iva2").value().toDouble();
            this->porc_iva3 = registro.field("porc_iva3").value().toDouble();
            this->porc_iva4 = registro.field("porc_iva4").value().toDouble();
            this->iva1 = registro.field("iva1").value().toDouble();
            this->iva2 = registro.field("iva2").value().toDouble();
            this->iva3 = registro.field("iva3").value().toDouble();
            this->iva4 = registro.field("iva4").value().toDouble();
            this->total_iva = registro.field("total_iva").value().toDouble();
            this->porc_rec1 = registro.field("porc_rec1").value().toDouble();
            this->porc_rec2 = registro.field("porc_rec2").value().toDouble();
            this->porc_rec3 = registro.field("porc_rec3").value().toDouble();
            this->porc_rec4 = registro.field("porc_rec4").value().toDouble();
            this->rec1 = registro.field("rec1").value().toDouble();
            this->rec2 = registro.field("rec2").value().toDouble();
            this->rec3 = registro.field("rec3").value().toDouble();
            this->rec4 = registro.field("rec4").value().toDouble();
            this->total_recargo = registro.field("total_recargo").value().toDouble();
            this->total1 = registro.field("total1").value().toDouble();
            this->total2 = registro.field("total2").value().toDouble();
            this->total3 = registro.field("total3").value().toDouble();
            this->total4 = registro.field("total4").value().toDouble();
            this->recargo_equivalencia = registro.field("recargo_equivalencia").value().toBool();
            this->editable = registro.field("editable").value().toBool();
            return true;
        }
     }
    return false;
}

bool Presupuesto::siguiente()
{
    return RecuperarPresupuesto("Select * from cab_pre where id >'"+
                                    QString::number(id)+"' order by id limit 1 ");
}

bool Presupuesto::anterior()
{
   return RecuperarPresupuesto("Select * from cab_pre where id <'"+QString::number(id)+
                               "' order by id desc limit 1 ");
}

bool Presupuesto::GuardarPres(int nid_Presupuesto)
{

    if (this->presupuesto == 0)
        this->presupuesto = NuevoNumeroPresupuesto();
    QHash <QString,QVariant> cab_pre;
    QString error;


    cab_pre["fecha"] = fecha;
    cab_pre["valido_hasta"] = valido_hasta;
    cab_pre["id_cliente"] = id_cliente;
    cab_pre["codigo_cliente"] = codigo_cliente;
    cab_pre["cliente"] = cliente;
    cab_pre["cif"] = cif;
    cab_pre["direccion1"] = direccion1;
    cab_pre["direccion2"] = direccion2;
    cab_pre["cp"] = cp;
    cab_pre["poblacion"] = poblacion;
    cab_pre["provincia"] = provincia;
    cab_pre["id_pais"] = id_pais;
    cab_pre["direccion1_entrega"] = direccion1_entrega;
    cab_pre["direccion2_entrega"] = direccion2_entrega;
    cab_pre["cp_entrega"] = cp_entrega;
    cab_pre["poblacion_entrega"] = poblacion_entrega;
    cab_pre["provincia_entrega"] = provincia_entrega;
    cab_pre["id_pais_entrega"] = id_pais_entrega;
    cab_pre["email_entrega"] = email_entrega;
    cab_pre["comentarios_entrega"] = comentarios_entrega;

    cab_pre["telefono"] = telefono;
    cab_pre["movil"] = movil;
    cab_pre["fax"] = fax;
    cab_pre["comentarios"] = comentarios;
    cab_pre["importe"] = subtotal;
    cab_pre["dto"] = dto;
    cab_pre["dto_pp"] = dto_pp;
    cab_pre["porc_dto"] = porc_dto;
    cab_pre["porc_dto_pp"] = porc_dto_pp;
    cab_pre["total"] = total;
    cab_pre["impreso"] = impreso;
    cab_pre["aprobado"] = aprobado;
    cab_pre["fecha_aprobacion"] = fecha_aprobacion;
    cab_pre["importe_factura"] = importe_factura;
    cab_pre["importe_pendiente"] = 0;//TODO preguntar a Marc
    cab_pre["factura"] = factura;
    cab_pre["albaran"] = albaran;
    cab_pre["pedido"] = pedido;
    cab_pre["id_forma_pago"] = id_forma_pago;
    cab_pre["lugar_entrega"] = lugar_entrega;
    cab_pre["atencion_de"] = atencion_de;
    cab_pre["base1"] = base1;
    cab_pre["base2"] = base2;
    cab_pre["base3"] = base3;
    cab_pre["base4"] = base4;
    cab_pre["porc_iva1"] = porc_iva1;
    cab_pre["porc_iva2"] = porc_iva2;
    cab_pre["porc_iva3"] = porc_iva3;
    cab_pre["porc_iva4"] = porc_iva4;
    cab_pre["iva1"] = iva1;
    cab_pre["iva2"] = iva2;
    cab_pre["iva3"] = iva3;
    cab_pre["iva4"] = iva4;
    cab_pre["porc_rec1"] = porc_rec1;
    cab_pre["porc_rec2"] = porc_rec2;
    cab_pre["porc_rec3"] = porc_rec3;
    cab_pre["porc_rec4"] = porc_rec4;
    cab_pre["rec1"] = rec1;
    cab_pre["rec2"] = rec2;
    cab_pre["rec3"] = rec3;
    cab_pre["rec4"] = rec4;
    cab_pre["total1"] = total1;
    cab_pre["total2"] = total2;
    cab_pre["total3"] = total3;
    cab_pre["total4"] = total4;
    cab_pre["recargo_equivalencia"] = recargo_equivalencia;
    cab_pre["email"] = email;
    cab_pre["total_iva"] = total_iva;
    cab_pre["total_recargo"] = total_recargo;
    cab_pre["gastos_distribuidos1"] = desc_gasto1;
    cab_pre["gastos_distribuidos2"] = desc_gasto2;
    cab_pre["gastos_distribuidos3"] = desc_gasto3;
    cab_pre["importe_gasto1"] = importe_gasto1;
    cab_pre["importe_gasto2"] = importe_gasto2;
    cab_pre["importe_gasto3"] = importe_gasto3;
    cab_pre["porc_iva_gasto1"] = porc_iva_gasto1;
    cab_pre["porc_iva_gasto2"] = porc_iva_gasto2;
    cab_pre["porc_iva_gasto3"] = porc_iva_gasto3;
    cab_pre["iva_gasto1"] = iva_gasto1;
    cab_pre["iva_gasto2"] = iva_gasto2;
    cab_pre["iva_gasto3"] = iva_gasto3;

    cab_pre["presupuesto"] = this->presupuesto;
    cab_pre["editable"] = editable;

    bool updated = SqlCalls::SqlUpdate(cab_pre,"cab_pre",Configuracion_global->empresaDB,QString("id=%1").arg(nid_Presupuesto),error);

    if(!updated)
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Presupuesto:", error);
        return false;
    }else
        return true;
}

bool Presupuesto::BorrarLineas(int nid_Presupuesto)
{
    QSqlQuery query(Configuracion_global->empresaDB);
    QString sql = QString("DELETE FROM lin_pre WHERE id_Cab = %1").arg(nid_Presupuesto);
    query.prepare(sql);
    if(query.exec())
        return true;
    else
    {
        QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
        return false;
    }
}

int Presupuesto::NuevoNumeroPresupuesto()
{
    QSqlQuery cab_pre(Configuracion_global->empresaDB);
    int presupuesto;
    cab_pre.prepare("Select presupuesto from cab_pre order by presupuesto desc limit 1");
    if(cab_pre.exec()) {
        cab_pre.next();
        presupuesto= cab_pre.value(0).toInt();
        presupuesto ++;
    } else {
         QMessageBox::critical(qApp->activeWindow(), "Error:", cab_pre.lastError().text());
    }
    return presupuesto;
}

