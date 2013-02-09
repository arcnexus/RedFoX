#include "ticket.h"

Ticket::Ticket(QObject *parent) :
    QObject(parent)
{
    Id = 0;
    nCaja = 0;
    nTicket = 0;
    dFecha = QDate::currentDate();
    cHora = QDateTime::currentDateTime().toString("hh:mm:ss");
    idpersonalemp = 0;
    id_Cliente = 0;
    cCliente = "";
    cNombreCliente = "";
    lticketEnEpera = 0;
    lPendienteCobro = 0;
    idFormaPago = 0;
    cTipoTarjeta = "";
    lImpreso = 0;
    rImporte = 0;
    rBase1 = 0;
    rBase2 = 0;
    rBase3 = 0;
    rBase4 = 0;
    nPorcentajeIva1 = 0;
    nPorcentajeIva2 = 0;
    nPorcentajeIva3 = 0;
    nPorcentajeIva4 = 0;
    rImporteIva1 = 0;
    rImporteIva2 = 0;
    rImporteIva3 = 0;
    rImporteIva4 = 0;
    rTotal1 = 0;
    rTotal2 = 0;
    rTotal3 = 0;
    rTotal4 = 0;
    rSubotal = 0;
    rDto = 0;
    nPorcentajeRe1 = 0;
    nPorcentajeRe2 = 0;
    nPorcentajeRe3 = 0;
    nPorcentajeRe4 = 0;
    rImporteRecargoEq1 = 0;
    rImporteRecargoEq2 = 0;
    rImporteRecargoEq3 = 0;
    rImporteRecargoEq4 = 0;
    rTotalRecargoEq = 0;
    rImporteTotalIva = 0;
    rCobrado = 0;
    rPendienteCobro = 0;
    rImporteEfectivo = 0;
    rImporteTarjeta = 0;
    rImporteCheque = 0;
    rImporteCredito = 0;
    rImporteDtoRedondeo = 0;
    rImporteCambio = 0;
    rEntregado = 0;
    lPasadoAlbaranFactura = 0;
    nNumeroAlbaran = 0;
    nNumeroFactura = 0;
    id_cierre = 0;
    id_Dependient = 0;
}

void Ticket::set_table(QTableWidget *table)
{
    this->table = table;
}

void Ticket::add_linea(QString cCodigo, QString cDescripcion,  double rPvp, double nCantidad, double  rImporte, double  nPorcentajeIva, double  nPorcDto, double  nTotal, double  rSubTotal, QDate dFechaLinea)
{
    LineaTicket * newLinea = new LineaTicket(this);

    newLinea->Id = Id;
    newLinea->id_Cab = this->Id;

    QSqlQuery q(QSqlDatabase::database("empresa"));
    if(q.exec(QString("SELECT Id FROM articulos WHERE cCodigo = '%1'").arg(cCodigo)))
        if(q.next())
            newLinea->id_Articulo = q.record().value("Id").toInt();
    newLinea->cCodigo = cCodigo;
    newLinea->cDescripcion = cDescripcion;
    newLinea->rPvp = rPvp;
    newLinea->nCantidad =  nCantidad;
    newLinea->rImporte = rImporte;
    newLinea->nPorcentajeIva = nPorcentajeIva;
    newLinea->nPorcDto = nPorcDto;
    newLinea->nTotal = nTotal;
    newLinea->rSubTotal = rSubTotal;
    newLinea->dFechaLinea =  dFechaLinea;

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
    QTableWidgetItem * cod = new QTableWidgetItem(cCodigo);
    QTableWidgetItem * cant = new QTableWidgetItem(QString::number(nCantidad));
    QTableWidgetItem * pvp = new QTableWidgetItem(QString::number(rPvp));
    QTableWidgetItem * tot = new QTableWidgetItem(QString::number(nTotal));
    QTableWidgetItem * desc = new QTableWidgetItem(cDescripcion);

    table->setItem(row,0,cod);
    table->setItem(row,1,desc);
    table->setItem(row,2,cant);
    table->setItem(row,3,pvp);
    table->setItem(row,4,tot);
}

void Ticket::remove_linea(int row)
{
    lineas.remove(row);
    table->removeRow(row);
}

bool Ticket::save_Ticket(int ID)
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
