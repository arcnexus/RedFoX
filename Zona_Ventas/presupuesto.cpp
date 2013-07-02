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

    this->iva1 = Configuracion_global->ivaList.at(0).toDouble();
    this->iva2 = Configuracion_global->ivaList.at(1).toDouble();
    this->iva3 = Configuracion_global->ivaList.at(2).toDouble();
    this->iva4 = Configuracion_global->ivaList.at(3).toDouble();

    this->porc_porc_recuivalencia1 = Configuracion_global->reList.at(0).toDouble();
    this->porc_porc_recuivalencia2 = Configuracion_global->reList.at(1).toDouble();
    this->porc_porc_recuivalencia3 = Configuracion_global->reList.at(2).toDouble();
    this->porc_porc_recuivalencia4 = Configuracion_global->reList.at(3).toDouble();

    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    cab_pre.prepare("INSERT INTO cab_pre (nPresupuesto, fecha, dValidoHasta,"
                    "id_cliente, codigo_cliente, cliente, cif, direccion1, direccion2, cp,"
                    "poblacion, provincia, id_pais, telefono, movil, fax, dto, comentarios,"
                    "importe, base, rDescuento, total, impreso, lAprobado, fechaAprobacion,"
                    "importeFactura, importe_pendiente, factura, albaran, pedido, id_forma_pago,"
                    "tLugarEntrega, cAtencionde, base1, base2, base3, base4, iva1, iva2, iva3,"
                    "iva4, iva1, iva2, iva3, iva4, porc_porc_recuivalencia1, porc_porc_recuivalencia2,"
                    "porc_porc_recuivalencia3, porc_porc_recuivalencia4, rec1, rec2, rec3, rec4, total1,"
                    "total2, total3, total4, recargo_equivalencia, email, total_iva, totalRec,"
                    "importe1, importe2, importe3, importe4) "
                    "VALUES"
                    "(:nPresupuesto, :fecha, :dValidoHasta,"
                    ":id_cliente, :codigo_cliente, :cliente, :cif, :direccion1, :direccion2, :cp,"
                    ":poblacion, :provincia, :id_pais, :telefono, :movil, :fax, :dto, :comentarios,"
                    ":importe, :subtotal, :rDescuento, :total, :impreso, :lAprobado, :fechaAprobacion,"
                    ":importeFactura, :importe_pendiente, :factura, :albaran, :pedido, :id_forma_pago,"
                    ":tLugarEntrega, :cAtencionde, :base1, :base2, :base3, :base4, :iva1, :iva2, :iva3,"
                    ":iva4, :iva1, :iva2, :iva3, :iva4, :porc_porc_recuivalencia1, :porc_porc_recuivalencia2,"
                    ":porc_porc_recuivalencia3, :porc_porc_recuivalencia4, :rec1, :rec2, :rec3, :rec4, :total1,"
                    ":total2, :total3, :total4, :recargo_equivalencia, :email, :total_iva, :totalRec,"
                    ":importe1, :importe2, :importe3, :importe4)");

    cab_pre.bindValue(":nPresupuesto",nPresupuesto);
    cab_pre.bindValue(":fecha",fecha);
    cab_pre.bindValue(":dValidoHasta",dValidoHasta);
    cab_pre.bindValue(":id_cliente",id_cliente);
    cab_pre.bindValue(":codigo_cliente",codigo_cliente);
    cab_pre.bindValue(":cliente",cliente);
    cab_pre.bindValue(":cif",cif);
    cab_pre.bindValue(":direccion1",direccion1);
    cab_pre.bindValue(":direccion2",direccion2);
    cab_pre.bindValue(":cp",cp);
    cab_pre.bindValue(":poblacion",poblacion);
    cab_pre.bindValue(":provincia",provincia);
    cab_pre.bindValue(":id_pais",id_pais);
    cab_pre.bindValue(":telefono",telefono);
    cab_pre.bindValue(":movil",movil);
    cab_pre.bindValue(":fax",fax);
    cab_pre.bindValue(":dto",0);
    cab_pre.bindValue(":comentarios",comentarios);
    cab_pre.bindValue(":importe",0);
    cab_pre.bindValue(":subtotal",0);
    cab_pre.bindValue(":rDescuento",0);
    cab_pre.bindValue(":total",0);
    cab_pre.bindValue(":impreso",impreso);
    cab_pre.bindValue(":lAprobado",lAprobado);
    cab_pre.bindValue(":fechaAprobacion",fechaAprobacion);
    cab_pre.bindValue(":importeFactura",0);
    cab_pre.bindValue(":importe_pendiente",0);
    cab_pre.bindValue(":factura",factura);
    cab_pre.bindValue(":albaran",albaran);
    cab_pre.bindValue(":pedido",pedido);
    cab_pre.bindValue(":id_forma_pago",id_forma_pago);
    cab_pre.bindValue(":tLugarEntrega",tLugarEntrega);
    cab_pre.bindValue(":cAtencionde",cAtencionde);
    cab_pre.bindValue(":base1",0);
    cab_pre.bindValue(":base2",0);
    cab_pre.bindValue(":base3",0);
    cab_pre.bindValue(":base4",0);
    cab_pre.bindValue(":iva1",iva1);
    cab_pre.bindValue(":iva2",iva2);
    cab_pre.bindValue(":iva3",iva3);
    cab_pre.bindValue(":iva4",iva4);
    cab_pre.bindValue(":iva1",0);
    cab_pre.bindValue(":iva2",0);
    cab_pre.bindValue(":iva3",0);
    cab_pre.bindValue(":iva4",0);
    cab_pre.bindValue(":porc_porc_recuivalencia1",porc_porc_recuivalencia1);
    cab_pre.bindValue(":porc_porc_recuivalencia2",porc_porc_recuivalencia2);
    cab_pre.bindValue(":porc_porc_recuivalencia3",porc_porc_recuivalencia3);
    cab_pre.bindValue(":porc_porc_recuivalencia4",porc_porc_recuivalencia4);
    cab_pre.bindValue(":rec1",0);
    cab_pre.bindValue(":rec2",0);
    cab_pre.bindValue(":rec3",0);
    cab_pre.bindValue(":rec4",0);
    cab_pre.bindValue(":total1",0);
    cab_pre.bindValue(":total2",0);
    cab_pre.bindValue(":total3",0);
    cab_pre.bindValue(":total4",0);
    cab_pre.bindValue(":recargo_equivalencia",recargo_equivalencia);
    cab_pre.bindValue(":email",email);
    cab_pre.bindValue(":total_iva",0);
    cab_pre.bindValue(":totalRec",0);
    cab_pre.bindValue(":importe1",0);
    cab_pre.bindValue(":importe2",0);
    cab_pre.bindValue(":importe3",0);
    cab_pre.bindValue(":importe4",0);

     if(!cab_pre.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Presupuesto:", cab_pre.lastError().text());
         return false;
     }
     else
     {         
         this->id = cab_pre.lastInsertId().toInt();
         return true;
     }
}

bool Presupuesto::RecuperarPresupuesto(QString cSQL)
{
    QSqlQuery qCab_pre(QSqlDatabase::database("empresa"));
    qCab_pre.prepare(cSQL);
    if( qCab_pre.exec() )
    {
        if (qCab_pre.next())
        {
            QSqlRecord registro = qCab_pre.record();
            this->id = registro.field("id").value().toInt();
            this->nPresupuesto = registro.field("nPresupuesto").value().toInt();
            this->fecha = registro.field("fecha").value().toDate();
            this->dValidoHasta = registro.field("dValidoHasta").value().toDate();
            this->id_cliente = registro.field("id_cliente").value().toInt();
            this->codigo_cliente= registro.field("codigo_cliente").value().toString();
            this->cliente = registro.field("cliente").value().toString();
            this->cif = registro.field("cif").value().toString();
            this->direccion1 = registro.field("direccion1").value().toString();
            this->direccion2 = registro.field("direccion2").value().toString();
            this->cp = registro.field("cp").value().toString();
            this->poblacion = registro.field("poblacion").value().toString();
            this->provincia = registro.field("provincia").value().toString();
            this->id_pais = registro.field("id_pais").value().toInt();
            this->telefono = registro.field("telefono").value().toString();
            this->movil = registro.field("movil").value().toString();
            this->fax = registro.field("fax").value().toString();
            this->email = registro.field("email").value().toString();
            this->dto = registro.field("dto").value().toDouble();
            this->comentarios = registro.field("comentarios").value().toString();
            this->importe = registro.field("importe").value().toDouble();
            this->base = registro.field("base").value().toDouble();
            this->rDescuento = registro.field("rDescuento").value().toDouble();
            this->total = registro.field("total").value().toDouble();
            this->impreso = registro.field("impreso").value().toBool();
            this->lAprobado = registro.field("lAprobado").value().toBool();
            this->fechaAprobacion = registro.field("fechaAprobacion").value().toDate();
            this->importeFactura = registro.field("importeFactura").value().toDouble();
            this->factura = registro.field("factura").value().toString();
            this->albaran = registro.field("albaran").value().toInt();
            this->pedido = registro.field("pedido").value().toInt();

            this->id_forma_pago = registro.field("id_forma_pago").value().toInt();
            QSqlQuery q(QSqlDatabase::database("empresa"));
            if(q.exec("SELECT * FROM formpago WHERE id = "+QString::number(id_forma_pago)))
                if(q.next())
                {
                    this->codigoFormaPago = q.record().value("codigo").toString();
                    this->descripcionFormaPago = q.record().value("forma_pago").toString();
                }

            this->tLugarEntrega = registro.field("tLugarEntrega").value().toString();
            this->cAtencionde = registro.field("cAtencionde").value().toString();
            this->base1 = registro.field("importe1").value().toDouble();
            this->base2 = registro.field("importe2").value().toDouble();
            this->base3 = registro.field("importe3").value().toDouble();
            this->base4 = registro.field("importe4").value().toDouble();
            this->iva1 = registro.field("iva1").value().toDouble();
            this->iva2 = registro.field("iva2").value().toDouble();
            this->iva3 = registro.field("iva3").value().toDouble();
            this->iva4 = registro.field("iva4").value().toDouble();
            this->iva1 = registro.field("iva1").value().toDouble();
            this->iva2 = registro.field("iva2").value().toDouble();
            this->iva3 = registro.field("iva3").value().toDouble();
            this->iva4 = registro.field("iva4").value().toDouble();
            this->total_iva = registro.field("total_iva").value().toDouble();
            this->porc_porc_recuivalencia1 = registro.field("porc_porc_recuivalencia1").value().toDouble();
            this->porc_porc_recuivalencia2 = registro.field("porc_porc_recuivalencia2").value().toDouble();
            this->porc_porc_recuivalencia3 = registro.field("porc_porc_recuivalencia3").value().toDouble();
            this->porc_porc_recuivalencia4 = registro.field("porc_porc_recuivalencia4").value().toDouble();
            this->rec1 = registro.field("rec1").value().toDouble();
            this->rec2 = registro.field("rec2").value().toDouble();
            this->rec3 = registro.field("rec3").value().toDouble();
            this->rec4 = registro.field("rec4").value().toDouble();
            this->totalRec = registro.field("totalRec").value().toDouble();
            this->total1 = registro.field("total1").value().toDouble();
            this->total2 = registro.field("total2").value().toDouble();
            this->total3 = registro.field("total3").value().toDouble();
            this->total4 = registro.field("total4").value().toDouble();
            this->recargo_equivalencia = registro.field("recargo_equivalencia").value().toBool();
            return true;
        }
     }
    return false;
}

bool Presupuesto::siguiente()
{
    return RecuperarPresupuesto("Select * from cab_pre where id >'"+
                                    QString::number(id)+"' order by id  limit 1 ");
}

bool Presupuesto::anterior()
{
   return RecuperarPresupuesto("Select * from cab_pre where id <'"+QString::number(id)+
                               "' order by id desc limit 1 ");
}

bool Presupuesto::GuardarPres(int nid_Presupuesto)
{

    if (this->nPresupuesto == 0)
        this->nPresupuesto = NuevoNumeroPresupuesto();

    QSqlQuery cab_pre(QSqlDatabase::database("empresa"));
    cab_pre.prepare("UPDATE cab_pre SET "
                    "fecha = :fecha, dValidoHasta = :dValidoHasta, id_cliente = :id_cliente,"
                    "codigo_cliente =  :codigo_cliente, cliente = :cliente, cif = :cif,"
                    "direccion1 = :direccion1, direccion2 = :direccion2, cp = :cp,"
                    "poblacion = :poblacion, provincia = :provincia, id_pais = :id_pais,"
                    "telefono = :telefono, movil = :movil, fax = :fax,"
                    "comentarios = :comentarios, importe = :importe, base =:subtotal,"
                    "rDescuento = :rDescuento, total = :total, impreso = :impreso,"
                    "lAprobado = :lAprobado, fechaAprobacion = :fechaAprobacion,"
                    "importeFactura =:importeFactura, importe_pendiente = :importe_pendiente,"
                    "factura = :factura, albaran = :albaran, pedido=:pedido,"
                    "id_forma_pago =:id_forma_pago, tLugarEntrega=:tLugarEntrega,"
                    "cAtencionde=:cAtencionde, base1=:base1, base2=:base2, base3=:base3,"
                    "base4=:base4, iva1=:iva1, iva2=:iva2, iva3=:iva3,"
                    "iva4=:iva4, iva1=:iva1, iva2=:iva2, iva3=:iva3, iva4=:iva4,"
                    "porc_porc_recuivalencia1=:porc_porc_recuivalencia1,"
                    "porc_porc_recuivalencia2=:porc_porc_recuivalencia2,"
                    "porc_porc_recuivalencia3=:porc_porc_recuivalencia3,"
                    "porc_porc_recuivalencia4=:porc_porc_recuivalencia4,"
                    "rec1=:rec1, rec2=:rec2, rec3=:rec3, rec4=:rec4, total1=:total1,"
                    "total2=:total2, total3=:total3, total4=:total4,"
                    "recargo_equivalencia=:recargo_equivalencia, email=:email,"
                    "total_iva=:total_iva, totalRec=:totalRec,"
                    "importe1=:importe1, importe2=:importe2,"
                    "importe3=:importe3, importe4=:importe4 "
                    "WHERE id = :nnPresupuesto");

    cab_pre.bindValue(":fecha",fecha);
    cab_pre.bindValue(":dValidoHasta",dValidoHasta);
    cab_pre.bindValue(":id_cliente",id_cliente);
    cab_pre.bindValue(":codigo_cliente",codigo_cliente);
    cab_pre.bindValue(":cliente",cliente);
    cab_pre.bindValue(":cif",cif);
    cab_pre.bindValue(":direccion1",direccion1);
    cab_pre.bindValue(":direccion2",direccion2);
    cab_pre.bindValue(":cp",cp);
    cab_pre.bindValue(":poblacion",poblacion);
    cab_pre.bindValue(":provincia",provincia);
    cab_pre.bindValue(":id_pais",id_pais);
    cab_pre.bindValue(":telefono",telefono);
    cab_pre.bindValue(":movil",movil);
    cab_pre.bindValue(":fax",fax);
    cab_pre.bindValue(":comentarios",comentarios);
    cab_pre.bindValue(":importe",importe);
    cab_pre.bindValue(":subtotal",base);
    cab_pre.bindValue(":rDescuento",rDescuento);
    cab_pre.bindValue(":total",total);
    cab_pre.bindValue(":impreso",impreso);
    cab_pre.bindValue(":lAprobado",lAprobado);
    cab_pre.bindValue(":fechaAprobacion",fechaAprobacion);
    cab_pre.bindValue(":importeFactura",importeFactura);
    cab_pre.bindValue(":importe_pendiente",0);//TODO preguntar a Marc
    cab_pre.bindValue(":factura",factura);
    cab_pre.bindValue(":albaran",albaran);
    cab_pre.bindValue(":pedido",pedido);
    cab_pre.bindValue(":id_forma_pago",id_forma_pago);
    cab_pre.bindValue(":tLugarEntrega",tLugarEntrega);
    cab_pre.bindValue(":cAtencionde",cAtencionde);
    cab_pre.bindValue(":base1",base1);
    cab_pre.bindValue(":base2",base2);
    cab_pre.bindValue(":base3",base3);
    cab_pre.bindValue(":base4",base4);
    cab_pre.bindValue(":iva1",iva1);
    cab_pre.bindValue(":iva2",iva2);
    cab_pre.bindValue(":iva3",iva3);
    cab_pre.bindValue(":iva4",iva4);
    cab_pre.bindValue(":iva1",iva1);
    cab_pre.bindValue(":iva2",iva2);
    cab_pre.bindValue(":iva3",iva3);
    cab_pre.bindValue(":iva4",iva4);
    cab_pre.bindValue(":porc_porc_recuivalencia1",porc_porc_recuivalencia1);
    cab_pre.bindValue(":porc_porc_recuivalencia2",porc_porc_recuivalencia2);
    cab_pre.bindValue(":porc_porc_recuivalencia3",porc_porc_recuivalencia3);
    cab_pre.bindValue(":porc_porc_recuivalencia4",porc_porc_recuivalencia4);
    cab_pre.bindValue(":rec1",rec1);
    cab_pre.bindValue(":rec2",rec2);
    cab_pre.bindValue(":rec3",rec3);
    cab_pre.bindValue(":rec4",rec4);
    cab_pre.bindValue(":total1",total1);
    cab_pre.bindValue(":total2",total2);
    cab_pre.bindValue(":total3",total3);
    cab_pre.bindValue(":total4",total4);
    cab_pre.bindValue(":recargo_equivalencia",recargo_equivalencia);
    cab_pre.bindValue(":email",email);
    cab_pre.bindValue(":total_iva",total_iva);
    cab_pre.bindValue(":totalRec",totalRec);
    cab_pre.bindValue(":importe1",0);
    cab_pre.bindValue(":importe2",0);
    cab_pre.bindValue(":importe3",0);
    cab_pre.bindValue(":importe4",0);
    cab_pre.bindValue(":nnPresupuesto",nid_Presupuesto);

    if(!cab_pre.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Presupuesto:", cab_pre.lastError().text());
        return false;
    }
    return true;
}

bool Presupuesto::BorrarLineas(int nid_Presupuesto)
{
    QSqlQuery query(QSqlDatabase::database("empresa"));
    QString sql = QString("DELETE FROM lin_pre WHERE id_Cab = %1").arg(nid_Presupuesto);
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
