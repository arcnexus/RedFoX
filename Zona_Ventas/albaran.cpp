#include "albaran.h"
//

Albaran::Albaran(QObject *parent) :
    QObject(parent),
    id(0)
{
    //this->id =0;
    oArticulo = new Articulo(this);
}

Albaran::~Albaran()
{

}
// Metodos utilidad Clase
int Albaran::AnadirAlbaran(QString serie)
{

    QHash <QString,QVariant> h_cab_alb;
    QString error;

    h_cab_alb["albaran"] = "-1";
    h_cab_alb["fecha"] = fecha;
    h_cab_alb["ejercicio"] = Configuracion_global->cEjercicio;
    h_cab_alb["pedido_cliente"] = pedido_cliente;
    h_cab_alb["id_cliente"] = id_cliente;
    h_cab_alb["codigo_cliente"] = codigo_cliente;
    h_cab_alb["cliente"] = cliente;

    h_cab_alb["direccion1"] = direccion1;
    h_cab_alb["direccion2"] = direccion2;
    h_cab_alb["poblacion"] = poblacion;
    h_cab_alb["provincia"] = provincia;
    h_cab_alb["cp"] = cp;
    h_cab_alb["id_pais"] = id_pais;
    h_cab_alb["cif"] = cif;

    h_cab_alb["recargo_equivalencia"] = recargo_equivalencia;
    h_cab_alb["subtotal"] = 0;
    h_cab_alb["dto"] = 0;
    h_cab_alb["porc_dto"] = 0;
    h_cab_alb["base1"] = 0;
    h_cab_alb["base2"] = 0;
    h_cab_alb["base3"] = 0;
    h_cab_alb["base4"] = 0;
    h_cab_alb["porc_iva1"] = Configuracion_global->ivaList.at(0).toDouble();
    h_cab_alb["porc_iva2"] = Configuracion_global->ivaList.at(1).toDouble();
    h_cab_alb["porc_iva3"] = Configuracion_global->ivaList.at(2).toDouble();
    h_cab_alb["porc_iva4"] = Configuracion_global->ivaList.at(3).toDouble();

    h_cab_alb["iva1"] = 0;
    h_cab_alb["iva2"] = 0;
    h_cab_alb["iva3"] = 0;
    h_cab_alb["iva4"] = 0;
    h_cab_alb["porc_rec1"] = 0;
    h_cab_alb["porc_rec2"] = 0;
    h_cab_alb["porc_rec3"] = 0;
    h_cab_alb["porc_rec4"] = 0;

    h_cab_alb["rec1"] = 0;
    h_cab_alb["rec2"] = 0;
    h_cab_alb["rec3"] = 0;
    h_cab_alb["rec4"] = 0;

    h_cab_alb["total1"] = 0;
    h_cab_alb["total2"] = 0;
    h_cab_alb["total3"] = 0;
    h_cab_alb["total4"] = 0;
    h_cab_alb["base_total"] = 0;
    h_cab_alb["iva_total"] = 0;
    h_cab_alb["rec_total"] = 0;

    h_cab_alb["total_albaran"] = 0;
    h_cab_alb["impreso"] = 0;
    h_cab_alb["facturado"] = 0;
    h_cab_alb["factura"] = factura;
    h_cab_alb["fecha_factura"] = fecha_factura;
    h_cab_alb["comentario"] = comentario;

    h_cab_alb["entregado_a_cuenta"] = 0;
    h_cab_alb["ejercicio"] =  Configuracion_global->cEjercicio.toInt();
    h_cab_alb["serie"] = serie;

    int new_id = SqlCalls::SqlInsert(h_cab_alb,"cab_alb",Configuracion_global->empresaDB,error);

    if(new_id == -1)
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Albaran:",
                              QString(tr("Ocurrió un error al insertar :")+"%1").arg(error));
        return false;
    }
    else
    {
        this->id = new_id;
        return id;
    }
}

bool Albaran::GuardarAlbaran(int nid_Albaran)
{

    QHash <QString,QVariant> h_cab_alb;
    QString condiciones =QString("id =%1").arg(nid_Albaran);
    QString error;

    if(albaran == "-1")
    {
        QString x = NuevoNumeroAlbaran(serie);
        h_cab_alb["albaran"] = x;
    }
    else
    {
        h_cab_alb["albaran"] = albaran;
    }
    h_cab_alb["id_divisa"] = id_divisa;
    h_cab_alb["fecha"] = fecha;
    h_cab_alb["ejercicio"] = ejercicio;
    h_cab_alb["pedido_cliente"] = pedido_cliente;
    h_cab_alb["id_cliente"] = id_cliente;
    h_cab_alb["codigo_cliente"] = codigo_cliente;
    h_cab_alb["cliente"] = cliente;

    h_cab_alb["direccion1"] = direccion1;
    h_cab_alb["direccion2"] = direccion2;
    h_cab_alb["poblacion"] = poblacion;
    h_cab_alb["provincia"] = provincia;
    h_cab_alb["cp"] = cp;
    h_cab_alb["id_pais"] = id_pais;

    h_cab_alb["direccion1_entrega"] = direccion1_entrega;
    h_cab_alb["direccion2_entrega"] = direccion2_entrega;
    h_cab_alb["poblacion_entrega"] = poblacion_entrega;
    h_cab_alb["provincia_entrega"] = provincia_entrega;
    h_cab_alb["cp_entrega"] = cp_entrega;
    h_cab_alb["id_pais_entrega"] = id_pais_entrega;
    h_cab_alb["email_entrega"] = email_entrega;
    h_cab_alb["comentarios_entrega"] = comentarios_entrega;

    h_cab_alb["id_transportista"] = id_transportista;

    h_cab_alb["cif"] = cif;

    h_cab_alb["recargo_equivalencia"] = recargo_equivalencia;
    h_cab_alb["subtotal"] = subtotal;
    h_cab_alb["porc_dto"] = porc_dto;
    h_cab_alb["dto"] = dto;
    h_cab_alb["porc_dto_pp"] = porc_dto_pp;
    h_cab_alb["dto_pp"] = dto_pp;
    h_cab_alb["base1"] = base1;
    h_cab_alb["base2"] = base2;
    h_cab_alb["base3"] = base3;
    h_cab_alb["base4"] = base4;
    h_cab_alb["porc_iva1"] = porc_iva1;
    h_cab_alb["porc_iva2"] = porc_iva2;
    h_cab_alb["porc_iva3"] = porc_iva3;
    h_cab_alb["porc_iva4"] = porc_iva4;

    h_cab_alb["iva1"] = iva1;
    h_cab_alb["iva2"] = iva2;
    h_cab_alb["iva3"] = iva3;
    h_cab_alb["iva4"] = iva4;
    h_cab_alb["porc_rec1"] = porc_rec1;
    h_cab_alb["porc_rec2"] = porc_rec2;
    h_cab_alb["porc_rec3"] = porc_rec3;
    h_cab_alb["porc_rec4"] = porc_rec4;

    h_cab_alb["rec1"] = rec1;
    h_cab_alb["rec2"] = rec2;
    h_cab_alb["rec3"] = rec3;
    h_cab_alb["rec4"] = rec4;

    h_cab_alb["total1"] = total1;
    h_cab_alb["total2"] = total2;
    h_cab_alb["total3"] = total3;
    h_cab_alb["total4"] = total4;
    h_cab_alb["base_total"] = base_total;
    h_cab_alb["iva_total"] = iva_total;
    h_cab_alb["rec_total"] = rec_total;

    h_cab_alb["total_albaran"] = total_albaran;
    h_cab_alb["id_forma_pago"] = id_forma_pago;
    h_cab_alb["impreso"] = impreso;
    h_cab_alb["facturado"] = facturado;
    h_cab_alb["factura"] = factura;
    h_cab_alb["fecha_factura"] = fecha_factura;
    h_cab_alb["comentario"] = comentario;

    h_cab_alb["desc_gasto1"] = desc_gasto1;
    h_cab_alb["desc_gasto2"] = desc_gasto2;
    h_cab_alb["desc_gasto3"] = desc_gasto3;

    h_cab_alb["imp_gasto1"] = imp_gasto1;
    h_cab_alb["imp_gasto2"] = imp_gasto2;
    h_cab_alb["imp_gasto3"] = imp_gasto3;

    h_cab_alb["porc_iva_gasto1"] = porc_iva_gasto1;
    h_cab_alb["porc_iva_gasto2"] = porc_iva_gasto2;
    h_cab_alb["porc_iva_gasto3"] = porc_iva_gasto3;

    h_cab_alb["iva_gasto1"] = iva_gasto1;
    h_cab_alb["iva_gasto2"] = iva_gasto2;
    h_cab_alb["iva_gasto3"] = iva_gasto3;

    h_cab_alb["entregado_a_cuenta"] = entregado_a_cuenta;
    h_cab_alb["serie"] = serie;
    h_cab_alb["id_agente"] = id_agente;

    bool updated = SqlCalls::SqlUpdate(h_cab_alb,"cab_alb",Configuracion_global->empresaDB,condiciones,error);

    if(!updated)
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Albaran:", error);
        return false;
    }
    else
    {
        return true;
    }
}

bool Albaran::RecuperarAlbaran(QString cSQL)
{
        QSqlQuery cab_alb(Configuracion_global->empresaDB);
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
                id = r.value("id").toInt();
                serie = r.value("serie").toString();
                albaran= r.value("albaran").toString();
                id_divisa = r.value("id_divisa").toInt();
                ejercicio = r.value("ejercicio").toInt();
                fecha= r.value("fecha").toDate();
                pedido_cliente= r.value("pedido_cliente").toString();
                id_cliente= r.value("id_cliente").toInt();
                codigo_cliente= r.value("codigo_cliente").toString();
                cliente= r.value("cliente").toString();
                direccion1= r.value("direccion1").toString();
                direccion2= r.value("direccion2").toString();
                poblacion= r.value("poblacion").toString();
                provincia= r.value("provincia").toString();
                cp= r.value("cp").toString();
                id_pais= r.value("id_pais").toInt();
                direccion1_entrega = r.value("direccion1_entrega").toString();
                direccion2_entrega = r.value("direccion2_entrega").toString();
                cp_entrega = r.value("cp_entrega").toString();
                poblacion_entrega = r.value("poblacion_entrega").toString();
                provincia_entrega = r.value("provincia_entrega").toString();
                id_pais_entrega = r.value("id_pais_entrega").toInt();
                email_entrega = r.value("email_entrega").toString();
                comentarios_entrega = r.value("comentarios_entrega").toString();
                id_transportista = r.value("id_transportista").toInt();
                //pais= Configuracion_global->Devolver_pais(id_pais);
                cif= r.value("cif").toString();
                recargo_equivalencia= r.value("recargo_equivalencia").toBool();
                subtotal= r.value("subtotal").toDouble();
                porc_dto= r.value("porc_dto").toFloat();
                dto= r.value("dto").toDouble();
                porc_dto_pp = r.value("porc_dto_pp").toFloat();
                dto_pp = r.value("dto_pp").toDouble();
                base1= r.value("base1").toDouble();
                base2= r.value("base2").toDouble();
                base3= r.value("base3").toDouble();
                base4= r.value("base4").toDouble();
                porc_iva1= r.value("porc_iva1").toDouble();
                porc_iva2= r.value("porc_iva2").toDouble();
                porc_iva3= r.value("porc_iva3").toDouble();
                porc_iva4= r.value("porc_iva4").toDouble();
                iva1= r.value("iva1").toDouble();
                iva2= r.value("iva2").toDouble();
                iva3= r.value("iva3").toDouble();
                iva4= r.value("iva4").toDouble();
                porc_rec1= r.value("porc_rec1").toDouble();
                porc_rec2= r.value("porc_rec2").toDouble();
                porc_rec3= r.value("porc_rec3").toDouble();
                porc_rec4= r.value("porc_rec4").toDouble();
                rec1= r.value("rec1").toDouble();
                rec2= r.value("rec2").toDouble();
                rec3= r.value("rec3").toDouble();
                rec4= r.value("rec4").toDouble();
                total1= r.value("total1").toDouble();
                total2= r.value("total2").toDouble();
                total3= r.value("total3").toDouble();
                total4= r.value("total4").toDouble();
                base_total= r.value("base_total").toDouble();
                iva_total= r.value("iva_total").toDouble();
                rec_total= r.value("rec_total").toDouble();
                total_albaran= r.value("total_albaran").toDouble();
                id_forma_pago = r.value("id_forma_pago").toInt();
                impreso= r.value("impreso").toBool();
                facturado= r.value("facturado").toBool();
                factura= r.value("factura").toString();
                fecha_factura= r.value("fecha_factura").toDate();
                comentario= r.value("comentario").toString();
                entregado_a_cuenta= r.value("entregado_a_cuenta").toDouble();
                desc_gasto1 = r.value("desc_gasto1").toString();
                desc_gasto2 = r.value("desc_gasto2").toString();
                desc_gasto3 = r.value("desc_gasto3").toString();
                imp_gasto1 = r.value("imp_gasto1").toDouble();
                imp_gasto2 = r.value("imp_gasto2").toDouble();
                imp_gasto3 = r.value("imp_gasto3").toDouble();
                porc_iva_gasto1 = r.value("porc_iva_gasto1").toFloat();
                porc_iva_gasto2 = r.value("porc_iva_gasto2").toFloat();
                porc_iva_gasto3 = r.value("porc_iva_gasto3").toFloat();
                iva_gasto1 = r.value("iva_gasto1").toDouble();
                iva_gasto2 = r.value("iva_gasto2").toDouble();
                iva_gasto3 = r.value("iva_gasto3").toDouble();
                editable = r.value("editable").toBool();
                id_agente = r.value("id_agente").toInt();
                return true;
               }
            else //if not next
                return false;
        }
}

QString Albaran::NuevoNumeroAlbaran(QString serie)
{
    QSqlQuery cab_alb(Configuracion_global->empresaDB);    
    QString cNumped;
    double inum = 0;

    QString cSQL = QString("Select albaran from cab_alb  where serie = '%1' order by albaran desc limit 1").arg(serie);
    cab_alb.prepare(cSQL);
    if(cab_alb.exec())
    {
        cab_alb.next();
        cNumped= cab_alb.value(0).toString();
        inum = cNumped.toDouble();
    }
    else
    {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_alb.lastError().text());
    }
    inum++;

    QString codigo_nuevo;
    QString formato = QString("%1.0f").arg(Configuracion_global->ndigitos_factura);
    formato.prepend("%0");
    std::string _x = formato.toStdString();

    codigo_nuevo.sprintf(_x.c_str(),inum);

    return codigo_nuevo;
}

bool Albaran::borrar_linea(int id_lin)
{
    QMap <int,QSqlRecord> lin;
    QHash <QString,QVariant> h;
    QString error;
    lin = SqlCalls::SelectRecord("lin_alb",QString("id=%1").arg(id_lin),Configuracion_global->empresaDB,error);
    if(!error.isEmpty())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Albaranes"),
                             tr("Ocurrió un error al cargar datos de línea a borrar: %1").arg(error),
                             tr("Aceptar"));
        return false;
    }

        bool success = SqlCalls::SqlDelete("lin_alb",Configuracion_global->empresaDB,QString("id=%1").arg(id_lin),error);
        if(success)
        {
            if(oArticulo->acumulado_devoluciones(lin.value(id_lin).value("id_articulo").toInt(),
                                         lin.value(id_lin).value("cantidad").toFloat(),
                                                lin.value(id_lin).value("total").toDouble(),
                                                QDate::currentDate(),"V"))
                return true;
        }
        else
        {
            QMessageBox::critical(qApp->activeWindow(), "Error:",error);
            return false;
        }
}


