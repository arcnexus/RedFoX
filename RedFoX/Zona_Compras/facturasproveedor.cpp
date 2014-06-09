#include "facturasproveedor.h"

FacturasProveedor::FacturasProveedor(QObject *parent) :
    QObject(parent)
{
    id = 0;
}
bool FacturasProveedor::anadir_factura()
{
    QHash<QString,QVariant> _data;
    _data["fecha"] = QDate::currentDate();
    _data["porc_iva1"] = Configuracion_global->ivaList.at(0);
    _data["porc_iva2"] = Configuracion_global->ivaList.at(1);
    _data["porc_iva3"] = Configuracion_global->ivaList.at(2);
    _data["porc_iva4"] = Configuracion_global->ivaList.at(3);
    _data["porc_rec1"] = Configuracion_global->reList.at(0);
    _data["porc_rec2"] = Configuracion_global->reList.at(1);
    _data["porc_rec3"] = Configuracion_global->reList.at(2);
    _data["porc_rec4"] = Configuracion_global->reList.at(3);

    QString error;
    int _id = SqlCalls::SqlInsert(_data,"fac_pro",Configuracion_global->empresaDB,error);

    if(_id != -1)
    {
        this->id = _id;
        return true;
    }
    else
    {       
        QMessageBox::critical(qApp->activeWindow(),tr("No se pudo crear la nueva factura"),error);
        return false;
    }
}

bool FacturasProveedor::guardar_factura()
{
    QHash<QString,QVariant> _data;
    _data["factura"] = factura;
    _data["fecha"] = fecha;
    _data["fecha_recepcion"] = fecha_recepcion;
    _data["pedido"] = pedido;
    _data["id_proveedor"] = id_proveedor;
    _data["proveedor"] = proveedor;
    _data["cif_proveedor"] = cif_proveedor;
    _data["direccion1"] = direccion1;
    _data["direccion2"] = direccion2;
    _data["cp"] = cp;
    _data["poblacion"] = poblacion;
    _data["provincia"] = provincia;
    _data["id_pais"] = id_pais;
    _data["telefono"] = telefono;
    _data["movil"] = movil;
    _data["fax"] = fax;
    _data["porc_dto"] = porc_dto;
    _data["porc_irpf"] = porc_irpf;
    _data["recargo_equivalencia"] = recargo_equivalencia;
    _data["base1"] = base1;
    _data["base2"] = base2;
    _data["base3"] = base3;
    _data["base4"] = base4;
    _data["porc_iva1"] = porc_iva1;
    _data["porc_iva2"] = porc_iva2;
    _data["porc_iva3"] = porc_iva3;
    _data["porc_iva4"] = porc_iva4;
    _data["iva1"] = iva1;
    _data["iva2"] = iva2;
    _data["iva3"] = iva3;
    _data["iva4"] = iva4;
    _data["porc_rec1"] = porc_rec1;
    _data["porc_rec2"] = porc_rec2;
    _data["porc_rec3"] = porc_rec3;
    _data["porc_rec4"] = porc_rec4;
    _data["rec1"] = rec1;
    _data["rec2"] = rec2;
    _data["rec3"] = rec3;
    _data["rec4"] = rec4;
    _data["total1"] = total1;
    _data["total2"] = total2;
    _data["total3"] = total3;
    _data["total4"] = total4;
    _data["base_total"] = base_total;
    _data["iva_total"] = iva_total;
    _data["irpf"] = irpf;
    _data["total_recargo"] = total_recargo;
    _data["total"] = total;
    _data["id_forma_pago"] = id_forma_pago;
    _data["id_tipo_gasto"] = id_tipo_gasto;
    _data["comentario"] = comentario;
    _data["pagado"] = pagado;
    _data["albaran"] = albaran;
    _data["desc_gasto1"] = desc_gasto1;
    _data["desc_gasto2"] = desc_gasto2;
    _data["desc_gasto3"] = desc_gasto3;
    _data["imp_gasto1"] = imp_gasto1;
    _data["imp_gasto2"] = imp_gasto2;
    _data["imp_gasto3"] = imp_gasto3;
    _data["gasto_to_coste"] = gasto_to_coste;
    _data["porc_iva_gasto1"] = porc_iva_gasto1;
    _data["porc_iva_gasto2"] = porc_iva_gasto2;
    _data["porc_iva_gasto3"] = porc_iva_gasto3;
    _data["iva_gasto1"] = iva_gasto1;
    _data["iva_gasto2"] = iva_gasto2;
    _data["iva_gasto3"] = iva_gasto3;
    _data["dto"] = dto;
    _data["ejercicio"] = ejercicio;
    _data["contabilizada"] = contabilizada;
    _data["subtotal"] = subtotal;
    _data["codigo_proveedor"] = codigo_proveedor;
    _data["impreso"] = impreso;
    _data["editable"] = editable;

    QString error;
    if(SqlCalls::SqlUpdate(_data,"fac_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id),error))
    {
        return true;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Falló la modificación de la factura: %1"),error);
        return false;
    }
}

bool FacturasProveedor::recuperar_factura(QString cSQL, int accion)
{
    QSqlQuery queryFactura(Configuracion_global->empresaDB);
    if(queryFactura.exec(cSQL))
    {
        return cargar_factura(queryFactura,accion);
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
        return false;
    }
}

bool FacturasProveedor::Recuperar(int id_factura)
{
    QSqlQuery queryFactura(Configuracion_global->empresaDB);
    queryFactura.prepare("select * from fac_pro where id = :id");
    queryFactura.bindValue(":id",id_factura);
    if(queryFactura.exec())
    {
        return cargar_factura(queryFactura,0);
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
        return false;
    }
}

bool FacturasProveedor::cargar_factura(QSqlQuery queryFact,int accion)
{
  // accion == 0 - nada : 1 - Anterior : 2 - Siguiente
    if(queryFact.next())
    {
        id = queryFact.record().value("id").toInt();
        factura = queryFact.record().value("factura").toString();
        fecha = queryFact.record().value("fecha").toDate();
        fecha_recepcion = queryFact.record().value("fecha_recepcion").toDate();
        pedido = queryFact.record().value("pedido").toString();
        id_proveedor = queryFact.record().value("id_proveedor").toInt();
        proveedor = queryFact.record().value("proveedor").toString();
        cif_proveedor = queryFact.record().value("cif_proveedor").toString();
        direccion1 = queryFact.record().value("direccion1").toString();
        direccion2 = queryFact.record().value("direccion2").toString();
        cp = queryFact.record().value("cp").toString();
        poblacion = queryFact.record().value("poblacion").toString();
        provincia = queryFact.record().value("provincia").toString();
        id_pais = queryFact.record().value("id_pais").toInt();
        telefono = queryFact.record().value("telefono").toString();
        movil = queryFact.record().value("movil").toString();
        fax = queryFact.record().value("fax").toString();
        porc_dto = queryFact.record().value("porc_dto").toDouble();
        porc_irpf = queryFact.record().value("porc_irpf").toDouble();
        recargo_equivalencia = queryFact.record().value("recargo_equivalencia").toBool();
        base1 = queryFact.record().value("base1").toDouble();
        base2 = queryFact.record().value("base2").toDouble();
        base3 = queryFact.record().value("base3").toDouble();
        base4 = queryFact.record().value("base4").toDouble();
        porc_iva1 = queryFact.record().value("porc_iva1").toDouble();
        porc_iva2 = queryFact.record().value("porc_iva2").toDouble();
        porc_iva3 = queryFact.record().value("porc_iva3").toDouble();
        porc_iva4 = queryFact.record().value("porc_iva4").toDouble();
        iva1 = queryFact.record().value("iva1").toDouble();
        iva2 = queryFact.record().value("iva2").toDouble();
        iva3 = queryFact.record().value("iva3").toDouble();
        iva4 = queryFact.record().value("iva4").toDouble();
        porc_rec1 = queryFact.record().value("porc_rec1").toDouble();
        porc_rec2 = queryFact.record().value("porc_rec2").toDouble();
        porc_rec3 = queryFact.record().value("porc_rec3").toDouble();
        porc_rec4 = queryFact.record().value("porc_rec4").toDouble();
        rec1 = queryFact.record().value("rec1").toDouble();
        rec2 = queryFact.record().value("rec2").toDouble();
        rec3 = queryFact.record().value("rec3").toDouble();
        rec4 = queryFact.record().value("rec4").toDouble();
        total1 = queryFact.record().value("total1").toDouble();
        total2 = queryFact.record().value("total2").toDouble();
        total3 = queryFact.record().value("total3").toDouble();
        total4 = queryFact.record().value("total4").toDouble();
        base_total = queryFact.record().value("base_total").toDouble();
        iva_total = queryFact.record().value("iva_total").toDouble();
        irpf = queryFact.record().value("irpf").toDouble();
        total_recargo = queryFact.record().value("total_recargo").toDouble();
        total = queryFact.record().value("total").toDouble();
        id_forma_pago = queryFact.record().value("id_forma_pago").toDouble();
        id_tipo_gasto = queryFact.record().value("id_tipo_gasto").toDouble();
        comentario = queryFact.record().value("comentario").toString();
        pagado = queryFact.record().value("pagado").toDouble();
        albaran = queryFact.record().value("albaran").toString();
        desc_gasto1 = queryFact.record().value("desc_gasto1").toString();
        desc_gasto2 = queryFact.record().value("desc_gasto2").toString();
        desc_gasto3 = queryFact.record().value("desc_gasto3").toString();
        imp_gasto1 = queryFact.record().value("imp_gasto1").toDouble();
        imp_gasto2 = queryFact.record().value("imp_gasto2").toDouble();
        imp_gasto3 = queryFact.record().value("imp_gasto3").toDouble();
        gasto_to_coste = queryFact.record().value("gasto_to_coste").toDouble();
        porc_iva_gasto1 = queryFact.record().value("porc_iva_gasto1").toDouble();
        porc_iva_gasto2 = queryFact.record().value("porc_iva_gasto2").toDouble();
        porc_iva_gasto3 = queryFact.record().value("porc_iva_gasto3").toDouble();
        iva_gasto1 = queryFact.record().value("iva_gasto1").toDouble();
        iva_gasto2 = queryFact.record().value("iva_gasto2").toDouble();
        iva_gasto3 = queryFact.record().value("iva_gasto3").toDouble();
        dto = queryFact.record().value("dto").toDouble();
        ejercicio = queryFact.record().value("ejercicio").toDouble();
        contabilizada = queryFact.record().value("contabilizada").toDouble();
        subtotal = queryFact.record().value("subtotal").toDouble();
        codigo_proveedor = queryFact.record().value("codigo_proveedor").toString();
        impreso = queryFact.record().value("impreso").toDouble();
        editable = queryFact.record().value("editable").toDouble();

        return true;
    }
    else
    {
        if(accion == 1)
            TimedMessageBox::Box(qApp->activeWindow(),"Se ha llegado al inicio del fichero");
        if(accion == 2)
            TimedMessageBox::Box(qApp->activeWindow(),"Se ha llegado al final del fichero");
    }
    return false;
}

