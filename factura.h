#ifndef FACTURA_H
#define FACTURA_H

#include "Auxiliares/Globlal_Include.h"

class Factura : public QObject
{
    Q_OBJECT
public:
    explicit Factura(QObject *parent = 0);
    int Id;
    QString cCodigoCliente;
    QString cFactura;
    QDate dFecha;
    QDate dFechaCobro;
    int iId_Cliente;
    QString cCliente;
    QString cDireccion;
    QString cDireccion2;
    QString cCp;
    QString cPoblacion;
    QString cProvincia;
    int idPais;
    QString cCif;
    int lRecargoEquivalencia;
    double rSubtotal;
    int nDto;
    int nDtoPP;
    double rImporteDescuento;
    double rImporteDescuentoPP;
    double rBase;
    int nIRPF;
    double rImporteIRPF;
    int nIva;
    double rImporteIva;
    double rTotal;
    int lImpresa;
    int lCobrada;
    int lContablilizada;
    int id_FormaPago;
    QString cFormaPago;
    QString tComentario;
    double rBase1;
    double rBase2;
    double rBase3;
    double rBase4;
    int nPorcentajeIVA1;
    int nPorcentajeIVA2;
    int nPorcentajeIVA3;
    int nPorcentajeIVA4;
    double rIVA1;
    double rIVA2;
    double rIVA3;
    double rIVA4;
    double rTotal1;
    double rTotal2;
    double rTotal3;
    double rTotal4;
    double nRec1;
    double nRec2;
    double nRec3;
    double nRec4;
    double rRecargoEq1;
    double rRecargoEq2;
    double rRecargoEq3;
    double rRecargoEq4;
    double rTotalRecargoEq;
    double rEntregadoaCuenta;
    double rImportePendiente;
    QString cCodigoEntidad;
    QString cOficinaEntidad;
    QString cDCCuenta;
    QString cNumeroCuenta;
    QString cPedidoCliente;

signals:

public slots:
    bool AnadirFactura();
    bool RecuperarFactura(QString cSQL);
    bool GuardarFactura(int nId_Factura, bool FacturaLegal);
    QString NuevoNumeroFactura();
    bool BorrarLineasFactura(int id_lin);
    bool CobrarFactura();
};

#endif // FACTURA_H
