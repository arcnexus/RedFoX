#include "facturasproveedor.h"

FacturasProveedor::FacturasProveedor(QObject *parent) :
    QObject(parent)
{
}
int FacturasProveedor::anadir_factura()
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    queryFactura.prepare("insert into fac_pro (id_proveedor,porc_iva1,porc_iva2,porc_iva3,porc_iva4,porc_rec1,porc_rec2,porc_rec3,porc_rec4) "
                         " values (0,:iva1,:iva2,:iva3,:iva4,:porc_rec1,:porc_rec2,:porc_rec3,:porc_rec4)");
    queryFactura.bindValue(":porc_iva1",Configuracion_global->ivaList.at(0));
    queryFactura.bindValue(":porc_iva2",Configuracion_global->ivaList.at(1));
    queryFactura.bindValue(":porc_iva3",Configuracion_global->ivaList.at(2));
    queryFactura.bindValue(":porc_iva4",Configuracion_global->ivaList.at(3));
    queryFactura.bindValue(":porc_rec1",Configuracion_global->reList.at(0));
    queryFactura.bindValue(":porc_rec2",Configuracion_global->reList.at(1));
    queryFactura.bindValue(":porc_rec3",Configuracion_global->reList.at(2));
    queryFactura.bindValue(":porc_rec4",Configuracion_global->reList.at(3));

    if(queryFactura.exec())
    {
        this->id = queryFactura.lastInsertId().toInt();
        return this->id;
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Facturas a proveedor"),
                             tr("No se pudo crear la nueva factura : %1").arg(queryFactura.lastError().text()),
                             tr("Aceptar"));
        return -1;
    }
}

void FacturasProveedor::guardar_factura()
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    queryFactura.prepare("UPDATE fac_pro SET "
                         "factura =:factura,"
                         "fecha =:fecha,"
                         "fecha_recepcion =:fecha_recepcion,"
                         "pedido =:pedido,"
                         "id_proveedor =:id_proveedor,"
                         "proveedor =:proveedor,"
                         "cif_proveedor =:cif_proveedor,"
                         "retencion_irpf =:retencion_irpf,"
                         "recargo_equivalencia =:recargo_equivalencia,"
                         "base1 =:base1,"
                         "base2 =:base2,"
                         "base3 =:base3,"
                         "base4 =:base4,"
                         "porc_iva1 =:porc_iva1,"
                         "porc_iva2 =:porc_iva2,"
                         "porc_iva3 =:porc_iva3,"
                         "porc_iva4 =:porc_iva4,"
                         "iva1 =:iva1,"
                         "iva2 =:iva2,"
                         "iva3 =:iva3,"
                         "iva4 =:iva4,"
                         "porc_rec1 =:porc_rec1,"
                         "porc_rec2 =:porc_rec2,"
                         "porc_rec3 =:porc_rec3,"
                         "porc_rec4 =:porc_rec4,"
                         "rec1 =:rec1,"
                         "rec2 =:rec2,"
                         "rec3 =:rec3,"
                         "rec4 =:rec4,"
                         "total1 =:total1,"
                         "total2 =:total2,"
                         "total3 =:total3,"
                         "total4 =:total4,"
                         "total_base =:total_base,"
                         "total_iva =:total_iva,"
                         "total_retencion =:total_retencion,"
                         "total_recargo =:total_recargo,"
                         "total =:total,"
                         "forma_pago =:forma_pago,"
                         "importe_pagado_total =:importe_pagado_total,"
                         "id_tipo_gasto =:id_tipo_gasto,"
                         "comentario =:comentario,"
                         "pagado =:pagado,"
                         "importe_deuda_pendiente =:importe_deuda_pendiente,"
                         "desc_gasto1 = :desc_gasto1,"
                         "desc_gasto2 = :desc_gasto2,"
                         "desc_gasto3 = :desc_gasto3,"
                         "imp_gasto1 = :imp_gasto1,"
                         "imp_gasto2 = :imp_gasto2,"
                         "imp_gasto3 = :imp_gasto3,"
                         "gasto_to_coste =:gasto_to_coste,"
                         "albaran =:albaran "
                         "WHERE id =:id");

    queryFactura.bindValue(":factura",this->factura);
    queryFactura.bindValue(":fecha",this->fecha);
    queryFactura.bindValue(":fecha_recepcion",this->fecha_recepcion);
    queryFactura.bindValue(":pedido",this->pedido);
    queryFactura.bindValue(":id_proveedor",this->id_proveedor);
    queryFactura.bindValue(":proveedor",this->proveedor);
    queryFactura.bindValue(":cif_proveedor",this->cif_proveedor);
    queryFactura.bindValue(":retencion_irpf",this->retencion_irpf);
    queryFactura.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
    queryFactura.bindValue(":base1",this->base1);
    queryFactura.bindValue(":base2",this->base2);
    queryFactura.bindValue(":base3",this->base3);
    queryFactura.bindValue(":base4",this->base4);
    queryFactura.bindValue(":porc_iva1",this->porc_iva1);
    queryFactura.bindValue(":porc_iva2",this->porc_iva2);
    queryFactura.bindValue(":porc_iva3",this->porc_iva3);
    queryFactura.bindValue(":porc_iva4",this->porc_iva4);
    queryFactura.bindValue(":iva1",this->iva1);
    queryFactura.bindValue(":iva2",this->iva2);
    queryFactura.bindValue(":iva3",this->iva3);
    queryFactura.bindValue(":iva4",this->iva4);
    queryFactura.bindValue(":porc_rec1",this->porc_rec1);
    queryFactura.bindValue(":porc_rec2",this->porc_rec2);
    queryFactura.bindValue(":porc_rec3",this->porc_rec3);
    queryFactura.bindValue(":porc_rec4",this->porc_rec4);
    queryFactura.bindValue(":rec1",this->rec1);
    queryFactura.bindValue(":rec2",this->rec2);
    queryFactura.bindValue(":rec3",this->rec3);
    queryFactura.bindValue(":rec4",this->rec4);
    queryFactura.bindValue(":total1",this->total1);
    queryFactura.bindValue(":total2",this->total2);
    queryFactura.bindValue(":total3",this->total3);
    queryFactura.bindValue(":total4",this->total4);
    queryFactura.bindValue(":total_base",this->total_base);
    queryFactura.bindValue(":total_iva",this->total_iva);
    queryFactura.bindValue(":total_retencion",this->total_retencion);
    queryFactura.bindValue(":total_recargo",this->total_recargo);
    queryFactura.bindValue(":total",this->total);
    queryFactura.bindValue(":forma_pago",this->forma_pago);
    queryFactura.bindValue(":importe_pagado_total",this->importe_pagado_total);
    queryFactura.bindValue(":id_tipo_gasto",this->id_tipo_gasto);
    queryFactura.bindValue(":comentario",this->comentario);
    queryFactura.bindValue(":pagado",this->pagado);
    queryFactura.bindValue(":importe_deuda_pendiente",this->importe_deuda_pendiente);
    queryFactura.bindValue(":desc_gasto1",this->desc_gasto1);
    queryFactura.bindValue(":desc_gasto2",this->desc_gasto2);
    queryFactura.bindValue(":desc_gasto3",this->desc_gasto3);
    queryFactura.bindValue(":imp_gasto1",this->imp_gasto1);
    queryFactura.bindValue(":imp_gasto2",this->imp_gasto2);
    queryFactura.bindValue(":imp_gasto3",this->imp_gasto3);
    queryFactura.bindValue(":gasto_to_coste",this->gasto_to_coste);
    queryFactura.bindValue(":albaran",this->albaran);
    queryFactura.bindValue(":id",this->id);

    if(queryFactura.exec())
    {
        // TODO - acumulados + contabilidad
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Facturas a proveedores"),
                             tr("Falló la modificación de la factura: %1").arg(queryFactura.lastError().text()));
    }

}

void FacturasProveedor::recuperar_factura(QString cSQL, int accion)
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    if(queryFactura.exec(cSQL))
        cargar_factura(queryFactura,accion);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
}

void FacturasProveedor::recuperar_factura(int id_factura)
{
    QSqlQuery queryFactura(QSqlDatabase::database("empresa"));
    queryFactura.prepare("select * from fac_pro where id = :id");
    queryFactura.bindValue(":id",id_factura);
    if(queryFactura.exec())
            cargar_factura(queryFactura,0);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión facturas proveedor"),
                             tr("Falló al recuperar la factura: %1").arg(queryFactura.lastError().text()));
}

void FacturasProveedor::cargar_factura(QSqlQuery queryFact,int accion)
{

  // accion == 0 - nada : 1 - Anterior : 2 - Siguiente
    if(queryFact.next())
    {
        this->factura = queryFact.record().value("factura").toString();
        this->fecha = queryFact.record().value("fecha").toDate();
        this->fecha_recepcion = queryFact.record().value("fecha_recepcion").toDate();
        this->pedido = queryFact.record().value("pedido").toString();
        this->id_proveedor = queryFact.record().value("id_proveedor").toInt();
        this->proveedor = queryFact.record().value("proveedor").toString();
        this->cif_proveedor = queryFact.record().value("cif_proveedor").toString();
        this->retencion_irpf = queryFact.record().value("retencion_irpf").toBool();
        this->recargo_equivalencia = queryFact.record().value("recargo_equivalencia").toBool();
        this->base1 = queryFact.record().value("base1").toDouble();
        this->base2 = queryFact.record().value("base2").toDouble();
        this->base3 = queryFact.record().value("base3").toDouble();
        this->base4 = queryFact.record().value("base4").toDouble();
        this->porc_iva1 = queryFact.record().value("porc_iva1").toDouble();
        this->porc_iva2 = queryFact.record().value("porc_iva2").toDouble();
        this->porc_iva3 = queryFact.record().value("porc_iva3").toDouble();
        this->porc_iva4 = queryFact.record().value("porc_iva4").toDouble();
        this->iva1 = queryFact.record().value("iva1").toDouble();
        this->iva2 = queryFact.record().value("iva2").toDouble();
        this->iva3 = queryFact.record().value("iva3").toDouble();
        this->iva4 = queryFact.record().value("iva4").toDouble();
        this->porc_rec1 = queryFact.record().value("porc_rec1").toDouble();
        this->porc_rec2 = queryFact.record().value("porc_rec2").toDouble();
        this->porc_rec3 = queryFact.record().value("porc_rec3").toDouble();
        this->porc_rec4 = queryFact.record().value("porc_rec4").toDouble();
        this->rec1 = queryFact.record().value("rec1").toDouble();
        this->rec2 = queryFact.record().value("rec2").toDouble();
        this->rec3 = queryFact.record().value("rec3").toDouble();
        this->rec4 = queryFact.record().value("rec4").toDouble();
        this->total1 = queryFact.record().value("total1").toDouble();
        this->total2 = queryFact.record().value("total2").toDouble();
        this->total3 = queryFact.record().value("total3").toDouble();
        this->total4 = queryFact.record().value("total4").toDouble();
        this->total_base = queryFact.record().value("total_base").toDouble();
        this->total_iva = queryFact.record().value("total_iva").toDouble();
        this->total_retencion = queryFact.record().value("total_retencion").toDouble();
        this->total_recargo = queryFact.record().value("total_recargo").toDouble();
        this->total = queryFact.record().value("total").toDouble();
        this->forma_pago = queryFact.record().value("forma_pago").toDouble();
        this->importe_pagado_total = queryFact.record().value("importe_pagado_total").toDouble();
        this->id_tipo_gasto = queryFact.record().value("id_tipo_gasto").toDouble();
        this->comentario = queryFact.record().value("comentario").toString();
        this->pagado = queryFact.record().value("pagado").toBool();
        this->importe_deuda_pendiente = queryFact.record().value("importe_deuda_pendiente").toDouble();
        this->albaran = queryFact.record().value("albaran").toString();
        this->id = queryFact.record().value("id").toInt();
        this->desc_gasto1 = queryFact.record().value("desc_gasto1").toString();
        this->desc_gasto2 = queryFact.record().value("desc_gasto2").toString();
        this->desc_gasto3 = queryFact.record().value("desc_gasto3").toString();
        this->imp_gasto1 = queryFact.record().value("imp_gasto1").toDouble();
        this->imp_gasto2 = queryFact.record().value("imp_gasto2").toDouble();
        this->imp_gasto3 = queryFact.record().value("imp_gasto3").toDouble();
        this->gasto_to_coste = queryFact.record().value("gasto_to_coste").toBool();

    } else
    {
        if(accion == 1)
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al inicio del fichero");
        if(accion == 2)
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al final del fichero");
    }
}

