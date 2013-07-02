#ifndef TICKET_H
#define TICKET_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
#include "lineaticket.h"
class Ticket : public QObject
{
    Q_OBJECT
public:
    explicit Ticket(QObject *parent = 0);

    void set_table(QTableWidget * table);
    void add_linea(
            QString codigo,
            QString descripcion,
            double 	pvp,
            double 	cantidad,
            double 	importe,
            double 	porc_iva,
            double 	porc_dto,
            double 	total,
            double 	subtotal,
            QDate 	fecha_linea);

    void remove_linea(int row);

    bool save_Ticket(int id);
    
    int 	id;
    int 	caja;
    int 	ticket;
    QDate 	fecha;
    QString 	hora;
    int 	id_usuario;
    int 	id_cliente;
    QString 	cliente;
    QString 	nombre_cliente;
    int 	ticket_en_espera;
    int 	pendiente_cobro;
    int 	id_forma_pago;
    QString 	tipo_tarjeta;
    int 	impreso;
    double 	importe;
    double 	base1;
    double 	base2;
    double 	base3;
    double 	base4;
    double 	porc_iva1;
    double 	porc_iva2;
    double 	porc_iva3;
    double 	porc_iva4;
    double 	iva1;
    double 	iva2;
    double 	iva3;
    double 	iva4;
    double 	total1;
    double 	total2;
    double 	total3;
    double 	total4;
    double 	subtotal;
    double 	dto;
    double 	porc_rec1;
    double 	porc_rec2;
    double 	porc_rec3;
    double 	porc_rec4;
    double 	rec1;
    double 	rec2;
    double 	rec3;
    double 	rec4;
    double 	total_recargo;
    double 	iva;
    double 	cobrado;
    //double 	pendiente_cobro;
    double 	importe_efectivo;
    double 	importe_tarjeta;
    double 	importe_cheque;
    double 	importe_credito;
    double 	importe_dto_redondeo;
    double 	importe_cambio;
    double 	entregado_a_cuenta;
    int 	pasado_a_albaran_factura;
    int 	numero_albaran;
    int 	numero_factura;
    int 	id_cierre;

signals:
    
public slots:
    bool next();
    bool prev();
private:
    QVector<LineaTicket*> lineas;
    QTableWidget * table;
};

#endif // TICKET_H
