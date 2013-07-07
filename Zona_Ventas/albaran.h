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
    QString cif;
    int recargo_equivalencia;
    double subtotal;
    double dto;
    float porc_dto;
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
signals:

public slots:
    // Metodos de trabajo Clase Albaran
    bool AnadirAlbaran();
    bool RecuperarAlbaran(QString cSQL);
    bool GuardarAlbaran(int nid_Albaran);
    int NuevoNumeroAlbaran();

    void FacturarAlbaran();
};

#endif // ALBARAN_H
