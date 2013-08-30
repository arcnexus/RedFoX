#ifndef ALBARAN_H
#define ALBARAN_H

#include "../Auxiliares/Globlal_Include.h"

class Albaran : public QObject
{
    Q_OBJECT
public:
    explicit Albaran(QObject *parent = 0);
    ~Albaran();

    int id;
    int albaran;
    QDate fecha;
    QString pedido_cliente;
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
    QString direccion1_entrega;
    QString direccion2_entrega;
    QString poblacion_entrega;
    QString provincia_entrega;
    QString cp_entrega;
    int id_pais_entrega;
    QString email_entrega;
    QString comentarios_entrega;
    QString cif;
    bool recargo_equivalencia;
    double subtotal;
    double dto;
    float porc_dto;
    float porc_dto_pp;
    double dto_pp;
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
    bool impreso;
    bool facturado;
    QString factura;
    QDate fecha_factura;
    QString comentario;
    double entregado_a_cuenta;
    QString desc_gasto1,desc_gasto2,desc_gasto3;
    double imp_gasto1,imp_gasto2,imp_gasto3;
    float porc_iva_gasto1,porc_iva_gasto2,porc_iva_gasto3;
    double iva_gasto1,iva_gasto2,iva_gasto3;
    bool editable;
    QString serie;
    int ejercicio;
    int id_transportista;
    int id_forma_pago;

signals:

public slots:
    // Metodos de trabajo Clase Albaran
    int AnadirAlbaran(QString serie);
    bool RecuperarAlbaran(QString cSQL);
    bool GuardarAlbaran(int nid_Albaran);
    int NuevoNumeroAlbaran(QString serie);
};

#endif // ALBARAN_H
