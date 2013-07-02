#include "albaranproveedor.h"

AlbaranProveedor::AlbaranProveedor(QObject *parent) :
    QObject(parent)
{
}

int AlbaranProveedor::anadir()
{
     QSqlQuery queryAlbaran(QSqlDatabase::database("empresa"));
     queryAlbaran.prepare("insert into alb_pro (iva1,iva2,iva3,iva4) values (:iva1,:iva2,:iva3,:iva4);");
     queryAlbaran.bindValue(":iva1",Configuracion_global->ivaList.at(0));
     queryAlbaran.bindValue(":iva2",Configuracion_global->ivaList.at(1));
     queryAlbaran.bindValue(":iva3",Configuracion_global->ivaList.at(2));
     queryAlbaran.bindValue(":iva4",Configuracion_global->ivaList.at(3));
     if(queryAlbaran.exec())
     {
             this->id = queryAlbaran.lastInsertId().toInt();
        return this->id;
     } else
     {
         this->id =0;
         QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Albaranes de proveedores"),
                              tr("Ocurrió un error al crear un nuevo albarán : %1").arg(queryAlbaran.lastError().text()));
        return this->id;
     }

}

void AlbaranProveedor::Recuperar(int id)
{
    QSqlQuery queryAlbaran(QSqlDatabase::database("empresa"));
    if (queryAlbaran.exec("Select * from alb_pro where id = "+QString::number(id)))
        Cargar(queryAlbaran);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes de proveedor"),
                             tr("Ocurrió un error al recuperar:")+queryAlbaran.lastError().text(),
                             tr("Aceptar"));

}

bool AlbaranProveedor::borrar(int id_alb)
{
    bool transaccion = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlQuery queryAlbpro(QSqlDatabase::database("empresa"));

    transaccion &= queryAlbpro.exec("delete from lin_alb_pro where id_cab = "+QString::number(id_alb));
    transaccion &= queryAlbpro.exec("delete from alb_pro where id ="+QString::number(id_alb));

    // TODO --- Actualizar stock
    if(transaccion)
        transaccion = QSqlDatabase::database("empresa").commit();
    if(!transaccion)
        QSqlDatabase::database("empresa").rollback();
    return transaccion;
}


void AlbaranProveedor::Recuperar(QString cSQL)
{
    QSqlQuery queryAlbaran(QSqlDatabase::database("empresa"));
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
    QSqlQuery queryAlbaran(QSqlDatabase::database("empresa"));
    if (queryAlbaran.exec(cSQL))
        Cargar(queryAlbaran);
    else
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes de proveedor"),
                             tr("Ocurrió un error al recuperar:")+queryAlbaran.lastError().text(),
                             tr("Aceptar"));

}

void AlbaranProveedor::Cargar(QSqlQuery query)
{
    if(query.next()){
        this->id = query.record().value("id").toInt();
        this->albaran = query.record().value("albaran").toString();
        this->fecha = query.record().value("fecha").toDate();
        this->id_proveedor = query.record().value("id_proveedor").toInt();
        this->proveedor = query.record().value("proveedor").toString();
        this->cif_proveedor = query.record().value("cif_proveedor").toString();
        this->total1 = query.record().value("total1").toDouble();
        this->total2 = query.record().value("total2").toDouble();
        this->total3 = query.record().value("total3").toDouble();
        this->total4 = query.record().value("total4").toDouble();
        this->iva1 = query.record().value("iva1").toDouble();
        this->iva2 = query.record().value("iva2").toDouble();
        this->iva3 = query.record().value("iva3").toDouble();
        this->iva4 = query.record().value("iva4").toDouble();
        this->iva1 = query.record().value("iva1").toDouble();
        this->iva2 = query.record().value("iva2").toDouble();
        this->iva3 = query.record().value("iva3").toDouble();
        this->iva4 = query.record().value("iva4").toDouble();
        this->base1 = query.record().value("base1").toDouble();
        this->base2 = query.record().value("base2").toDouble();
        this->base3 = query.record().value("base3").toDouble();
        this->base4 = query.record().value("base4").toDouble();
        this->factura = query.record().value("factura").toString();
        this->base_total = query.record().value("base_total").toDouble();
        this->iva_total = query.record().value("iva_total").toDouble();
        this->total = query.record().value("total").toDouble();
        this->comentario = query.record().value("comentario").toString();
    int pedido;
    } else
    {
        TimedMessageBox * t;
        switch (this->accion) {
        case 1: // siguiente
            t = new TimedMessageBox(qApp->activeWindow(),
                                                      QObject::tr("Se ha llegado al último albarán"));

            break;
        case 2: // anterior
            t = new TimedMessageBox(qApp->activeWindow(),
                                                      QObject::tr("Se ha llegado al primer albarán"));

            break;
        default:
            break;
        }
    }

}

void AlbaranProveedor::guardar()
{
    QSqlQuery queryAlb_pro(QSqlDatabase::database("empresa"));
    queryAlb_pro.prepare("UPDATE alb_pro SET "
                         "albaran = :albaran,"
                         "fecha = :fecha,"
                         "id_proveedor = :id_proveedor,"
                         "proveedor = :proveedor,"
                         "cif_proveedor = :cif_proveedor,"
                         "total1 = :total1,"
                         "total2 = :total2,"
                         "total3 = :total3,"
                         "total4 = :total4,"
                         "porc_iva1 = :iva1,"
                         "porc_iva2 = :iva2,"
                         "porc_iva3 = :iva3,"
                         "porc_iva4 = :iva4,"
                         "iva1 = :iva1,"
                         "iva2 = :iva2,"
                         "iva3 = :iva3,"
                         "iva4 = :iva4,"
                         "base1 = :base1,"
                         "base2 = :base2,"
                         "base3 = :base3,"
                         "base4 = :base4,"
                         "factura = :factura,"
                         "base_total = :base_total,"
                         "iva_total = :iva_total,"
                         "total = :total,"
                         "comentario = :comentario,"
                         "pedido = :pedido "
                         " WHERE id =:id;");

    queryAlb_pro.bindValue(":albaran",this->albaran);
    queryAlb_pro.bindValue(":fecha",this->fecha);
    queryAlb_pro.bindValue(":id_proveedor",this->id_proveedor);
    queryAlb_pro.bindValue(":proveedor",this->proveedor);
    queryAlb_pro.bindValue(":cif_proveedor",this->cif_proveedor);
    queryAlb_pro.bindValue(":total1",this->total1);
    queryAlb_pro.bindValue(":total2",this->total2);
    queryAlb_pro.bindValue(":total3",this->total3);
    queryAlb_pro.bindValue(":total4",this->total4);
    queryAlb_pro.bindValue(":iva1",this->iva1);
    queryAlb_pro.bindValue(":iva2",this->iva2);
    queryAlb_pro.bindValue(":iva3",this->iva3);
    queryAlb_pro.bindValue(":iva4",this->iva4);
    queryAlb_pro.bindValue(":iva1",this->iva1);
    queryAlb_pro.bindValue(":iva2",this->iva2);
    queryAlb_pro.bindValue(":iva3",this->iva3);
    queryAlb_pro.bindValue(":iva4",this->iva4);
    queryAlb_pro.bindValue(":base1",this->base1);
    queryAlb_pro.bindValue(":base2",this->base2);
    queryAlb_pro.bindValue(":base3",this->base3);
    queryAlb_pro.bindValue(":base4",this->base4);
    queryAlb_pro.bindValue(":factura",this->factura);
    queryAlb_pro.bindValue(":base_total",this->base_total);
    queryAlb_pro.bindValue(":iva_total",this->iva_total);
    queryAlb_pro.bindValue(":total",this->total);
    queryAlb_pro.bindValue(":comentario",this->comentario);
    queryAlb_pro.bindValue(":pedido",this->pedido);
    queryAlb_pro.bindValue(":id",this->id);
    if(!queryAlb_pro.exec())
        QMessageBox::warning(qApp->activeWindow(),
                             tr("Guardado de Albaranes"),
                             tr("Ocurrió un error al guardar el albarán: %1").arg(queryAlb_pro.lastError().text()),
                             tr("Aceptar"));

}
