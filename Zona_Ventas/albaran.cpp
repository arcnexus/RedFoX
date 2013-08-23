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
int Albaran::AnadirAlbaran(QString serie)
{
    int x = NuevoNumeroAlbaran(serie);
    QSqlQuery q(Configuracion_global->empresaDB);
    q.prepare("INSERT INTO cab_alb"
              "(albaran, fecha, pedido_cliente, id_cliente, codigo_cliente, cliente,"
              "direccion1, direccion2, poblacion, provincia, cp, id_pais, cif,"
              "recargo_equivalencia, subtotal, porc_dto, dto,porc_dto_pp, dto_pp, base1, base2, base3,"
              "base4, porc_iva1, porc_iva2, porc_iva3, porc_iva4,"
              "iva1, iva2, iva3, iva4, porc_rec1,"
              "porc_rec2, porc_rec3, porc_rec4,"
              "rec1, rec2, rec3, rec4,"
              "total1, total2, total3, total4, base_total, iva_total, rec_total,"
              "total_albaran, impreso, facturado, factura, fecha_factura, comentario,"
              "entregado_a_cuenta,ejercicio,serie) "
              "VALUES"
              "(:albaran, :fecha, :pedido_cliente, :id_cliente, :codigo_cliente, :cliente,"
              ":direccion1, :direccion2, :poblacion, :provincia, :cp, :id_pais, :cif,"
              ":recargo_equivalencia, :subtotal, :porc_dto, :dto, :porc_dto_pp,:dto_pp, :base1, :base2, :base3,"
              ":base4, :porc_iva1, :porc_iva2, :porc_iva3, :porc_iva4,"
              ":iva1, :iva2, :iva3, :iva4, :porc_rec1,"
              ":porc_rec2, :porc_rec3, :porc_rec4,"
              ":rec1, :rec2, :rec3, :rec4,"
              ":total1, :total2, :total3, :total4, :base_total, :iva_total, :rec_total,"
              ":total_albaran, :impreso, :facturado, :factura, :fecha_factura, :comentario,"
              ":entregado_a_cuenta,:ejercicio,:serie) ");
    q.bindValue(":albaran",x);
    q.bindValue(":fecha",fecha);
    q.bindValue(":pedido_cliente",pedido_cliente);
    q.bindValue(":id_cliente",id_cliente);
    q.bindValue(":codigo_cliente",codigo_cliente);
    q.bindValue(":cliente",cliente);

    q.bindValue(":direccion1",direccion1);
    q.bindValue(":direccion2",direccion2);
    q.bindValue(":poblacion",poblacion);
    q.bindValue(":provincia",provincia);
    q.bindValue(":cp",cp);
    q.bindValue(":id_pais",id_pais);
    q.bindValue(":cif",cif);

    q.bindValue(":recargo_equivalencia",recargo_equivalencia);
    q.bindValue(":subtotal",0);
    q.bindValue(":dto",0);
    q.bindValue(":porc_dto",0);
    q.bindValue(":base1",0);
    q.bindValue(":base2",0);
    q.bindValue(":base3",0);
    q.bindValue(":base4",0);
    q.bindValue(":porc_iva1",Configuracion_global->ivaList.at(0).toDouble());
    q.bindValue(":porc_iva2",Configuracion_global->ivaList.at(1).toDouble());
    q.bindValue(":porc_iva3",Configuracion_global->ivaList.at(2).toDouble());
    q.bindValue(":porc_iva4",Configuracion_global->ivaList.at(3).toDouble());

    q.bindValue(":iva1",0);
    q.bindValue(":iva2",0);
    q.bindValue(":iva3",0);
    q.bindValue(":iva4",0);
    q.bindValue(":porc_rec1",0);
    q.bindValue(":porc_rec2",0);
    q.bindValue(":porc_rec3",0);
    q.bindValue(":porc_rec4",0);

    q.bindValue(":rec1",0);
    q.bindValue(":rec2",0);
    q.bindValue(":rec3",0);
    q.bindValue(":rec4",0);

    q.bindValue(":total1",0);
    q.bindValue(":total2",0);
    q.bindValue(":total3",0);
    q.bindValue(":total4",0);
    q.bindValue(":base_total",0);
    q.bindValue(":iva_total",0);
    q.bindValue(":rec_total",0);

    q.bindValue(":total_albaran",0);
    q.bindValue(":impreso",0);
    q.bindValue(":facturado",0);
    q.bindValue(":factura",factura);
    q.bindValue(":fecha_factura",fecha_factura);
    q.bindValue(":comentario",comentario);

    q.bindValue(":entregado_a_cuenta",0);
    q.bindValue(":ejercicio", Configuracion_global->cEjercicio.toInt());
    q.bindValue(":serie",serie);

    if(!q.exec())
    {
        QMessageBox::critical(qApp->activeWindow(),"Error al guardar datos Albaran:", q.lastError().text());
        return false;
    }
    else
    {
        this->id = q.lastInsertId().toInt();
        return id;
    }
}

bool Albaran::GuardarAlbaran(int nid_Albaran)
{
    QSqlQuery q(Configuracion_global->empresaDB);
    q.prepare("UPDATE cab_alb SET "
              "fecha=:fecha, pedido_cliente=:pedido_cliente, id_cliente=:id_cliente,"
              "codigo_cliente=:codigo_cliente, cliente=:cliente,"
              "direccion1=:direccion1,direccion2=:direccion2, poblacion=:poblacion,"
              "provincia=:provincia, cp=:cp, id_pais=:id_pais, cif=:cif,"
              "direccion1_entrega=:direccion1_entrega,direccion2_entrega =:direccion2_entrega,"
              "cp_entrega =:cp_entrega,poblacion_entrega =:poblacion_entrega,"
              "provincia_entrega =:provincia_entrega,id_pais_entrega :=id_pais_entrega,"
              "email_entrega =:email_entrega, comentarios_entrega =:comentarios_entrega,"
              "recargo_equivalencia=:recargo_equivalencia, subtotal=:subtotal,"
              "porc_dto=:porc_dto, dto=:dto,porc_dto_pp=:porc_dto_pp,dto_pp =:dto_pp, base1=:base1, base2=:base2, base3=:base3,"
              "base4=:base4, porc_iva1=:porc_iva1, porc_iva2=:porc_iva2,"
              "porc_iva3=:porc_iva3, porc_iva4=:porc_iva4,"
              "iva1=:iva1, iva2=:iva2, iva3=:iva3,"
              "iva4=:iva4, porc_rec1=:porc_rec1,"
              "porc_rec2=:porc_rec2, porc_rec3=:porc_rec3,"
              "porc_rec4=:porc_rec4,"
              "rec1=:rec1, rec2=:rec2,"
              "rec3=:rec3, rec4=:rec4,"
              "total1=:total1, total2=:total2, total3=:total3, total4=:total4,"
              "base_total=:base_total, iva_total=:iva_total, rec_total=:rec_total,"
              "total_albaran=:total_albaran, impreso=:impreso, facturado=:facturado,"
              "factura=:factura, fecha_factura=:fecha_factura, comentario=:comentario,"
              "desc_gasto1 =:desc_gasto1,"
              "desc_gasto2 =:desc_gasto2,"
              "desc_gasto3 =:desc_gasto3,"
              "imp_gasto1 =:imp_gasto1,"
              "imp_gasto2 =:imp_gasto2,"
              "imp_gasto3 =:imp_gasto3,"
              "porc_iva_gasto1 =:porc_iva_gasto1,"
              "porc_iva_gasto2 =:porc_iva_gasto2,"
              "porc_iva_gasto3 =:porc_iva_gasto3,"
              "iva_gasto1 =:iva_gasto1,"
              "iva_gasto2 =:iva_gasto2,"
              "iva_gasto3 =:iva_gasto3,"
              "entregado_a_cuenta=:entregado_a_cuenta,"
              "serie = :serie"

              " WHERE id = :albaran");

    q.bindValue(":albaran",nid_Albaran);
    q.bindValue(":fecha",fecha);
    q.bindValue(":pedido_cliente",pedido_cliente);
    q.bindValue(":id_cliente",id_cliente);
    q.bindValue(":codigo_cliente",codigo_cliente);
    q.bindValue(":cliente",cliente);

    q.bindValue(":direccion1",direccion1);
    q.bindValue(":direccion2",direccion2);
    q.bindValue(":poblacion",poblacion);
    q.bindValue(":provincia",provincia);
    q.bindValue(":cp",cp);
    q.bindValue(":id_pais",id_pais);

    q.bindValue(":direccion1_entrega",direccion1_entrega);
    q.bindValue(":direccion2_entrega",direccion2_entrega);
    q.bindValue(":poblacion_entrega",poblacion_entrega);
    q.bindValue(":provincia_entrega",provincia_entrega);
    q.bindValue(":cp_entrega",cp_entrega);
    q.bindValue(":id_pais_entrega",id_pais_entrega);
    q.bindValue(":email_entrega",email_entrega);
    q.bindValue(":comentarios_entrega",comentarios_entrega);

    q.bindValue(":cif",cif);

    q.bindValue(":recargo_equivalencia",recargo_equivalencia);
    q.bindValue(":subtotal",subtotal);
    q.bindValue(":porc_dto",porc_dto);
    q.bindValue(":dto",dto);
    q.bindValue(":porc_dto_pp",porc_dto_pp);
    q.bindValue(":dto_pp",dto_pp);
    q.bindValue(":base1",base1);
    q.bindValue(":base2",base2);
    q.bindValue(":base3",base3);
    q.bindValue(":base4",base4);
    q.bindValue(":porc_iva1",porc_iva1);
    q.bindValue(":porc_iva2",porc_iva2);
    q.bindValue(":porc_iva3",porc_iva3);
    q.bindValue(":porc_iva4",porc_iva4);

    q.bindValue(":iva1",iva1);
    q.bindValue(":iva2",iva2);
    q.bindValue(":iva3",iva3);
    q.bindValue(":iva4",iva4);
    q.bindValue(":porc_rec1",porc_rec1);
    q.bindValue(":porc_rec2",porc_rec2);
    q.bindValue(":porc_rec3",porc_rec3);
    q.bindValue(":porc_rec4",porc_rec4);

    q.bindValue(":rec1",rec1);
    q.bindValue(":rec2",rec2);
    q.bindValue(":rec3",rec3);
    q.bindValue(":rec4",rec4);

    q.bindValue(":total1",total1);
    q.bindValue(":total2",total2);
    q.bindValue(":total3",total3);
    q.bindValue(":total4",total4);
    q.bindValue(":base_total",base_total);
    q.bindValue(":iva_total",iva_total);
    q.bindValue(":rec_total",rec_total);

    q.bindValue(":total_albaran",total_albaran);
    q.bindValue(":impreso",impreso);
    q.bindValue(":facturado",facturado);
    q.bindValue(":factura",factura);
    q.bindValue(":fecha_factura",fecha_factura);
    q.bindValue(":comentario",comentario);

    q.bindValue(":desc_gasto1",desc_gasto1);
    q.bindValue(":desc_gasto2",desc_gasto2);
    q.bindValue(":desc_gasto3",desc_gasto3);

    q.bindValue("imp_gasto1",imp_gasto1);
    q.bindValue("imp_gasto2",imp_gasto2);
    q.bindValue("imp_gasto3",imp_gasto3);

    q.bindValue(":porc_iva_gasto1",porc_iva_gasto1);
    q.bindValue(":porc_iva_gasto2",porc_iva_gasto2);
    q.bindValue(":porc_iva_gasto3",porc_iva_gasto3);

    q.bindValue(":iva_gasto1",iva_gasto1);
    q.bindValue(":iva_gasto2",iva_gasto2);
    q.bindValue(":iva_gasto3",iva_gasto3);

    q.bindValue(":entregado_a_cuenta",entregado_a_cuenta);
    q.bindValue(":serie",serie);

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
                albaran= r.value("albaran").toInt();
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

                return true;
               }
            else //if not next
                return false;
        }
}

int Albaran::NuevoNumeroAlbaran(QString serie)
{
    QSqlQuery cab_alb(Configuracion_global->empresaDB);
    int albaran = 1;
    QString cSQL = QString("Select albaran from cab_alb  where serie = '%1' order by albaran desc limit 1").arg(serie);
    cab_alb.prepare(cSQL);
    if(cab_alb.exec())
    {
        if(cab_alb.next()) {
            albaran= cab_alb.value(0).toInt();
            albaran ++;
        } else
            albaran = 1;
    }
    else
    {
         QMessageBox::critical(qApp->activeWindow(), "error:", cab_alb.lastError().text());
    }
    return albaran;
}

void Albaran::FacturarAlbaran()
{
    //TODO Albaran::FacturarAlbaran()
}
