#include "albaranproveedor.h"
#include "../Almacen/articulo.h"
#include "proveedor.h"

AlbaranProveedor::AlbaranProveedor(QObject *parent) :
    QObject(parent)
{
}

bool AlbaranProveedor::anadir()
{
     QSqlQuery queryAlbaran(Configuracion_global->empresaDB);
     queryAlbaran.prepare("insert into alb_pro (porc_iva1,porc_iva2,porc_iva3,porc_iva4,"
                          "porc_rec1,porc_rec2,porc_rec3,porc_rec4) "
                          "values (:porc_iva1,:porc_iva2,:porc_iva3,:porc_iva4,"
                          ":porc_rec1,:porc_rec2,:porc_rec3,:porc_rec4);");
     queryAlbaran.bindValue(":porc_iva1",Configuracion_global->ivaList.at(0));
     queryAlbaran.bindValue(":porc_iva2",Configuracion_global->ivaList.at(1));
     queryAlbaran.bindValue(":porc_iva3",Configuracion_global->ivaList.at(2));
     queryAlbaran.bindValue(":porc_iva4",Configuracion_global->ivaList.at(3));
     queryAlbaran.bindValue(":porc_rec1",Configuracion_global->reList.at(0));
     queryAlbaran.bindValue(":porc_rec2",Configuracion_global->reList.at(1));
     queryAlbaran.bindValue(":porc_rec3",Configuracion_global->reList.at(2));
     queryAlbaran.bindValue(":porc_rec4",Configuracion_global->reList.at(3));
     if(queryAlbaran.exec())
     {
         this->id = queryAlbaran.lastInsertId().toInt();
        return true;
     }
     else
     {
         QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Albaranes de proveedores"),
                              tr("Ocurrió un error al crear un nuevo albarán : %1").arg(queryAlbaran.lastError().text()));
         return false;
     }
}

void AlbaranProveedor::Recuperar(int id)
{
    QSqlQuery queryAlbaran(Configuracion_global->empresaDB);
    if (queryAlbaran.exec("Select * from alb_pro where id = "+QString::number(id)))
        Cargar(queryAlbaran);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes de proveedor"),
                             tr("Ocurrió un error al recuperar:")+queryAlbaran.lastError().text(),
                             tr("Aceptar"));

}

bool AlbaranProveedor::borrar()
{
    bool succes = true;
    Configuracion_global->empresaDB.transaction();

    bool borrar_stock = (QMessageBox::question(qApp->activeWindow(),tr("Stock de artículos"), tr("¿Borrar el stock de artículos?"),
                         tr("No"),tr("Borrar")) == QMessageBox::Accepted);

    QString error;
    QMap<int, QSqlRecord> lineas = SqlCalls::SelectRecord("lin_alb_pro",QString("id_cab = %1").arg(this->id),Configuracion_global->empresaDB,error);
    for(auto i = lineas.begin(); i != lineas.end(); ++i)
    {
        QSqlRecord r = i.value();
        int id_art = r.value("id_articulo").toInt();
        double cantidad = r.value("cantidad").toDouble();
        double total_art = r.value("total").toDouble();

        succes &= Articulo::acumulado_compras(id_art,-1.0*cantidad, -1.0*total_art,this->fecha);
        if(borrar_stock)
            succes &= Articulo::agregar_stock_fisico(id_art, -1.0*cantidad);

    }
    succes &= Proveedor::acumular(this->id_proveedor,this->fecha.month(),-1.0*base_total);
    succes &= SqlCalls::SqlDelete("lin_alb_pro",Configuracion_global->empresaDB,QString("id_cab = %1").arg(id),error);
    succes &= SqlCalls::SqlDelete("alb_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id),error);

    if(!succes)
    {
        Configuracion_global->rollback();
        QMessageBox::warning(qApp->activeWindow(),tr("Ocurrió un error al borrar albarán de proveedores"),error,tr("Aceptar"));
    }
    else
    {
        Configuracion_global->commit();
    }
    return succes;
}


void AlbaranProveedor::Recuperar(QString cSQL)
{
    QSqlQuery queryAlbaran(Configuracion_global->empresaDB);
    if (queryAlbaran.exec(cSQL))
        Cargar(queryAlbaran);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes de proveedor"),
                             tr("Ocurrió un error al recuperar:")+queryAlbaran.lastError().text(),
                             tr("Aceptar"));

}

void AlbaranProveedor::Recuperar(QString cSQL, int val_accion)
{
    this->accion = val_accion;
    QSqlQuery queryAlbaran(Configuracion_global->empresaDB);
    if (queryAlbaran.exec(cSQL))
        Cargar(queryAlbaran);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes de proveedor"),
                             tr("Ocurrió un error al recuperar:")+queryAlbaran.lastError().text(),
                             tr("Aceptar"));

}

void AlbaranProveedor::Cargar(QSqlQuery query)
{
    if(query.next())
    {
        QSqlRecord r = query.record();
        id = r.value("id").toInt();
        albaran = r.value("albaran").toString();
        fecha = r.value("fecha").toDate();
        id_proveedor = r.value("id_proveedor").toInt();
        proveedor = r.value("proveedor").toString();
        cif_proveedor = r.value("cif_proveedor").toString();
        total1 = r.value("total1").toDouble();
        total2 = r.value("total2").toDouble();
        total3 = r.value("total3").toDouble();
        total4 = r.value("total4").toDouble();
        porc_iva1 = r.value("porc_iva1").toDouble();
        porc_iva2 = r.value("porc_iva2").toDouble();
        porc_iva3 = r.value("porc_iva3").toDouble();
        porc_iva4 = r.value("porc_iva4").toDouble();
        iva1 = r.value("iva1").toDouble();
        iva2 = r.value("iva2").toDouble();
        iva3 = r.value("iva3").toDouble();
        iva4 = r.value("iva4").toDouble();
        base1 = r.value("base1").toDouble();
        base2 = r.value("base2").toDouble();
        base3 = r.value("base3").toDouble();
        base4 = r.value("base4").toDouble();
        factura = r.value("factura").toString();
        base_total = r.value("base_total").toDouble();
        iva_total = r.value("iva_total").toDouble();
        total = r.value("total").toDouble();
        comentario = r.value("comentario").toString();
        pedido = r.value("pedido").toInt();
        recargo_equivalencia = r.value("recargo_equivalencia").toBool();
        porc_rec1 = r.value("porc_rec1").toDouble();
        porc_rec2 = r.value("porc_rec2").toDouble();
        porc_rec3 = r.value("porc_rec3").toDouble();
        porc_rec4 = r.value("porc_rec4").toDouble();
        rec1 = r.value("rec1").toDouble();
        rec2 = r.value("rec2").toDouble();
        rec3 = r.value("rec3").toDouble();
        rec4 = r.value("rec4").toDouble();
        total_recargo = r.value("total_recargo").toDouble();
        ejercicio = r.value("ejercicio").toInt();
        subtotal = r.value("subtotal").toDouble();
        direccion1 = r.value("direccion1").toString();
        direccion2 = r.value("direccion2").toString();
        cp = r.value("cp").toString();
        poblacion = r.value("poblacion").toString();
        provincia = r.value("provincia").toString();
        id_pais = r.value("id_pais").toInt();
        porc_dto = r.value("porc_dto").toDouble();
        dto = r.value("dto").toDouble();
        telefono = r.value("telefono").toString();
        fax = r.value("fax").toString();
        movil = r.value("movil").toString();
        codigo_proveedor = r.value("codigo_proveedor").toString();
        id_forma_pago = r.value("id_forma_pago").toInt();
        impreso = r.value("impreso").toBool();
        desc_gasto1 = r.value("desc_gasto1").toString();
        desc_gasto2 = r.value("desc_gasto2").toString();
        desc_gasto3 = r.value("desc_gasto3").toString();
        imp_gasto1 = r.value("imp_gasto1").toDouble();
        imp_gasto2 = r.value("imp_gasto2").toDouble();
        imp_gasto3 = r.value("imp_gasto3").toDouble();
        porc_iva_gasto1 = r.value("porc_iva_gasto1").toDouble();
        porc_iva_gasto2 = r.value("porc_iva_gasto2").toDouble();
        porc_iva_gasto3 = r.value("porc_iva_gasto3").toDouble();
        iva_gasto1 = r.value("iva_gasto1").toDouble();
        iva_gasto2 = r.value("iva_gasto2").toDouble();
        iva_gasto3 = r.value("iva_gasto3").toDouble();
        gasto_to_coste = r.value("gasto_to_coste").toBool();
        editable = r.value("editable").toBool();
    }
    else
    {
        switch (this->accion)
        {
        case 1: // siguiente
            TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha llegado al último albarán"));
            break;
        case 2: // anterior
            TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha llegado al primer albarán"));
            break;
        default:
            TimedMessageBox::Box(qApp->activeWindow(),tr("No se encontró el albarán"));
            break;
        }
    }

}

bool AlbaranProveedor::guardar()
{
    QSqlQuery queryAlb_pro(Configuracion_global->empresaDB);
    QHash<QString,QVariant> _data;
    _data["albaran"] = albaran;
    _data["fecha"] = fecha;
    _data["id_proveedor"] = id_proveedor;
    _data["proveedor"] = proveedor;
    _data["cif_proveedor"] = cif_proveedor;
    _data["total1"] = total1;
    _data["total2"] = total2;
    _data["total3"] = total3;
    _data["total4"] = total4;
    _data["porc_iva1"] = porc_iva1;
    _data["porc_iva2"] = porc_iva2;
    _data["porc_iva3"] = porc_iva3;
    _data["porc_iva4"] = porc_iva4;
    _data["iva1"] = iva1;
    _data["iva2"] = iva2;
    _data["iva3"] = iva3;
    _data["iva4"] = iva4;
    _data["base1"] = base1;
    _data["base2"] = base2;
    _data["base3"] = base3;
    _data["base4"] = base4;
    _data["factura"] = factura;
    _data["base_total"] = base_total;
    _data["iva_total"] = iva_total;
    _data["total"] = total;
    _data["comentario"] = comentario;
    _data["pedido"] = pedido;
    _data["recargo_equivalencia"] = recargo_equivalencia;
    _data["porc_rec1"] = porc_rec1;
    _data["porc_rec2"] = porc_rec2;
    _data["porc_rec3"] = porc_rec3;
    _data["porc_rec4"] = porc_rec4;
    _data["rec1"] = rec1;
    _data["rec2"] = rec2;
    _data["rec3"] = rec3;
    _data["rec4"] = rec4;
    _data["total_recargo"] = total_recargo;
    _data["ejercicio"] = ejercicio;
    _data["subtotal"] = subtotal;
    _data["direccion1"] = direccion1;
    _data["direccion2"] = direccion2;
    _data["cp"] = cp;
    _data["poblacion"] = poblacion;
    _data["provincia"] = provincia;
    _data["id_pais"] = id_pais;
    _data["porc_dto"] = porc_dto;
    _data["dto"] = dto;
    _data["telefono"] = telefono;
    _data["fax"] = fax;
    _data["movil"] = movil;
    _data["codigo_proveedor"] = codigo_proveedor;
    _data["id_forma_pago"] = id_forma_pago;
    _data["impreso"] = impreso;
    _data["desc_gasto1"] = desc_gasto1;
    _data["desc_gasto2"] = desc_gasto2;
    _data["desc_gasto3"] = desc_gasto3;
    _data["imp_gasto1"] = imp_gasto1;
    _data["imp_gasto2"] = imp_gasto2;
    _data["imp_gasto3"] = imp_gasto3;
    _data["porc_iva_gasto1"] = porc_iva_gasto1;
    _data["porc_iva_gasto2"] = porc_iva_gasto2;
    _data["porc_iva_gasto3"] = porc_iva_gasto3;
    _data["iva_gasto1"] = iva_gasto1;
    _data["iva_gasto2"] = iva_gasto2;
    _data["iva_gasto3"] = iva_gasto3;
    _data["gasto_to_coste"] = gasto_to_coste;
    _data["editable"] = editable;
    QString error;
    bool succes = SqlCalls::SqlUpdate(_data,"alb_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id),error);
    if(!succes)
    {
        QMessageBox::warning(qApp->activeWindow(),
                             tr("Guardado de Albaranes"),
                             tr("Ocurrió un error al guardar el albarán: %1").arg(queryAlb_pro.lastError().text()),
                             tr("Aceptar"));
    }
    return succes;
}
