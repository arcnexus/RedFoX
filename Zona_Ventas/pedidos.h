#ifndef PEDIDOS_H
#define PEDIDOS_H
#include "Auxiliares/Globlal_Include.h"

class Pedidos
{
public:
    Pedidos();
    ~Pedidos();

    int id;
    int nAlbaran;
    int nPedido;
    QDate dFecha;
    QString cPedido;
    int iId_Cliente;
    QString cCodigoCliente;
    QString cCliente;
    QString cDireccion;
    QString cDireccion2;
    QString cPoblacion;
    QString cProvincia;
    QString cCp;
    int id_pais;
    QString cPais;
    QString cCif;
    int lRecargoEquivalencia;
    double rSubtotal;
    double rDto;
    double nDto;
    double rBase1;
    double rBase2;
    double rBase3;
    double rBase4;
    int nPorcentajeIVA1;
    int nPorcentajeIVA2;
    int nPorcentajeIVA3;
    int nPorcentajeIVA4;
    double rImporteIva1;
    double rImporteIva2;
    double rImporteIva3;
    double rImporteIva4;
    double nPorcentajeRecargoEq1;
    double nPorcentajeRecargoEq2;
    double nPorcentajeRecargoEq3;
    double nPorcentajeRecargoEq4;
    double rImporteRecargoEq1;
    double rImporteRecargoEq2;
    double rImporteRecargoEq3;
    double rImporteRecargoEq4;
    double rTotal1;
    double rTotal2;
    double rTotal3;
    double rTotal4;
    double rBaseTotal;
    double rIvaTotal;
    double rRecargoEqTotal;
    double rTotalAlbaran;
    int lImpreso;
    int lFacturado;
    QString cFactura;
    QDate dFechaFactura;
    QString tComentario;
    double rACuenta;
    int lTraspasadoAlb;
    int lTraspasadoFac;
    QString cDireccionEntrega;
    QString cDireccionEntrega2;
    QString cCPEntrega;
    QString cPoblacionEntrega;
    QString cProvinciaEntrega;
    QString cPaisEntrega;
    int lEnviado;
    int lCompleto;
    int lEntregado;
    QDate dFechaLimiteEntrega;
    double rTotalPedido;

public slots:
    bool BorrarLineas(int Iped);
    // Metodos de trabajo Clase Pedido
    bool AnadirPedido();
    bool RecuperarPedido(QString cSQL);
    bool GuardarPedido(int nId_Pedido);
    int NuevoNumeroPedido();

    void calcularPedido();
    void FacturarPedido();
};

#endif // PEDIDOS_H
