#include "albaranproveedor.h"

AlbaranProveedor::AlbaranProveedor(QObject *parent) :
    QObject(parent)
{
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

void AlbaranProveedor::Cargar(QSqlQuery query)
{
    if(query.next()){
        this->id = query.record().value("id").toInt();
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
    }

}
