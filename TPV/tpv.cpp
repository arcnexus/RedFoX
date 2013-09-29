#include "tpv.h"

tpv::tpv(QObject *parent) :
    QObject(parent)
{
    ticket =0;
}

int tpv::nuevoticket()
{
    QString error;
    QStringList condiciones;
    condiciones << QString("serie=%1").arg(this->serie) <<QString("ejercicio=%1").arg(Configuracion_global->cEjercicio)
                << QString("caja= %1").arg(this->caja);

    int num_last_ticket = SqlCalls::SelectOneField("cab_tpv","ticket",condiciones,Configuracion_global->empresaDB,error).toInt();
    int ticket = num_last_ticket ++;
    QHash <QString, QVariant> h;

    this->ticket = ticket;
    this->fecha =  QDate::currentDate();
    this->hora = QDateTime::currentDateTime().toString("hh:mm");
    this->porc_iva1 = Configuracion_global->ivaList.at(0).toFloat();
    this->porc_iva2 = Configuracion_global->ivaList.at(0).toFloat();
    this->porc_iva3 = Configuracion_global->ivaList.at(0).toFloat();
    this->porc_iva4 = Configuracion_global->ivaList.at(0).toFloat();

    h["ticket"] = ticket;
    h["caja"] = this->caja;
    h["serie"] = this->serie;
    h["fecha"] = this->fecha;
    h["hora"] = this->hora;
    h["id_usuario"] = this->id_usuario;
    h["porc_iva1"] = this->porc_iva1;
    h["porc_iva2"] = this->porc_iva2;
    h["porc_iva3"] = this->porc_iva3;
    h["porc_iva4"] = this->porc_iva4;


    int new_id = SqlCalls::SqlInsert(h,"cab_tpv",Configuracion_global->empresaDB,error);
    return new_id;

}

void tpv::recuperar(int id)
{
    QMap <int,QSqlRecord> m;
    QString error;
    m = SqlCalls::SelectRecord("cab_tpv",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        this->ticket = m.value(id).value("ticket").toInt();
        this->id_usuario = m.value(id).value("id_usuario").toInt();
        this->id = id;
        this->caja = m.value(id).value("caja").toInt();
        this->ticket = m.value(id).value("ticket").toInt();
        this->fecha = m.value(id).value("fecha").toDate();
        this->hora = m.value(id).value("hora").toString();
        this->id_usuario = m.value(id).value("id_usuario").toInt();
        this->id_cliente = m.value(id).value("id_cliente").toInt();
        this->codigo = m.value(id).value("codigo").toString();
        this->nombre_cliente = m.value(id).value("nombre_cliente").toString();
        this->ticket_en_espera = m.value(id).value("ticket_en_espera").toBool();
        this->pendiente_cobro = m.value(id).value("pendiente_cobro").toBool();
        this->id_forma_pago = m.value(id).value("id_forma_pago").toInt();
        this->tipo_targeta = m.value(id).value("id_tipo_targeta").toInt();
        this->impreso = m.value(id).value("impreso").toBool();
        this->importe = m.value(id).value("importe").toDouble();
        this->base1 = m.value(id).value("base1").toDouble();
        this->base2 = m.value(id).value("base2").toDouble();
        this->base3 = m.value(id).value("base3").toDouble();
        this->base4 = m.value(id).value("base4").toDouble();
        this->porc_iva1 = m.value(id).value("porc_iva1").toFloat();
        this->porc_iva2 = m.value(id).value("porc_iva2").toFloat();
        this->porc_iva3 = m.value(id).value("porc_iva3").toFloat();
        this->porc_iva4 = m.value(id).value("porc_iva4").toFloat();
        this->iva1 = m.value(id).value("iva1").toDouble();
        this->iva2 = m.value(id).value("iva2").toDouble();
        this->iva3 = m.value(id).value("iva3").toDouble();
        this->iva4 = m.value(id).value("iva4").toDouble();
        this->total1 = m.value(id).value("total1").toDouble();
        this->total2 = m.value(id).value("total2").toDouble();
        this->total3 = m.value(id).value("total3").toDouble();
        this->total4 = m.value(id).value("total4").toDouble();
        this->subtotal = m.value(id).value("subtotal").toDouble();
        this->dto = m.value(id).value("dto").toDouble();
        this->porc_rec1 = m.value(id).value("porc_rec1").toFloat();
        this->porc_rec2 = m.value(id).value("porc_rec2").toFloat();
        this->porc_rec3 = m.value(id).value("porc_rec3").toFloat();
        this->porc_rec4 = m.value(id).value("porc_rec4").toFloat();
        this->rec1 = m.value(id).value("rec1").toDouble();
        this->rec2 = m.value(id).value("rec2").toDouble();
        this->rec3 = m.value(id).value("rec3").toDouble();
        this->rec4 = m.value(id).value("rec4").toDouble();
        this->total_recargo =  m.value(id).value("total_recargo").toDouble();
        this->total_iva = m.value(id).value("total_iva").toDouble();
        this->cobrado = m.value(id).value("cobrado").toDouble();
        this->importe_pendiente_cobro = m.value(id).value("importe_pendiente_cobro").toDouble();
        this->importe_efectivo = m.value(id).value("importe_efectivo").toDouble();
        this->importe_tarjeta = m.value(id).value("importe_tarjeta").toDouble();
        this->importe_cheque = m.value(id).value("importe_cheque").toDouble();
        this->importe_credito = m.value(id).value("importe_credito").toDouble();
        this->importe_vale = m.value(id).value("importe_vale").toDouble();
        this->importe_dto_redondeo = m.value(id).value("importe_dto_redondeo").toDouble();
        this->importe_cambio = m.value(id).value("importe_cambio").toDouble();
        this->entregado_a_cuenta = m.value(id).value("entregado_a_cuenta").toDouble();
        this->pasado_a_albaran_factura = m.value(id).value("pasado_a_albaran_factura").toBool();
        this->numero_albaran = m.value(id).value("numero_albaran").toInt();
        this->numero_factura = m.value(id).value("numero_factura").toInt();
        this->id_cierre = m.value(id).value("id_cierre").toInt();
        this->ejercicio = m.value(id).value("ejercicio").toInt();
        this->editable = m.value(id).value("editable").toBool();
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Tickets"),tr("No se puede recuperar el registro: %1").arg(error),
                             tr("Aceptar"));
    }

}
