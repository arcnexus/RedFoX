#ifndef PEDidOS_H
#define PEDidOS_H
#include "../Auxiliares/Globlal_Include.h"

class Pedidos
{
public:
    Pedidos();
    ~Pedidos();

    int id;
    int albaran;
    int pedido;
    QDate fecha;
    int id_cliente;
    QString codigo_cliente;
    QString cliente;
    QString direccion1;
    QString direccion2;
    QString poblacion;
    QString provincia;
    QString cp;
    int id_pais;
    QString pais;
    QString direccion_entrega1;
    QString direccion_entrega2;
    QString cp_entrega;
    QString poblacion_entrega;
    QString provincia_entrega;
    int id_pais_entrega;
    QString pais_entrega;
    QString cif;
    int recargo_equivalencia;
    double subtotal;
    double dto;
    double dto_pp;
    float porc_dto;
    float porc_dto_pp;
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
    double base_total;
    double iva_total;
    double rec_total;
    double total_albaran;
    int impreso;
    int facturado;
    QString factura;
    QDate fecha_factura;
    QString comentario;
    double entregado_a_cuenta;
    int traspasado_albaran;
    int traspasado_factura;
    int enviado;
    int completo;
    int entregado;
    QDate fecha_limite_entrega;
    double total_pedido;
    int tarifa_cliente;
    QString gasto1,gasto2,gasto3;
    double imp_gasto1, imp_gasto2, imp_gasto3;
    float porc_iva_gasto1,porc_iva_gasto2,porc_iva_gasto3;
    double iva_gasto1,iva_gasto2,iva_gasto3;

public slots:
    bool BorrarLineas(int Iped);
    // Metodos de trabajo Clase Pedido
    bool AnadirPedido();
    bool RecuperarPedido(QString cSQL);
    bool GuardarPedido(int nid_Pedido);
    int NuevoNumeroPedido();

    void calcularPedido();
    void FacturarPedido();
};

#endif // PEDidOS_H
