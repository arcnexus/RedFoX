#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include "../Auxiliares/Globlal_Include.h"

class Presupuesto
{
public:
    Presupuesto();
    ~Presupuesto();
    int id;
    int presupuesto;
    QDate fecha;
    QDate valido_hasta;
    int id_cliente;
    QString codigo_cliente;
    QString cliente;
    QString cif;
    QString direccion1;
    QString direccion2;
    QString cp;
    QString poblacion;
    QString provincia;
    int id_pais;
    QString telefono;
    QString movil;
    QString fax;
    double dto;
    QString comentarios;
    double subtotal;
//    double subtotal1;
//    double subtotal2;
//    double subtotal3;
//    double subtotal4;
    double base;
    float porc_dto;
    double total;
    bool impreso;
    bool aprobado;
    QDate fecha_aprobacion;
    double importe_factura;
    double importe_pendiente;
    QString factura;
    int albaran;
    int pedido;
    int id_forma_pago;
    QString codigoFormaPago;
    QString descripcionFormaPago;
    QString lugar_entrega;
    QString atencion_de;
    double base1;
    double base2;
    double base3;
    double base4;
    double iva1;
    double iva2;
    double iva3;
    double iva4;
    float porc_iva1;
    float porc_iva2;
    float porc_iva3;
    float porc_iva4;
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
    bool recargo_equivalencia;
    QString email;
    double total_iva;
    double total_recargo;

public slots:
    // Metodos de trabajo Clase Presupuesto
    bool AnadirPresupuesto();
    bool RecuperarPresupuesto(QString cSQL);
    bool siguiente();
    bool anterior();
    bool GuardarPres(int nid_Presupuesto);
    bool BorrarLineas(int nid_Presupuesto);
    int NuevoNumeroPresupuesto();
    void PresupuestoToPedido();
    void PresupuestoToAlbaran();
    void PresupuestoToFactura();
};

#endif // PRESUPUESTO_H
