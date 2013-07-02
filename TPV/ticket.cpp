#include "ticket.h"

Ticket::Ticket(QObject *parent) :
    QObject(parent)
{
    id = 0;
    caja = 0;
    ticket = 0;
    fecha = QDate::currentDate();
    hora = QDateTime::currentDateTime().toString("hh:mm:ss");
    id_usuario = 0;
    id_cliente = 0;
    cliente = "";
    nombre_cliente = "";
    ticket_en_espera = 0;
    pendiente_cobro = 0;
    id_forma_pago = 0;
    tipo_tarjeta = "";
    impreso = 0;
    importe = 0;
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
    iva = 0;
    cobrado = 0;
    pendiente_cobro = 0;
    importe_efectivo = 0;
    importe_tarjeta = 0;
    importe_cheque = 0;
    importe_credito = 0;
    importe_dto_redondeo = 0;
    importe_cambio = 0;
    entregado_a_cuenta = 0;
    pasado_a_albaran_factura = 0;
    numero_albaran = 0;
    numero_factura = 0;
    id_cierre = 0;
    id_usuario = 0;
}

void Ticket::set_table(QTableWidget *table)
{
    this->table = table;
}

void Ticket::add_linea(QString codigo, QString descripcion,  double pvp, double cantidad, double  importe, double  porc_iva, double  porc_dto,
                       double  total, double  subtotal, QDate fecha_linea)
{
    LineaTicket * newLinea = new LineaTicket(this);

    newLinea->id = id;
    newLinea->id_Cab = this->id;

    QSqlQuery q(QSqlDatabase::database("empresa"));
    if(q.exec(QString("SELECT id FROM articulos WHERE codigo = '%1'").arg(codigo)))
        if(q.next())
            newLinea->id_articulo = q.record().value("id").toInt();
    newLinea->codigo = codigo;
    newLinea->descripcion = descripcion;
    newLinea->pvp = pvp;
    newLinea->cantidad =  cantidad;
    newLinea->importe = importe;
    newLinea->porc_iva = porc_iva;
    newLinea->porc_dto = porc_dto;
    newLinea->total = total;
    newLinea->subtotal = subtotal;
    newLinea->fecha_linea =  fecha_linea;

    lineas.append(newLinea);
    int width = table->width();
    int c_width = width / 6;

    table->horizontalHeader()->setUpdatesEnabled(false);
    for (int i =0 ; i< table->columnCount();i++)
        table->setColumnWidth(i,c_width);
    table->setColumnWidth(1,c_width*2);
    table->horizontalHeader()->setUpdatesEnabled(true);

    table->setRowCount(table->rowCount()+1);
    int row = table->rowCount()-1;
    QTableWidgetItem * cod = new QTableWidgetItem(codigo);
    QTableWidgetItem * cant = new QTableWidgetItem(QString::number(cantidad));
    QTableWidgetItem * pvp1 = new QTableWidgetItem(QString::number(pvp));
    QTableWidgetItem * tot = new QTableWidgetItem(QString::number(total));
    QTableWidgetItem * desc = new QTableWidgetItem(descripcion);

    table->setItem(row,0,cod);
    table->setItem(row,1,desc);
    table->setItem(row,2,cant);
    table->setItem(row,3,pvp1);
    table->setItem(row,4,tot);
}

void Ticket::remove_linea(int row)
{
    lineas.remove(row);
    table->removeRow(row);
}

bool Ticket::save_Ticket(int id)
{
    return true;
}

bool Ticket::next()
{
    return true;
}

bool Ticket::prev()
{
    return true;
}
