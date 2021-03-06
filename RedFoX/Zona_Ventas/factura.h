#ifndef FACTURA_H
#define FACTURA_H

#include "Auxiliares/Globlal_Include.h"
#include"../Almacen/articulo.h"

class Factura : public QObject
{
    Q_OBJECT
public:
    explicit Factura(QObject *parent = 0);
    int id;
    QString codigo_cliente;
    QString serie;
    QString factura;
    QDate fecha;
    QDate fecha_cobro;
    int ejercicio;
    int id_cliente;
    int tarifa_cliente;
    QString cliente;
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
    QString cif;
    bool recargo_equivalencia;
    double subtotal;
    double dto;
    double dto_pp;
    float porc_dto;
    float porc_dto_pp;
    double base;
    double irpf;
    float porc_irpf;
    double iva;
    double total;
    bool impreso;
    bool cobrado;
    bool contablilizada;
    int id_forma_pago;
    QString forma_pago;
    QString comentario;
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
    double total1;
    double total2;
    double total3;
    double total4;
    float porc_rec1;
    float porc_rec2;
    float porc_rec3;
    float porc_rec4;
    double rec1;
    double rec2;
    double rec3;
    double rec4;
    double total_recargo;
    double entregado_a_cuenta;
    double importe_pendiente;
    QString codigo_entidad;
    QString oficina_entidad;
    QString dc_cuenta;
    QString cuenta_corriente;
    int pedido_cliente;
    int apunte;
    QString desc_gasto1;
    QString desc_gasto2;
    QString desc_gasto3;
    double imp_gasto1;
    double imp_gasto2;
    double imp_gasto3;
    float porc_iva_gasto1;
    float porc_iva_gasto2;
    float porc_iva_gasto3;
    double iva_gasto1;
    double iva_gasto2;
    double iva_gasto3;
    int id_transportista;
    bool editable;
    int id_divisa;
    bool set_impresa(bool state);
    void clear();
    bool borrar(int id);

signals:

public slots:
    bool AnadirFactura();
    bool RecuperarFactura(QString cSQL);
    bool RecuperarFactura(int id);
    void cargar(QSqlRecord *registro);
    bool GuardarFactura(int nid_factura, bool FacturaLegal);
    QString NuevoNumeroFactura(QString serie);
    bool BorrarLineasFactura(int id_lin);
    bool CobrarFactura();
    bool GuardarApunte(int nasiento, int nid);
    bool Apunte();
    bool EditApunte(int num_apunte);
private:
    Articulo *oArticulo;
};

#endif // FACTURA_H
