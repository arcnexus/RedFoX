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

    QSqlQuery ped_cli(QSqlDatabase::database("empresa"));
     ped_cli.prepare("INSERT INTO ped_cli (nPedido,nPorcentajeIva1,nPorcentajeIva2,nPorcentajeIva3,nPorcentajeIva4,"
                     "nPorcentajeRecargoEq1,nPorcentajeRecargoEq2,nPorcentajeRecargoEq3,nPorcentajeRecargoEq4,id_Cliente)"
                   " VALUES (:nPedido,:nPorcentajeIva1,:nPorcentajeIva2,:nPorcentajeIva3,:nPorcentajeIva4,"
                     ":nPorcentajeRecargoEq1,:nPorcentajeRecargoEq2,:nPorcentajeRecargoEq3,:nPorcentajeRecargoEq4,:id_Cliente)");

     this->nPedido = NuevoNumeroPedido();
     ped_cli.bindValue(":nPedido",nPedido);
     ped_cli.bindValue(":nPorcentajeIva1",this->nPorcentajeIVA1);
     ped_cli.bindValue(":nPorcentajeIva2",this->nPorcentajeIVA2);
     ped_cli.bindValue(":nPorcentajeIva3",this->nPorcentajeIVA3);
     ped_cli.bindValue(":nPorcentajeIva4",this->nPorcentajeIVA4);
     ped_cli.bindValue(":nPorcentajeRecargoEq1",this->nPorcentajeRecargoEq1);
     ped_cli.bindValue(":nPorcentajeRecargoEq2",this->nPorcentajeRecargoEq1);
     ped_cli.bindValue(":nPorcentajeRecargoEq3",this->nPorcentajeRecargoEq1);
     ped_cli.bindValue(":nPorcentajeRecargoEq4",this->nPorcentajeRecargoEq1);
     ped_cli.bindValue(":id_Cliente",iId_Cliente);
     if(!ped_cli.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Pedido:", ped_cli.lastError().text());
         return false;
     }
     else
     {
         this->id = ped_cli.lastInsertId().toInt();
         return true;
     }
}
// Guardar el Pedido
bool Pedidos::GuardarPedido(int nId_Pedido)
{
    QSqlQuery ped_cli(QSqlDatabase::database("empresa"));

    ped_cli.prepare( "UPDATE ped_cli SET "
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
                     "lImpreso  =:lImpreso  ,lFacturado  =:lFacturado  , "
                     "dFechaFactura  =:dFechaFactura  ,  tComentario  =:tComentario  ,"
                     "rACuenta  =:rACuenta  ,lTraspasadoAlb  =:lTraspasadoAlb  ,"
                     "lTraspasadoFac  =:lTraspasadoFac  ,  cDireccionEntrega  =:cDireccionEntrega  ,"
                     "cDireccionEntrega2  =:cDireccionEntrega2  ,cCPEntrega  =:cCPEntrega  ,"
                     "cPoblacionEntrega  =:cPoblacionEntrega  ,  cProvinciaEntrega  =:cProvinciaEntrega  ,"
                     "cPaisEntrega  =:cPaisEntrega  ,lEnviado  =:lEnviado  ,  lCompleto  =:lCompleto  ,"
                     "lEntregado  =:lEntregado  ,  dFechaLimiteEntrega  =:dFechaLimiteEntrega  ,"
                     "rTotalPedido  =:rTotalPedido"
                     " WHERE Id = :id");

    ped_cli.bindValue(":nAlbaran",nAlbaran);
    ped_cli.bindValue(":nPedido",nPedido);
    ped_cli.bindValue(":dFecha",dFecha);
    ped_cli.bindValue(":cPedido",cPedido);
    ped_cli.bindValue(":id_Cliente",iId_Cliente);
    ped_cli.bindValue(":cCodigoCliente",cCodigoCliente);
    ped_cli.bindValue(":cCliente",cCliente);
    ped_cli.bindValue(":cDireccion",cDireccion);
    ped_cli.bindValue(":cDireccion2",cDireccion2);
    ped_cli.bindValue(":cPoblacion",cPoblacion);
    ped_cli.bindValue(":cProvincia",cProvincia);
    ped_cli.bindValue(":cCP",cCp);
    ped_cli.bindValue(":idpais",id_pais);
    ped_cli.bindValue(":cCif",cCif);
    ped_cli.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    ped_cli.bindValue(":rSubtotal",rSubtotal);
    ped_cli.bindValue(":rDto",rDto);
  //  ped_cli.bindValue(":nDto",nDto);
    ped_cli.bindValue(":rBase1",rBase1);
    ped_cli.bindValue(":rBase2",rBase2);
    ped_cli.bindValue(":rBase3",rBase3);
    ped_cli.bindValue(":rBase4",rBase4);
    ped_cli.bindValue(":nPorcentajeIva1",nPorcentajeIVA1);
    ped_cli.bindValue(":nPorcentajeIva2",nPorcentajeIVA2);
    ped_cli.bindValue(":nPorcentajeIva3",nPorcentajeIVA3);
    ped_cli.bindValue(":nPorcentajeIva4",nPorcentajeIVA4);
    ped_cli.bindValue(":rImporteIva1",rImporteIva1);
    ped_cli.bindValue(":rImporteIva2",rImporteIva2);
    ped_cli.bindValue(":rImporteIva3",rImporteIva3);
    ped_cli.bindValue(":rImporteIva4",rImporteIva4);
    ped_cli.bindValue(":nPorcentajeRecargoEq1",nPorcentajeRecargoEq1);
    ped_cli.bindValue(":nPorcentajeRecargoEq2",nPorcentajeRecargoEq2);
    ped_cli.bindValue(":nPorcentajeRecargoEq3",nPorcentajeRecargoEq3);
    ped_cli.bindValue(":nPorcentajeRecargoEq4",nPorcentajeRecargoEq4);
    ped_cli.bindValue(":rImporteRecargoEq1",rImporteRecargoEq1);
    ped_cli.bindValue(":rImporteRecargoEq2",rImporteRecargoEq2);
    ped_cli.bindValue(":rImporteRecargoEq3",rImporteRecargoEq3);
    ped_cli.bindValue(":rImporteRecargoEq4",rImporteRecargoEq4);
    ped_cli.bindValue(":rTotal1",rTotal1);
    ped_cli.bindValue(":rTotal2",rTotal2);
    ped_cli.bindValue(":rTotal3",rTotal3);
    ped_cli.bindValue(":rTotal4",rTotal4);
    ped_cli.bindValue(":rBaseTotal",rBaseTotal);
    ped_cli.bindValue(":rIvaTotal",rIvaTotal);
    ped_cli.bindValue(":rRecargoEqTotal",rRecargoEqTotal);
    ped_cli.bindValue(":rTotalAlbaran",rTotalAlbaran);
    ped_cli.bindValue(":lImpreso",lImpreso);
    ped_cli.bindValue(":lFacturado",lFacturado);
   // ped_cli.bindValue(":cFactura",cFactura);
    ped_cli.bindValue(":dFechaFactura",dFechaFactura);
    ped_cli.bindValue(":tComentario",tComentario);
    ped_cli.bindValue(":rACuenta",rACuenta);
    ped_cli.bindValue(":lTraspasadoAlb",lTraspasadoAlb);
    ped_cli.bindValue(":lTraspasadoFac",lTraspasadoFac);
    ped_cli.bindValue(":cDireccionEntrega",cDireccionEntrega);
    ped_cli.bindValue(":cDireccionEntrega2",cDireccionEntrega2);
    ped_cli.bindValue(":cCPEntrega",cCPEntrega);
    ped_cli.bindValue(":cPoblacionEntrega",cPoblacionEntrega);
    ped_cli.bindValue(":cProvinciaEntrega",cProvinciaEntrega);
    ped_cli.bindValue(":cPaisEntrega",cPaisEntrega);
    ped_cli.bindValue(":lEnviado",lEnviado);
    ped_cli.bindValue(":lCompleto",lCompleto);
    ped_cli.bindValue(":lEntregado",lEntregado);
    ped_cli.bindValue(":dFechaLimiteEntrega",dFechaLimiteEntrega);
    ped_cli.bindValue(":rTotalPedido",rTotalPedido);
    ped_cli.bindValue(":id",nId_Pedido);
    if(!ped_cli.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),QObject::tr("error al guardar datos Pedido:"), ped_cli.lastError().text());
        qDebug() << ped_cli.lastQuery();
        return false;
    }
    else
    {      
        return true;
    }
}

bool Pedidos::RecuperarPedido(QString cSQL)
{
    QSqlQuery ped_cli(QSqlDatabase::database("empresa"));
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
    QSqlQuery ped_cli(QSqlDatabase::database("empresa"));
    int nPedido = 0;
    ped_cli.prepare("Select nPedido from ped_cli order by nPedido desc limit 1");
    if(ped_cli.exec())
    {
        if(ped_cli.next())
        {
            nPedido= ped_cli.value(0).toInt();
            nPedido ++;
        }
    }
    return qMax(1,nPedido);
}

void Pedidos::FacturarPedido()
{
    //TODO Pedidos::FacturarPedido()
}
