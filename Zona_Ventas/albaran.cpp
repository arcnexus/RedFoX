#include "albaran.h"
//

Albaran::Albaran(QObject *parent) :
    QObject(parent),
    id(0)
{
    //this->id =0;
}

Albaran::~Albaran()
{

}
// Metodos utilidad Clase
bool Albaran::AnadirAlbaran()
{
    int x = NuevoNumeroAlbaran();
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("INSERT INTO cab_alb"
              "(nAlbaran, dFecha, cPedidoCli, id_Cliente, cCodigoCliente, cCliente,"
              "cDireccion, cDireccion2, cPoblacion, cProvincia, cCP, idpais, cCif,"
              "lRecargoEquivalencia, rSubtotal, rDto, nDto, rBase1, rBase2, rBase3,"
              "rBase4, nPorcentajeIva1, nPorcentajeIva2, nPorcentajeIva3, nPorcentajeIva4,"
              "rImporteIva1, rImporteIva2, rImporteIva3, rImporteIva4, nPorcentajeRecargoEq1,"
              "nPorcentajeRecargoEq2, nPorcentajeRecargoEq3, nPorcentajeRecargoEq4,"
              "rImporteRecargoEq1, rImporteRecargoEq2, rImporteRecargoEq3, rImporteRecargoEq4,"
              "rTotal1, rTotal2, rTotal3, rTotal4, rBaseTotal, rIvaTotal, rRecargoEqTotal,"
              "rTotalAlbaran, lImpreso, lFacturado, cFactura, dFechaFactura, tComentario,"
              "rACuenta) "
              "VALUES"
              "(:nAlbaran, :dFecha, :cPedidoCli, :id_Cliente, :cCodigoCliente, :cCliente,"
              ":cDireccion, :cDireccion2, :cPoblacion, :cProvincia, :cCP, :idpais, :cCif,"
              ":lRecargoEquivalencia, :rSubtotal, :rDto, :nDto, :rBase1, :rBase2, :rBase3,"
              ":rBase4, :nPorcentajeIva1, :nPorcentajeIva2, :nPorcentajeIva3, :nPorcentajeIva4,"
              ":rImporteIva1, :rImporteIva2, :rImporteIva3, :rImporteIva4, :nPorcentajeRecargoEq1,"
              ":nPorcentajeRecargoEq2, :nPorcentajeRecargoEq3, :nPorcentajeRecargoEq4,"
              ":rImporteRecargoEq1, :rImporteRecargoEq2, :rImporteRecargoEq3, :rImporteRecargoEq4,"
              ":rTotal1, :rTotal2, :rTotal3, :rTotal4, :rBaseTotal, :rIvaTotal, :rRecargoEqTotal,"
              ":rTotalAlbaran, :lImpreso, :lFacturado, :cFactura, :dFechaFactura, :tComentario,"
              ":rACuenta) ");
    q.bindValue(":nAlbaran",x);
    q.bindValue(":dFecha",dFecha);
    q.bindValue(":cPedidoCli",cPedidoCli);
    q.bindValue(":id_Cliente",iId_Cliente);
    q.bindValue(":cCodigoCliente",cCodigoCliente);
    q.bindValue(":cCliente",cCliente);

    q.bindValue(":cDireccion",cDireccion);
    q.bindValue(":cDireccion2",cDireccion2);
    q.bindValue(":cPoblacion",cPoblacion);
    q.bindValue(":cProvincia",cProvincia);
    q.bindValue(":cCP",cCp);
    q.bindValue(":idpais",idPais);
    q.bindValue(":cCif",cCif);

    q.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    q.bindValue(":rSubtotal",0);
    q.bindValue(":rDto",0);
    q.bindValue(":nDto",0);
    q.bindValue(":rBase1",0);
    q.bindValue(":rBase2",0);
    q.bindValue(":rBase3",0);
    q.bindValue(":rBase4",0);
    q.bindValue(":nPorcentajeIva1",0);
    q.bindValue(":nPorcentajeIva2",0);
    q.bindValue(":nPorcentajeIva3",0);
    q.bindValue(":nPorcentajeIva4",0);

    q.bindValue(":rImporteIva1",0);
    q.bindValue(":rImporteIva2",0);
    q.bindValue(":rImporteIva3",0);
    q.bindValue(":rImporteIva4",0);
    q.bindValue(":nPorcentajeRecargoEq1",0);
    q.bindValue(":nPorcentajeRecargoEq2",0);
    q.bindValue(":nPorcentajeRecargoEq3",0);
    q.bindValue(":nPorcentajeRecargoEq4",0);

    q.bindValue(":rImporteRecargoEq1",0);
    q.bindValue(":rImporteRecargoEq2",0);
    q.bindValue(":rImporteRecargoEq3",0);
    q.bindValue(":rImporteRecargoEq4",0);

    q.bindValue(":rTotal1",0);
    q.bindValue(":rTotal2",0);
    q.bindValue(":rTotal3",0);
    q.bindValue(":rTotal4",0);
    q.bindValue(":rBaseTotal",0);
    q.bindValue(":rIvaTotal",0);
    q.bindValue(":rRecargoEqTotal",0);

    q.bindValue(":rTotalAlbaran",0);
    q.bindValue(":lImpreso",0);
    q.bindValue(":lFacturado",0);
    q.bindValue(":cFactura",cFactura);
    q.bindValue(":dFechaFactura",dFechaFactura);
    q.bindValue(":tComentario",tComentario);

    q.bindValue(":rACuenta",0);

    if(!q.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Albaran:", q.lastError().text());
        return false;
    }
    else
    {
        this->id = q.lastInsertId().toInt();
        return true;
    }
}

bool Albaran::GuardarAlbaran(int nId_Albaran)
{
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("UPDATE cab_alb SET "
              "dFecha=:dFecha, cPedidoCli=:cPedidoCli, id_Cliente=:id_Cliente,"
              "cCodigoCliente=:cCodigoCliente, cCliente=:cCliente,"
              "cDireccion=:cDireccion,cDireccion2=:cDireccion2, cPoblacion=:cPoblacion,"
              "cProvincia=:cProvincia, cCP=:cCP, idpais=:idpais, cCif=:cCif,"
              "lRecargoEquivalencia=:lRecargoEquivalencia, rSubtotal=:rSubtotal,"
              "rDto=:rDto, nDto=:nDto, rBase1=:rBase1, rBase2=:rBase2, rBase3=:rBase3,"
              "rBase4=:rBase4, nPorcentajeIva1=:nPorcentajeIva1, nPorcentajeIva2=:nPorcentajeIva2,"
              "nPorcentajeIva3=:nPorcentajeIva3, nPorcentajeIva4=:nPorcentajeIva4,"
              "rImporteIva1=:rImporteIva1, rImporteIva2=:rImporteIva2, rImporteIva3=:rImporteIva3,"
              "rImporteIva4=:rImporteIva4, nPorcentajeRecargoEq1=:nPorcentajeRecargoEq1,"
              "nPorcentajeRecargoEq2=:nPorcentajeRecargoEq2, nPorcentajeRecargoEq3=:nPorcentajeRecargoEq3,"
              "nPorcentajeRecargoEq4=:nPorcentajeRecargoEq4,"
              "rImporteRecargoEq1=:rImporteRecargoEq1, rImporteRecargoEq2=:rImporteRecargoEq2,"
              "rImporteRecargoEq3=:rImporteRecargoEq3, rImporteRecargoEq4=:rImporteRecargoEq4,"
              "rTotal1=:rTotal1, rTotal2=:rTotal2, rTotal3=:rTotal3, rTotal4=:rTotal4,"
              "rBaseTotal=:rBaseTotal, rIvaTotal=:rIvaTotal, rRecargoEqTotal=:rRecargoEqTotal,"
              "rTotalAlbaran=:rTotalAlbaran, lImpreso=:lImpreso, lFacturado=:lFacturado,"
              "cFactura=:cFactura, dFechaFactura=:dFechaFactura, tComentario=:tComentario,"
              "rACuenta=:rACuenta"
              " WHERE Id = :nAlbaran");

    q.bindValue(":nAlbaran",nId_Albaran);
    q.bindValue(":dFecha",dFecha);
    q.bindValue(":cPedidoCli",cPedidoCli);
    q.bindValue(":id_Cliente",iId_Cliente);
    q.bindValue(":cCodigoCliente",cCodigoCliente);
    q.bindValue(":cCliente",cCliente);

    q.bindValue(":cDireccion",cDireccion);
    q.bindValue(":cDireccion2",cDireccion2);
    q.bindValue(":cPoblacion",cPoblacion);
    q.bindValue(":cProvincia",cProvincia);
    q.bindValue(":cCP",cCp);
    q.bindValue(":idpais",idPais);
    q.bindValue(":cCif",cCif);

    q.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    q.bindValue(":rSubtotal",rSubtotal);
    q.bindValue(":rDto",rDto);
    q.bindValue(":nDto",nDto);
    q.bindValue(":rBase1",rBase1);
    q.bindValue(":rBase2",rBase2);
    q.bindValue(":rBase3",rBase3);
    q.bindValue(":rBase4",rBase4);
    q.bindValue(":nPorcentajeIva1",nPorcentajeIVA1);
    q.bindValue(":nPorcentajeIva2",nPorcentajeIVA2);
    q.bindValue(":nPorcentajeIva3",nPorcentajeIVA3);
    q.bindValue(":nPorcentajeIva4",nPorcentajeIVA4);

    q.bindValue(":rImporteIva1",rImporteIva1);
    q.bindValue(":rImporteIva2",rImporteIva2);
    q.bindValue(":rImporteIva3",rImporteIva3);
    q.bindValue(":rImporteIva4",rImporteIva4);
    q.bindValue(":nPorcentajeRecargoEq1",nPorcentajeRecargoEq1);
    q.bindValue(":nPorcentajeRecargoEq2",nPorcentajeRecargoEq2);
    q.bindValue(":nPorcentajeRecargoEq3",nPorcentajeRecargoEq3);
    q.bindValue(":nPorcentajeRecargoEq4",nPorcentajeRecargoEq4);

    q.bindValue(":rImporteRecargoEq1",rImporteRecargoEq1);
    q.bindValue(":rImporteRecargoEq2",rImporteRecargoEq2);
    q.bindValue(":rImporteRecargoEq3",rImporteRecargoEq3);
    q.bindValue(":rImporteRecargoEq4",rImporteRecargoEq4);

    q.bindValue(":rTotal1",rTotal1);
    q.bindValue(":rTotal2",rTotal2);
    q.bindValue(":rTotal3",rTotal3);
    q.bindValue(":rTotal4",rTotal4);
    q.bindValue(":rBaseTotal",rBaseTotal);
    q.bindValue(":rIvaTotal",rIvaTotal);
    q.bindValue(":rRecargoEqTotal",rRecargoEqTotal);

    q.bindValue(":rTotalAlbaran",rTotalAlbaran);
    q.bindValue(":lImpreso",lImpreso);
    q.bindValue(":lFacturado",lFacturado);
    q.bindValue(":cFactura",cFactura);
    q.bindValue(":dFechaFactura",dFechaFactura);
    q.bindValue(":tComentario",tComentario);

    q.bindValue(":rACuenta",rACuenta);

    if(!q.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Albaran:", q.lastError().text());
        qDebug() << q.lastError();
        qDebug() << q.executedQuery();
        return false;
    }
    else
    {
        return true;
    }
}

bool Albaran::RecuperarAlbaran(QString cSQL)
{
        QSqlQuery cab_alb(QSqlDatabase::database("empresa"));
        cab_alb.prepare(cSQL);
        if( !cab_alb.exec() )
        {
            QMessageBox::critical(qApp->activeWindow(), "Error:", cab_alb.lastError().text());
            return false;
        }
        else
        {
            if (cab_alb.next())
            {
                QSqlRecord r = cab_alb.record();
                id = r.value("Id").toInt();;
                nAlbaran= r.value("nAlbaran").toInt();
                dFecha= r.value("dFecha").toDate();
                cPedidoCli= r.value("cPedidoCli").toString();
                iId_Cliente= r.value("id_Cliente").toInt();
                cCodigoCliente= r.value("cCodigoCliente").toString();
                cCliente= r.value("cCliente").toString();
                cDireccion= r.value("cDireccion").toString();
                cDireccion2= r.value("cDireccion2").toString();
                cPoblacion= r.value("cPoblacion").toString();
                cProvincia= r.value("cProvincia").toString();
                cCp= r.value("cCp").toString();
                idPais= r.value("idPais").toInt();
                //cPais= r.value("");
                cCif= r.value("cCif").toString();
                lRecargoEquivalencia= r.value("lRecargoEquivalencia").toInt();
                rSubtotal= r.value("rSubtotal").toDouble();
                nDto= r.value("nDto").toDouble();
                rDto= r.value("rDto").toDouble();
                rBase1= r.value("rBase1").toDouble();
                rBase2= r.value("rBase2").toDouble();
                rBase3= r.value("rBase3").toDouble();
                rBase4= r.value("rBase4").toDouble();
                nPorcentajeIVA1= r.value("nPorcentajeIVA1").toDouble();
                nPorcentajeIVA2= r.value("nPorcentajeIVA2").toDouble();
                nPorcentajeIVA3= r.value("nPorcentajeIVA3").toDouble();
                nPorcentajeIVA4= r.value("nPorcentajeIVA4").toDouble();
                rImporteIva1= r.value("rImporteIva1").toDouble();
                rImporteIva2= r.value("rImporteIva2").toDouble();
                rImporteIva3= r.value("rImporteIva3").toDouble();
                rImporteIva4= r.value("rImporteIva4").toDouble();
                nPorcentajeRecargoEq1= r.value("nPorcentajeRecargoEq1").toDouble();
                nPorcentajeRecargoEq2= r.value("nPorcentajeRecargoEq2").toDouble();
                nPorcentajeRecargoEq3= r.value("nPorcentajeRecargoEq3").toDouble();
                nPorcentajeRecargoEq4= r.value("nPorcentajeRecargoEq4").toDouble();
                rImporteRecargoEq1= r.value("rImporteRecargoEq1").toDouble();
                rImporteRecargoEq2= r.value("rImporteRecargoEq2").toDouble();
                rImporteRecargoEq3= r.value("rImporteRecargoEq3").toDouble();
                rImporteRecargoEq4= r.value("rImporteRecargoEq4").toDouble();
                rTotal1= r.value("rTotal1").toDouble();
                rTotal2= r.value("rTotal2").toDouble();
                rTotal3= r.value("rTotal3").toDouble();
                rTotal4= r.value("rTotal4").toDouble();
                rBaseTotal= r.value("rBaseTotal").toDouble();
                rIvaTotal= r.value("rIvaTotal").toDouble();
                rRecargoEqTotal= r.value("rRecargoEqTotal").toDouble();
                rTotalAlbaran= r.value("rTotalAlbaran").toDouble();
                lImpreso= r.value("lImpreso").toInt();
                lFacturado= r.value("lFacturado").toInt();
                cFactura= r.value("cFactura").toString();
                dFechaFactura= r.value("dFechaFactura").toDate();
                tComentario= r.value("tComentario").toString();
                rACuenta= r.value("rACuenta").toDouble();
                return true;
               }
            else //if not next
                return false;
        }
}

int Albaran::NuevoNumeroAlbaran()
{
    QSqlQuery cab_alb(QSqlDatabase::database("empresa"));
    int nAlbaran = 1;
    cab_alb.prepare("Select nAlbaran from cab_alb order by nAlbaran desc limit 1");
    if(cab_alb.exec())
    {
        cab_alb.next();
        nAlbaran= cab_alb.value(0).toInt();
        nAlbaran ++;
    }
    else
    {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_alb.lastError().text());
    }
    return nAlbaran;
}

void Albaran::FacturarAlbaran()
{
    //TODO Albaran::FacturarAlbaran()
}
