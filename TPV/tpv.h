#ifndef TPV_H
#define TPV_H

#include <QObject>
#include"../Auxiliares/Globlal_Include.h"

class tpv : public QObject
{
    Q_OBJECT
public:
    explicit tpv(QObject *parent = 0);
    int nuevoticket();
    int ticket;
    int id_usuario;

    int id;
    QString caja;
    QDate fecha;
    QString hora;
    int id_cliente;
    QString codigo;
    QString nombre_cliente;
    bool ticket_en_espera;
    bool pendiente_cobro;
    int id_forma_pago;
    QString tipo_targeta;
    bool impreso;
    double importe;
    double subtotal1, subtotal2,subtotal3,subtotal4;
    double dto1,dto2,dto3,dto4;
    double base1, base2, base3, base4;
    float porc_iva1, porc_iva2, porc_iva3, porc_iva4;
    double iva1, iva2, iva3, iva4;
    double total1, total2, total3, total4, subtotal, dto;
    float porc_rec1, porc_rec2, porc_rec3, porc_rec4;
    double rec1, rec2, rec3, rec4, total_recargo, total_iva;
    bool cobrado;
    double importe_pendiente_cobro;
    double importe_efectivo;
    double importe_tarjeta;
    double importe_cheque, importe_credito, importe_vale, importe_dto_redondeo, importe_cambio, entregado_a_cuenta;
    int numero_pedido, numero_albaran, numero_factura, id_cierre, ejercicio;
    bool editable;


    void recuperar(int id);


private:
    QString serie;

signals:

public slots:

};

#endif // TPV_H
