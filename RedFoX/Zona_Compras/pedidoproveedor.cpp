
#include "pedidoproveedor.h"
#include <QPrintDialog>
#include "Auxiliares/frmdialogoimprimir.h"
#include "../Almacen/articulo.h"

PedidoProveedor::PedidoProveedor(QObject *parent) :
    QObject(parent)
{
    this->pedido = "0";
}

QString PedidoProveedor::nuevoNumPedido()
{
    QSqlQuery cab_pre(Configuracion_global->empresaDB);
    double presupuesto = 1;
    cab_pre.prepare("Select pedido from ped_pro order by pedido desc limit 1");
    if(cab_pre.exec())
    {
        cab_pre.next();
        presupuesto= cab_pre.value(0).toDouble();
        presupuesto ++;
        presupuesto = qMax(1.0,presupuesto);
    }
    else
    {
         QMessageBox::critical(qApp->activeWindow(), "Error:", cab_pre.lastError().text());
    }

    QString codigo_nuevo;
    QString formato = QString("%1.0f").arg(Configuracion_global->ndigitos_factura);
    formato.prepend("%0");
    std::string _x = formato.toStdString();

    codigo_nuevo.sprintf(_x.c_str(),presupuesto);

    return codigo_nuevo;
}

int PedidoProveedor::nuevo_pedido_proveedor()
{
    QHash<QString,QVariant> _data;
    _data["fecha"] = QDate::currentDate();
    _data["ejercicio"] = Configuracion_global->cEjercicio.toInt();
    _data["porc_iva1"] = Configuracion_global->ivaList.at(0);
    _data["porc_iva2"] = Configuracion_global->ivaList.at(1);
    _data["porc_iva3"] = Configuracion_global->ivaList.at(2);
    _data["porc_iva4"] = Configuracion_global->ivaList.at(3);

    QString error;
    int new_id = SqlCalls::SqlInsert(_data,"ped_pro",Configuracion_global->empresaDB,error);

    if(new_id == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),tr("No se puede insertar un nuevo pedido:\n%1").arg(error),tr("aceptar"));

    return new_id;
}

bool PedidoProveedor::guardar()
{
    // Si se trata de un nuevo pedido se le asigna numero de pedido
    if(this->pedido == "0")
        this->pedido = nuevoNumPedido();

    QSqlQuery queryGuardarPedido(Configuracion_global->empresaDB);
    QHash<QString,QVariant> _data;
    _data["pedido"] = pedido;
    _data["ejercicio"]= ejercicio;
    _data["fecha"]= fecha;
    _data["recepcion"]= recepcion;
    _data["id_proveedor"]= id_proveedor;
    _data["codigo_proveedor"]= codigo_proveedor;
    _data["proveedor"]= proveedor;
    _data["direccion1"]= direccion1;
    _data["direccion2"]= direccion2;
    _data["cp"]= cp;
    _data["poblacion"]= poblacion;
    _data["provincia"]= provincia;
    _data["id_pais"]= id_pais;
    _data["cif_nif"]= cif_nif;
    _data["base_total"]= base_total;
    _data["subtotal"]= subtotal;
    _data["dto"]= dto;
    _data["porc_dto"] = porc_dto;
    _data["rec_total"]= rec_total;
    _data["total"]= total;
    _data["enviado"]= enviado;
    _data["recibido"]= recibido;
    _data["recibido_completo"]= recibido_completo;
    _data["genero_pendiente"]= genero_pendiente;
    _data["recargo_equivalencia"]= recargo_equivalencia;
    _data["traspasado"]= traspasado;
    _data["pedido_cliente"]= pedido_cliente;
    _data["id_forma_pago"]= id_forma_pago;
    _data["comentario"]= comentario;
    _data["fecha_entrega"]= fecha_entrega;
    _data["direccion_entrega1"]= direccion_entrega1;
    _data["direccion_entrega2"]= direccion_entrega2;
    _data["cp_entrega"]= cp_entrega;
    _data["poblacion_entrega"]= poblacion_entrega;
    _data["provincia_entrega"]= provincia_entrega;
    _data["id_pais_entrega"]= id_pais_entrega;
    _data["nombre_cliente"]= nombre_cliente;
    _data["horario_activo"]= horario_activo;
    _data["base1"]= base1;
    _data["base2"]= base2;
    _data["base3"]= base3;
    _data["base4"]= base4;
    _data["iva1"]= iva1;
    _data["iva2"]= iva2;
    _data["iva3"]= iva3;
    _data["iva4"]= iva4;
    _data["porc_rec1"]= porc_rec1;
    _data["porc_rec2"]= porc_rec2;
    _data["porc_rec3"]= porc_rec3;
    _data["porc_rec4"]= porc_rec4;
    _data["rec1"]= rec1;
    _data["rec2"]= rec2;
    _data["rec3"]= rec3;
    _data["rec4"]= rec4;
    _data["total1"]= total1;
    _data["total2"]= total2;
    _data["total3"]= total3;
    _data["total4"]= total4;
    _data["porc_iva1"]= porc_iva1;
    _data["porc_iva2"]= porc_iva2;
    _data["porc_iva3"]= porc_iva3;
    _data["porc_iva4"]= porc_iva4;


    _data["telefono"]= telefono;
    _data["fax"]= fax;
    _data["movil"]= movil;
    /*
    _data["impreso"]= ;
    _data["editable"]= ;
    _data["gasto_to_coste"]= ;*/

    _data["desc_gasto1"]= desc_gasto1;
    _data["desc_gasto2"]= desc_gasto2;
    _data["desc_gasto3"]= desc_gasto3;
    _data["imp_gasto1"]= imp_gasto1;
    _data["imp_gasto2"]= imp_gasto2;
    _data["imp_gasto3"]= imp_gasto3;
    _data["porc_iva_gasto1"]= porc_iva_gasto1;
    _data["porc_iva_gasto2"]= porc_iva_gasto2;
    _data["porc_iva_gasto3"]= porc_iva_gasto3;
    _data["iva_gasto1"]= iva_gasto1;
    _data["iva_gasto2"]= iva_gasto2;
    _data["iva_gasto3"]= iva_gasto3;

    QString error;
    bool b = SqlCalls::SqlUpdate(_data,"ped_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id),error);
    if(!b)
    {
        QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN:"),
                             tr("Ha ocurrido un error al guardar el pedido: %1").arg(queryGuardarPedido.lastError().text()),
                             tr("Aceptar"));
    }
    return b;
}

bool PedidoProveedor::recuperar(int id)
{
    return recuperar(QString("select * from ped_pro where id = %1").arg(id));
}

bool PedidoProveedor::recuperar(QString cadenaSQL)
{
    QSqlQuery queryPedido(Configuracion_global->empresaDB);

    if(queryPedido.exec(cadenaSQL))
        return cargar(&queryPedido);
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Atención"),
                             tr("Se produjo un error al recuperar los datos: %1").arg(queryPedido.lastError().text()),
                             tr("Aceptar"));
        return false;
    }
}

void PedidoProveedor::clear()
{
    this->id = 0;
    this->pedido = "0";
    this->ejercicio = 0;
    this->fecha = QDate::currentDate();
    this->recepcion = QDate::currentDate();
    this->id_proveedor = 0;
    this->codigo_proveedor = "";
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
    this->rec_total = 0;
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
    this->direccion_entrega1 = "";
    this->direccion_entrega2 = "";
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
    this->porc_iva1 = 0;
    this->porc_iva2 = 0;
    this->porc_iva3 = 0;
    this->porc_iva4 = 0;
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

bool PedidoProveedor::cargar(QSqlQuery *queryPedido)
{
    if(queryPedido->next())
    {
        this->id = queryPedido->record().value("id").toInt();
        this->pedido = queryPedido->record().value("pedido").toString();
        this->ejercicio = queryPedido->record().value("ejercicio").toInt();
        this->fecha = queryPedido->record().value("fecha").toDate();
        this->recepcion = queryPedido->record().value("recepcion").toDate();
        this->id_proveedor = queryPedido->record().value("id_proveedor").toInt();
        this->codigo_proveedor = queryPedido->record().value("codigo_proveedor").toString();
        this->proveedor = queryPedido->record().value("proveedor").toString();
        this->direccion1 = queryPedido->record().value("direccion1").toString();
        this->direccion2 = queryPedido->record().value("direccion2").toString();
        this->cp = queryPedido->record().value("cp").toString();
        this->poblacion = queryPedido->record().value("poblacion").toString();
        this->provincia = queryPedido->record().value("provincia").toString();
        this->id_pais = queryPedido->record().value("id_pais").toInt();
        this->cif_nif = queryPedido->record().value("cif_nif").toString();
        this->base_total = queryPedido->record().value("base_total").toDouble();
        this->subtotal = queryPedido->record().value("subtotal").toDouble();
        this->dto = queryPedido->record().value("dto").toDouble();
        this->iva = queryPedido->record().value("iva").toInt();
        this->rec_total = queryPedido->record().value("rec_total").toDouble();
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
        this->direccion_entrega1 = queryPedido->record().value("direccion_entrega1").toString();
        this->direccion_entrega2 = queryPedido->record().value("direccion_entrega2").toString();
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
        this->porc_iva1 = queryPedido->record().value("porc_iva1").toDouble();
        this->porc_iva2 = queryPedido->record().value("porc_iva2").toDouble();
        this->porc_iva3 = queryPedido->record().value("porc_iva3").toDouble();
        this->porc_iva4 = queryPedido->record().value("porc_iva4").toDouble();
        this->iva1 = queryPedido->record().value("iva1").toDouble();
        this->iva2 = queryPedido->record().value("iva2").toDouble();
        this->iva3 = queryPedido->record().value("iva3").toDouble();
        this->iva4 = queryPedido->record().value("iva4").toDouble();
        this->dto1 = queryPedido->record().value("dto1").toDouble();
        this->dto2 = queryPedido->record().value("dto2").toDouble();
        this->dto3 = queryPedido->record().value("dto3").toDouble();
        this->dto4 = queryPedido->record().value("dto4").toDouble();
        this->porc_rec1 = queryPedido->record().value("porc_rec1").toDouble();
        this->porc_rec2 = queryPedido->record().value("porc_rec2").toDouble();
        this->porc_rec3 = queryPedido->record().value("porc_rec3").toDouble();
        this->porc_rec4 = queryPedido->record().value("porc_rec4").toDouble();
        this->rec1 = queryPedido->record().value("rec1").toDouble();
        this->rec2 = queryPedido->record().value("rec2").toDouble();
        this->rec3 = queryPedido->record().value("rec3").toDouble();
        this->rec4 = queryPedido->record().value("rec4").toDouble();
        this->total1 = queryPedido->record().value("total1").toDouble();
        this->total2 = queryPedido->record().value("total2").toDouble();
        this->total3 = queryPedido->record().value("total3").toDouble();
        this->total4 = queryPedido->record().value("total4").toDouble();
        desc_gasto1 = queryPedido->record().value("desc_gasto1").toString();
        desc_gasto2 = queryPedido->record().value("desc_gasto2").toString();
        desc_gasto3 = queryPedido->record().value("desc_gasto3").toString();
        imp_gasto1 = queryPedido->record().value("imp_gasto1").toDouble();
        imp_gasto2 = queryPedido->record().value("imp_gasto2").toDouble();
        imp_gasto3 = queryPedido->record().value("imp_gasto3").toDouble();
        porc_iva_gasto1 = queryPedido->record().value("porc_iva_gasto1").toDouble();
        porc_iva_gasto2 = queryPedido->record().value("porc_iva_gasto2").toDouble();
        porc_iva_gasto3 = queryPedido->record().value("porc_iva_gasto3").toDouble();
        iva_gasto1 = queryPedido->record().value("iva_gasto1").toDouble();
        iva_gasto2 = queryPedido->record().value("iva_gasto2").toDouble();
        iva_gasto3 = queryPedido->record().value("iva_gasto3").toDouble();

        telefono= queryPedido->record().value("telefono").toString();
        fax = queryPedido->record().value("fax").toString();
        movil = queryPedido->record().value("movil").toString();

        porc_dto = queryPedido->record().value("porc_dto").toDouble();
        return true;
    }
        return false;
}

bool PedidoProveedor::borrar(int id)
{
    Configuracion_global->transaction();
    QString error;
    bool succes = true;

    QMap<int, QSqlRecord> lineas = SqlCalls::SelectRecord("lin_ped_pro",QString("id_cab = %1").arg(id),Configuracion_global->empresaDB,error);
    for(auto i = lineas.begin(); i != lineas.end(); ++i)
    {
        QSqlRecord r = i.value();
        int id_art = r.value("id_articulo").toInt();
        double cantidad = r.value("cantidad").toDouble();
        double cant_rec = r.value("cantidad_recibida").toDouble();
        double cantidad_alb = r.value("en_albaran").toDouble();
        if(cant_rec > cantidad_alb)
        {
            error = tr("Este pedido tiene lineas recibidas sin pasar a albarán/factura");
            succes = false;
            break;
        }
        cantidad -= cant_rec;
        succes &= Articulo::set_pendiente_recibir(id_art , -1.0 * cantidad);
    }
    succes &= SqlCalls::SqlDelete("lin_ped_pro",Configuracion_global->empresaDB,QString("id_cab = %1").arg(id),error);
    succes &= SqlCalls::SqlDelete("ped_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id),error);

    if(!succes)
    {
        Configuracion_global->rollback();
        QMessageBox::warning(qApp->activeWindow(),tr("Ocurrió un error al borrar pedidos a proveedores"),error,tr("Aceptar"));
    }
    else
    {
        Configuracion_global->commit();
    }
    return succes;
}

bool PedidoProveedor::get(int id)
{
    QSqlQuery q(Configuracion_global->empresaDB);
    if(q.exec(QString("SELECT * FROM ped_pro WHERE id = %1").arg(id)))
        return cargar(&q);
    return false;
}

bool PedidoProveedor::next()
{
    QSqlQuery q(Configuracion_global->empresaDB);
    if(q.exec(QString("SELECT * FROM ped_pro WHERE pedido > %1 limit 0,1").arg(pedido)))
        return cargar(&q);
    return false;
}

bool PedidoProveedor::prev()
{
    QSqlQuery q(Configuracion_global->empresaDB);
    if(q.exec(QString("SELECT * FROM ped_pro WHERE pedido < %1 order by pedido desc  limit 0,1").arg(pedido)))
        return cargar(&q);
    return false;
}
