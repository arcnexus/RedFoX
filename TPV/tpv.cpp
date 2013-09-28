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
    if(!error.isEmpty())
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
//        float porc_rec1, porc_rec2, porc_rec3, porc_rec4;
//        double rec1, rec2, rec3, rec4, total_recargo, total_iva;
//        bool cobrado;
//        double importe_pendiente_cobro;
//        double importe_efectivo;
//        double importe_tarjeta;
//        double importe_cheque, importe_credito, importe_vale, importe_dto_redondeo, importe_cambio, entregado_a_cuenta;
//        bool pasado_a_albaran_factura;
//        int numero_albaran, numero_factura, id_cierre, ejercicio;
//        bool editable;
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Tickets"),tr("No se puede recuperar el registro: %1").arg(error),
                             tr("Aceptar"));
    }

}
