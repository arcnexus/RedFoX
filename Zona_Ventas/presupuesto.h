#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include "../Auxiliares/Globlal_Include.h"

class Presupuesto
{
public:
    Presupuesto();
    ~Presupuesto();
    int id;
    int nPresupuesto;
    QDate fecha;
    QDate dValidoHasta;
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
    double importe;
    double importe1;
    double importe2;
    double importe3;
    double importe4;
    double base;
    double rDescuento;
    double total;
    bool impreso;
    bool lAprobado;
    QDate fechaAprobacion;
    double importeFactura;
    double importe_pendiente;
    QString factura;
    int albaran;
    int pedido;
    int id_forma_pago;
    QString codigoFormaPago;
    QString descripcionFormaPago;
    QString tLugarEntrega;
    QString cAtencionde;
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
    float porc_porc_recuivalencia1;
    float porc_porc_recuivalencia2;
    float porc_porc_recuivalencia3;
    float porc_porc_recuivalencia4;
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
    double totalRec;

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
