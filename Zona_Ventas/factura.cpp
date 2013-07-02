#include "factura.h"

Factura::Factura(QObject *parent) :
    QObject(parent)
{
    this->id =0;
}

// Metodos utilidad Clase
bool Factura::AnadirFactura()
{
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    this->porc_iva1 = Configuracion_global->ivas[keys.at(0)].value("iva").toDouble();
    this->porc_iva2 = Configuracion_global->ivas[keys.at(1)].value("iva").toDouble();
    this->porc_iva3 = Configuracion_global->ivas[keys.at(2)].value("iva").toDouble();
    this->porc_iva4 = Configuracion_global->ivas[keys.at(3)].value("iva").toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(0).toDouble();
    this->porc_rec2 = Configuracion_global->reList.at(1).toDouble();
    this->porc_rec3 = Configuracion_global->reList.at(2).toDouble();
    this->porc_rec4 = Configuracion_global->reList.at(3).toDouble();

    this->recargo_equivalencia = 0;
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
     cab_fac.prepare("INSERT INTO cab_fac (codigo_cliente,factura,fecha,fecha_cobro,id_cliente,cliente,direccion1,direccion2,"
                   "cp,poblacion,provincia,id_pais,cif,recargo_equivalencia,subtotal,dto,dto_pp,importe_descuento,importe_descuento_pp,"
                   "base,iva,iva,total,impreso,cobrado,contabilizada,id_forma_pago,forma_pago,comentario,"
                   "base1,base2,base3,base4,porc_iva1,porc_iva2,porc_iva3,porc_iva4,iva1,iva2,iva3,iva4,"
                   "total1,total2,total3,total4,porc_rec1,porc_rec2,porc_rec3,porc_rec4,rec1,rec2,rec3,rec4,"
                   "total_recargo,entregado_a_cuenta,importe_pendiente,codigo_entidad,oficina_entidad,dc_cuenta,numero_cuenta,pedido_cliente)"
                   " VALUES (:codigo_cliente,:factura,:fecha,:fecha_cobro,:id_cliente,:cliente,:direccion1,:direccion2,"
                   ":cp,:poblacion,:provincia,:pais,:cif,:recargo_equivalencia,:subtotal,:dto,:dto_pp,:importe_descuento,:importe_descuento_pp,"
                   ":base,:iva,:iva,:total,:impreso,:cobrado,:contabilizada,:id_forma_pago,:forma_pago,:comentario,"
                   ":base1,:base2,:base3,:base4,:porc_iva1,:porc_iva2,:porc_iva3,:porc_iva4,:iva1,:iva2,:iva3,:iva4,"
                   ":total1,:total2,:total3,:total4,:porc_rec1,:porc_rec2,:porc_rec3,:porc_rec4,:rec1,:rec2,:rec3,:rec4,"
                   ":total_recargo,:entregado_a_cuenta,:importe_pendiente,:codigo_entidad,:oficina_entidad,:dc_cuenta,:numero_cuenta,:pedido_cliente)");

     cab_fac.bindValue(":codigo_cliente",this->codigo_cliente);
     cab_fac.bindValue(":factura","BORRADOR");
     cab_fac.bindValue(":fecha",this->fecha);
     cab_fac.bindValue(":fecha_cobro",this->fecha_cobro);
     cab_fac.bindValue(":id_cliente", this->id_cliente);
     cab_fac.bindValue(":cliente",this->cliente);
     cab_fac.bindValue(":direccion1",this->direccion1);
     cab_fac.bindValue(":direccion2",this->direccion2);
     cab_fac.bindValue(":cp",this->cp);
     cab_fac.bindValue(":poblacion",this->poblacion);
     cab_fac.bindValue(":provincia",this->provincia);
     cab_fac.bindValue(":pais",this->id_pais);
     cab_fac.bindValue(":cif",this->cif);
     cab_fac.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
     cab_fac.bindValue(":subtotal",0);
     cab_fac.bindValue(":dto",0);
     cab_fac.bindValue(":dto_pp",0);
     cab_fac.bindValue(":importe_descuento",0);
     cab_fac.bindValue(":importe_descuento_pp",0);
     cab_fac.bindValue(":base",0);
     cab_fac.bindValue(":iva",0);
     cab_fac.bindValue(":iva",0);
     cab_fac.bindValue(":total",0);
     cab_fac.bindValue(":impreso",0);
     cab_fac.bindValue(":cobrado",0);
     cab_fac.bindValue(":contabilizada",0);
     cab_fac.bindValue(":id_forma_pago",this->id_forma_pago);
     cab_fac.bindValue(":forma_pago",this->forma_pago);
     cab_fac.bindValue(":comentario",this->comentario);
     cab_fac.bindValue(":base1",0);
     cab_fac.bindValue(":base2",0);
     cab_fac.bindValue(":base3",0);
     cab_fac.bindValue(":base4",0);
     cab_fac.bindValue(":porc_iva1",this->porc_iva1);
     cab_fac.bindValue(":porc_iva2",this->porc_iva2);
     cab_fac.bindValue(":porc_iva3",this->porc_iva3);
     cab_fac.bindValue(":porc_iva4",this->porc_iva4);
     cab_fac.bindValue(":iva1",0);
     cab_fac.bindValue(":iva2",0);
     cab_fac.bindValue(":iva3",0);
     cab_fac.bindValue(":iva4",0);
     cab_fac.bindValue(":total1",0);
     cab_fac.bindValue(":total2",0);
     cab_fac.bindValue(":total3",0);
     cab_fac.bindValue(":total4",0);
     cab_fac.bindValue(":porc_rec1",this->porc_rec1);
     cab_fac.bindValue(":porc_rec2",this->porc_rec2);
     cab_fac.bindValue(":porc_rec3",this->porc_rec3);
     cab_fac.bindValue(":porc_rec4",this->porc_rec4);
     cab_fac.bindValue(":rec1",0);
     cab_fac.bindValue(":rec2",0);
     cab_fac.bindValue(":rec3",0);
     cab_fac.bindValue(":rec4",0);
     cab_fac.bindValue(":total_recargo",0);
     cab_fac.bindValue(":entregado_a_cuenta",0);
     cab_fac.bindValue(":importe_pendiente",0);
     cab_fac.bindValue(":codigo_entidad",this->codigo_entidad);
     cab_fac.bindValue(":oficina_entidad",this->oficina_entidad);
     cab_fac.bindValue(":dc_cuenta",this->dc_cuenta);
     cab_fac.bindValue(":numero_cuenta",this->numero_cuenta);
     cab_fac.bindValue(":pedido_cliente",0);
     if(!cab_fac.exec())
     {
         QMessageBox::critical(qApp->activeWindow(),"error al guardar datos Factura:", cab_fac.lastError().text());
         return false;
     }
     else
     {
         this->id = cab_fac.lastInsertId().toInt();
         QString cSQL = "Select * from cab_fac where id ="+QString::number(this->id);
         RecuperarFactura(cSQL);
         return true;
     }

}
// Guardar la factura
bool Factura::GuardarFactura(int nid_factura, bool FacturaLegal)
{
    bool succes = true;
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    cab_fac.prepare( "UPDATE cab_fac set "
                     "codigo_cliente = :codigo_cliente,"
                     "factura = :factura,"
                     "fecha = :fecha,"
                     "fecha_cobro = :fecha_cobro,"
                     "id_cliente = :id_cliente,"
                     "cliente = :cliente,"
                     "direccion1 = :direccion1,"
                     "direccion2 = :direccion2,"
                     "cp=:cp,"
                     "poblacion =:poblacion,"
                     "provincia =:provincia,"
                     "id_pais = :pais,"
                     "cif = :cif,"
                     "recargo_equivalencia = :recargo_equivalencia,"
                     "subtotal =:subtotal,"
                     "dto =:dto,"
                     "dto_pp =:dto_pp,"
                     "importe_descuento =:importe_descuento,"
                     "importe_descuento_pp =:importe_descuento_pp,"
                     "base =:base,"
                     "iva =:iva,"
                     "iva = :iva,"
                     "total = :total,"
                     "impreso =:impreso,"
                     "cobrado =:cobrado,"
                     "contabilizada =:contabilizada,"
                     "id_forma_pago =:id_forma_pago,"
                     "forma_pago = :forma_pago,"
                     "comentario =:comentario,"
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
                     "total1=:total1,"
                     "total2=:total2,"
                     "total3=:total3,"
                     "total4=:total4,"
                     "porc_rec1 =:porc_rec1,"
                     "porc_rec2 =:porc_rec2,"
                     "porc_rec3 =:porc_rec3,"
                     "porc_rec4 =:porc_rec4,"
                     "rec1 =:rec1,"
                     "rec2 =:rec2,"
                     "rec3 =:rec3,"
                     "rec4 =:rec4,"
                     "total_recargo =:total_recargo,"
                     "entregado_a_cuenta =:entregado_a_cuenta,"
                     "importe_pendiente =:importe_pendiente,"
                     "codigo_entidad =:codigo_entidad,"
                     "oficina_entidad =:oficina_entidad,"
                     "dc_cuenta =:dc_cuenta,"
                     "numero_cuenta =:numero_cuenta,"
                     "pedido_cliente =:pedido_cliente,"
                     "irpf =:irpf,"
                     "importe_irpf =:importe_irpf"
                     " where id=:id");

    // Pasamos valores reales a la Select
    cab_fac.bindValue(":id",nid_factura);
    cab_fac.bindValue(":codigo_cliente",this->codigo_cliente);
    cab_fac.bindValue(":factura",this->factura);
    cab_fac.bindValue(":fecha",this->fecha);
    cab_fac.bindValue(":fecha_cobro",this->fecha_cobro);
    cab_fac.bindValue(":id_cliente", this->id_cliente);
    cab_fac.bindValue(":cliente",this->cliente);
    cab_fac.bindValue(":direccion1",this->direccion1);
    cab_fac.bindValue(":direccion2",this->direccion2);
    cab_fac.bindValue(":cp",this->cp);
    cab_fac.bindValue(":poblacion",this->poblacion);
    cab_fac.bindValue(":provincia",this->provincia);
    cab_fac.bindValue(":pais",this->id_pais);
    cab_fac.bindValue(":cif",this->cif);
    cab_fac.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
    cab_fac.bindValue(":subtotal",this->subtotal);
    cab_fac.bindValue(":dto",this->dto);
    cab_fac.bindValue(":dto_pp",this->dto_pp);
    cab_fac.bindValue(":importe_descuento",this->importe_descuento);
    cab_fac.bindValue(":importe_descuento_pp",this->importe_descuento_pp);
    cab_fac.bindValue(":base",this->base);
    cab_fac.bindValue(":iva",this->iva);
    cab_fac.bindValue(":iva",this->iva);
    cab_fac.bindValue(":total",this->total);
    cab_fac.bindValue(":impreso",this->impreso);
    cab_fac.bindValue(":cobrado",this->cobrado);
    cab_fac.bindValue(":contabilizada",this->lContablilizada);
    cab_fac.bindValue(":id_forma_pago",this->id_forma_pago);
    cab_fac.bindValue(":forma_pago",this->forma_pago);
    cab_fac.bindValue(":comentario",this->comentario);
    cab_fac.bindValue(":base1",this->base1);
    cab_fac.bindValue(":base2",this->base2);
    cab_fac.bindValue(":base3",this->base3);
    cab_fac.bindValue(":base4",this->base4);
    cab_fac.bindValue(":porc_iva1",this->porc_iva1);
    cab_fac.bindValue(":porc_iva2",this->porc_iva2);
    cab_fac.bindValue(":porc_iva3",this->porc_iva3);
    cab_fac.bindValue(":porc_iva4",this->porc_iva4);
    cab_fac.bindValue(":iva1",this->iva1);
    cab_fac.bindValue(":iva2",this->iva2);
    cab_fac.bindValue(":iva3",this->iva3);
    cab_fac.bindValue(":iva4",this->iva4);
    cab_fac.bindValue(":total1",this->total1);
    cab_fac.bindValue(":total2",this->total2);
    cab_fac.bindValue(":total3",this->total3);
    cab_fac.bindValue(":total4",this->total4);
    cab_fac.bindValue(":porc_rec1",this->porc_rec1);
    cab_fac.bindValue(":porc_rec2",this->porc_rec2);
    cab_fac.bindValue(":porc_rec3",this->porc_rec3);
    cab_fac.bindValue(":porc_rec4",this->porc_rec4);
    cab_fac.bindValue(":rec1",this->rec1);
    cab_fac.bindValue(":rec2",this->rec2);
    cab_fac.bindValue(":rec3",this->rec3);
    cab_fac.bindValue(":rec4",this->rec4);
    cab_fac.bindValue(":total_recargo",this->total_recargo);
    cab_fac.bindValue(":entregado_a_cuenta",this->entregado_a_cuenta);
    cab_fac.bindValue(":importe_pendiente",this->importe_pendiente);
    cab_fac.bindValue(":codigo_entidad",this->codigo_entidad);
    cab_fac.bindValue(":oficina_entidad",this->oficina_entidad);
    cab_fac.bindValue(":dc_cuenta",this->dc_cuenta);
    cab_fac.bindValue(":numero_cuenta",this->numero_cuenta);
    cab_fac.bindValue(":pedido_cliente",this->pedido_cliente);
    cab_fac.bindValue(":irpf",this->irpf);
    cab_fac.bindValue(":importe_irpf",this->importe_irpf);
    if(!cab_fac.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),tr("error al guardar datos Factura:"), cab_fac.lastError().text());
        succes =  false;
    }
    else
    {
       // TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("La Factura se ha guardado corectamente:"));
        QString cSQL = "Select * from cab_fac where id ="+QString::number(nid_factura);
        RecuperarFactura(cSQL);
        if (FacturaLegal)
        {
            if(QMessageBox::question(qApp->activeWindow(),tr("Guardar Factura"),
                                     tr("¿Desea cobrar la factura ahora o generar una deuda al cliente?"),
                                     tr("Cobrar"),tr("Generar deuda")) == QMessageBox::Accepted)
            {
                succes = CobrarFactura();
            }
            else
            {
                // Busco ficha cliente
                QSqlQuery Cliente(QSqlDatabase::database("empresa"));
                Cliente.prepare("Select * from clientes where codigo_cliente = :codigo_cliente");
                Cliente.bindValue(":codigo_cliente",this->codigo_cliente);
                if (Cliente.exec())
                {
                    Cliente.next();
                    QSqlRecord record = Cliente.record();

                    // Genero deuda cliente

                    QSqlQuery Deudacliente(QSqlDatabase::database("empresa"));
                    Deudacliente.prepare("Insert into clientes_deuda (id_cliente,fecha,vencimiento,documento,id_ticket,id_factura,tipo,"
                                         "importe,pagado,pendiente_cobro,entidad,oficina,dc,cuenta)"
                                         " values (:id_cliente,:fecha,:vencimiento,:documento,:id_tiquet,:id_factura,:tipo,"
                                         ":importe,:pagado,:pendiente_cobro,:entidad,:oficina,:dc,:cuenta)");
                    Deudacliente.bindValue(":id_cliente",record.field("id").value().toInt());
                    Deudacliente.bindValue(":fecha",QDate::currentDate());
                    Deudacliente.bindValue(":vencimiento",QDate::currentDate());
                    // TODO Deudacliente->bindValue(":vencimiento",Configuracion_global->CalcularVencimiento());
                    Deudacliente.bindValue(":documento",this->factura);
                    Deudacliente.bindValue(":id_tiquet",0);
                    Deudacliente.bindValue(":id_factura",nid_factura);
                    Deudacliente.bindValue("tipo",1);
                    Deudacliente.bindValue(":importe",this->total);
                    Deudacliente.bindValue(":pagado",0);
                    Deudacliente.bindValue(":pendiente_cobro",this->total);
                    Deudacliente.bindValue(":entidad",record.field("entidad_bancaria").value().toString());
                    Deudacliente.bindValue(":oficina",record.field("oficina_bancaria").value().toString());
                    Deudacliente.bindValue(":dc",record.field("dc").value().toString());
                    Deudacliente.bindValue(":cuenta",record.field("cuenta_corriente").value().toString());
                    if(!Deudacliente.exec())
                    {
                        qDebug() << Deudacliente.lastQuery();
                        QMessageBox::warning(qApp->activeWindow(),tr("Añadir deuda"),tr("No se ha podido añadir la deuda ")+Deudacliente.lastError().text() ,tr("OK"));
                        succes = false;
                    }
                    else
                    {
                        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Deuda añadida corectamente:"));
                        // Añadimos acumulados ficha cliente.
                        Cliente.prepare("Update clientes set fecha_ultima_compra = :fecha_ultima_compra, "
                                        "acumulado_ventas = acumulado_ventas + :acumulado_ventas,"
                                        "ventas_ejercicio = ventas_ejercicio + :ventas_ejercicio,"
                                        "deuda_actual = deuda_actual + :deuda_actual "
                                        " where id = :id_cliente");
                        Cliente.bindValue(":fecha_ultima_compra",QDate::currentDate());
                        Cliente.bindValue(":acumulado_ventas",this->total);
                        Cliente.bindValue(":ventas_ejercicio",this->total);
                        Cliente.bindValue(":deuda_actual",this->total);
                        Cliente.bindValue(":id_cliente",record.field("id").value().toInt());
                        if (!Cliente.exec())
                        {
                            succes =  false;
                            QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                                                 tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                                                 tr("OK"));
                        }
                    }
                }
                else
                    succes = false;
            }
        }
    }
    return succes;
}

bool Factura::RecuperarFactura(QString cSQL){
        QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
        cab_fac.prepare(cSQL);
        if( !cab_fac.exec() )
        {
            QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
            return false;
        }
        else
        {
            if (cab_fac.next())
            {
                QSqlRecord registro = cab_fac.record();
                this->id = registro.field("id").value().toInt();
                this->codigo_cliente= registro.field("codigo_cliente").value().toString();
                this->factura = registro.field("factura").value().toString();
                this->fecha = registro.field("fecha").value().toDate();
                this->fecha_cobro = registro.field("fecha_cobro").value().toDate();
                this->id_cliente = registro.field("id_cliente").value().toInt();
                this->cliente = registro.field("cliente").value().toString();
                this->direccion1 = registro.field("direccion1").value().toString();
                this->direccion2 = registro.field("direccion2").value().toString();
                this->cp = registro.field("cp").value().toString();
                this->poblacion = registro.field("poblacion").value().toString();
                this->provincia = registro.field("provincia").value().toString();
                this->id_pais = registro.field("id_pais").value().toInt();
                this->cif =registro.field("cif").value().toString();
                this->recargo_equivalencia = registro.field("recargo_equivalencia").value().toInt();
                this->subtotal = registro.field("subtotal").value().toDouble();
                this->dto = registro.field("dto").value().toInt();
                this->dto_pp = registro.field("dto_pp").value().toInt();
                this->importe_descuento = registro.field("importe_descuento").value().toDouble();
                this->importe_descuento_pp = registro.field("importe_descuento_pp").value().toDouble();
                this->base = registro.field("base").value().toDouble();
                this->iva = registro.field("iva").value().toInt();
                this->iva = registro.field("iva").value().toDouble();
                this->total = registro.field("total").value().toDouble();
                this->impreso = registro.field("impreso").value().toInt();
                this->cobrado = registro.field("cobrado").value().toInt();
                this->lContablilizada = registro.field("contabilizada").value().toInt();
                this->id_forma_pago = registro.field("id_forma_pago").value().toInt();
                this->forma_pago = registro.field("forma_pago").value().toString();
                this->comentario = registro.field("comentario").value().toString();
                this->base1 = registro.field("base1").value().toDouble();
                this->base2 = registro.field("base2").value().toDouble();
                this->base3 = registro.field("base3").value().toDouble();
                this->base4 = registro.field("base4").value().toDouble();
                this->porc_iva1 = registro.field("porc_iva1").value().toInt();
                this->porc_iva2 = registro.field("porc_iva2").value().toInt();
                this->porc_iva3 = registro.field("porc_iva3").value().toInt();
                this->porc_iva4 = registro.field("porc_iva4").value().toInt();
                this->iva1 = registro.field("iva1").value().toDouble();
                this->iva2 = registro.field("iva3").value().toDouble();
                this->iva3 = registro.field("iva3").value().toDouble();
                this->iva4 = registro.field("iva4").value().toDouble();
                this->total1 = registro.field("total1").value().toDouble();
                this->total2 = registro.field("total2").value().toDouble();
                this->total3 = registro.field("total3").value().toDouble();
                this->total4 = registro.field("total4").value().toDouble();
                this->porc_rec1 = registro.field("porc_rec1").value().toDouble();
                this->porc_rec2 = registro.field("porc_rec2").value().toDouble();
                this->porc_rec3 = registro.field("porc_rec3").value().toDouble();
                this->porc_rec4 = registro.field("porc_rec4").value().toDouble();
                this->rec1 = registro.field("rec1").value().toDouble();
                this->rec2 = registro.field("rec2").value().toDouble();
                this->rec3 = registro.field("rec3").value().toDouble();
                this->rec4 = registro.field("rec4").value().toDouble();
                this->total_recargo = registro.field("total_recargo").value().toDouble();
                this->entregado_a_cuenta = registro.field("entregado_a_cuenta").value().toDouble();
                this->importe_pendiente = registro.field("importe_pendiente").value().toDouble();
                this->codigo_entidad = registro.field("codigo_entidad").value().toString();
                this->oficina_entidad = registro.field("oficina_entidad").value().toString();
                this->dc_cuenta = registro.field("dc_cuenta").value().toString();
                this->numero_cuenta = registro.field("numero_cuenta").value().toString();
                this->pedido_cliente = registro.field("pedido_cliente").value().toInt();
                this->irpf = registro.field("irpf").value().toInt();
                this->importe_irpf = registro.field("importe_irpf").value().toDouble();
                return true;
               }
            else
            {
                return false;
            }
        }
}

QString Factura::NuevoNumeroFactura() {
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    QString cNum,serie;
    QString cNumFac;
    int inum;

    cab_fac.prepare("Select factura from cab_fac  where factura <> '"+QObject::tr("BORRADOR")+"' order by factura desc limit 1");
    if(cab_fac.exec()) {
        cab_fac.next();
        cNumFac = cab_fac.value(0).toString();
        cNum = cNumFac.right(Configuracion_global->ndigitos_factura);
        serie = Configuracion_global->serie;
        inum = cNum.toInt();
        inum++;
        cNum = cNum.number(inum);
        while (cNum.length()< Configuracion_global->ndigitos_factura) {
            cNum.prepend("0");
        }
    } else {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_fac.lastError().text());
    }
    cNumFac = serie + cNum;
    return cNumFac;
}

bool Factura::BorrarLineasFactura(int id_lin)
{
    QSqlQuery query(QSqlDatabase::database("empresa"));
        QString sql = QString("DELETE FROM lin_fac WHERE id_Cab = %1").arg(id_lin);
        query.prepare(sql);
        if(query.exec())
            return true;
        else
        {
            QMessageBox::critical(qApp->activeWindow(), "Error:",query.lastError().text());
            return false;
        }
}

bool Factura::CobrarFactura()
{
    // marcar factura como cobrada
    this->cobrado = true;
    QSqlQuery Cliente(QSqlDatabase::database("Maya"));

    // Añadimos acumulados ficha cliente.
    Cliente.prepare("Update clientes set fecha_ultima_compra = :fecha_ultima_compra, "
                             "acumulado_ventas = acumulado_ventas + :acumulado_ventas,"
                             "ventas_ejercicio = ventas_ejercicio + :ventas_ejercicio "
                             " where codigo_cliente = :codigo_cliente");
    Cliente.bindValue(":fecha_ultima_compra",QDate::currentDate());
    Cliente.bindValue(":acumulado_ventas",this->total);
    Cliente.bindValue(":ventas_ejercicio",this->total);
    Cliente.bindValue(":codigo_cliente",this->codigo_cliente);
    if (!Cliente.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Acumulados"),
                             tr("No se ha podido añadir los correspondientes acumulados a la ficha del cliente"),
                             tr("OK"));
        return false;
    }
    QSqlQuery cab_fac(QSqlDatabase::database("empresa"));
    cab_fac.prepare("update cab_fac set cobrado = 1 where id =:id_cab");
    cab_fac.bindValue(":id_cab",this->id);
    if(!cab_fac.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Guardar Factura"),tr("No se ha podido marcar la factura como cobrada"),tr("OK"));
        return false;
    }
    return true;
}

bool Factura::GuardarApunte(int nasiento, int nid)
{
    QSqlQuery factura(QSqlDatabase::database("empresa"));

    factura.prepare("Update cab_fac set asiento =:asiento, contabilizada = 1 where id =:id ");
    factura.bindValue(":asiento",nasiento);
    factura.bindValue(":id",nid);
    if (!factura.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("facturas clientes"),
                             tr("No se ha podido guardar el numero de apunte"),
                             tr("OK"));
        return false;
    } else
        return true;
}
