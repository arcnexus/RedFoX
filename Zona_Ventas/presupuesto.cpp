#include "presupuesto.h"


Presupuesto::Presupuesto()
{
    this->id = 0;
}

Presupuesto::~Presupuesto()
{
}

bool Presupuesto::AnadirPresupuesto()
{
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();

    this->nIva1 = Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble();
    this->nIva2 = Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble();
    this->nIva3 = Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble();
    this->nIva4 = Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble();

    this->nRecargoEquivalencia1 = Configuracion_global->ivas[keys.at(0)].value("nRegargoEquivalencia").toDouble();
    this->nRecargoEquivalencia2 = Configuracion_global->ivas[keys.at(1)].value("nRegargoEquivalencia").toDouble();
    this->nRecargoEquivalencia3 = Configuracion_global->ivas[keys.at(2)].value("nRegargoEquivalencia").toDouble();
    this->nRecargoEquivalencia4 = Configuracion_global->ivas[keys.at(3)].value("nRegargoEquivalencia").toDouble();

    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    cab_pre.prepare("INSERT INTO cab_pre (nPresupuesto, dFecha, dValidoHasta,"
                    "id_Cliente, cCodigoCliente, cCliente, cCif, cDireccion, cDireccion2, cCP,"
                    "cPoblacion, cProvincia, idpais, cTelefono, cMovil, cFax, nDto, tComentarios,"
                    "rImporte, rSubtotal, rDescuento, rTotal, lImpreso, lAprobado, dFechaAprobacion,"
                    "rImporteFactura, rImportePendiente, cFactura, nAlbaran, nPedido, id_FormaPago,"
                    "tLugarEntrega, cAtencionde, rBase1, rBase2, rBase3, rBase4, nIva1, nIva2, nIva3,"
                    "nIva4, rIva1, rIva2, rIva3, rIva4, nRecargoEquivalencia1, nRecargoEquivalencia2,"
                    "nRecargoEquivalencia3, nRecargoEquivalencia4, rRec1, rRec2, rRec3, rRec4, rTotal1,"
                    "rTotal2, rTotal3, rTotal4, lRecargoEquivalencia, cEmail, rTotalIva, rTotalRec,"
                    "rImporte1, rImporte2, rImporte3, rImporte4) "
                    "VALUES"
                    "(:nPresupuesto, :dFecha, :dValidoHasta,"
                    ":id_Cliente, :cCodigoCliente, :cCliente, :cCif, :cDireccion, :cDireccion2, :cCP,"
                    ":cPoblacion, :cProvincia, :idpais, :cTelefono, :cMovil, :cFax, :nDto, :tComentarios,"
                    ":rImporte, :rSubtotal, :rDescuento, :rTotal, :lImpreso, :lAprobado, :dFechaAprobacion,"
                    ":rImporteFactura, :rImportePendiente, :cFactura, :nAlbaran, :nPedido, :id_FormaPago,"
                    ":tLugarEntrega, :cAtencionde, :rBase1, :rBase2, :rBase3, :rBase4, :nIva1, :nIva2, :nIva3,"
                    ":nIva4, :rIva1, :rIva2, :rIva3, :rIva4, :nRecargoEquivalencia1, :nRecargoEquivalencia2,"
                    ":nRecargoEquivalencia3, :nRecargoEquivalencia4, :rRec1, :rRec2, :rRec3, :rRec4, :rTotal1,"
                    ":rTotal2, :rTotal3, :rTotal4, :lRecargoEquivalencia, :cEmail, :rTotalIva, :rTotalRec,"
                    ":rImporte1, :rImporte2, :rImporte3, :rImporte4)");

    cab_pre.bindValue(":nPresupuesto",nPresupuesto);
    cab_pre.bindValue(":dFecha",dFecha);
    cab_pre.bindValue(":dValidoHasta",dValidoHasta);
    cab_pre.bindValue(":id_Cliente",id_cliente);
    cab_pre.bindValue(":cCodigoCliente",cCodigoCliente);
    cab_pre.bindValue(":cCliente",cCliente);
    cab_pre.bindValue(":cCif",cCif);
    cab_pre.bindValue(":cDireccion",cDireccion);
    cab_pre.bindValue(":cDireccion2",cDireccion2);
    cab_pre.bindValue(":cCP",cCP);
    cab_pre.bindValue(":cPoblacion",cPoblacion);
    cab_pre.bindValue(":cProvincia",cProvincia);
    cab_pre.bindValue(":idpais",idPais);
    cab_pre.bindValue(":cTelefono",cTelefono);
    cab_pre.bindValue(":cMovil",cMovil);
    cab_pre.bindValue(":cFax",cFax);
    cab_pre.bindValue(":nDto",nDto);
    cab_pre.bindValue(":tComentarios",tComentarios);
    cab_pre.bindValue(":rImporte",rImporte);
    cab_pre.bindValue(":rSubtotal",rSubTotal);
    cab_pre.bindValue(":rDescuento",rDescuento);
    cab_pre.bindValue(":rTotal",rTotal);
    cab_pre.bindValue(":lImpreso",lImpreso);
    cab_pre.bindValue(":lAprobado",lAprobado);
    cab_pre.bindValue(":dFechaAprobacion",dFechaAprobacion);
    cab_pre.bindValue(":rImporteFactura",rImporteFactura);
    cab_pre.bindValue(":rImportePendiente",rImportePendiente);
    cab_pre.bindValue(":cFactura",cFactura);
    cab_pre.bindValue(":nAlbaran",nAlbaran);
    cab_pre.bindValue(":nPedido",nPedido);
    cab_pre.bindValue(":id_FormaPago",id_FormaPago);
    cab_pre.bindValue(":tLugarEntrega",tLugarEntrega);
    cab_pre.bindValue(":cAtencionde",cAtencionde);
    cab_pre.bindValue(":rBase1",rBase1);
    cab_pre.bindValue(":rBase2",rBase2);
    cab_pre.bindValue(":rBase3",rBase3);
    cab_pre.bindValue(":rBase4",rBase4);
    cab_pre.bindValue(":nIva1",nIva1);
    cab_pre.bindValue(":nIva2",nIva2);
    cab_pre.bindValue(":nIva3",nIva3);
    cab_pre.bindValue(":nIva4",nIva4);
    cab_pre.bindValue(":rIva1",rIva1);
    cab_pre.bindValue(":rIva2",rIva2);
    cab_pre.bindValue(":rIva3",rIva3);
    cab_pre.bindValue(":rIva4",rIva4);
    cab_pre.bindValue(":nRecargoEquivalencia1",nRecargoEquivalencia1);
    cab_pre.bindValue(":nRecargoEquivalencia2",nRecargoEquivalencia2);
    cab_pre.bindValue(":nRecargoEquivalencia3",nRecargoEquivalencia3);
    cab_pre.bindValue(":nRecargoEquivalencia4",nRecargoEquivalencia4);
    cab_pre.bindValue(":rRec1",rRec1);
    cab_pre.bindValue(":rRec2",rRec2);
    cab_pre.bindValue(":rRec3",rRec3);
    cab_pre.bindValue(":rRec4",rRec4);
    cab_pre.bindValue(":rTotal1",rTotal1);
    cab_pre.bindValue(":rTotal2",rTotal2);
    cab_pre.bindValue(":rTotal3",rTotal3);
    cab_pre.bindValue(":rTotal4",rTotal4);
    cab_pre.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    cab_pre.bindValue(":cEmail",cEmail);
    cab_pre.bindValue(":rTotalIva",rTotalIva);
    cab_pre.bindValue(":rTotalRec",rTotalRec);
    cab_pre.bindValue(":rImporte1",rImporte1);
    cab_pre.bindValue(":rImporte2",rImporte2);
    cab_pre.bindValue(":rImporte3",rImporte3);
    cab_pre.bindValue(":rImporte4",rImporte4);

     if(!cab_pre.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Presupuesto:", cab_pre.lastError().text());
         return false;
     }
     else
     {         
         this->id = cab_pre.lastInsertId().toInt();
         QString cSQL = "Select * from cab_pre where id ="+QString::number(this->id);
         RecuperarPresupuesto(cSQL);
         return true;
     }
}

bool Presupuesto::RecuperarPresupuesto(QString cSQL)
{
    QSqlQuery qCab_pre(QSqlDatabase::database("empresa"));
    qCab_pre.prepare(cSQL);
    if( !qCab_pre.exec() )
    {
        return false;
    }
    else
    {
        if (qCab_pre.next())
        {
            QSqlRecord registro = qCab_pre.record();
            this->id = registro.field("Id").value().toInt();
            this->nPresupuesto = registro.field("nPresupuesto").value().toInt();
            this->dFecha = registro.field("dFecha").value().toDate();
            this->dValidoHasta = registro.field("dValidoHasta").value().toDate();
            this->id_cliente = registro.field("id_cliente").value().toInt();
            this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
            this->cCliente = registro.field("cCliente").value().toString();
            this->cCif = registro.field("cCif").value().toString();
            this->cDireccion = registro.field("cDireccion").value().toString();
            this->cDireccion2 = registro.field("cDireccion2").value().toString();
            this->cCP = registro.field("cCP").value().toString();
            this->cPoblacion = registro.field("cPoblacion").value().toString();
            this->cProvincia = registro.field("cProvincia").value().toString();
            this->idPais = registro.field("idpais").value().toInt();
            this->cTelefono = registro.field("cTelefono").value().toString();
            this->cMovil = registro.field("cMovil").value().toString();
            this->cFax = registro.field("cFax").value().toString();
            this->cEmail = registro.field("cEmail").value().toString();
            this->nDto = registro.field("nDto").value().toDouble();
            this->tComentarios = registro.field("tComentarios").value().toString();
            this->rImporte = registro.field("rImporte").value().toDouble();
            this->rSubTotal = registro.field("rSubtotal").value().toDouble();
            this->rDescuento = registro.field("rDescuento").value().toDouble();
            this->rTotal = registro.field("rTotal").value().toDouble();
            this->lImpreso = registro.field("lImpreso").value().toBool();
            this->lAprobado = registro.field("lAprobado").value().toBool();
            this->dFechaAprobacion = registro.field("dFechaAprobacion").value().toDate();
            this->rImporteFactura = registro.field("rImporteFactura").value().toDouble();
            this->cFactura = registro.field("cFactura").value().toString();
            this->nAlbaran = registro.field("nAlbaran").value().toInt();
            this->nPedido = registro.field("nPedido").value().toInt();

            this->id_FormaPago = registro.field("id_FormaPago").value().toInt();
            QSqlQuery q(QSqlDatabase::database("empresa"));
            if(q.exec("SELECT * FROM formpago WHERE id = "+QString::number(id_FormaPago)))
                if(q.next())
                {
                    this->cCodigoFormaPago = q.record().value("cCodigo").toString();
                    this->cDescripcionFormaPago = q.record().value("cFormapago").toString();
                }

            this->tLugarEntrega = registro.field("tLugarEntrega").value().toString();
            this->cAtencionde = registro.field("cAtencionde").value().toString();
            this->rBase1 = registro.field("rImporte1").value().toDouble();
            this->rBase2 = registro.field("rImporte2").value().toDouble();
            this->rBase3 = registro.field("rImporte3").value().toDouble();
            this->rBase4 = registro.field("rImporte4").value().toDouble();
            this->nIva1 = registro.field("nIva1").value().toDouble();
            this->nIva2 = registro.field("nIva2").value().toDouble();
            this->nIva3 = registro.field("nIva3").value().toDouble();
            this->nIva4 = registro.field("nIva4").value().toDouble();
            this->rIva1 = registro.field("rIva1").value().toDouble();
            this->rIva2 = registro.field("rIva2").value().toDouble();
            this->rIva3 = registro.field("rIva3").value().toDouble();
            this->rIva4 = registro.field("rIva4").value().toDouble();
            this->rTotalIva = registro.field("rTotalIva").value().toDouble();
            this->nRecargoEquivalencia1 = registro.field("nRecargoEquivalencia1").value().toDouble();
            this->nRecargoEquivalencia2 = registro.field("nRecargoEquivalencia2").value().toDouble();
            this->nRecargoEquivalencia3 = registro.field("nRecargoEquivalencia3").value().toDouble();
            this->nRecargoEquivalencia4 = registro.field("nRecargoEquivalencia4").value().toDouble();
            this->rRec1 = registro.field("rRec1").value().toDouble();
            this->rRec2 = registro.field("rRec2").value().toDouble();
            this->rRec3 = registro.field("rRec3").value().toDouble();
            this->rRec4 = registro.field("rRec4").value().toDouble();
            this->rTotalRec = registro.field("rTotalRec").value().toDouble();
            this->rTotal1 = registro.field("rTotal1").value().toDouble();
            this->rTotal2 = registro.field("rTotal2").value().toDouble();
            this->rTotal3 = registro.field("rTotal3").value().toDouble();
            this->rTotal4 = registro.field("rTotal4").value().toDouble();
            this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toBool();
            return true;
        }
        else
        {
         return false;
        }
    }
}

bool Presupuesto::siguiente()
{
    return RecuperarPresupuesto("Select * from cab_pre where nPresupuesto >'"+
                                    QString::number(nPresupuesto)+"' order by nPresupuesto  limit 1 ");
}

bool Presupuesto::anterior()
{
   return RecuperarPresupuesto("Select * from cab_pre where nPresupuesto <'"+QString::number(nPresupuesto)+
                               "' order by nPresupuesto desc limit 1 ");
}

bool Presupuesto::GuardarPres(int nId_Presupuesto)
{

    if (this->nPresupuesto == 0)
        this->nPresupuesto = NuevoNumeroPresupuesto();

    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    cab_pre.prepare("UPDATE cab_pre SET "
                    "dFecha = :dFecha, dValidoHasta = :dValidoHasta, id_Cliente = :id_Cliente,"
                    "cCodigoCliente =  :cCodigoCliente, cCliente = :cCliente, cCif = :cCif,"
                    "cDireccion = :cDireccion, cDireccion2 = :cDireccion2, cCP = :cCP,"
                    "cPoblacion = :cPoblacion, cProvincia = :cProvincia, idpais = :idpais,"
                    "cTelefono = :cTelefono, cMovil = :cMovil, cFax = :cFax, nDto = :nDto,"
                    "tComentarios = :tComentarios, rImporte = :rImporte, rSubtotal =:rSubtotal,"
                    "rDescuento = :rDescuento, rTotal = :rTotal, lImpreso = :lImpreso,"
                    "lAprobado = :lAprobado, dFechaAprobacion = :dFechaAprobacion,"
                    "rImporteFactura =:rImporteFactura, rImportePendiente = :rImportePendiente,"
                    "cFactura = :cFactura, nAlbaran = :nAlbaran, nPedido=:nPedido,"
                    "id_FormaPago =:id_FormaPago, tLugarEntrega=:tLugarEntrega,"
                    "cAtencionde=:cAtencionde, rBase1=:rBase1, rBase2=:rBase2, rBase3=:rBase3,"
                    "rBase4=:rBase4, nIva1=:nIva1, nIva2=:nIva2, nIva3=:nIva3,"
                    "nIva4=:nIva4, rIva1=:rIva1, rIva2=:rIva2, rIva3=:rIva3, rIva4=:rIva4,"
                    "nRecargoEquivalencia1=:nRecargoEquivalencia1,"
                    "nRecargoEquivalencia2=:nRecargoEquivalencia2,"
                    "nRecargoEquivalencia3=:nRecargoEquivalencia3,"
                    "nRecargoEquivalencia4=:nRecargoEquivalencia4,"
                    "rRec1=:rRec1, rRec2=:rRec2, rRec3=:rRec3, rRec4=:rRec4, rTotal1=:rTotal1,"
                    "rTotal2=:rTotal2, rTotal3=:rTotal3, rTotal4=:rTotal4,"
                    "lRecargoEquivalencia=:lRecargoEquivalencia, cEmail=:cEmail,"
                    "rTotalIva=:rTotalIva, rTotalRec=:rTotalRec,"
                    "rImporte1=:rImporte1, rImporte2=:rImporte2,"
                    "rImporte3=:rImporte3, rImporte4=:rImporte4 "
                    "WHERE nPresupuesto = :nnPresupuesto");

    cab_pre.bindValue(":dFecha",dFecha);
    cab_pre.bindValue(":dValidoHasta",dValidoHasta);
    cab_pre.bindValue(":id_Cliente",id_cliente);
    cab_pre.bindValue(":cCodigoCliente",cCodigoCliente);
    cab_pre.bindValue(":cCliente",cCliente);
    cab_pre.bindValue(":cCif",cCif);
    cab_pre.bindValue(":cDireccion",cDireccion);
    cab_pre.bindValue(":cDireccion2",cDireccion2);
    cab_pre.bindValue(":cCP",cCP);
    cab_pre.bindValue(":cPoblacion",cPoblacion);
    cab_pre.bindValue(":cProvincia",cProvincia);
    cab_pre.bindValue(":idpais",idPais);
    cab_pre.bindValue(":cTelefono",cTelefono);
    cab_pre.bindValue(":cMovil",cMovil);
    cab_pre.bindValue(":cFax",cFax);
    cab_pre.bindValue(":nDto",nDto);
    cab_pre.bindValue(":tComentarios",tComentarios);
    cab_pre.bindValue(":rImporte",rImporte);
    cab_pre.bindValue(":rSubtotal",rSubTotal);
    cab_pre.bindValue(":rDescuento",rDescuento);
    cab_pre.bindValue(":rTotal",rTotal);
    cab_pre.bindValue(":lImpreso",lImpreso);
    cab_pre.bindValue(":lAprobado",lAprobado);
    cab_pre.bindValue(":dFechaAprobacion",dFechaAprobacion);
    cab_pre.bindValue(":rImporteFactura",rImporteFactura);
    cab_pre.bindValue(":rImportePendiente",rImportePendiente);
    cab_pre.bindValue(":cFactura",cFactura);
    cab_pre.bindValue(":nAlbaran",nAlbaran);
    cab_pre.bindValue(":nPedido",nPedido);
    cab_pre.bindValue(":id_FormaPago",id_FormaPago);
    cab_pre.bindValue(":tLugarEntrega",tLugarEntrega);
    cab_pre.bindValue(":cAtencionde",cAtencionde);
    cab_pre.bindValue(":rBase1",rBase1);
    cab_pre.bindValue(":rBase2",rBase2);
    cab_pre.bindValue(":rBase3",rBase3);
    cab_pre.bindValue(":rBase4",rBase4);
    cab_pre.bindValue(":nIva1",nIva1);
    cab_pre.bindValue(":nIva2",nIva2);
    cab_pre.bindValue(":nIva3",nIva3);
    cab_pre.bindValue(":nIva4",nIva4);
    cab_pre.bindValue(":rIva1",rIva1);
    cab_pre.bindValue(":rIva2",rIva2);
    cab_pre.bindValue(":rIva3",rIva3);
    cab_pre.bindValue(":rIva4",rIva4);
    cab_pre.bindValue(":nRecargoEquivalencia1",nRecargoEquivalencia1);
    cab_pre.bindValue(":nRecargoEquivalencia2",nRecargoEquivalencia2);
    cab_pre.bindValue(":nRecargoEquivalencia3",nRecargoEquivalencia3);
    cab_pre.bindValue(":nRecargoEquivalencia4",nRecargoEquivalencia4);
    cab_pre.bindValue(":rRec1",rRec1);
    cab_pre.bindValue(":rRec2",rRec2);
    cab_pre.bindValue(":rRec3",rRec3);
    cab_pre.bindValue(":rRec4",rRec4);
    cab_pre.bindValue(":rTotal1",rTotal1);
    cab_pre.bindValue(":rTotal2",rTotal2);
    cab_pre.bindValue(":rTotal3",rTotal3);
    cab_pre.bindValue(":rTotal4",rTotal4);
    cab_pre.bindValue(":lRecargoEquivalencia",lRecargoEquivalencia);
    cab_pre.bindValue(":cEmail",cEmail);
    cab_pre.bindValue(":rTotalIva",rTotalIva);
    cab_pre.bindValue(":rTotalRec",rTotalRec);
    cab_pre.bindValue(":rImporte1",rImporte1);
    cab_pre.bindValue(":rImporte2",rImporte2);
    cab_pre.bindValue(":rImporte3",rImporte3);
    cab_pre.bindValue(":rImporte4",rImporte4);
    cab_pre.bindValue(":nnPresupuesto",nId_Presupuesto);

    if(!cab_pre.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Presupuesto:", cab_pre.lastError().text());
        return false;
    }
    else
    {
        this->id = cab_pre.lastInsertId().toInt();
        QString cSQL = "Select * from cab_pre where id ="+QString::number(this->id);
        RecuperarPresupuesto(cSQL);
        return true;
    }
}

bool Presupuesto::BorrarLineas(int nId_Presupuesto)
{
    QSqlQuery query(QSqlDatabase::database("empresa"));
    QString sql = QString("DELETE FROM lin_pre WHERE Id_Cab = %1").arg(nId_Presupuesto);
    query.prepare(sql);
    if(query.exec())
        return true;
    else
    {
        QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
        return false;
    }
}

int Presupuesto::NuevoNumeroPresupuesto()
{
    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    int nPresupuesto;
    cab_pre.prepare("Select nPresupuesto from cab_pre order by nPresupuesto desc limit 1");
    if(cab_pre.exec()) {
        cab_pre.next();
        nPresupuesto= cab_pre.value(0).toInt();
        nPresupuesto ++;
    } else {
         QMessageBox::critical(qApp->activeWindow(), "Error:", cab_pre.lastError().text());
    }
    return nPresupuesto;
}

void Presupuesto::PresupuestoToPedido()
{
}

void Presupuesto::PresupuestoToAlbaran()
{
}

void Presupuesto::PresupuestoToFactura()
{
}
