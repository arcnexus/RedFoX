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
    bool save_Linea(int id_Cab);

    QList<QTableWidgetItem*> toTableRow();

    int 	id;
    int 	id_Cab;
    int 	id_articulo;
    QString codigo;
    QString descripcion;
    double 	pvp;
    double 	cantidad;
    double 	importe;//< esto que es en la bd
    double 	porc_iva;
    double 	porc_dto;
    double 	total;
    double 	subtotal;
    QDate 	fecha_linea;
signals:
    
public slots:
    
};

#endif // LINEATICKET_H
