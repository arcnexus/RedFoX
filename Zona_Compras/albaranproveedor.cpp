#include "albaranproveedor.h"

AlbaranProveedor::AlbaranProveedor(QObject *parent) :
    QObject(parent)
{
}

int AlbaranProveedor::anadir()
{
     QSqlQuery queryAlbaran(QSqlDatabase::database("empresa"));
     queryAlbaran.prepare("insert into alb_pro (nIva1,nIva2,nIva3,nIva4) values (:nIva1,:nIva2,:nIva3,:nIva4);");
     queryAlbaran.bindValue(":nIva1",Configuracion_global->ivaList.at(0));
     queryAlbaran.bindValue(":nIva2",Configuracion_global->ivaList.at(1));
     queryAlbaran.bindValue(":nIva3",Configuracion_global->ivaList.at(2));
     queryAlbaran.bindValue(":nIva4",Configuracion_global->ivaList.at(3));
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
        this->id = query.record().value("Id").toInt();
        this->cAlbaran = query.record().value("cAlbaran").toString();
        this->dFecha = query.record().value("dFecha").toDate();
        this->id_Proveedor = query.record().value("id_Proveedor").toInt();
        this->cProveedor = query.record().value("cProveedor").toString();
        this->cCifproveedor = query.record().value("cCifproveedor").toString();
        this->rTotal1 = query.record().value("rTotal1").toDouble();
        this->rTotal2 = query.record().value("rTotal2").toDouble();
        this->rTotal3 = query.record().value("rTotal3").toDouble();
        this->rTotal4 = query.record().value("rTotal4").toDouble();
        this->nIva1 = query.record().value("nIva1").toDouble();
        this->nIva2 = query.record().value("nIva2").toDouble();
        this->nIva3 = query.record().value("nIva3").toDouble();
        this->nIva4 = query.record().value("nIva4").toDouble();
        this->rIva1 = query.record().value("rIva1").toDouble();
        this->rIva2 = query.record().value("rIva2").toDouble();
        this->rIva3 = query.record().value("rIva3").toDouble();
        this->rIva4 = query.record().value("rIva4").toDouble();
        this->rBase1 = query.record().value("rBase1").toDouble();
        this->rBase2 = query.record().value("rBase2").toDouble();
        this->rBase3 = query.record().value("rBase3").toDouble();
        this->rBase4 = query.record().value("rBase4").toDouble();
        this->cFactura = query.record().value("cFactura").toString();
        this->rBaseTotal = query.record().value("rBaseTotal").toDouble();
        this->rIvaTotal = query.record().value("rIvaTotal").toDouble();
        this->rTotal = query.record().value("rTotal").toDouble();
        this->tComentario = query.record().value("tComentario").toString();
    int nPedido;
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
                         "cAlbaran = :cAlbaran,"
                         "dFecha = :dFecha,"
                         "id_Proveedor = :id_Proveedor,"
                         "cProveedor = :cProveedor,"
                         "cCifProveedor = :cCifProveedor,"
                         "rTotal1 = :rTotal1,"
                         "rTotal2 = :rTotal2,"
                         "rTotal3 = :rTotal3,"
                         "rTotal4 = :rTotal4,"
                         "nIva1 = :nIva1,"
                         "nIva2 = :nIva2,"
                         "nIva3 = :nIva3,"
                         "nIva4 = :nIva4,"
                         "rIva1 = :rIva1,"
                         "rIva2 = :rIva2,"
                         "rIva3 = :rIva3,"
                         "rIva4 = :rIva4,"
                         "rBase1 = :rBase1,"
                         "rBase2 = :rBase2,"
                         "rBase3 = :rBase3,"
                         "rBase4 = :rBase4,"
                         "cFactura = :cFactura,"
                         "rBaseTotal = :rBaseTotal,"
                         "rIvaTotal = :rIvaTotal,"
                         "rTotal = :rTotal,"
                         "tComentario = :tComentario,"
                         "nPedido = :nPedido "
                         " WHERE id =:id;");

    queryAlb_pro.bindValue(":cAlbaran",this->cAlbaran);
    queryAlb_pro.bindValue(":dFecha",this->dFecha);
    queryAlb_pro.bindValue(":id_Proveedor",this->id_Proveedor);
    queryAlb_pro.bindValue(":cProveedor",this->cProveedor);
    queryAlb_pro.bindValue(":cCifProveedor",this->cCifproveedor);
    queryAlb_pro.bindValue(":rTotal1",this->rTotal1);
    queryAlb_pro.bindValue(":rTotal2",this->rTotal2);
    queryAlb_pro.bindValue(":rTotal3",this->rTotal3);
    queryAlb_pro.bindValue(":rTotal4",this->rTotal4);
    queryAlb_pro.bindValue(":nIva1",this->nIva1);
    queryAlb_pro.bindValue(":nIva2",this->nIva2);
    queryAlb_pro.bindValue(":nIva3",this->nIva3);
    queryAlb_pro.bindValue(":nIva4",this->nIva4);
    queryAlb_pro.bindValue(":rIva1",this->rIva1);
    queryAlb_pro.bindValue(":rIva2",this->rIva2);
    queryAlb_pro.bindValue(":rIva3",this->rIva3);
    queryAlb_pro.bindValue(":rIva4",this->rIva4);
    queryAlb_pro.bindValue(":rBase1",this->rBase1);
    queryAlb_pro.bindValue(":rBase2",this->rBase2);
    queryAlb_pro.bindValue(":rBase3",this->rBase3);
    queryAlb_pro.bindValue(":rBase4",this->rBase4);
    queryAlb_pro.bindValue(":cFactura",this->cFactura);
    queryAlb_pro.bindValue(":rBaseTotal",this->rBaseTotal);
    queryAlb_pro.bindValue(":rIvaTotal",this->rIvaTotal);
    queryAlb_pro.bindValue(":rTotal",this->rTotal);
    queryAlb_pro.bindValue(":tComentario",this->tComentario);
    queryAlb_pro.bindValue(":nPedido",this->nPedido);
    queryAlb_pro.bindValue(":id",this->id);
    if(!queryAlb_pro.exec())
        QMessageBox::warning(qApp->activeWindow(),
                             tr("Guardado de Albaranes"),
                             tr("Ocurrió un error al guardar el albarán: %1").arg(queryAlb_pro.lastError().text()),
                             tr("Aceptar"));

}
