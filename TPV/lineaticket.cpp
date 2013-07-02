#include "lineaticket.h"

LineaTicket::LineaTicket(QObject *parent) :
    QObject(parent)
{
}

void LineaTicket::getDatos_linea(QSqlRecord r)
{
}

bool LineaTicket::save_Linea(int id_Cab)
{
    return true;
}

QList<QTableWidgetItem *> LineaTicket::toTableRow()
{
    QList<QTableWidgetItem *> lista;
    return lista;
}
