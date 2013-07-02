
#include "pedidoproveedor.h"
#include <QPrintDialog>
#include "../Auxiliares/frmdialogoimprimir.h"

PedidoProveedor::PedidoProveedor(QObject *parent) :
    QObject(parent)
{
    this->pedido = 0;
}

int PedidoProveedor::nuevo_pedido_proveedor()
{
    QSqlQuery queryPedido(QSqlDatabase::database("empresa"));
    queryPedido.prepare("INSERT INTO ped_pro (`fecha`,`ejercicio`,`iva1`,`iva2`,`iva3`,`iva4`) "
                        "VALUES(:fecha,:ejercicio,:iva1,:iva2,:iva3,:iva4);");
    queryPedido.bindValue(":fecha",QDate::currentDate());
    queryPedido.bindValue(":ejercicio",Configuracion_global->cEjercicio.toInt());
    queryPedido.bindValue(":iva1",Configuracion_global->ivaList.at(0));
    queryPedido.bindValue(":iva2",Configuracion_global->ivaList.at(1));
    queryPedido.bindValue(":iva3",Configuracion_global->ivaList.at(2));
    queryPedido.bindValue(":iva4",Configuracion_global->ivaList.at(3));
    if(!queryPedido.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),
                             tr("No se puede insertar un nuevo pedido: %1").arg(queryPedido.lastError().text()),
                             tr("aceptar"));

    } else
        return queryPedido.lastInsertId().toInt();
}

void PedidoProveedor::guardar()
{
    //-------------------------------------------------------------
    // Si se trata de un nuevo pedido se le asigna numero de pedido
    //-------------------------------------------------------------
    QString cSql;
    int pedido =0;
    QSqlQuery queryPedido(QSqlDatabase::database("empresa"));

    if(this->pedido == 0)
    {

        if(queryPedido.exec("select pedido from ped_pro order by pedido desc limit 0,1"))
        {
            queryPedido.next();
            pedido = queryPedido.record().value("pedido").toInt();
            pedido++;
            this->pedido = pedido;
        }

    }
    QSqlQuery queryGuardarPedido(QSqlDatabase::database("empresa"));
    queryGuardarPedido.prepare("UPDATE ped_pro SET pedido =  :pedido,"
    "ejercicio =:serie,"
    "fecha =:fecha,"
    "recepcion =:recepcion,"
    "id_proveedor =:id_proveedor,"
    "codigoProveedor =:codigoProveedor,"
    "proveedor =:proveedor,"
    "direccion1 =:direccion1,"
    "direccion2 =:direccion2,"
    "cp =:cp,"
    "poblacion =:poblacion,"
    "provincia =:provincia,"
    "id_pais =:id_pais,"
    "cif_nif =:cif_nif,"
    "base_total =:base_total,"
    "subtotal =:subtotal,"
    "dto =:dto,"
     "recTotal =:recTotal,"
     "total =:total,"
     "enviado =:enviado,"
     "recibido =:recibido,"
     "recibido_completo =:recibido_completo,"
     "genero_pendiente =:genero_pendiente,"
     "recargo_equivalencia =:recargo_equivalencia,"
     "traspasado =:traspasado,"
     "pedido_cliente =:pedido_cliente,"
     "id_forma_pago =:id_forma_pago,"
     "comentario =:comentario,"
     "fecha_entrega =:fecha_entrega,"
     "direccion1_entrega =:direccion1_entrega,"
     "direccion2_entrega =:direccion2_entrega,"
     "cp_entrega =:cp_entrega,"
     "poblacion_entrega =:poblacion_entrega,"
     "provincia_entrega =:provincia_entrega,"
     "id_pais_entrega =:id_pais_entrega,"
     "nombre_cliente =:nombre_cliente,"
     "horario_activo =:horario_activo,"
     "base1 =:base1,"
     "base2 =:base2,"
     "base3 =:base3,"
     "base4 =:base4,"
     "iva1 =:iva1,"
     "iva2 =:iva2,"
     "iva3 =:iva3,"
     "iva4 =:iva4,"
     "iva1 =:iva1,"
     "iva2 =:iva2,"
     "iva3 =:iva3,"
     "iva4 =:iva4,"
     "porc_rec1 =:porc_rec1,"
     "porc_rec2 =:porc_rec2,"
     "porc_rec3 =:porc_rec3,"
     "porc_rec4 =:porc_rec4,"
     "rec1 =:rec1,"
     "rec2 =:rec2,"
     "rec3 =:rec3,"
     "rec4 =:rec4,"
     "total1 =:total1,"
     "total2 =:total2,"
     "total3 =:total3,"
     "total4 =:total4 "
    " WHERE id = :id;");

    queryGuardarPedido.bindValue(":serie",this->ejercicio);
    queryGuardarPedido.bindValue(":pedido",this->pedido);
    queryGuardarPedido.bindValue(":fecha",this->fecha);
    queryGuardarPedido.bindValue(":recepcion",this->recepcion);
    queryGuardarPedido.bindValue(":id_proveedor",this->id_proveedor);
    queryGuardarPedido.bindValue(":codigoProveedor",this->codigoProveedor);
    queryGuardarPedido.bindValue(":proveedor",this->proveedor);
    queryGuardarPedido.bindValue(":direccion1",this->direccion1);
    queryGuardarPedido.bindValue(":direccion2",this->direccion2);
    queryGuardarPedido.bindValue(":cp",this->cp);
    queryGuardarPedido.bindValue(":poblacion",this->poblacion);
    queryGuardarPedido.bindValue(":provincia",this->provincia);
    queryGuardarPedido.bindValue(":id_pais",this->id_pais);
    queryGuardarPedido.bindValue(":cif_nif",this->cif_nif);
    queryGuardarPedido.bindValue(":base_total",this->base_total);
    queryGuardarPedido.bindValue(":subtotal",this->subtotal);
    queryGuardarPedido.bindValue(":dto",this->dto);
    queryGuardarPedido.bindValue(":recTotal",this->recTotal);
    queryGuardarPedido.bindValue(":total",this->total);
    queryGuardarPedido.bindValue(":enviado",this->enviado);
    queryGuardarPedido.bindValue(":recibido",this->recibido);
    queryGuardarPedido.bindValue(":recibido_completo",this->recibido_completo);
    queryGuardarPedido.bindValue(":genero_pendiente",this->genero_pendiente);
    queryGuardarPedido.bindValue(":recargo_equivalencia",this->recargo_equivalencia);
    queryGuardarPedido.bindValue(":traspasado",this->traspasado);
    queryGuardarPedido.bindValue(":pedido_cliente",this->pedido_cliente);
    queryGuardarPedido.bindValue(":id_forma_pago",this->id_forma_pago);
    queryGuardarPedido.bindValue(":comentario",this->comentario);
    queryGuardarPedido.bindValue(":fecha_entrega",this->fecha_entrega);
    queryGuardarPedido.bindValue(":direccion1_entrega",this->direccion1_entrega);
    queryGuardarPedido.bindValue(":direccion2_entrega",this->direccion2_entrega);
    queryGuardarPedido.bindValue(":cp_entrega",this->cp_entrega);
    queryGuardarPedido.bindValue(":poblacion_entrega",this->poblacion_entrega);
    queryGuardarPedido.bindValue(":provincia_entrega",this->provincia_entrega);
    queryGuardarPedido.bindValue(":id_pais_entrega",this->id_pais_entrega);
    queryGuardarPedido.bindValue(":nombre_cliente",this->nombre_cliente);
    queryGuardarPedido.bindValue(":horario_activo",this->horario_activo);
    queryGuardarPedido.bindValue(":base1",this->base1);
    queryGuardarPedido.bindValue(":base2",this->base2);
    queryGuardarPedido.bindValue(":base3",this->base3);
    queryGuardarPedido.bindValue(":base4",this->base4);
    queryGuardarPedido.bindValue(":iva1",this->iva1);
    queryGuardarPedido.bindValue(":iva2",this->iva2);
    queryGuardarPedido.bindValue(":iva3",this->iva3);
    queryGuardarPedido.bindValue(":iva4",this->iva4);
    queryGuardarPedido.bindValue(":iva1",this->iva1);
    queryGuardarPedido.bindValue(":iva2",this->iva2);
    queryGuardarPedido.bindValue(":iva3",this->iva3);
    queryGuardarPedido.bindValue(":iva4",this->iva4);
    queryGuardarPedido.bindValue(":porc_rec1",this->porc_rec1);
    queryGuardarPedido.bindValue(":porc_rec2",this->porc_rec2);
    queryGuardarPedido.bindValue(":porc_rec3",this->porc_rec3);
    queryGuardarPedido.bindValue(":porc_rec4",this->porc_rec4);
    queryGuardarPedido.bindValue(":rec1",this->rec1);
    queryGuardarPedido.bindValue(":rec2",this->rec2);
    queryGuardarPedido.bindValue(":rec3",this->rec3);
    queryGuardarPedido.bindValue(":rec4",this->rec4);
    queryGuardarPedido.bindValue(":total1",this->total1);
    queryGuardarPedido.bindValue(":total2",this->total2);
    queryGuardarPedido.bindValue(":total3",this->total3);
    queryGuardarPedido.bindValue(":total4",this->total4);
    queryGuardarPedido.bindValue(":id",this->id);

    if(!queryGuardarPedido.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),
                             tr("Ha ocurrido un error al guardar el pedido: %1").arg(queryGuardarPedido.lastError().text()),
                             tr("Aceptar"));

//    qDebug() << queryGuardarPedido.lastQuery();
//    qDebug() << "id: " << this->id;
//    qDebug() << "P: " << this->proveedor;

}

void PedidoProveedor::recuperar(int id)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));
    queryPedido->prepare("select * from ped_pro where id = :id");
    queryPedido->bindValue(":id",id);
    if(queryPedido->exec())
    {
        cargar(queryPedido,0);
  }else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;

}


void PedidoProveedor::recuperar(QString cadenaSQL)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));

    if(queryPedido->exec(cadenaSQL))
    {
        cargar(queryPedido,0);
    }else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;
}

void PedidoProveedor::recuperar(QString cadenaSQL, int accion)
{
    QSqlQuery *queryPedido = new QSqlQuery(QSqlDatabase::database("empresa"));

    if(queryPedido->exec(cadenaSQL))
    {
        cargar(queryPedido,accion);
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido->lastError().text()),
                             tr("Aceptar"));
    }
    delete queryPedido;
}

void PedidoProveedor::clear()
{
    this->id = 0;
    this->pedido = 0;
    this->ejercicio = 0;
    this->fecha = QDate::currentDate();
    this->recepcion = QDate::currentDate();
    this->id_proveedor = 0;
    this->codigoProveedor = "";
    this->proveedor = "";
    this->direccion1 = "";
    this->direccion2 = "";
    this->cp = "";
    this->poblacion = "";
    this->provincia = "";
    this->id_pais = 0;
    this->cif_nif = "";
    this->base_total = 0;
    this->subtotal = 0;
    this->dto = 0;
    this->iva = 0;
    this->recTotal = 0;
    this->total = 0;
    this->enviado = false;
    this->recibido = false;
    this->recibido_completo = false;
    this->genero_pendiente = false;
    this->traspasado = false;
    this->recargo_equivalencia = false;
    this->pedido_cliente = 0;
    this->id_forma_pago = 0;
    this->comentario = "";
    this->fecha_entrega = QDate::currentDate();
    this->direccion1_entrega = "";
    this->direccion2_entrega = "";
    this->cp_entrega = "";
    this->poblacion_entrega = "";
    this->provincia_entrega = "";
    this->id_pais_entrega = 0;
    this->nombre_cliente = "";
    this->horario_activo = "";

    this->base1 = 0;
    this->base2 = 0;
    this->base3 = 0;
    this->base4 = 0;
    this->iva1 = 0;
    this->iva2 = 0;
    this->iva3 = 0;
    this->iva4 = 0;
    this->iva1 = 0;
    this->iva2 = 0;
    this->iva3 = 0;
    this->iva4 = 0;
    this->dto1 = 0;
    this->dto2 = 0;
    this->dto3 = 0;
    this->dto4 = 0;
    this->porc_rec1 = 0;
    this->porc_rec2 = 0;
    this->porc_rec3 = 0;
    this->porc_rec4 = 0;
    this->rec1 = 0;
    this->rec2 = 0;
    this->rec3 = 0;
    this->rec4 = 0;
    this->total1 = 0;
    this->total2 = 0;
    this->total3 = 0;
    this->total4 = 0;
}

void PedidoProveedor::cargar(QSqlQuery *queryPedido, int accion)
{
    if(queryPedido->next()){
        this->id = queryPedido->record().value("id").toInt();
        this->pedido = queryPedido->record().value("pedido").toInt();
        this->ejercicio = queryPedido->record().value("ejercicio").toInt();
        this->fecha = queryPedido->record().value("fecha").toDate();
        this->recepcion = queryPedido->record().value("recepcion").toDate();
        this->id_proveedor = queryPedido->record().value("id_proveedor").toInt();
        this->codigoProveedor = queryPedido->record().value("codigo_proveedor").toString();
        this->proveedor = queryPedido->record().value("proveedor").toString();
        this->direccion1 = queryPedido->record().value("direccion1").toString();
        this->direccion2 = queryPedido->record().value("direccion2").toString();
        this->cp = queryPedido->record().value("cp").toString();
        this->poblacion = queryPedido->record().value("poblacion").toString();
        this->provincia = queryPedido->record().value("provincia").toString();
        this->id_pais = queryPedido->record().value("id_pais").toInt();
        this->cif_nif = queryPedido->record().value("cif_nif").toString();
        this->base_total = queryPedido->record().value("base").toDouble();
        this->subtotal = queryPedido->record().value("sub_total").toDouble();
        this->dto = queryPedido->record().value("dto").toDouble();
        this->iva = queryPedido->record().value("iva").toInt();
        this->recTotal = queryPedido->record().value("rec_total").toDouble();
        this->total = queryPedido->record().value("total").toDouble();
        this->enviado = queryPedido->record().value("enviado").toBool();
        this->recibido = queryPedido->record().value("recibido").toBool();
        this->recibido_completo = queryPedido->record().value("recibido_completo").toBool();
        this->recargo_equivalencia = queryPedido->record().value("recargo_equivalencia").toBool();
        this->genero_pendiente = queryPedido->record().value("genero_pendiente").toBool();
        this->traspasado = queryPedido->record().value("traspasado").toBool();
        this->pedido_cliente = queryPedido->record().value("pedido_cliente").toInt();
        this->id_forma_pago = queryPedido->record().value("id_forma_pago").toInt();
        this->comentario = queryPedido->record().value("comentario").toString();
        this->fecha_entrega = queryPedido->record().value("fecha_entrega").toDate();
        this->direccion1_entrega = queryPedido->record().value("direccion1_entrega").toString();
        this->direccion2_entrega = queryPedido->record().value("direccion2_entrega").toString();
        this->cp_entrega = queryPedido->record().value("cp_entrega").toString();
        this->poblacion_entrega = queryPedido->record().value("poblacion_entrega").toString();
        this->provincia_entrega = queryPedido->record().value("provincia_entrega").toString();
        this->id_pais_entrega = queryPedido->record().value("id_pais_entrega").toInt();
        this->nombre_cliente = queryPedido->record().value("nombre_cliente").toString();
        this->horario_activo = queryPedido->record().value("horario_activo").toString();
        this->base1 = queryPedido->record().value("base1").toDouble();
        this->base2 = queryPedido->record().value("base2").toDouble();
        this->base3 = queryPedido->record().value("base3").toDouble();
        this->base4 = queryPedido->record().value("base4").toDouble();
        this->porc_iva1 = queryPedido->record().value("porc_iva1").toInt();
        this->porc_iva2 = queryPedido->record().value("porc_iva2").toInt();
        this->porc_iva3 = queryPedido->record().value("porc_iva3").toInt();
        this->porc_iva4 = queryPedido->record().value("porc_iva4").toInt();
        this->iva1 = queryPedido->record().value("iva1").toDouble();
        this->iva2 = queryPedido->record().value("iva2").toDouble();
        this->iva3 = queryPedido->record().value("iva3").toDouble();
        this->iva4 = queryPedido->record().value("iva4").toDouble();
        this->dto1 = queryPedido->record().value("dto1").toDouble();
        this->dto2 = queryPedido->record().value("dto2").toDouble();
        this->dto3 = queryPedido->record().value("dto3").toDouble();
        this->dto4 = queryPedido->record().value("dto4").toDouble();
        this->porc_rec1 = queryPedido->record().value("porc_rec1").toInt();
        this->porc_rec2 = queryPedido->record().value("porc_rec2").toInt();
        this->porc_rec3 = queryPedido->record().value("porc_rec3").toInt();
        this->porc_rec4 = queryPedido->record().value("porc_rec4").toInt();
        this->rec1 = queryPedido->record().value("rec1").toDouble();
        this->rec2 = queryPedido->record().value("rec2").toDouble();
        this->rec3 = queryPedido->record().value("rec3").toDouble();
        this->rec4 = queryPedido->record().value("rec4").toDouble();
        this->total1 = queryPedido->record().value("total1").toDouble();
        this->total2 = queryPedido->record().value("total2").toDouble();
        this->total3 = queryPedido->record().value("total3").toDouble();
        this->total4 = queryPedido->record().value("total4").toDouble();
    } else
    {
        switch (accion) {
        case 0:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("No se ha encontrado el pedido."),tr("Aceptar"));
            break;
        case 1:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("Se ha llegado al final del fichero."),tr("Aceptar"));
            break;
        case 2:
            QMessageBox::warning(qApp->activeWindow(),tr("Atención"),tr("Se ha llegado al inicio del fichero."),tr("Aceptar"));
            break;

        }

    }
}
void PedidoProveedor::convertir_en_albaran()
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Gestión de proveedores"),
                             tr("¿Desea traspasar la información a un nuevo albarán?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
    {

    }
}

void PedidoProveedor::convertir_en_factura()
{

}

bool PedidoProveedor::borrar(int id)
{
    QSqlQuery queryPedido(QSqlDatabase::database("empresa"));
    QSqlDatabase::database("empresa").transaction();
    bool error = false;
    queryPedido.prepare("delete from lin_ped_pro where id_cab = :id");
    queryPedido.bindValue(":id",id);
    if(!queryPedido.exec())
        error = true;
    queryPedido.prepare("delete from ped_pro where id = :id");
    queryPedido.bindValue(":id",id);
    if(!queryPedido.exec())
        error = true;
    if(error == true) {
        QSqlDatabase::database("empresa").rollback();
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de pedidos a proveedores"),
                             tr("Ocurrió un error al borrar: %1").arg(queryPedido.lastError().text()),
                             tr("Aceptar"));
        return false;
    } else{
        QSqlDatabase::database("empresa").commit();
        return true;
    }
}

void PedidoProveedor::imprimir(int id)
{
    FrmDialogoImprimir imprimir(qApp->activeWindow());
    if(imprimir.exec() == QDialog::Accepted)
    {
        QPrintDialog print;
        print.exec();
//        QSqlQuery query_ped_pro(QSqlDatabase::database("empresa"));
//        query_ped_pro.prepare("update ped_pro set ");
    }

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
    int idplus = id+1;
    q.bindValue(":id",idplus);
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
    int idminus = id-1;
    q.bindValue(":id",idminus);
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
    QSqlQuery q(QSqlDatabase::database("empresa"));
    q.prepare("INSERT INTO ped_pro"
              "( pedido ,  serie ,  fecha ,  recepcion ,  id_proveedor ,  codigoProveedor ,"
              " proveedor ,  direccion1 ,  direccion2 ,  cp ,  poblacion ,  provincia ,  id_pais ,"
              " cif_nif ,  base ,  subtotal ,  dto ,  iva ,  recTotal ,  total ,  enviado ,  recibido ,"
              " recibido_completo ,  genero_pendiente ,  base1 ,  base2 ,  base3 ,  base4 ,  porc_iva1 ,"
              " porc_iva2 ,  porc_iva3 ,  porc_iva4 ,  iva1 ,  iva2 ,  iva3 ,  iva4 ,  total1 ,  total2 ,"
              " total3 ,  total4 ,  nMargeporc_rec1 ,  nMargeporc_rec2 ,  nMargeporc_rec3 ,  nMargeporc_rec4 ,  rec1 ,  rec2 ,"
              " rec3 ,  rec4 ,  traspasado ,  pedido_cliente ,  id_forma_pago ,  vencimiento1 ,  vencimiento2 ,"
              " vencimiento3 ,  vencimiento4 ,  pagado1 ,  pagado2 ,  pagado3 ,  pagado4 ,  comentario ,"
              " fecha_entrega ,  direccion1_entrega ,  direccion2_entrega ,  cp_entrega ,  poblacion_entrega ,"
              " provincia_entrega ,  id_pais_entrega ,  nombre_cliente ,  horario_activo )"
              " VALUES "
              "( :pedido ,  :serie ,  :fecha ,  :recepcion ,  :id_proveedor ,  :codigoProveedor ,"
              " :proveedor ,  :direccion1 ,  :direccion2 ,  :cp ,  :poblacion ,  :provincia ,  :id_pais ,"
              " :cif_nif ,  :base ,  :subtotal ,  :dto ,  :iva ,  :recTotal ,  :total ,  :enviado ,  :recibido ,"
              " :recibido_completo ,  :genero_pendiente ,  :base1 ,  :base2 ,  :base3 ,  :base4 ,  :porc_iva1 ,"
              " :porc_iva2 ,  :porc_iva3 ,  :porc_iva4 ,  :iva1 ,  :iva2 ,  :iva3 ,  :iva4 ,  :total1 ,  :total2 ,"
              " :total3 ,  :total4 ,  :nMargeporc_rec1 ,  :nMargeporc_rec2 ,  :nMargeporc_rec3 ,  :nMargeporc_rec4 ,  :rec1 ,  :rec2 ,"
              " :rec3 ,  :rec4 ,  :traspasado ,  :pedido_cliente ,  :id_forma_pago ,  :vencimiento1 ,  :vencimiento2 ,"
              " :vencimiento3 ,  :vencimiento4 ,  :pagado1 ,  :pagado2 ,  :pagado3 ,  :pagado4 ,  :comentario ,"
              " :fecha_entrega ,  :direccion1_entrega ,  :direccion2_entrega ,  :cp_entrega ,  :poblacion_entrega ,"
              " :provincia_entrega ,  :id_pais_entrega ,  :nombre_cliente ,  :horario_activo ");
            return -1;
}

bool PedidoProveedor::update()
{
    return false;
}

void PedidoProveedor::fillPedido(QSqlRecord r)
{
    id = r.value("id").toInt();
    pedido = r.value("pedido").toULongLong();
    //serie = r.value("serie").toString();
    fecha = r.value("fecha").toDate();
    recepcion = r.value("recepcion").toDate();
    id_proveedor = r.value("id_proveedor").toULongLong();
    codigoProveedor = r.value("codigoProveedor").toString();
    proveedor = r.value("proveedor").toString();
    direccion1 = r.value("direccion1").toString();
    direccion2 = r.value("direccion2").toString();
    cp = r.value("cp").toString();
    poblacion= r.value("poblacion").toString();
    provincia= r.value("provincia").toString();
    id_pais= r.value("id_pais").toInt();
    cif_nif= r.value("cif_nif").toString();
    //base= r.value("base").toDouble();
    subtotal= r.value("subtotal").toDouble();
    dto= r.value("dto").toDouble();
    iva= r.value("iva").toDouble();
    recTotal= r.value("recTotal").toDouble();
    total= r.value("total").toDouble();
    enviado= r.value("enviado").toULongLong();
    recibido= r.value("recibido").toULongLong();
    recibido_completo= r.value("recibido_completo").toULongLong();
    genero_pendiente= r.value("genero_pendiente").toULongLong();
    recargo_equivalencia = r.value("recargo_equivalencia").toBool();
    base1= r.value("base1").toDouble();
    base2= r.value("base2").toDouble();
    base3= r.value("base3").toDouble();
    base4= r.value("base4").toDouble();
    porc_iva1= r.value("porc_iva1").toDouble();
}

