#include "proveedor.h"

Proveedor::Proveedor(QObject *parent) :
    QObject(parent)
{
}

int Proveedor::getid()
{
    return this->id;
}

QString Proveedor::getcCodigo()
{
    return this->cCodigo;
}

QString Proveedor::getcProveedor()
{
    return this->cProveedor;
}

QString Proveedor::getcCif()
{
    return this->cCif;
}

QString Proveedor::getcDireccion1()
{
    return cDireccion1;
}

QString Proveedor::getcDireccion2()
{
    return cDireccion2;
}

QString Proveedor::getcCP()
{
    return this->cCP;
}

QString Proveedor::getcPoblacion()
{
    return this->cPoblacion;
}

QString Proveedor::getcProvincia()
{
    return this->cProvincia;
}

QString Proveedor::getcPais()
{
    return this->cPais;
}

QString Proveedor::getcTelefono1()
{
    return this->cTelefono1;
}

QString Proveedor::getcTelefono2()
{
    return this->cTelefono2;
}

QString Proveedor::getcTelefono3()
{
    return this->cTelefono3;
}

QString Proveedor::getcFax()
{
    return this->cFax;
}

QString Proveedor::getcMovil()
{
    return this->cMovil;
}

QString Proveedor::getcEMail()
{
    return this->cEMail;
}

QString Proveedor::getcWeb()
{
    return this->cWeb;
}

QString Proveedor::getcPersonaContacto()
{
    return this->cPersonaContacto;
}

int Proveedor::getnDiaCobro()
{
    return this->nDiaCobro;
}

QString Proveedor::getcDireccionAlmacen()
{
    return this->cDireccionAlmacen;
}

QString Proveedor::getcCPAlmacen()
{
    return this->cCPAlmacen;
}

QString Proveedor::getcPoblacionAlmacen()
{
    return this->cPoblacionAlmacen;

}

QString Proveedor::getcProvinciaAlmacen()
{
    return this->cProvinciaAlmacen;
}

QString Proveedor::getcPaisAlmacen()
{
    return this->cPaisAlmacen;
}

QString Proveedor::getcTelefonoAlmacen()
{
    return this->cTelefonoAlmacen;
}

QString Proveedor::getcFaxAlmacen()
{
    return this->cFaxAlmacen;
}

QString Proveedor::getcCodigoFormaPago()
{
    return this->cCodigoFormaPago;
}

QDate Proveedor::getdFechaUltimaCompra()
{
    return this->dFechaUltimaCompra;
}

double Proveedor::getrAcumuladoCompras()
{
    return this->rAcumuladoCompras;
}

QString Proveedor::getcEntidadBancariaProveedor()
{
    return this->cEntidadBancariaProveedor;
}

QString Proveedor::getcOficinaBancariaProveedor()
{
    return this->cOficinaBancariaProveedor;
}

QString Proveedor::getcDCProveedor()
{
    return this->cDCProveedor;
}

QString Proveedor::getcCCProveedor()
{
    return this->cCCProveedor;
}

QString Proveedor::getcEntidadPagoProveedor()
{
    return this->cEntidadPagoProveedor;
}

QString Proveedor::getcOficinaPagoProveedor()
{
    return this->cOficinaPagoProveedor;
}

QString Proveedor::getcDCPagoProveedor()
{
    return this->cDCPagoProveedor;
}

QString Proveedor::getcCuentaPagoProveedor()
{
    return this->cCuentaPagoProveedor;
}

QString Proveedor::getcCuentaIvaSoportado()
{
    return this->cCuentaIvaSoportado;
}

double Proveedor::getrRetencionIRPF()
{
    return this->rRetencionIRPF;
}

QString Proveedor::getnTipoRetencion()
{
    return this->nTipoRetencion;
}

QString Proveedor::getcCuentaAplicacion()
{
    return this->cCuentaAplicacion;
}

QString Proveedor::getcCuentaPagos()
{
    return this->cCuentaPagos;

}

QString Proveedor::gettComentarios()
{
    return this->tComentarios;
}

double Proveedor::getnDto()
{
    return this->nDto;
}

QDate Proveedor::getdFechaAlta()
{
    return this->dFechaAlta;
}

double Proveedor::getrDeudaMaxima()
{
    return this->rDeudaMaxima;
}

double Proveedor::getrDeudaActual()
{
    return this->rDeudaActual;
}

int Proveedor::getlRecargoEquivalencia()
{
    return this->lRecargoEquivalencia;
}

QString Proveedor::gettTextoparaPedidos()
{
    return this->tTextoparaPedidos;
}


/***************************************************
// SETTERS
***************************************************/
void Proveedor::setId(int id)
{
    this->id = id;
}


