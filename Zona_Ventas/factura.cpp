#include "factura.h"
#include "../Zona_Contabilidad/apuntes.h"
#include "../Zona_Pacientes/cliente.h"

Factura::Factura(QObject *parent) :
    QObject(parent)
{
    this->id =0;
}

bool Factura::set_impresa(bool state)
{
    QSqlQuery q_cab_fac(Configuracion_global->empresaDB);
    q_cab_fac.prepare("update cab_fac set impreso = :state where id = :id");
    q_cab_fac.bindValue(":state",state);
    q_cab_fac.bindValue(":id",this->id);
    if(!q_cab_fac.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Impresión de facturas"),
                             tr("Ocurrió un error al guardar estado de impresión: %1").arg(q_cab_fac.lastError().text()),
                             tr("Aceptar"));
        return false;
    } else
    {
        return true;
    }
}

// Metodos utilidad Clase
bool Factura::AnadirFactura()
{
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    this->porc_iva1 = Configuracion_global->ivaList.at(0).toFloat();
    this->porc_iva2 = Configuracion_global->ivaList.at(1).toFloat();
    this->porc_iva3 = Configuracion_global->ivaList.at(2).toFloat();
    this->porc_iva4 = Configuracion_global->ivaList.at(3).toFloat();
    this->porc_rec1 = Configuracion_global->reList.at(0).toFloat();
    this->porc_rec2 = Configuracion_global->reList.at(1).toFloat();
    this->porc_rec3 = Configuracion_global->reList.at(2).toFloat();
    this->porc_rec4 = Configuracion_global->reList.at(3).toFloat();

    this->recargo_equivalencia = 0;
    QHash <QString, QVariant> cab_fac;
    QString error;
    cab_fac["factura"] = "BORRADOR";
    cab_fac["fecha"] = this->fecha;
    cab_fac["ejercicio"] = Configuracion_global->cEjercicio.toInt();
    cab_fac["editable"] = true;
    int new_id = SqlCalls::SqlInsert(cab_fac,"cab_fac",Configuracion_global->empresaDB,error);

    if(new_id > -1)
    {
        QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Factura:", error);
        return false;
    }
    else
    {
        this->id = new_id;
        QString cSQL = "Select * from cab_fac where id ="+QString::number(this->id);
        RecuperarFactura(cSQL);
        return true;
    }
}
// Guardar la factura
bool Factura::GuardarFactura(int nid_factura, bool FacturaLegal)
{
    bool succes = true;
    QHash <QString,QVariant> cab_fac;
    QString error;
    QString clausula = QString("id= %1").arg(nid_factura);
    //---------------------------------
    // Pasamos valores reales al QHash
    //---------------------------------
    cab_fac["codigo_cliente"] = this->codigo_cliente;
    cab_fac["factura"] = this->factura;
    cab_fac["serie"] = this->serie;
    cab_fac["fecha"] = this->fecha;
    cab_fac["fecha_cobro"] = this->fecha_cobro;
    cab_fac["id_cliente"] = this->id_cliente;
    cab_fac["cliente"] = this->cliente;
    cab_fac["direccion1"] = this->direccion1;
    cab_fac["direccion2"] = this->direccion2;
    cab_fac["cp"] = this->cp;
    cab_fac["poblacion"] = this->poblacion;
    cab_fac["provincia"] = this->provincia;
    cab_fac["id_pais"] = this->id_pais;
    cab_fac["direccion1_entrega"] = this->direccion1_entrega;
    cab_fac["direccion2_entrega"] = this->direccion2_entrega;
    cab_fac["cp_entrega"] = this->cp_entrega;
    cab_fac["poblacion_entrega"] = this->poblacion_entrega;
    cab_fac["provincia_entrega"] = this->provincia_entrega;
    cab_fac["id_pais_entrega"] = this->id_pais_entrega;
    cab_fac["cif"] = this->cif;
    cab_fac["recargo_equivalencia"] = this->recargo_equivalencia;
    cab_fac["subtotal"] = this->subtotal;
    cab_fac["porc_dto"] = this->porc_dto;
    cab_fac["porc_dto_pp"] = this->porc_dto_pp;
    cab_fac["dto"] = this->dto;
    cab_fac["dto_pp"] = this->dto_pp;
    cab_fac["base"] = this->base;
    cab_fac["iva"] = this->iva;
    cab_fac["total"] = this->total;
    cab_fac["impreso"] = this->impreso;
    cab_fac["cobrado"] = this->cobrado;
    cab_fac["contabilizado"] = this->contablilizada;
    cab_fac["id_forma_pago"] = this->id_forma_pago;
    cab_fac["forma_pago"] = this->forma_pago;
    cab_fac["comentario"] = this->comentario;
    cab_fac["base1"] = this->base1;
    cab_fac["base2"] = this->base2;
    cab_fac["base3"] = this->base3;
    cab_fac["base4"] = this->base4;
    cab_fac["porc_iva1"] = this->porc_iva1;
    cab_fac["porc_iva2"] = this->porc_iva2;
    cab_fac["porc_iva3"] = this->porc_iva3;
    cab_fac["porc_iva4"] = this->porc_iva4;
    cab_fac["iva1"] = this->iva1;
    cab_fac["iva2"] = this->iva2;
    cab_fac["iva3"] = this->iva3;
    cab_fac["iva4"] = this->iva4;
    cab_fac["total1"] = this->total1;
    cab_fac["total2"] = this->total2;
    cab_fac["total3"] = this->total3;
    cab_fac["total4"] = this->total4;
    cab_fac["porc_rec1"] = this->porc_rec1;
    cab_fac["porc_rec2"] = this->porc_rec2;
    cab_fac["porc_rec3"] = this->porc_rec3;
    cab_fac["porc_rec4"] = this->porc_rec4;
    cab_fac["rec1"] = this->rec1;
    cab_fac["rec2"] = this->rec2;
    cab_fac["rec3"] = this->rec3;
    cab_fac["rec4"] = this->rec4;
    cab_fac["total_recargo"] = this->total_recargo;
    cab_fac["entregado_a_cuenta"] = this->entregado_a_cuenta;
    cab_fac["importe_pendiente"] = this->importe_pendiente;
    cab_fac["codigo_entidad"] = this->codigo_entidad;
    cab_fac["oficina_entidad"] = this->oficina_entidad;
    cab_fac["dc_cuenta"] = this->dc_cuenta;
    cab_fac["cuenta_corriente"] = this->cuenta_corriente;
    cab_fac["pedido_cliente"] = this->pedido_cliente;
    cab_fac["irpf"] = this->irpf;
    cab_fac["irpf"] = this->irpf;
    cab_fac["id_transportista"] = this->id_transportista;
    cab_fac["desc_gasto1"] = this->desc_gasto1;
    cab_fac["desc_gasto2"] = this->desc_gasto2;
    cab_fac["desc_gasto3"] = this->desc_gasto3;
    cab_fac["imp_gasto1"] = this->imp_gasto1;
    cab_fac["imp_gasto2"] = this->imp_gasto2;
    cab_fac["imp_gasto3"] = this->imp_gasto3;
    cab_fac["porc_iva_gasto1"] = this->porc_iva_gasto1;
    cab_fac["porc_iva_gasto2"] = this->porc_iva_gasto2;
    cab_fac["porc_iva_gasto3"] = this->porc_iva_gasto3;
    cab_fac["iva_gasto1"] = this->iva_gasto1;
    cab_fac["iva_gasto2"] = this->iva_gasto2;
    cab_fac["iva_gasto3"] = this->iva_gasto3;
    cab_fac["editable"] = this->editable;

    bool updated = SqlCalls::SqlUpdate(cab_fac,"cab_fac",Configuracion_global->empresaDB,clausula,error);

    if(!updated)
    {
        QMessageBox::critical(qApp->activeWindow(),tr("error al guardar datos Factura:"), error);
        succes =  false;
    }
    else
    {
       // TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("La Factura se ha guardado corectamente:"));
        QString cSQL = "Select * from cab_fac where id ="+QString::number(nid_factura);
        RecuperarFactura(cSQL);
        if (FacturaLegal)
        {
            // Busco ficha cliente
            QSqlQuery Cliente(Configuracion_global->empresaDB);
            Cliente.prepare("Select * from clientes where codigo_cliente = :codigo_cliente");
            Cliente.bindValue(":codigo_cliente",this->codigo_cliente);
            if (Cliente.exec())
            {
                Cliente.next();
                QSqlRecord record = Cliente.record();

                // Genero deuda cliente

                QSqlQuery Deudacliente(Configuracion_global->empresaDB);
                Deudacliente.prepare("Insert into clientes_deuda (id_cliente,fecha,vencimiento,documento,id_ticket,id_factura,tipo,"
                                     "importe,pagado,pendiente_cobro,entidad,oficina,dc,cuenta)"
                                     " values (:id_cliente,:fecha,:vencimiento,:documento,:id_tiquet,:id_factura,:tipo,"
                                     ":importe,:pagado,:pendiente_cobro,:entidad,:oficina,:dc,:cuenta)");
                Deudacliente.bindValue(":id_cliente",record.field("id").value().toInt());
                Deudacliente.bindValue(":fecha",QDate::currentDate());
                Deudacliente.bindValue(":vencimiento",QDate::currentDate());
                // TODO Deudacliente->bindValue(":vencimiento",Configuracion_global->CalcularVencimiento());
                Deudacliente.bindValue(":documento",this->factura);
                Deudacliente.bindValue(":id_tiquet",0);
                Deudacliente.bindValue(":id_factura",nid_factura);
                Deudacliente.bindValue("tipo",1);
                Deudacliente.bindValue(":importe",this->total);
                Deudacliente.bindValue(":pagado",0);
                Deudacliente.bindValue(":pendiente_cobro",this->total);
                Deudacliente.bindValue(":entidad",record.field("entidad_bancaria").value().toString());
                Deudacliente.bindValue(":oficina",record.field("oficina_bancaria").value().toString());
                Deudacliente.bindValue(":dc",record.field("dc").value().toString());
                Deudacliente.bindValue(":cuenta",record.field("cuenta_corriente").value().toString());
                if(!Deudacliente.exec())
                {
                    qDebug() << Deudacliente.lastQuery();
                    QMessageBox::warning(qApp->activeWindow(),tr("Añadir deuda"),tr("No se ha podido añadir la deuda ")+Deudacliente.lastError().text() ,tr("OK"));
                    succes = false;
                }
                else
                {
                    TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Deuda añadida corectamente:"));
                    // Añadimos acumulados ficha cliente.
                    Cliente.prepare("Update clientes set fecha_ultima_compra = :fecha_ultima_compra, "
                                    "acumulado_ventas = acumulado_ventas + :acumulado_ventas,"
                                    "ventas_ejercicio = ventas_ejercicio + :ventas_ejercicio,"
                                    "deuda_actual = deuda_actual + :deuda_actual "
                                    " where id = :id_cliente");
                    Cliente.bindValue(":fecha_ultima_compra",QDate::currentDate());
                    Cliente.bindValue(":acumulado_ventas",this->total);
                    Cliente.bindValue(":ventas_ejercicio",this->total);
                    Cliente.bindValue(":deuda_actual",this->total);
                    Cliente.bindValue(":id_cliente",record.field("id").value().toInt());

                    if (!Cliente.exec())
                    {
                        succes =  false;
                        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                                             tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                                             tr("OK"));
                    }
                }
            }
                else
                    succes = false;
            }

    }
    return succes;
}

bool Factura::RecuperarFactura(QString cSQL){
        QSqlQuery cab_fac(Configuracion_global->empresaDB);
        cab_fac.prepare(cSQL);
        if( !cab_fac.exec() )
        {
            QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
            return false;
        }
        else
        {
            if (cab_fac.next())
            {
                QSqlRecord registro = cab_fac.record();
                cargar(&registro);
                return true;
               }
            else
            {
                return false;
            }
        }
}

bool Factura::RecuperarFactura(int id)
{
    QSqlQuery cab_fac(Configuracion_global->empresaDB);
    if( !cab_fac.exec("select * from cab_fac where id ="+QString::number(id)))
    {
        QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
        return false;
    }
    else
    {
        if (cab_fac.next())
        {
            QSqlRecord registro = cab_fac.record();
            cargar(&registro);
            return true;
           }
        else
        {
            return false;
        }
    }

}

void Factura::cargar(QSqlRecord *registro)
{
    this->id = registro->field("id").value().toInt();
    this->codigo_cliente= registro->field("codigo_cliente").value().toString();
    this->factura = registro->field("factura").value().toString();
    this->serie = registro->field("serie").value().toString();
    this->fecha = registro->field("fecha").value().toDate();
    this->fecha_cobro = registro->field("fecha_cobro").value().toDate();
    this->ejercicio = registro->field("ejercicio").value().toInt();
    this->id_cliente = registro->field("id_cliente").value().toInt();
    this->cliente = registro->field("cliente").value().toString();
    this->direccion1 = registro->field("direccion1").value().toString();
    this->direccion2 = registro->field("direccion2").value().toString();
    this->cp = registro->field("cp").value().toString();
    this->poblacion = registro->field("poblacion").value().toString();
    this->provincia = registro->field("provincia").value().toString();
    this->id_pais = registro->field("id_pais").value().toInt();
    this->direccion1_entrega = registro->field("direccion1_entrega").value().toString();
    this->direccion2_entrega = registro->field("direccion2_entrega").value().toString();
    this->cp_entrega = registro->field("cp_entrega").value().toString();
    this->poblacion_entrega = registro->field("poblacion_entrega").value().toString();
    this->provincia_entrega = registro->field("provincia_entrega").value().toString();
    this->id_pais_entrega = registro->field("id_pais_entrega").value().toInt();
    this->cif =registro->field("cif").value().toString();
    this->recargo_equivalencia = registro->field("recargo_equivalencia").value().toBool();
    this->subtotal = registro->field("subtotal").value().toDouble();
    this->porc_dto = registro->field("porc_dto").value().toFloat();
    this->porc_dto_pp = registro->field("porc_dto_pp").value().toFloat();
    this->dto = registro->field("dto").value().toDouble();
    this->dto_pp = registro->field("dto_pp").value().toDouble();
    this->base = registro->field("base").value().toDouble();
    this->iva = registro->field("iva").value().toDouble();
    this->total = registro->field("total").value().toDouble();
    this->impreso = registro->field("impreso").value().toBool();
    this->cobrado = registro->field("cobrado").value().toBool();
    this->contablilizada = registro->field("contabilizado").value().toBool();
    this->id_forma_pago = registro->field("id_forma_pago").value().toInt();
    this->forma_pago = registro->field("forma_pago").value().toString();
    this->comentario = registro->field("comentario").value().toString();
    this->base1 = registro->field("base1").value().toDouble();
    this->base2 = registro->field("base2").value().toDouble();
    this->base3 = registro->field("base3").value().toDouble();
    this->base4 = registro->field("base4").value().toDouble();
    this->porc_iva1 = registro->field("porc_iva1").value().toInt();
    this->porc_iva2 = registro->field("porc_iva2").value().toInt();
    this->porc_iva3 = registro->field("porc_iva3").value().toInt();
    this->porc_iva4 = registro->field("porc_iva4").value().toInt();
    this->iva1 = registro->field("iva1").value().toDouble();
    this->iva2 = registro->field("iva2").value().toDouble();
    this->iva3 = registro->field("iva3").value().toDouble();
    this->iva4 = registro->field("iva4").value().toDouble();
    this->total1 = registro->field("total1").value().toDouble();
    this->total2 = registro->field("total2").value().toDouble();
    this->total3 = registro->field("total3").value().toDouble();
    this->total4 = registro->field("total4").value().toDouble();
    this->porc_rec1 = registro->field("porc_rec1").value().toDouble();
    this->porc_rec2 = registro->field("porc_rec2").value().toDouble();
    this->porc_rec3 = registro->field("porc_rec3").value().toDouble();
    this->porc_rec4 = registro->field("porc_rec4").value().toDouble();
    this->rec1 = registro->field("rec1").value().toDouble();
    this->rec2 = registro->field("rec2").value().toDouble();
    this->rec3 = registro->field("rec3").value().toDouble();
    this->rec4 = registro->field("rec4").value().toDouble();
    this->total_recargo = registro->field("total_recargo").value().toDouble();
    this->entregado_a_cuenta = registro->field("entregado_a_cuenta").value().toDouble();
    this->importe_pendiente = registro->field("importe_pendiente").value().toDouble();
    this->codigo_entidad = registro->field("codigo_entidad").value().toString();
    this->oficina_entidad = registro->field("oficina_entidad").value().toString();
    this->dc_cuenta = registro->field("dc_cuenta").value().toString();
    this->cuenta_corriente = registro->field("cuenta_corriente").value().toString();
    this->pedido_cliente = registro->field("pedido_cliente").value().toInt();
    this->porc_irpf = registro->field("porc_irpf").value().toInt();
    this->irpf = registro->field("irpf").value().toDouble();
    this->apunte = registro->field("asiento").value().toInt();
    this->id_transportista = registro->field("id_transportista").value().toInt();
    this->desc_gasto1 = registro->field("desc_gasto1").value().toString();
    this->desc_gasto2 = registro->field("desc_gasto2").value().toString();
    this->desc_gasto3 = registro->field("desc_gasto3").value().toString();

    this->imp_gasto1 = registro->field("imp_gasto1").value().toDouble();
    this->imp_gasto2 = registro->field("imp_gasto2").value().toDouble();
    this->imp_gasto3 = registro->field("imp_gasto3").value().toDouble();
    this->porc_iva_gasto1 = registro->field("porc_iva_gasto1").value().toDouble();
    this->porc_iva_gasto2 = registro->field("porc_iva_gasto2").value().toDouble();
    this->porc_iva_gasto3 = registro->field("porc_iva_gasto3").value().toDouble();
    this->iva_gasto1 = registro->field("iva_gasto1").value().toDouble();
    this->iva_gasto2 = registro->field("iva_gasto2").value().toDouble();
    this->iva_gasto3 = registro->field("iva_gasto3").value().toDouble();
    this->editable = registro->field("editable").value().toBool();

}

QString Factura::NuevoNumeroFactura(QString serie) {
    QSqlQuery cab_fac(Configuracion_global->empresaDB);
    QString cNum;
    QString cNumFac;
    int inum;

    cab_fac.prepare("Select factura from cab_fac  where factura <> '"+QObject::tr("BORRADOR")+
                    "' and serie ='"+serie+"' order by factura desc limit 1");
    if(cab_fac.exec()) {
        cab_fac.next();
        cNumFac = cab_fac.value(0).toString();
        cNum = cNumFac.right(Configuracion_global->ndigitos_factura);

        inum = cNum.toInt();
        inum++;
        cNum = cNum.number(inum);
        while (cNum.length()< Configuracion_global->ndigitos_factura) {
            cNum.prepend("0");
        }
    } else {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
    }
    cNumFac = cNum;
    return cNumFac;
}

bool Factura::BorrarLineasFactura(int id_lin)
{
    QSqlQuery query(Configuracion_global->empresaDB);
        QString sql = QString("DELETE FROM lin_fac WHERE id_Cab = %1").arg(id_lin);
        query.prepare(sql);
        if(query.exec())
            return true;
        else
        {
            QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
            return false;
        }
}

bool Factura::CobrarFactura()
{

    // marcar factura como cobrada
    this->cobrado = true;
    QSqlQuery Cliente(Configuracion_global->groupDB);

    // Añadimos acumulados ficha cliente.
    Cliente.prepare("Update clientes set fecha_ultima_compra = :fecha_ultima_compra, "
                             "acumulado_ventas = acumulado_ventas + :acumulado_ventas,"
                             "ventas_ejercicio = ventas_ejercicio + :ventas_ejercicio "
                             " where codigo_cliente = :codigo_cliente");
    Cliente.bindValue(":fecha_ultima_compra",QDate::currentDate());
    Cliente.bindValue(":acumulado_ventas",this->total);
    Cliente.bindValue(":ventas_ejercicio",this->total);
    Cliente.bindValue(":codigo_cliente",this->codigo_cliente);
    if (!Cliente.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                             tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                             tr("OK"));
        return false;
    }
    QSqlQuery cab_fac(Configuracion_global->empresaDB);
    cab_fac.prepare("update cab_fac set cobrado = 1 where id =:id_cab");
    cab_fac.bindValue(":id_cab",this->id);
    if(!cab_fac.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Guardar Factura"),tr("No se ha podido marcar la factura como cobrada"),tr("OK"));
        return false;
    }
    return true;
}

bool Factura::GuardarApunte(int nasiento, int nid)
{
    QSqlQuery factura(Configuracion_global->empresaDB);

    factura.prepare("Update cab_fac set asiento =:asiento, contabilizado = 1 where id =:id ");
    factura.bindValue(":asiento",nasiento);
    factura.bindValue(":id",nid);
    if (!factura.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("facturas clientes"),
                             tr("No se ha podido guardar el numero de apunte"),
                             tr("OK"));
        return false;
    }

    else
    {
        this->apunte = nasiento;
        return true;
    }
}

bool Factura::Apunte()
{
    {
        apuntes oApunte;
        Cliente oCliente;
        int apunte;
        bool ok = true;
        bool nuevo;
        int pos = 1;
        oCliente.Recuperar("select * from clientes where id = " + QString::number(this->id_cliente));

        oApunte.cuenta_d = oCliente.codigo_cliente;
        oApunte.cuenta_relacion = oCliente.codigo_cliente;
        oApunte.cuenta_h = Configuracion_global->cuenta_venta_mercaderias;
        oApunte.descripcion_d = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_d);
        oApunte.id_cuenta_d = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_d);
        oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
        oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
        oApunte.comentario_d = tr("Nuestra factura num: ")+this->factura;
        oApunte.comentario_h = tr("Ventas mercaderias factura num: ")+this->factura;
        oApunte.fecha_asiento = this->fecha;
        oApunte.id_documento = this->id;
        oApunte.importe_d = this->total;
        oApunte.importe_h = this->base;
        oApunte.pos_en_asiento = pos;
        oApunte.asiento = oApunte.nuevo_numero_apunte();
        apunte = oApunte.asiento;
        nuevo = oApunte.nuevalinea();
        if(!nuevo)
            ok = false;
        pos++;
        if(this->iva1 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado1;
            oApunte.importe_h = this->iva1;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("IVA ")+Configuracion_global->ivaList.at(0)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->rec1 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado_re1;
            oApunte.importe_h = this->porc_rec1;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("RE ")+Configuracion_global->reList.at(0)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->iva2 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado2;
            oApunte.importe_h = this->iva2;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("IVA ")+Configuracion_global->ivaList.at(1)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo =oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->rec2 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado_re2;
            oApunte.importe_h = this->porc_rec2;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("RE ")+Configuracion_global->reList.at(1)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->iva3 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado3;
            oApunte.importe_h = this->iva3;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("IVA ")+Configuracion_global->ivaList.at(2)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->rec3 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado_re3;
            oApunte.importe_h = this->porc_rec3;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("RE ")+Configuracion_global->reList.at(2)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->iva4 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado4;
            oApunte.importe_h = this->iva4;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("IVA ")+Configuracion_global->ivaList.at(3)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        if(this->rec4 !=0)
        {
            oApunte.clear();
            oApunte.asiento = apunte;
            oApunte.cuenta_h = Configuracion_global->cuenta_iva_soportado_re4;
            oApunte.importe_h = this->porc_rec4;
            oApunte.descripcion_h = Configuracion_global->Devolver_descripcion_cuenta_contable(oApunte.cuenta_h);
            oApunte.id_cuenta_h = Configuracion_global->Devolver_id_cuenta_contable(oApunte.cuenta_h);
            oApunte.comentario_h = tr("RE ")+Configuracion_global->reList.at(3)+tr("% - factura num: ")+this->factura;
            oApunte.pos_en_asiento = pos;
            pos ++;
            nuevo = oApunte.nuevalinea();
            if(!nuevo)
                ok = false;
        }
        ok = this->GuardarApunte(apunte,this->id);
        return ok;
    }
}

bool Factura::EditApunte(int num_apunte)
{

 // TODO: editar apunte al editar factura
        Cliente oCliente;
        oCliente.Recuperar("select * from clientes where id = " + QString::number(this->id_cliente));
        QString error;
        bool b_error = false;
        QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("diario", "asiento = "+QString::number(num_apunte),Configuracion_global->contaDB, error);
        QMapIterator<int, QSqlRecord> i(map);
        while (i.hasNext())
        {
            i.next();
            if(i.value().value("cuenta_d").toString() == oCliente.codigo_cliente)
            {
                QHash <QString,QVariant> update;
                update["importe_d"] = this->total;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                } if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_venta_mercaderias)
                {
                    QHash <QString,QVariant> update;
                    update["importe_h"] = this->base;
                    SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                    if(!error.isEmpty())
                    {
                        qDebug() << error;
                        b_error = true;
                    }
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_venta_mercaderias)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->base;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_venta_servicios)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->base;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_venta_mercaderias)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->base;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado1)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->iva1;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado2)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->iva2;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado3)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->iva3;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado4)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->iva4;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado_re1)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->rec1;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado_re2)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->rec2;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado_re3)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->rec3;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            } else if(i.value().value("cuenta_h").toString() == Configuracion_global->cuenta_iva_soportado_re4)
            {
                QHash <QString,QVariant> update;
                update["importe_h"] = this->rec4;
                SqlCalls::SqlUpdate(update,"diario",Configuracion_global->contaDB,"id="+QString::number(i.value().value("id").toInt()),error);
                if(!error.isEmpty())
                {
                    qDebug() << error;
                    b_error = true;
                }
            }

        }
        return !b_error;
}
