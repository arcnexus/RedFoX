#ifndef VENCIMIENTOS_H
#define VENCIMIENTOS_H

#include <QObject>
#include "Auxiliares/Globlal_Include.h"

class vencimientos : public QObject
{
    Q_OBJECT
public:

    int id;
    QString codigo,codigo_cuenta_contable;
    QString forma_pago;
    int dia_pago1, dia_pago2, dia_pago3, dia_pago4, dias_entre_plazos;
    QString desc_cuenta_cont;
    int numero_plazos;
    int dias_hasta_pago;
    bool al_contado;


    explicit vencimientos(QObject *parent = 0);

    void recuperar(QStringList condiciones);
    void anadir();
    void guardar();
    void borrar();
    void clear();


    // Fecha = fecha_calculo,
    //codigo_fp = codigo forma de pago,
    //id_documento = idfactura o id_tpv
    //tipo= 1 - factura 2 - tiquet
    // compra_venta = c - compras v - ventas.
    bool calcular_vencimiento(QDate fecha, int id_forma_pago, int id_cliente, int id_ticket, int id_factura, QString documento, int tipo, bool is_venta, double importe);
private:

signals:

public slots:

};

#endif // VENCIMIENTOS_H
