#include "pedidoproveedor.h"

PedidoProveedor::PedidoProveedor(QObject *parent) :
    QObject(parent)
{
}

bool PedidoProveedor::get(int id)
{
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("SELECT * FROM ped_pro WHERE id = :id");
    q.bindValue(":id",id);
    if(q.exec())
    {
        if(q.next())
        {
            fillPedido(q.record());
            return true;
        }
    }
    return false;
}

bool PedidoProveedor::next()
{
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("SELECT * FROM ped_pro WHERE id = :id");
    q.bindValue(":id",id+1);
    if(q.exec())
    {
        if(q.next())
        {
            fillPedido(q.record());
            return true;
        }
    }
    return false;
}

bool PedidoProveedor::prev()
{
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("SELECT * FROM ped_pro WHERE id = :id");
    q.bindValue(":id",id-1);
    if(q.exec())
    {
        if(q.next())
        {
            fillPedido(q.record());
            return true;
        }
    }
    return false;
}

long PedidoProveedor::save()
{
    return -1;
}

bool PedidoProveedor::update()
{
    return false;
}

void PedidoProveedor::fillPedido(QSqlRecord r)
{
    id = r.value("id").toInt();
    nPedido = r.value("nPedido").toULongLong();
    cSerie = r.value("cSerie").toString();
    dFecha = r.value("dFecha").toDate();
    dRecepcion = r.value("dRecepcion").toDate();
    Id_Proveedor = r.value("Id_Proveedor").toULongLong();
    cCodigoProveedor = r.value("cCodigoProveedor").toString();
    cProveedor = r.value("cProveedor").toString();
    cDireccion1 = r.value("cDireccion1").toString();
    cDireccion2 = r.value("cDireccion2").toString();
    cCP = r.value("cCP").toString();
    cPoblacion= r.value("cPoblacion").toString();
    cProvincia= r.value("cProvincia").toString();
    idpais= r.value("idpais").toInt();
    cCifNif= r.value("cCifNif").toString();
    rBase= r.value("rBase").toDouble();
    rSubotal= r.value("rSubotal").toDouble();
    rDto= r.value("rDto").toDouble();
    nIVA= r.value("nIVA").toDouble();
    rRecTotal= r.value("rRecTotal").toDouble();
    rTotal= r.value("rTotal").toDouble();
    lEnviado= r.value("lEnviado").toULongLong();
    lRecibido= r.value("lRecibido").toULongLong();
    lRecibidoCompleto= r.value("lRecibidoCompleto").toULongLong();
    lGeneroPendiente= r.value("lGeneroPendiente").toULongLong();
    rBase1= r.value("rBase1").toDouble();
    rBase2= r.value("rBase2").toDouble();
    rBase3= r.value("rBase3").toDouble();
    rBase4= r.value("rBase4").toDouble();
    nPorcIva1= r.value("nPorcIva1").toDouble();
}
