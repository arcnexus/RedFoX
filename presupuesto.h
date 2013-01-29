#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include "Auxiliares/Globlal_Include.h"

class Presupuesto
{
public:
    Presupuesto();
    ~Presupuesto();
    int id;
    int nPresupuesto;
    QDate dFecha;
    QDate dValidoHasta;
    int id_cliente;
    QString cCodigoCliente;
    QString cCliente;
    QString cCif;
    QString cDireccion;
    QString cDireccion2;
    QString cCP;
    QString cPoblacion;
    QString cProvincia;
    int idPais;
    QString cTelefono;
    QString cMovil;
    QString cFax;
    double nDto;
    QString tComentarios;
     double rImporte;
    double rImporte1;
    double rImporte2;
    double rImporte3;
    double rImporte4;
    double rSubTotal;
    double rDescuento;
    double rTotal;
    bool lImpreso;
    bool lAprobado;
    QDate dFechaAprobacion;
    double rImporteFactura;
    double rImportePendiente;
    QString cFactura;
    int nAlbaran;
    int nPedido;
    int id_FormaPago;
    QString cCodigoFormaPago;
    QString cDescripcionFormaPago;
    QString tLugarEntrega;
    QString cAtencionde;
    double rBase1;
    double rBase2;
    double rBase3;
    double rBase4;
    double nIva1;
    double nIva2;
    double nIva3;
    double nIva4;
    double rIva1;
    double rIva2;
    double rIva3;
    double rIva4;
    double nRecargoEquivalencia1;
    double nRecargoEquivalencia2;
    double nRecargoEquivalencia3;
    double nRecargoEquivalencia4;
    double rRec1;
    double rRec2;
    double rRec3;
    double rRec4;
    double rTotal1;
    double rTotal2;
    double rTotal3;
    double rTotal4;
    bool lRecargoEquivalencia;
    QString cEmail;
    double rTotalIva;
    double rTotalRec;

public slots:
    // Metodos de trabajo Clase Presupuesto
    void AnadirPresupuesto();
    void RecuperarPresupuesto(QString cSQL);
    void RecuperarPresupuesto(QString cSQL, int nAccion);
    void GuardarPres(int nId_Presupuesto);
    int NuevoNumeroPresupuesto();
    void AnadirLineaPresupuesto(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                                double porcdto, double dto, double total, double nPorcIva);
    void ModificarLineaPresupuesto(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                                   double porcdto, double dto, double total, double nPorcIva);
    void BorrarLineaPresupuesto(int id_lin);
    void calcularPresupuesto();
    void PresupuestoToPedido();
    void PresupuestoToAlbaran();
    void PresupuestoToFactura();
};

#endif // PRESUPUESTO_H
