#include "pedidos.h"


Pedidos::Pedidos()
{
    this->id =0;
    iva_gasto1 = 0;
    iva_gasto2 = 0;
    iva_gasto3 = 0;
}

Pedidos::~Pedidos()
{
}

bool Pedidos::BorrarLineas(int Iped)
{
    QMap <int,QSqlRecord>  l;
    QString error,error2;
    bool success,success2;
    l= SqlCalls::SelectRecord("lin_ped",QString("id_cab = %1").arg(Iped),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord>lin(l);
    QSqlQuery art(Configuracion_global->groupDB);
    QString cSQL;
    while(lin.hasNext())
    {
        lin.next();
        cSQL = QString("update articulos set unidades_reservadas = unidades_reservadas -%1 where id = %2").arg(
                    lin.value().value("cantidad").toFloat(),lin.value().value("id").toInt());
        success =art.exec(cSQL);
        success2 = SqlCalls::SqlDelete("lin_ped",Configuracion_global->empresaDB,QString("id = %1").arg(
                                           lin.value().value("id").toInt()),error2);
        if(!success || !success2)
            break;
    }

    if (success)
        return true;
    else
    {
//        if(!error.isEmpty())
//            QMessageBox::critical(qApp->activeWindow(), QObject::tr("Pedidos"),
//                              QObject::tr(QString("Se ha producido un error al borrar:%1").arg(error)),
//                              QObject::tr("Aceptar"));
//        if(!error2.isEmpty())
//            QMessageBox::critical(qApp->activeWindow(), QObject::tr("Pedidos"),
//                              QObject::tr(QString("Se ha producido un error al borrar:%1").arg(error2)),
//                              QObject::tr("Aceptar"));
        return false;
    }
}
int Pedidos::AnadirPedido()
{
    this->porc_iva1 = Configuracion_global->ivaList.at(0).toDouble();
    this->porc_iva2 = Configuracion_global->ivaList.at(1).toDouble();
    this->porc_iva3 = Configuracion_global->ivaList.at(2).toDouble();
    this->porc_iva4 = Configuracion_global->ivaList.at(3).toDouble();

    this->porc_rec1 = Configuracion_global->reList.at(0).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(1).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(2).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(3).toDouble();
    this->recargo_equivalencia = 0;

    QHash <QString,QVariant> ped;
    QString error;

     this->pedido = NuevoNumeroPedido();
     ped["pedido"] = this->pedido;
     ped["porc_iva1"] = this->porc_iva1;
     ped["porc_iva2"] = this->porc_iva2;
     ped["porc_iva3"] = this->porc_iva3;
     ped["porc_iva4"] = this->porc_iva4;
     ped["porc_rec1"] = this->porc_rec1;
     ped["porc_rec2"] = this->porc_rec1;
     ped["porc_rec3"] = this->porc_rec1;
     ped["porc_rec4"] = this->porc_rec1;
     ped["desc_gasto1"] = QObject::tr("Portes.");
     ped["id_cliente"] = this->id_cliente;
     ped["ejercicio"] = Configuracion_global->cEjercicio.toInt();
     int new_id = SqlCalls::SqlInsert(ped,"ped_cli",Configuracion_global->empresaDB,error);
     if(new_id <1)
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Pedido:", error);
         return -1;
     }
     else
     {
         this->id = new_id;
         return new_id;
     }
}
// Guardar el Pedido
bool Pedidos::
GuardarPedido(int nid_Pedido)
{
    QHash <QString,QVariant> ped_cli;

    ped_cli["albaran"]= albaran;
    ped_cli["pedido"] = pedido;
    ped_cli["id_divisa"] = id_divisa;
    ped_cli["fecha"] = fecha;
    ped_cli["pedido"] = pedido;
    ped_cli["id_cliente"] = id_cliente;
    ped_cli["id_transportista"] = id_transportista;
    ped_cli["codigo_cliente"] = codigo_cliente;
    ped_cli["cliente"] = cliente;
    ped_cli["direccion1"] = direccion1;
    ped_cli["direccion2"] = direccion2;
    ped_cli["poblacion"] = poblacion;
    ped_cli["provincia"] = provincia;
    ped_cli["cp"] = cp;
    ped_cli["id_pais"] = id_pais;
    ped_cli["cif"] = cif;
    ped_cli["recargo_equivalencia"] = recargo_equivalencia;
    ped_cli["subtotal"] = subtotal;
    ped_cli["dto"] = dto;
    ped_cli["dto_pp"] = dto_pp;
    ped_cli["porc_dto"] = porc_dto;
    ped_cli["porc_dto_pp"] = porc_dto_pp;
    ped_cli["base1"] = base1;
    ped_cli["base2"] = base2;
    ped_cli["base3"] = base3;
    ped_cli["base4"] = base4;
    ped_cli["porc_iva1"] = porc_iva1;
    ped_cli["porc_iva2"] = porc_iva2;
    ped_cli["porc_iva3"] = porc_iva3;
    ped_cli["porc_iva4"] = porc_iva4;
    ped_cli["iva1"] = iva1;
    ped_cli["iva2"] = iva2;
    ped_cli["iva3"] = iva3;
    ped_cli["iva4"] = iva4;
    ped_cli["porc_rec1"] = porc_rec1;
    ped_cli["porc_rec2"] = porc_rec2;
    ped_cli["porc_rec3"] = porc_rec3;
    ped_cli["porc_rec4"] = porc_rec4;
    ped_cli["rec1"] = rec1;
    ped_cli["rec2"] = rec2;
    ped_cli["rec3"] = rec3;
    ped_cli["rec4"] = rec4;
    ped_cli["total1"] = total1;
    ped_cli["total2"] = total2;
    ped_cli["total3"] = total3;
    ped_cli["total4"] = total4;
    ped_cli["base_total"] = base_total;
    ped_cli["iva_total"] = iva_total;
    ped_cli["rec_total"] = rec_total;
    ped_cli["total_albaran"] = total_albaran;
    ped_cli["impreso"] = impreso;
    ped_cli["facturado"] = facturado;
    ped_cli["id_forma_pago"] = id_forma_pago;
   // ped_cli["factura"] = factura;
    ped_cli["fecha_factura"] = fecha_factura;
    ped_cli["comentario"] = comentario;
    ped_cli["entregado_a_cuenta"] = entregado_a_cuenta;
    ped_cli["traspasado_albaran"] = traspasado_albaran;
    ped_cli["traspasado_factura"] = traspasado_factura;
    ped_cli["direccion_entrega1"] = direccion_entrega1;
    ped_cli["direccion_entrega2"] = direccion_entrega2;
    ped_cli["cp_entrega"] = cp_entrega;
    ped_cli["poblacion_entrega"] = poblacion_entrega;
    ped_cli["provincia_entrega"] = provincia_entrega;
    ped_cli["id_pais_entrega"] = id_pais_entrega;
    ped_cli["email_entrega"] = email_entrega;
    ped_cli["comentarios_entrega"] = comentarios_entrega;
    ped_cli["enviado"] = enviado;
    ped_cli["completo"] = completo;
    ped_cli["entregado"] = entregado;
    ped_cli["fecha_limite_entrega"] = fecha_limite_entrega;
    ped_cli["total_pedido"] = total_pedido;
    ped_cli["desc_gasto1"] = gasto1;
    ped_cli["desc_gasto2"] = gasto2;
    ped_cli["desc_gasto3"] = gasto3;
    ped_cli["imp_gasto1"] = imp_gasto1;
    ped_cli["imp_gasto2"] = imp_gasto2;
    ped_cli["imp_gasto3"] = imp_gasto3;
    ped_cli["porc_iva_gasto1"] = porc_iva_gasto1;
    ped_cli["porc_iva_gasto2"] = porc_iva_gasto2;
    ped_cli["porc_iva_gasto3"] = porc_iva_gasto3;
    ped_cli["iva_gasto1"] = iva_gasto1;
    ped_cli["iva_gasto2"] = iva_gasto2;
    ped_cli["iva_gasto3"] = iva_gasto3;
    ped_cli["id"] = nid_Pedido;

    QString error;
    bool succes = SqlCalls::SqlUpdate(ped_cli,"ped_cli",Configuracion_global->empresaDB,QString("id = %1").arg(id),error);

    if(!succes)
    {
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("error al guardar datos Pedido:"), error);
        return false;
    }
    else
    {      
        return true;
    }
}

bool Pedidos::RecuperarPedido(QString cSQL)
{
    QSqlQuery ped_cli(Configuracion_global->empresaDB);
    QString error;
    ped_cli.prepare(cSQL);
    if( !ped_cli.exec() )
    {
        QMessageBox::critical(qApp->activeWindow(), "Error:", ped_cli.lastError().text());
        return false;
    }
    else
    {
        if (ped_cli.next())
        {
            QSqlRecord r = ped_cli.record();
            id = r.value("id").toInt();
            albaran = r.value("albaran").toInt();
            pedido = r.value("pedido").toInt();
            id_divisa = r.value("id_divisa").toInt();
            id_tarifa = SqlCalls::SelectOneField("clientes","id_tarifa",QString("id=%1").arg(r.value("id_cliente").toInt()),
                                                 Configuracion_global->groupDB,error).toInt();
            fecha = r.value("fecha").toDate();
            editable = r.value("editable").toBool();
            id_cliente = r.value("id_cliente").toInt();
            id_transportista = r.value("id_transportista").toInt();
            codigo_cliente = r.value("codigo_cliente").toString();
            cliente = r.value("cliente").toString();
            direccion1 = r.value("direccion1").toString();
            direccion2 = r.value("direccion2").toString();
            poblacion = r.value("poblacion").toString();
            provincia = r.value("provincia").toString();
            cp = r.value("cp").toString();
            id_pais = r.value("id_pais").toInt();
            pais = Configuracion_global->Devolver_pais(id_pais);
            direccion_entrega1 = r.value("direccion_entrega1").toString();
            direccion_entrega2 = r.value("direccion_entrega2").toString();
            cp_entrega = r.value("cp_entrega").toString();
            poblacion_entrega = r.value("poblacion_entrega").toString();
            provincia_entrega = r.value("provincia_entrega").toString();
            id_pais_entrega = r.value("id_pais_entrega").toInt();
            pais_entrega = Configuracion_global->Devolver_pais(id_pais_entrega);
            email_entrega = r.value("email_entrega").toString();
            comentarios_entrega = r.value("comentarios_entrega").toString();
            cif = r.value("cif").toString();
            recargo_equivalencia = r.value("recargo_equivalencia").toInt();
            subtotal = r.value("subtotal").toDouble();
            porc_dto = r.value("porc_dto").toFloat();
            porc_dto_pp = r.value("porc_dto_pp").toFloat();
            dto = r.value("dto").toDouble();
            dto_pp = r.value("dto_pp").toDouble();
            base1 = r.value("base1").toDouble();
            base2 = r.value("base2").toDouble();
            base3 = r.value("base3").toDouble();
            base4 = r.value("base4").toDouble();
            porc_iva1 = r.value("porc_iva1").toDouble();
            porc_iva2 = r.value("porc_iva2").toDouble();
            porc_iva3 = r.value("porc_iva3").toDouble();
            porc_iva4 = r.value("porc_iva4").toDouble();
            iva1 = r.value("iva1").toDouble();
            iva2 = r.value("iva2").toDouble();
            iva3 = r.value("iva3").toDouble();
            iva4 = r.value("iva4").toDouble();
            porc_rec1 = r.value("porc_rec1").toDouble();
            porc_rec2 = r.value("porc_rec2").toDouble();
            porc_rec3 = r.value("porc_rec3").toDouble();
            porc_rec4 = r.value("porc_rec4").toDouble();
            rec1 = r.value("rec1").toDouble();
            rec2 = r.value("rec2").toDouble();
            rec3 = r.value("rec3").toDouble();
            rec4 = r.value("rec4").toDouble();
            total1 = r.value("total1").toDouble();
            total2 = r.value("total2").toDouble();
            total3 = r.value("total3").toDouble();
            total4 = r.value("total4").toDouble();
            base_total = r.value("base_total").toDouble();
            iva_total = r.value("iva_total").toDouble();
            rec_total = r.value("rec_total").toDouble();
            total_albaran = r.value("total_albaran").toDouble();
            impreso = r.value("impreso").toInt();
            facturado = r.value("facturado").toInt();
            factura = r.value("factura").toString();
            id_forma_pago = r.value("id_forma_pago").toInt();
            fecha_factura = r.value("fecha_factura").toDate();
            comentario = r.value("comentario").toString();
            entregado_a_cuenta = r.value("entregado_a_cuenta").toDouble();
            traspasado_albaran = r.value("traspasado_albaran").toInt();
            traspasado_factura = r.value("traspasado_factura").toInt();
            enviado = r.value("enviado").toInt();
            completo = r.value("completo").toInt();
            entregado = r.value("entregado").toInt();
            fecha_limite_entrega = r.value("fecha_limite_entrega").toDate();
            total_pedido = r.value("total_pedido").toDouble();
            gasto1 = r.value("desc_gasto1").toString();
            gasto2 = r.value("desc_gasto2").toString();
            gasto3 = r.value("desc_gasto3").toString();
            imp_gasto1 = r.value("imp_gasto1").toDouble();
            imp_gasto2 = r.value("imp_gasto2").toDouble();
            imp_gasto3 = r.value("imp_gasto3").toDouble();
            porc_iva_gasto1 = r.value("porc_iva_gasto1").toFloat();
            porc_iva_gasto2 = r.value("porc_iva_gasto2").toFloat();
            porc_iva_gasto3 = r.value("porc_iva_gasto3").toFloat();
            iva_gasto1 = r.value("iva_gasto1").toDouble();
            iva_gasto2 = r.value("iva_gasto2").toDouble();
            iva_gasto3 = r.value("iva_gasto3").toDouble();
            this->id_pais = r.field("id_pais").value().toInt();
            pais = Configuracion_global->Devolver_pais(id_pais);
            QSqlQuery queryCliente(Configuracion_global->groupDB);
            if(queryCliente.exec("select tarifa_cliente from clientes where id = "+QString::number(this->id_cliente)));
                if (queryCliente.next())
                    tarifa_cliente = queryCliente.record().value("tarifa_cliente").toInt();


           }
        else
            return false;
    }
    return true;
}

int Pedidos::NuevoNumeroPedido()
{
    QSqlQuery ped_cli(Configuracion_global->empresaDB);
    int pedido = 0;
    ped_cli.prepare("Select pedido from ped_cli order by pedido desc limit 1");
    if(ped_cli.exec())
    {
        if(ped_cli.next())
        {
            pedido= ped_cli.value(0).toInt();
            pedido ++;
        }
    }
    return qMax(1,pedido);
}


