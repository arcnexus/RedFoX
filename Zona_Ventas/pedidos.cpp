#include "pedidos.h"


Pedidos::Pedidos()
{
    this->id =0;
}

Pedidos::~Pedidos()
{
}

bool Pedidos::BorrarLineas(int Iped)
{
    QSqlQuery query(QSqlDatabase::database("empresa"));
    QString sql = QString("DELETE FROM lin_ped WHERE Id_Cab = %1").arg(Iped);
    query.prepare(sql);
    if(query.exec())
        return true;
    else
    {
        QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
        return false;
    }
}
bool Pedidos::AnadirPedido()
{
    this->nPorcentajeIVA1 = Configuracion_global->ivaList.at(0).toDouble();
    this->nPorcentajeIVA2 = Configuracion_global->ivaList.at(1).toDouble();
    this->nPorcentajeIVA3 = Configuracion_global->ivaList.at(2).toDouble();
    this->nPorcentajeIVA4 = Configuracion_global->ivaList.at(3).toDouble();

    this->nPorcentajeRecargoEq1 = Configuracion_global->reList.at(0).toDouble();
    this->nPorcentajeRecargoEq1 = Configuracion_global->reList.at(1).toDouble();
    this->nPorcentajeRecargoEq1 = Configuracion_global->reList.at(2).toDouble();
    this->nPorcentajeRecargoEq1 = Configuracion_global->reList.at(3).toDouble();
    this->lRecargoEquivalencia = 0;

    QSqlQuery cab_ped(QSqlDatabase::database("empresa"));
     cab_ped.prepare("INSERT INTO ped_cli (nPedido,nPorcentajeIva1,nPorcentajeIva2,nPorcentajeIva3,nPorcentajeIva4,"
                     "nPorcentajeRecargoEq1,nPorcentajeRecargoEq2,nPorcentajeRecargoEq3,nPorcentajeRecargoEq4,id_Cliente)"
                   " VALUES (:nPedido,:nPorcentajeIva1,:nPorcentajeIva2,:nPorcentajeIva3,:nPorcentajeIva4,"
                     ":nPorcentajeRecargoEq1,:nPorcentajeRecargoEq2,:nPorcentajeRecargoEq3,:nPorcentajeRecargoEq4,:id_Cliente)");

     this->nPedido = NuevoNumeroPedido();
     cab_ped.bindValue(":nPedido",nPedido);
     cab_ped.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
     cab_ped.bindValue(":nPorcentajeIva2",this->nPorcentajeIVA2);
     cab_ped.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
     cab_ped.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
     cab_ped.bindValue(":nPorcentajeRecargoEq1",this->nPorcentajeRecargoEq1);
     cab_ped.bindValue(":nPorcentajeRecargoEq2",this->nPorcentajeRecargoEq1);
     cab_ped.bindValue(":nPorcentajeRecargoEq3",this->nPorcentajeRecargoEq1);
     cab_ped.bindValue(":nPorcentajeRecargoEq4",this->nPorcentajeRecargoEq1);
     cab_ped.bindValue(":id_Cliente",iId_Cliente);
     if(!cab_ped.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Pedido:", cab_ped.lastError().text());
         return false;
     }
     else
     {
         this->id = cab_ped.lastInsertId().toInt();
         return true;
     }
}
// Guardar el Pedido
bool Pedidos::GuardarPedido(int nId_Pedido)
{
    QSqlQuery cab_ped(QSqlDatabase::database("empresa"));

    cab_ped.prepare( "UPDATE ped_cli SET "
                     "nAlbaran =:nAlbaran,nPedido =:nPedido, dFecha =:dFecha  , cPedido =:cPedido  , id_Cliente =:id_Cliente  ,"
                     "cCodigoCliente  =:cCodigoCliente  ,  cCliente =:cCliente  , cDireccion =:cDireccion,"
                     "cDireccion2 =:cDireccion2  , cPoblacion =:cPoblacion  ,cProvincia  =:cProvincia  ,"
                     "cCP  =:cCP  ,  idpais  =:idpais  ,  cCif  =:cCif  ,"
                     "lRecargoEquivalencia  =:lRecargoEquivalencia  ,  rSubtotal  =:rSubtotal ,"
                     "rDto  =:rDto  ,  rBase1  =:rBase1  ,  rBase2  =:rBase2  ,"
                     "rBase3  =:rBase3  ,  rBase4  =:rBase4  ,  nPorcentajeIva1  =:nPorcentajeIva1  ,"
                     "nPorcentajeIva2  =:nPorcentajeIva2  ,  nPorcentajeIva3  =:nPorcentajeIva3  ,"
                     "nPorcentajeIva4  =:nPorcentajeIva4  ,  rImporteIva1  =:rImporteIva1  ,"
                     "rImporteIva2  =:rImporteIva2  ,  rImporteIva3  =:rImporteIva3  ,"
                     "rImporteIva4  =:rImporteIva4  ,  nPorcentajeRecargoEq1  =:nPorcentajeRecargoEq1  ,"
                     "nPorcentajeRecargoEq2  =:nPorcentajeRecargoEq2  ,  nPorcentajeRecargoEq3  =:nPorcentajeRecargoEq3  ,"
                     "nPorcentajeRecargoEq4  =:nPorcentajeRecargoEq4  ,"
                     "rImporteRecargoEq1  =:rImporteRecargoEq1  ,"
                     "rImporteRecargoEq2  =:rImporteRecargoEq2  ,"
                     "rImporteRecargoEq3  =:rImporteRecargoEq3  ,"
                     "rImporteRecargoEq4  =:rImporteRecargoEq4  ,"
                     "rTotal1  =:rTotal1  ,  rTotal2  =:rTotal2  ,  rTotal3  =:rTotal3  ,"
                     "rTotal4  =:rTotal4  ,rBaseTotal  =:rBaseTotal  ,  rIvaTotal  =:rIvaTotal  ,"
                     "rRecargoEqTotal  =:rRecargoEqTotal  ,  rTotalAlbaran  =:rTotalAlbaran  ,"
                     "lImpreso  =:lImpreso  ,lFacturado  =:lFacturado  ,  cFactura  =:cFactura  ,"
                     "dFechaFactura  =:dFechaFactura  ,  tComentario  =:tComentario  ,"
                     "rACuenta  =:rACuenta  ,lTraspasadoAlb  =:lTraspasadoAlb  ,"
                     "lTraspasadoFac  =:lTraspasadoFac  ,  cDireccionEntrega  =:cDireccionEntrega  ,"
                     "cDireccionEntrega2  =:cDireccionEntrega2  ,cCPEntrega  =:cCPEntrega  ,"
                     "cPoblacionEntrega  =:cPoblacionEntrega  ,  cProvinciaEntrega  =:cProvinciaEntrega  ,"
                     "cPaisEntrega  =:cPaisEntrega  ,lEnviado  =:lEnviado  ,  lCompleto  =:lCompleto  ,"
                     "lEntregado  =:lEntregado  ,  dFechaLimiteEntrega  =:dFechaLimiteEntrega  ,"
                     "rTotalPedido  =:rTotalPedido"
                     " WHERE Id = :id");

    cab_ped.bindValue(":nAlbaran",nAlbaran);
    cab_ped.bindValue(":nPedido",nPedido);
    cab_ped.bindValue(":dFecha",dFecha);
    cab_ped.bindValue(":cPedido",cPedido);
    cab_ped.bindValue(":id_Cliente",iId_Cliente);
    cab_ped.bindValue(":cCodigoCliente",cCodigoCliente);
    cab_ped.bindValue(":cCliente",cCliente);
    cab_ped.bindValue(":cDireccion",cDireccion);
    cab_ped.bindValue(":cDireccion2",cDireccion2);
    cab_ped.bindValue(":cPoblacion",cPoblacion);
    cab_ped.bindValue(":cProvincia",cProvincia);
    cab_ped.bindValue(":cCP",cCp);
    cab_ped.bindValue(":idpais",id_pais);
    cab_ped.bindValue(":cCif",cCif);
    cab_ped.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    cab_ped.bindValue(":rSubtotal",rSubtotal);
    cab_ped.bindValue(":rDto",rDto);
  //  cab_ped.bindValue(":nDto",nDto);
    cab_ped.bindValue(":rBase1",rBase1);
    cab_ped.bindValue(":rBase2",rBase2);
    cab_ped.bindValue(":rBase3",rBase3);
    cab_ped.bindValue(":rBase4",rBase4);
    cab_ped.bindValue(":nPorcentajeIva1",nPorcentajeIVA1);
    cab_ped.bindValue(":nPorcentajeIva2",nPorcentajeIVA2);
    cab_ped.bindValue(":nPorcentajeIva3",nPorcentajeIVA3);
    cab_ped.bindValue(":nPorcentajeIva4",nPorcentajeIVA4);
    cab_ped.bindValue(":rImporteIva1",rImporteIva1);
    cab_ped.bindValue(":rImporteIva2",rImporteIva2);
    cab_ped.bindValue(":rImporteIva3",rImporteIva3);
    cab_ped.bindValue(":rImporteIva4",rImporteIva4);
    cab_ped.bindValue(":nPorcentajeRecargoEq1",nPorcentajeRecargoEq1);
    cab_ped.bindValue(":nPorcentajeRecargoEq2",nPorcentajeRecargoEq2);
    cab_ped.bindValue(":nPorcentajeRecargoEq3",nPorcentajeRecargoEq3);
    cab_ped.bindValue(":nPorcentajeRecargoEq4",nPorcentajeRecargoEq4);
    cab_ped.bindValue(":rImporteRecargoEq1",rImporteRecargoEq1);
    cab_ped.bindValue(":rImporteRecargoEq2",rImporteRecargoEq2);
    cab_ped.bindValue(":rImporteRecargoEq3",rImporteRecargoEq3);
    cab_ped.bindValue(":rImporteRecargoEq4",rImporteRecargoEq4);
    cab_ped.bindValue(":rTotal1",rTotal1);
    cab_ped.bindValue(":rTotal2",rTotal2);
    cab_ped.bindValue(":rTotal3",rTotal3);
    cab_ped.bindValue(":rTotal4",rTotal4);
    cab_ped.bindValue(":rBaseTotal",rBaseTotal);
    cab_ped.bindValue(":rIvaTotal",rIvaTotal);
    cab_ped.bindValue(":rRecargoEqTotal",rRecargoEqTotal);
    cab_ped.bindValue(":rTotalAlbaran",rTotalAlbaran);
    cab_ped.bindValue(":lImpreso",lImpreso);
    cab_ped.bindValue(":lFacturado",lFacturado);
    cab_ped.bindValue(":cFactura",cFactura);
    cab_ped.bindValue(":dFechaFactura",dFechaFactura);
    cab_ped.bindValue(":tComentario",tComentario);
    cab_ped.bindValue(":rACuenta",rACuenta);
    cab_ped.bindValue(":lTraspasadoAlb",lTraspasadoAlb);
    cab_ped.bindValue(":lTraspasadoFac",lTraspasadoFac);
    cab_ped.bindValue(":cDireccionEntrega",cDireccionEntrega);
    cab_ped.bindValue(":cDireccionEntrega2",cDireccionEntrega2);
    cab_ped.bindValue(":cCPEntrega",cCPEntrega);
    cab_ped.bindValue(":cPoblacionEntrega",cPoblacionEntrega);
    cab_ped.bindValue(":cProvinciaEntrega",cProvinciaEntrega);
    cab_ped.bindValue(":cPaisEntrega",cPaisEntrega);
    cab_ped.bindValue(":lEnviado",lEnviado);
    cab_ped.bindValue(":lCompleto",lCompleto);
    cab_ped.bindValue(":lEntregado",lEntregado);
    cab_ped.bindValue(":dFechaLimiteEntrega",dFechaLimiteEntrega);
    cab_ped.bindValue(":rTotalPedido",rTotalPedido);
    cab_ped.bindValue(":id",nId_Pedido);
    if(!cab_ped.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("error al guardar datos Pedido:"), cab_ped.lastError().text());
        qDebug() << cab_ped.lastQuery();
        return false;
    }
    else
    {      
        return true;
    }
}

bool Pedidos::RecuperarPedido(QString cSQL)
{
    QSqlQuery cab_ped(QSqlDatabase::database("empresa"));
    cab_ped.prepare(cSQL);
    if( !cab_ped.exec() )
    {
        QMessageBox::critical(qApp->activeWindow(), "Error:", cab_ped.lastError().text());
        return false;
    }
    else
    {
        if (cab_ped.next())
        {
            QSqlRecord r = cab_ped.record();
            id = r.value("Id").toInt();
            nAlbaran = r.value("nAlbaran").toInt();
            nPedido = r.value("nPedido").toInt();
            dFecha = r.value("dFecha").toDate();
            cPedido = r.value("cPedido").toString();
            iId_Cliente = r.value("id_Cliente").toInt();
            cCodigoCliente = r.value("cCodigoCliente").toString();
            cCliente = r.value("cCliente").toString();
            cDireccion = r.value("cDireccion").toString();
            cDireccion2 = r.value("cDireccion2").toString();
            cPoblacion = r.value("cPoblacion").toString();
            cProvincia = r.value("cProvincia").toString();
            cCp = r.value("cCp").toString();
            cCif = r.value("cCif").toString();
            lRecargoEquivalencia = r.value("lRecargoEquivalencia").toInt();
            rSubtotal = r.value("rSubtotal").toDouble();
            rDto = r.value("rDto").toDouble();
            nDto = r.value("nDto").toDouble();
            rBase1 = r.value("rBase1").toDouble();
            rBase2 = r.value("rBase2").toDouble();
            rBase3 = r.value("rBase3").toDouble();
            rBase4 = r.value("rBase4").toDouble();
            nPorcentajeIVA1 = r.value("nPorcentajeIva1").toDouble();
            nPorcentajeIVA2 = r.value("nPorcentajeIva2").toDouble();
            nPorcentajeIVA3 = r.value("nPorcentajeIva3").toDouble();
            nPorcentajeIVA4 = r.value("nPorcentajeIva4").toDouble();
            rImporteIva1 = r.value("rImporteIva1").toDouble();
            rImporteIva2 = r.value("rImporteIva2").toDouble();
            rImporteIva3 = r.value("rImporteIva3").toDouble();
            rImporteIva4 = r.value("rImporteIva4").toDouble();
            nPorcentajeRecargoEq1 = r.value("nPorcentajeRecargoEq1").toDouble();
            nPorcentajeRecargoEq2 = r.value("nPorcentajeRecargoEq2").toDouble();
            nPorcentajeRecargoEq3 = r.value("nPorcentajeRecargoEq3").toDouble();
            nPorcentajeRecargoEq4 = r.value("nPorcentajeRecargoEq4").toDouble();
            rImporteRecargoEq1 = r.value("rImporteRecargoEq1").toDouble();
            rImporteRecargoEq2 = r.value("rImporteRecargoEq2").toDouble();
            rImporteRecargoEq3 = r.value("rImporteRecargoEq3").toDouble();
            rImporteRecargoEq4 = r.value("rImporteRecargoEq4").toDouble();
            rTotal1 = r.value("rTotal1").toDouble();
            rTotal2 = r.value("rTotal2").toDouble();
            rTotal3 = r.value("rTotal3").toDouble();
            rTotal4 = r.value("rTotal4").toDouble();
            rBaseTotal = r.value("rBaseTotal").toDouble();
            rIvaTotal = r.value("rIvaTotal").toDouble();
            rRecargoEqTotal = r.value("rRecargoEqTotal").toDouble();
            rTotalAlbaran = r.value("rTotalAlbaran").toDouble();
            lImpreso = r.value("lImpreso").toInt();
            lFacturado = r.value("lFacturado").toInt();
            cFactura = r.value("cFactura").toString();
            dFechaFactura = r.value("dFechaFactura").toDate();
            tComentario = r.value("tComentario").toString();
            rACuenta = r.value("rACuenta").toDouble();
            lTraspasadoAlb = r.value("lTraspasadoAlb").toInt();
            lTraspasadoFac = r.value("lTraspasadoFac").toInt();
            cDireccionEntrega = r.value("cDireccionEntrega").toString();
            cDireccionEntrega2 = r.value("cDireccionEntrega2").toString();
            cCPEntrega = r.value("cCPEntrega").toString();
            cPoblacionEntrega = r.value("cPoblacionEntrega").toString();
            cProvinciaEntrega = r.value("cProvinciaEntrega").toString();
            cPaisEntrega = r.value("cPaisEntrega").toString();
            lEnviado = r.value("lEnviado").toInt();
            lCompleto = r.value("lCompleto").toInt();
            lEntregado = r.value("lEntregado").toInt();
            dFechaLimiteEntrega = r.value("dFechaLimiteEntrega").toDate();
            rTotalPedido = r.value("rTotalPedido").toDouble();

            this->id_pais = r.field("idpais").value().toInt();
            QSqlQuery q(QSqlDatabase::database("empresa"));
            if(q.exec("SELECT * FROM paises WHERE id="+QString::number(id_pais)))
                if(q.next())
                    cPais = q.record().value("pais").toString();
           }
        else
            return false;
    }
    return true;
}

int Pedidos::NuevoNumeroPedido()
{
    QSqlQuery cab_ped(QSqlDatabase::database("empresa"));
    int nPedido = 0;
    cab_ped.prepare("Select nPedido from ped_cli order by nPedido desc limit 1");
    if(cab_ped.exec())
    {
        if(cab_ped.next())
        {
            nPedido= cab_ped.value(0).toInt();
            nPedido ++;
        }
    }
    return qMax(1,nPedido);
}

void Pedidos::FacturarPedido()
{
    //TODO Pedidos::FacturarPedido()
}
