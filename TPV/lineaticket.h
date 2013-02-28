#ifndef LINEATICKET_H
#define LINEATICKET_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
class LineaTicket : public QObject
{
    Q_OBJECT
public:
    explicit LineaTicket(QObject *parent = 0);
    
    void getDatos_linea(QSqlRecord r);
    bool save_Linea(int ID_Cab);

    QList<QTableWidgetItem*> toTableRow();

    int 	Id;
    int 	id_Cab;
    int 	id_Articulo;
    QString cCodigo;
    QString cDescripcion;
    double 	rPvp;
    double 	nCantidad;
    double 	rImporte;//< esto que es en la bd
    double 	nPorcentajeIva;
    double 	nPorcDto;
    double 	nTotal;
    double 	rSubTotal;
    QDate 	dFechaLinea;
signals:
    
public slots:
    
};

#endif // LINEATICKET_H
