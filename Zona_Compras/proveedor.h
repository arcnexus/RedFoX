#ifndef PROVEEDOR_H
#define PROVEEDOR_H

#include "Auxiliares/Globlal_Include.h"

class Proveedor : public QObject
{
    Q_OBJECT
public:
    explicit Proveedor(QObject *parent = 0);

    int id;
    QString cCodigo;
    QString cProveedor;
    QString cCif;
    QString cDireccion1;
    QString cDireccion2;
    QString cCP;
    QString cPoblacion;
    QString cProvincia;
    int idpais;
    QString cPais;
    QString cTelefono1;
    QString cTelefono2;
    QString cTelefono3;
    QString cFax;
    QString cMovil;
    QString cEMail;
    QString cWeb;
    QString cPersonaContacto;
    int nDiaCobro;
    QString cDireccionAlmacen;
    QString cCPAlmacen;
    QString cPoblacionAlmacen;
    QString cProvinciaAlmacen;
    int idPaisAlmacen;
    QString cPaisAlmacen;
    QString cTelefonoAlmacen;
    QString cFaxAlmacen;
    int idFormadePago;
    QString cCodigoFormaPago;
    QDate dFechaUltimaCompra;
    double rAcumuladoCompras;
    QString cEntidadBancariaProveedor;
    QString cOficinaBancariaProveedor;
    QString cDCProveedor;
    QString cCCProveedor;
    QString cEntidadPagoProveedor;
    QString cOficinaPagoProveedor;
    QString cDCPagoProveedor;
    QString cCuentaPagoProveedor;
    QString cCuentaIvaSoportado;
    double rRetencionIRPF;
    int nTipoRetencion;
    QString cCuentaAplicacion;
    QString cCuentaPagos;
    QString tComentarios;
    double nDto;
    QDate dFechaAlta;
    double rDeudaMaxima;
    double rDeudaActual;
    int lRecargoEquivalencia;
    QString tTextoparaPedidos;
    double rEntregadoaCuenta;

    // Acumulados
    double enero,febrero,marzo,abril,mayo,junio,julio,agosto,septiembre,octubre,noviembre,diciembre;
    void cargaracumulados(int id_proveedor);
    bool acumular(int id_proveedor, int mes, double importe);
    void anadir_persona_contacto(int Id, QString Nombre, QString descTelefono1, QString Telefono1,
                                 QString descTelefono2, QString Telefono2, QString descTelefono3, QString Telefono3,
                                 QString descMovil1, QString Movil1, QString descMovil2, QString Movil2,QString cargo);

signals:
    
public slots:
    // Funcionamiento Base
    void Anadir();
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Guardar();
    void Vaciar();
    void Borrar(int nId);
    QString NuevoCodigoProveedor();


};

#endif // PROVEEDOR_H
