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
    int ejercicio;
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
    QString direccion1_entrega;
    QString direccion2_entrega;
    QString cp_entrega;
    QString poblacion_entrega;
    QString provincia_entrega;
    int id_pais_entrega;
    QString email_entrega;
    QString comentarios_entrega;
    QString telefono;
    QString movil;
    QString fax;
    double dto;
    double dto_pp;
    float porc_dto;
    float porc_dto_pp;
    QString comentarios;
    double subtotal;
//    double subtotal1;
//    double subtotal2;
//    double subtotal3;
//    double subtotal4;
    double base;
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
    QString desc_gasto1;
    QString desc_gasto2;
    QString desc_gasto3;
    double importe_gasto1;
    double importe_gasto2;
    double importe_gasto3;
    double porc_iva_gasto1;
    double porc_iva_gasto2;
    double porc_iva_gasto3;
    double iva_gasto1;
    double iva_gasto2;
    double iva_gasto3;
    int id_transportista;

public slots:
    // Metodos de trabajo Clase Presupuesto
    bool AnadirPresupuesto();
    bool RecuperarPresupuesto(QString cSQL);
    bool siguiente();
    bool anterior();
    bool GuardarPres(int nid_Presupuesto);
    bool BorrarLineas(int nid_Presupuesto);
    int NuevoNumeroPresupuesto();
};

#endif // PRESUPUESTO_H
