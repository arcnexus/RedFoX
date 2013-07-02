#ifndef FACTURASPROVEEDOR_H
#define FACTURASPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class FacturasProveedor : public QObject
{
    Q_OBJECT
public:
    explicit FacturasProveedor(QObject *parent = 0);

    int anadir_factura();
    void guardar_factura();
    void recuperar_factura(QString cSQL,int accion);
    void recuperar_factura(int id_factura);


    // ---------------------
    // Campos
    // ---------------------
    int id;
    QString factura;
    QDate fecha;
    QDate recepcion;
    QString pedido;
    int id_proveedor;
    QString proveedor;
    QString cif_proveedor;
    bool retencion_irpf;
    bool recargo_equivalencia;
    double base1;
    double base2;
    double base3;
    double base4;
    float porc_iva1;
    float porc_iva2;
    float porc_iva3;
    float porc_iva4;
    double iva1;
    double iva2;
    double iva3;
    double iva4;
    float porc_rec1;
    float porc_rec2;
    float porc_rec3;
    float porc_rec4;
    double rec1;
    double rec2;
    double rec3;
    double rec4;
    double total1;
    double total2;
    double total3;
    double total4;
    double total_base;
    double total_iva;
    double total_retencion;
    double total_recargo;
    double total;
    QString forma_pago;
    double importe_pagado_total;
    int id_tipo_gasto;
    QString comentario;
    bool pagado;
    double importe_deuda_pendiente;
    QString albaran;
    QString desc_gasto1;
    QString desc_gasto2;
    QString desc_gasto3;
    double imp_gasto1;
    double imp_gasto2;
    double imp_gasto3;
    bool gasto_to_coste;
 private:
    void cargar_factura(QSqlQuery queryFact, int accion);
    
signals:
    
public slots:
    
};

#endif // FACTURASPROVEEDOR_H
