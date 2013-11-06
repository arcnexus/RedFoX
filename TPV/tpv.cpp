#include "tpv.h"

tpv::tpv(QObject *parent) :
    QObject(parent)
{
    ticket =0;
    id = 0;
    caja = "";
    fecha = QDate::currentDate();
    hora = "00:00";
    id_cliente = 0;
    codigo = "";
    nombre_cliente = "";
    ticket_en_espera = false;
    pendiente_cobro = true;
    id_forma_pago = -1;
    tipo_targeta = "";
    impreso = false;
    importe = 0;
    subtotal1 = 0;
    subtotal2 = 0;
    subtotal3 = 0;
    subtotal4 = 0;
    dto1 = 0;
    dto2 = 0;
    dto3 = 0;
    dto4 = 0;
    base1 = 0;
    base2 = 0;
    base3 = 0;
    base4 = 0;
    porc_iva1 = 0;
    porc_iva2 = 0;
    porc_iva3 = 0;
    porc_iva4 = 0;
    iva1 = 0;
    iva2 = 0;
    iva3 = 0;
    iva4 = 0;
    total1 = 0;
    total2 = 0;
    total3 = 0;
    total4 = 0;
    subtotal = 0;
    dto = 0;
    porc_rec1 = 0;
    porc_rec2 = 0;
    porc_rec3 = 0;
    porc_rec4 = 0;
    rec1 = 0;
    rec2 = 0;
    rec3 = 0;
    rec4 = 0;
    total_recargo = 0;
    total_iva = 0;
    cobrado = false;
    importe_pendiente_cobro = 0;
    importe_efectivo = 0;
    importe_tarjeta = 0;
    importe_cheque = 0;
    importe_credito = 0;
    importe_vale = 0;
    importe_dto_redondeo = 0;
    importe_cambio = 0;
    entregado_a_cuenta = 0;
    numero_pedido = 0;
    numero_albaran = 0;
    numero_factura = 0;
    id_cierre = 0;
    ejercicio = Configuracion_global->cEjercicio.toInt();
    editable = false;
}

int tpv::nuevoticket(QString serie, QString caja)
{
    //--------------------------------
    // comprovamos si hay caja abierta
    //--------------------------------
    QMap <int,QSqlRecord> c;
    QString error;
    int new_id;
    QStringList condiciones;
    condiciones << QString("fecha_abertura = '%1'").arg(QDate::currentDate().toString("yyyyMMdd"));
    condiciones << QString("id_caja = %1").arg(Configuracion_global->caja);
    condiciones << QString("ejercicio = '%1'").arg(Configuracion_global->cEjercicio);
    c = SqlCalls::SelectRecord("cierrecaja",condiciones,Configuracion_global->empresaDB,error);
    if(c.size()>0)
    {
        QSqlQuery *t = new QSqlQuery(Configuracion_global->empresaDB);
        QString cSQL;
        QString error;
        cSQL = QString("select ticket from cab_tpv where serie='%1' and ejercicio=%2 and caja = %3 order by ticket desc limit 1").arg(
                    serie,Configuracion_global->cEjercicio,caja);

        if(t->exec(cSQL)) {
            t->next();
            int num_last_ticket = t->record().value("ticket").toInt();
            num_last_ticket ++;
            QHash <QString, QVariant> h;

            this->ticket = num_last_ticket;
            this->fecha =  QDate::currentDate();
            this->hora = QDateTime::currentDateTime().toString("hh:mm");
            this->porc_iva1 = Configuracion_global->ivaList.at(0).toFloat();
            this->porc_iva2 = Configuracion_global->ivaList.at(1).toFloat();
            this->porc_iva3 = Configuracion_global->ivaList.at(2).toFloat();
            this->porc_iva4 = Configuracion_global->ivaList.at(3).toFloat();

            h["ticket"] = ticket;
            h["caja"] = Configuracion_global->caja;
            h["serie"] = Configuracion_global->serie;
            h["ejercicio"] = Configuracion_global->cEjercicio;
            h["fecha"] = this->fecha;
            h["hora"] = this->hora;
            h["id_usuario"] = this->id_usuario;
            h["porc_iva1"] = this->porc_iva1;
            h["porc_iva2"] = this->porc_iva2;
            h["porc_iva3"] = this->porc_iva3;
            h["porc_iva4"] = this->porc_iva4;


            new_id = SqlCalls::SqlInsert(h,"cab_tpv",Configuracion_global->empresaDB,error);
        }
        else{
            QMessageBox::warning(qApp->activeWindow(),tr("Gestión de tickets"),tr("Ocurrió un error al crear el nuevo ticket: %1").arg(
                                     t->lastError().text()),tr("Aceptar"));
            new_id = -1;
        }
        delete t;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de tpv"),tr("Debe abrir caja antes de poder facturar"),tr("Aceptar"));
        new_id = -1;
    }
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
        this->numero_pedido = m.value(id).value("numero_pedido").toInt();
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
