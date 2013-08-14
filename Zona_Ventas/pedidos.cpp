#include "pedidos.h"


Pedidos::Pedidos()
{
    this->id =0;
    iva_gasto1 = 0;
    iva_gasto2 = 0;
    iva_gasto3 = 0;
}

Pedidos::~Pedidos()
{
}

bool Pedidos::BorrarLineas(int Iped)
{
    QSqlQuery query(Configuracion_global->empresaDB);
    QString sql = QString("DELETE FROM lin_ped WHERE id_Cab = %1").arg(Iped);
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
    this->porc_iva1 = Configuracion_global->ivaList.at(0).toDouble();
    this->porc_iva2 = Configuracion_global->ivaList.at(1).toDouble();
    this->porc_iva3 = Configuracion_global->ivaList.at(2).toDouble();
    this->porc_iva4 = Configuracion_global->ivaList.at(3).toDouble();

    this->porc_rec1 = Configuracion_global->reList.at(0).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(1).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(2).toDouble();
    this->porc_rec1 = Configuracion_global->reList.at(3).toDouble();
    this->recargo_equivalencia = 0;

    QSqlQuery ped_cli(Configuracion_global->empresaDB);
     ped_cli.prepare("INSERT INTO ped_cli (pedido,porc_iva1,porc_iva2,porc_iva3,porc_iva4,"
                     "porc_rec1,porc_rec2,porc_rec3,porc_rec4,id_cliente)"
                   " VALUES (:pedido,:porc_iva1,:porc_iva2,:porc_iva3,:porc_iva4,"
                     ":porc_rec1,:porc_rec2,:porc_rec3,:porc_rec4,:id_cliente)");

     this->pedido = NuevoNumeroPedido();
     ped_cli.bindValue(":pedido",pedido);
     ped_cli.bindValue(":porc_iva1",this->porc_iva1);
     ped_cli.bindValue(":porc_iva2",this->porc_iva2);
     ped_cli.bindValue(":porc_iva3",this->porc_iva3);
     ped_cli.bindValue(":porc_iva4",this->porc_iva4);
     ped_cli.bindValue(":porc_rec1",this->porc_rec1);
     ped_cli.bindValue(":porc_rec2",this->porc_rec1);
     ped_cli.bindValue(":porc_rec3",this->porc_rec1);
     ped_cli.bindValue(":porc_rec4",this->porc_rec1);
     ped_cli.bindValue(":id_cliente",id_cliente);
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
bool Pedidos::GuardarPedido(int nid_Pedido)
{
    QSqlQuery ped_cli(Configuracion_global->empresaDB);

    ped_cli.prepare( "UPDATE ped_cli SET "
                     "albaran =:albaran,pedido =:pedido, fecha =:fecha  , pedido =:pedido  , id_cliente =:id_cliente  ,"
                     "codigo_cliente  =:codigo_cliente  ,  cliente =:cliente  , direccion1 =:direccion1,"
                     "direccion2 =:direccion2  , poblacion =:poblacion  ,provincia  =:provincia  ,"
                     "cp  =:cp  ,  id_pais  =:id_pais  ,  cif  =:cif  ,"
                     "recargo_equivalencia  =:recargo_equivalencia  ,  subtotal  =:subtotal ,"
                     "dto  =:dto  ,  base1  =:base1  ,  base2  =:base2  ,"
                     "base3  =:base3  ,  base4  =:base4  ,  porc_iva1  =:porc_iva1  ,"
                     "porc_iva2  =:porc_iva2  ,  porc_iva3  =:porc_iva3  ,"
                     "porc_iva4  =:porc_iva4  ,  iva1  =:iva1  ,"
                     "iva2  =:iva2  ,  iva3  =:iva3  ,"
                     "iva4  =:iva4  ,  porc_rec1  =:porc_rec1  ,"
                     "porc_rec2  =:porc_rec2  ,  porc_rec3  =:porc_rec3  ,"
                     "porc_rec4  =:porc_rec4  ,"
                     "rec1  =:rec1  ,"
                     "rec2  =:rec2  ,"
                     "rec3  =:rec3  ,"
                     "rec4  =:rec4  ,"
                     "total1  =:total1  ,  total2  =:total2  ,  total3  =:total3  ,"
                     "total4  =:total4  ,base_total  =:base_total  ,  iva_total  =:iva_total  ,"
                     "rec_total  =:rec_total  ,  total_albaran  =:total_albaran  ,"
                     "impreso  =:impreso  ,facturado  =:facturado  , "
                     "fecha_factura  =:fecha_factura  ,  comentario  =:comentario  ,"
                     "entregado_a_cuenta  =:entregado_a_cuenta  ,traspasado_albaran  =:traspasado_albaran  ,"
                     "traspasado_factura  =:traspasado_factura  ,  direccion1_entrega  =:direccion_entrega1  ,"
                     "direccion2_entrega  =:direccion_entrega2  ,cp_entrega  =:cp_entrega  ,"
                     "poblacion_entrega  =:poblacion_entrega  ,  provincia_entrega  =:provincia_entrega  ,"
                     "pais_entrega  =:pais_entrega  ,enviado  =:enviado  ,  completo  =:completo  ,"
                     "entregado  =:entregado  ,  fecha_limite_entrega  =:fecha_limite_entrega  ,"
                     "total_pedido  =:total_pedido,"
                     "desc_gasto1 = :desc_gasto1,"
                     "desc_gasto2 = :desc_gasto2,"
                     "desc_gasto3 = :desc_gasto3,"
                     "imp_gasto1 = :imp_gasto1,"
                     "imp_gasto2 = :imp_gasto2,"
                     "imp_gasto3 = :imp_gasto3,"
                     "porc_iva_gasto1 = :porc_iva_gasto1,"
                     "porc_iva_gasto2 = :porc_iva_gasto2,"
                     "porc_iva_gasto3 = :porc_iva_gasto3,"
                     "iva_gasto1 =:iva_gasto1,"
                     "iva_gasto2 =:iva_gasto2,"
                     "iva_gasto3 =:iva_gasto3"
                     " WHERE id = :id");

    ped_cli.bindValue(":albaran",albaran);
    ped_cli.bindValue(":pedido",pedido);
    ped_cli.bindValue(":fecha",fecha);
    ped_cli.bindValue(":pedido",pedido);
    ped_cli.bindValue(":id_cliente",id_cliente);
    ped_cli.bindValue(":codigo_cliente",codigo_cliente);
    ped_cli.bindValue(":cliente",cliente);
    ped_cli.bindValue(":direccion1",direccion1);
    ped_cli.bindValue(":direccion2",direccion2);
    ped_cli.bindValue(":poblacion",poblacion);
    ped_cli.bindValue(":provincia",provincia);
    ped_cli.bindValue(":cp",cp);
    ped_cli.bindValue(":id_pais",id_pais);
    ped_cli.bindValue(":cif",cif);
    ped_cli.bindValue(":recargo_equivalencia",recargo_equivalencia);
    ped_cli.bindValue(":subtotal",subtotal);
    ped_cli.bindValue(":dto",dto);
  //  ped_cli.bindValue(":dto",dto);
    ped_cli.bindValue(":base1",base1);
    ped_cli.bindValue(":base2",base2);
    ped_cli.bindValue(":base3",base3);
    ped_cli.bindValue(":base4",base4);
    ped_cli.bindValue(":porc_iva1",porc_iva1);
    ped_cli.bindValue(":porc_iva2",porc_iva2);
    ped_cli.bindValue(":porc_iva3",porc_iva3);
    ped_cli.bindValue(":porc_iva4",porc_iva4);
    ped_cli.bindValue(":iva1",iva1);
    ped_cli.bindValue(":iva2",iva2);
    ped_cli.bindValue(":iva3",iva3);
    ped_cli.bindValue(":iva4",iva4);
    ped_cli.bindValue(":porc_rec1",porc_rec1);
    ped_cli.bindValue(":porc_rec2",porc_rec2);
    ped_cli.bindValue(":porc_rec3",porc_rec3);
    ped_cli.bindValue(":porc_rec4",porc_rec4);
    ped_cli.bindValue(":rec1",rec1);
    ped_cli.bindValue(":rec2",rec2);
    ped_cli.bindValue(":rec3",rec3);
    ped_cli.bindValue(":rec4",rec4);
    ped_cli.bindValue(":total1",total1);
    ped_cli.bindValue(":total2",total2);
    ped_cli.bindValue(":total3",total3);
    ped_cli.bindValue(":total4",total4);
    ped_cli.bindValue(":base_total",base_total);
    ped_cli.bindValue(":iva_total",iva_total);
    ped_cli.bindValue(":rec_total",rec_total);
    ped_cli.bindValue(":total_albaran",total_albaran);
    ped_cli.bindValue(":impreso",impreso);
    ped_cli.bindValue(":facturado",facturado);
   // ped_cli.bindValue(":factura",factura);
    ped_cli.bindValue(":fecha_factura",fecha_factura);
    ped_cli.bindValue(":comentario",comentario);
    ped_cli.bindValue(":entregado_a_cuenta",entregado_a_cuenta);
    ped_cli.bindValue(":traspasado_albaran",traspasado_albaran);
    ped_cli.bindValue(":traspasado_factura",traspasado_factura);
    ped_cli.bindValue(":direccion_entrega1",direccion_entrega1);
    ped_cli.bindValue(":direccion_entrega2",direccion_entrega2);
    ped_cli.bindValue(":cp_entrega",cp_entrega);
    ped_cli.bindValue(":poblacion_entrega",poblacion_entrega);
    ped_cli.bindValue(":provincia_entrega",provincia_entrega);
    ped_cli.bindValue(":pais_entrega",pais_entrega);
    ped_cli.bindValue(":enviado",enviado);
    ped_cli.bindValue(":completo",completo);
    ped_cli.bindValue(":entregado",entregado);
    ped_cli.bindValue(":fecha_limite_entrega",fecha_limite_entrega);
    ped_cli.bindValue(":total_pedido",total_pedido);
    ped_cli.bindValue(":desc_gasto1",gasto1);
    ped_cli.bindValue(":desc_gasto2",gasto2);
    ped_cli.bindValue(":desc_gasto3",gasto3);
    ped_cli.bindValue(":imp_gasto1",imp_gasto1);
    ped_cli.bindValue(":imp_gasto2",imp_gasto2);
    ped_cli.bindValue(":imp_gasto3",imp_gasto3);
    ped_cli.bindValue(":porc_iva_gasto1",porc_iva_gasto1);
    ped_cli.bindValue(":porc_iva_gasto2",porc_iva_gasto2);
    ped_cli.bindValue(":porc_iva_gasto3",porc_iva_gasto3);
    ped_cli.bindValue(":iva_gasto1",iva_gasto1);
    ped_cli.bindValue(":iva_gasto2",iva_gasto2);
    ped_cli.bindValue(":iva_gasto3",iva_gasto3);
    ped_cli.bindValue(":id",nid_Pedido);
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
    QSqlQuery ped_cli(Configuracion_global->empresaDB);
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
            id = r.value("id").toInt();
            albaran = r.value("albaran").toInt();
            pedido = r.value("pedido").toInt();
            fecha = r.value("fecha").toDate();
            //pedido = r.value("pedido").toString();
            id_cliente = r.value("id_cliente").toInt();
            codigo_cliente = r.value("codigo_cliente").toString();
            cliente = r.value("cliente").toString();
            direccion1 = r.value("direccion1").toString();
            direccion2 = r.value("direccion2").toString();
            poblacion = r.value("poblacion").toString();
            provincia = r.value("provincia").toString();
            cp = r.value("cp").toString();
            cif = r.value("cif").toString();
            recargo_equivalencia = r.value("recargo_equivalencia").toInt();
            subtotal = r.value("subtotal").toDouble();
            porc_dto = r.value("porc_dto").toDouble();
            dto = r.value("dto").toDouble();
            base1 = r.value("base1").toDouble();
            base2 = r.value("base2").toDouble();
            base3 = r.value("base3").toDouble();
            base4 = r.value("base4").toDouble();
            porc_iva1 = r.value("porc_iva1").toDouble();
            porc_iva2 = r.value("porc_iva2").toDouble();
            porc_iva3 = r.value("porc_iva3").toDouble();
            porc_iva4 = r.value("porc_iva4").toDouble();
            iva1 = r.value("iva1").toDouble();
            iva2 = r.value("iva2").toDouble();
            iva3 = r.value("iva3").toDouble();
            iva4 = r.value("iva4").toDouble();
            porc_rec1 = r.value("porc_rec1").toDouble();
            porc_rec2 = r.value("porc_rec2").toDouble();
            porc_rec3 = r.value("porc_rec3").toDouble();
            porc_rec4 = r.value("porc_rec4").toDouble();
            rec1 = r.value("rec1").toDouble();
            rec2 = r.value("rec2").toDouble();
            rec3 = r.value("rec3").toDouble();
            rec4 = r.value("rec4").toDouble();
            total1 = r.value("total1").toDouble();
            total2 = r.value("total2").toDouble();
            total3 = r.value("total3").toDouble();
            total4 = r.value("total4").toDouble();
            base_total = r.value("base_total").toDouble();
            iva_total = r.value("iva_total").toDouble();
            rec_total = r.value("rec_total").toDouble();
            total_albaran = r.value("total_albaran").toDouble();
            impreso = r.value("impreso").toInt();
            facturado = r.value("facturado").toInt();
            factura = r.value("factura").toString();
            fecha_factura = r.value("fecha_factura").toDate();
            comentario = r.value("comentario").toString();
            entregado_a_cuenta = r.value("entregado_a_cuenta").toDouble();
            traspasado_albaran = r.value("traspasado_albaran").toInt();
            traspasado_factura = r.value("traspasado_factura").toInt();
            direccion_entrega1 = r.value("direccion_entrega1").toString();
            direccion_entrega2 = r.value("direccion_entrega2").toString();
            cp_entrega = r.value("cp_entrega").toString();
            poblacion_entrega = r.value("poblacion_entrega").toString();
            provincia_entrega = r.value("provincia_entrega").toString();
            pais_entrega = r.value("pais_entrega").toString();
            enviado = r.value("enviado").toInt();
            completo = r.value("completo").toInt();
            entregado = r.value("entregado").toInt();
            fecha_limite_entrega = r.value("fecha_limite_entrega").toDate();
            total_pedido = r.value("total_pedido").toDouble();
            gasto1 = r.value("desc_gasto1").toString();
            gasto2 = r.value("desc_gasto2").toString();
            gasto3 = r.value("desc_gasto3").toString();
            imp_gasto1 = r.value("imp_gasto1").toDouble();
            imp_gasto2 = r.value("imp_gasto2").toDouble();
            imp_gasto3 = r.value("imp_gasto3").toDouble();
            porc_iva_gasto1 = r.value("porc_iva_gasto1").toFloat();
            porc_iva_gasto2 = r.value("porc_iva_gasto2").toFloat();
            porc_iva_gasto3 = r.value("porc_iva_gasto3").toFloat();
            iva_gasto1 = r.value("iva_gasto1").toDouble();
            iva_gasto2 = r.value("iva_gasto2").toDouble();
            iva_gasto3 = r.value("iva_gasto3").toDouble();
            this->id_pais = r.field("id_pais").value().toInt();
            QSqlQuery q(Configuracion_global->empresaDB);
            if(q.exec("SELECT * FROM paises WHERE id="+QString::number(id_pais)))
                if(q.next())
                    pais = q.record().value("pais").toString();
            // Tarifa
            QSqlQuery queryCliente(Configuracion_global->groupDB);
            if(queryCliente.exec("select tarifa_cliente from clientes where id = "+QString::number(this->id_cliente)));
                if (queryCliente.next())
                    tarifa_cliente = queryCliente.record().value("tarifa_cliente").toInt();


           }
        else
            return false;
    }
    return true;
}

int Pedidos::NuevoNumeroPedido()
{
    QSqlQuery ped_cli(Configuracion_global->empresaDB);
    int pedido = 0;
    ped_cli.prepare("Select pedido from ped_cli order by pedido desc limit 1");
    if(ped_cli.exec())
    {
        if(ped_cli.next())
        {
            pedido= ped_cli.value(0).toInt();
            pedido ++;
        }
    }
    return qMax(1,pedido);
}

void Pedidos::FacturarPedido()
{
    //TODO Pedidos::FacturarPedido()
}
