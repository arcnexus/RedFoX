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
    QString cPaisAlmacen;
    QString cTelefonoAlmacen;
    QString cFaxAlmacen;
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
    // GETS
    int getid();
    QString getcCodigo();
    QString getcProveedor();
    QString getcCif();
    QString getcDireccion1();
    QString getcDireccion2();
    QString getcCP();
    QString getcPoblacion();
    QString getcProvincia();
    QString getcPais();
    QString getcTelefono1();
    QString getcTelefono2();
    QString getcTelefono3();
    QString getcFax();
    QString getcMovil();
    QString getcEMail();
    QString getcWeb();
    QString getcPersonaContacto();
    int getnDiaCobro();
    QString getcDireccionAlmacen();
    QString getcCPAlmacen();
    QString getcPoblacionAlmacen();
    QString getcProvinciaAlmacen();
    QString getcPaisAlmacen();
    QString getcTelefonoAlmacen();
    QString getcFaxAlmacen();
    QString getcCodigoFormaPago();
    QDate getdFechaUltimaCompra();
    double getrAcumuladoCompras();
    QString getcEntidadBancariaProveedor();
    QString getcOficinaBancariaProveedor();
    QString getcDCProveedor();
    QString getcCCProveedor();
    QString getcEntidadPagoProveedor();
    QString getcOficinaPagoProveedor();
    QString getcDCPagoProveedor();
    QString getcCuentaPagoProveedor();
    QString getcCuentaIvaSoportado();
    double getrRetencionIRPF();
    int getnTipoRetencion();
    QString getcCuentaAplicacion();
    QString getcCuentaPagos();
    QString gettComentarios();
    double getnDto();
    QDate getdFechaAlta();
    double getrDeudaMaxima();
    double getrDeudaActual();
    int getlRecargoEquivalencia();
    QString gettTextoparaPedidos();
    double getrEntregadoaCuenta();
    
    // SETTERS

    void setId(int id);
    void setcCodigo(QString cCodigo);
    void setcProveedor(QString cProveedor);
    void setcCif(QString cCif);
    void setcDireccion1(QString cDireccion1);
    void setcDireccion2(QString cDireccion2);
    void setcCP(QString cCP);
    void setcPoblacion(QString cPoblacion);
    void setcProvincia(QString cProvincia);
    void setcPais(QString cPais);
    void setcTelefono1(QString cTelefono1);
    void setcTelefono2(QString cTelefono2);
    void setcTelefono3(QString cTelefono3);
    void setcFax(QString cFax);
    void setcMovil(QString cMovil);
    void setcEMail(QString cEMail);
    void setcWeb(QString cWeb);
    void setcPersonaContacto(QString cPersonaContacto);
    void setnDiaCobro(int nDiaCobro);
    void setcDireccionAlmacen(QString cDireccionAlmacen);
    void setcCPAlmacen(QString cCPAlmacen);
    void setcPoblacionAlmacen(QString cPoblacionAlmacen);
    void setcProvinciaAlmacen(QString cProvinciaAlmacen);
    void setcPaisAlmacen(QString cPaisAlmacen);
    void setcTelefonoAlmacen(QString cTelefonoAlmacen);
    void setcFaxAlmacen(QString cFaxAlmacen);
    void setcCodigoFormaPago(QString cCodigoFormaPago);
    void setdFechaUltimaCompra(QDate dFechaUltimaCompra);
    void setrAcumuladoCompras(double rAcumuladoCompras);
    void setcEntidadBancariaProveedor(QString cEntidadBancariaProveedor);
    void setcOficinaBancariaProveedor(QString cOficinaBancariaProveedor);
    void setcDCProveedor(QString cDCProveedor);
    void setcCCProveedor(QString cCCProveedor);
    void setcEntidadPagoPro(QString cEntidadPagoProveedor);
    void setcOficinaPagoProveedor(QString cOficinaPagoProveedor);
    void setcDCPagoProveedor(QString cDCPagoProveedor);
    void setcCuentaPagoProveedor(QString cCuentaPagoProveedor);
    void setcCuentaIvaSoportado(QString cCuentaIvaSoportado);
    void setrRetencion(double rRetencionIRPF);
    void setnTipoRetencion(int nTipoRetencion);
    void setcCuentaAplicacion(QString cCuentaAplicacion);
    void setcCuentaPagos(QString cCuentaPagos);
    void settComentarios(QString tComentarios);
    void setnDto(double nDto);
    void setdFechaAlta(QDate dFechaAlta);
    void setrDeudaMaxima(double rDeudaMaxima);
    void setrDeudaActual(double rDeudaActual);
    void setlRecargoEquivalencia(int lRecargoEquivalencia);
    void settTextoparaPedidos(QString tTextoparaPedidos);
    void setrEntregadoaCuenta(double rEntregadoaCuenta);


};

#endif // PROVEEDOR_H
