#ifndef FACTURASPROVEEDOR_H
#define FACTURASPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class FacturasProveedor : public QObject
{
    Q_OBJECT
public:
    explicit FacturasProveedor(QObject *parent = 0);

    int anadir_factura();
    void guardar_factura();
    void recuperar_factura(QString cSQL,int accion);
    void recuperar_factura(int id_factura);


    // ---------------------
    // Campos
    // ---------------------
    int id;
    QString cFactura;
    QDate dFecha;
    QDate dRecepcion;
    QString cPedido;
    int Id_Proveedor;
    QString cProveedor;
    QString cCifProveedor;
    bool lRetencionIRPF;
    bool lRecargoEquivalencia;
    double rBase1;
    double rBase2;
    double rBase3;
    double rBase4;
    double nPorcIva1;
    double nPorcIva2;
    double nPorcIva3;
    double nPorcIva4;
    double rIVA1;
    double rIVA2;
    double rIVA3;
    double rIVA4;
    double nREC1;
    double nREC2;
    double nREC3;
    double nREC4;
    double rREC1;
    double rREC2;
    double rREC3;
    double rREC4;
    double rTotal1;
    double rTotal2;
    double rTotal3;
    double rTotal4;
    double rTotalBase;
    double rTotalIVA;
    double rTotalRetencion;
    double rTotalRecargoEq;
    double rTotal;
    QString cFormaPago;
    double rImportePagadoTotal;
    int Id_TipoGasto;
    QString tComentario;
    bool lPagado;
    double rImporteDeudaPendiente;
    QString cCAlbaran;
    QString desc_gasto1;
    QString desc_gasto2;
    QString desc_gasto3;
    double imp_gasto1;
    double imp_gasto2;
    double imp_gasto3;
    bool gasto_to_coste;
 private:
    void cargar_factura(QSqlQuery queryFact, int accion);
    
signals:
    
public slots:
    
};

#endif // FACTURASPROVEEDOR_H
