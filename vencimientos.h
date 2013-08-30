#ifndef VENCIMIENTOS_H
#define VENCIMIENTOS_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class vencimientos : public QObject
{
    Q_OBJECT
public:
    explicit vencimientos(QObject *parent = 0);

    // Fecha = fecha_calculo,
    //codigo_fp = codigo forma de pago,
    //id_documento = idfactura o id_tpv
    //tipo= 1 - factura 2 - tiquet
    // compra_venta = c - compras v - ventas.
    void calcular_vencimiento(QDate fecha, int id_cliente, int id_ticket, int id_factura, QString documento, int tipo, QString compra_venta, double importe);
private:
    TimedMessageBox *t;
signals:

public slots:

};

#endif // VENCIMIENTOS_H
