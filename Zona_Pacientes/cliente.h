#ifndef CLIENTE_H
#define CLIENTE_H

#include <QObject>
#include <QDate>
#include <QtSql>
#include <QSqlQueryModel>

class Cliente : public QObject
{
    Q_OBJECT

public:
    explicit Cliente(QObject *parent = 0);
        QSqlQueryModel *model;
private:
    int id;
    QString cCodigoCliente;
    QString cApellido1;
    QString cApellido2;
    QString cNombre;
    QString cNombreFiscal;
    QString cNombreComercial;
    QString cPersonaContacto;
    QString cCifNif;
    QString cDireccion1;
    QString cDireccion2;
    QString cCp;
    QString cPoblacion;
    QString cProvincia;
    int idPais;
    QString cPais;
    QString cTelefono1;
    QString cTelefono2;
    QString cFax;
    QString cMovil;
    QString cEmail;
    QString cWeb;
    QString cDireccionFactura1;
    QString cDireccionFactura2;
    QString cCPFactura;
    QString cPoblacionFactura;
    QString cProvinciaFactura;
    int idPaisFactura;
    QString cPaisFactura;
    QString cDireccionAlmacen;
    QString cDireccionAlmacen2;
    QString cCPAlmacen;
    QString cPoblacionAlmacen;
    QString cProvinciaAlmacen;
    int idPaisAlmacen;
    QString cPaisAlmacen;
    QDate dFechaalta;
    QDate dFechaCompra;
    double rAcumuladoVentas;
    double rVentasEjercicio;
    double rRiesgoMaximo;
    double rDeudaActual;
    QString tComentarios;
    int lBloqueado;
    QString tComentarioBloqueo;
    double nPorcDtoCliente;
    int lRecargoEquivalencia;
    QString cCuentaContable;
    QString cCuentaIvaRepercutido;
    QString cCuentaDeudas;
    QString cCuentaCobros;
    QString cFormaPago;
    int nDiaPago1;
    int nDiaPago2;
    int nTarifaCliente;
    double rImporteACuenta;
    double rVales;
    QString cEntidadBancaria;
    QString cOficinaBancaria;
    QString cDc;
    QString cCuentaCorriente;
    QDate dFechaNacimiento;
    double rImportePendiente;
    QString cAccesoWeb;
    QString cPasswordWeb;
    bool lIRPF;

    QSqlDatabase db;
    QSqlQuery *qryCliente;

public:

signals:
    
public slots:
    // Funciones de clase
    void Recuperar(QString cSQL);
    void Guardar();
    void Anadir();
    void AnadirDeuda(int id_cliente, QDate dFechaDeuda,QDate dFechaVto,QString cDocumento, int id_Tiquet,
                     int id_Factura, int nTipo, double rImporteDeuda, double rPagado, double rPendienteCobro,
                     QString cEntidad, QString cOficina,QString cDC ,QString cCuenta);
    void DescontarDeuda(int id_deuda,double rPagado);
    void Borrar(int id_cliente);
    QString NuevoCodigoCliente();
    int BuscaridPais(QString Pais);
    QString RecuperarPais(int nid);


    // Gets
    int getId();
    QString getcCodigoCliente();
    QString getcApellido1();
    QString getcApellido2();
    QString getcNombre();
    QString getcNombreFiscal();
    QString getcNombreComercial();
    QString getcPersonaContacto();
    QString getcCifNif();
    QString getcDireccion1();
    QString getcDireccion2();
    QString getcCP();
    QString getcPoblacion();
    QString getcProvincia();
    int getidPais(){return this->idPais;}
    QString getcPais();
    QString getcTelefono1();
    QString getcTelefono2();
    QString getcFax();
    QString getcMovil();
    QString getcEmail();
    QString getcWeb();
    QString getcDireccionFactura1();
    QString getcDireccionFactura2();
    QString getcCPFactura();
    QString getcPoblacionFactura();
    QString getcProvinciaFactura();
    int getidPaisFactura() {return this->idPaisFactura;}
    QString getcPaisFactura();
    QString getcDireccionAlmacen();
    QString getcDireccionAlmacen2();
    QString getcCPAlmacen();
    QString getcPoblacionAlmacen();
    QString getcProvinciaAlmacen();
    int getidPaisAlmacen() {return this->idPaisAlmacen;}
    QString getcPaisAlmacen();
    QDate getdFechaAlta();
    QDate getdFechaUltimaCompra();
    double getrAcumuladoVentas();
    double getrVentasEjercicio();
    double getrRiesgoMaximo();
    double getrDeudaActual();
    QString gettComentarios();
    int islBloqueado();
    QString gettComentarioBloqueo();
    double getnPorcDtoCliente();
    int islRecargoEquivalencia();
    QString getcCuentaContable();
    QString getcCuentaIvaRepercutido();
    QString getcCuentaDeudas();
    QString getcCuentaCobros();
    QString getcFormaPago();
    int getnDiaPago1();
    int getnDiaPago2();
    int getnTarifaCliente();
    double getrImporteACuenta();
    double getrVales();
    QString getcEntidadBancaria();
    QString getcOficinaBancaria();
    QString getcDC();
    QString getcCuentaCorriente();
    QDate getdFechaNacimiento();
    double getrImportePendiente();
    QString getcAccesoWeb();
    QString getcPasswordweb();
    bool getlIRPF();



    // Sets
    void setId(int nId);
    void setcCodigoCliente(QString cCodigo);
    void setcApellido1(QString cApellido);
    void setcApellido2(QString capellido);
    void setcNombre(QString cNombre);
    void setcNombreFiscal(QString cNombreFiscal);
    void setcNombreComercial(QString cNombreComercial);
    void setcPersonaContacto(QString cPersonaContacto);
    void setcCifNif(QString cCifNif);
    void setcDireccion1(QString cDireccion1);
    void setcDireccion2(QString cDireccion2);
    void setcCp(QString cCp);
    void setcPoblacion(QString cPoblacion);
    void setcProvincia(QString cProvincia);
    void setidPais(int nid){this->idPais = nid;}
    void setcPais(QString cPais);
    void setcTelefono1(QString cTelefono1);
    void setcTelefono2(QString cTelefono2);
    void setcFax(QString cFax);
    void setcMovil(QString cMovil);
    void setcEmail(QString cEmail);
    void setcWeb(QString cWeb);
    void setcDireccionFactura1(QString cDireccionFactura1);
    void setcDireccionFactura2(QString cDireccionFactura2);
    void setcCPFactura(QString cCPFactura);
    void setcPoblacionFactura(QString cPoblacionFactura);
    void setcProvinciaFactura(QString cProvinciaFactura);
    void setidPaisFactura(int nid){this->idPaisFactura = nid;}
    void setcPaisFactura(QString cPaisFactura);
    void setcDireccionAlmacen1(QString cDireccionAlmacen);
    void setcDireccionAlmacen2(QString cDireccionAlmacen2);
    void setcCPAlmacen(QString cCPAlmacen);
    void setcPoblacionAlmacen(QString cPoblacionAlmacen);
    void setcProvinciaAlmacen(QString cProvinciaAlmacen);
    void setidPaisAlmacen(int nid){this->idPaisAlmacen = nid;}
    void setcPaisAlmacen(QString cPaisAlmacen);
    void setdFechaAlta(QDate dFechaAlta);
    void setdFechaUltimaCompra(QDate dFechaUltimaCompra);
    void setrAcumuladoVentas(double rAcumuladoVentas);
    void setrVentasEjercicio(double rVentasEjercicio);
    void setrRiesgoMaximo(double rRiesgoMaximo);
    void setrDeudaActual(double rDeudaActual);
    void settComentarios(QString tComentarios);
    void setisBloqueado(int lBloqueado);
    void settComentarioBloqueo(QString tComentarioBloqueo);
    void setnPorcDtoCliente(double nPorcDtoCliente);
    void setislRecargoEquivalencia(int lRecargoEquivalencia);
    void setcCuentaContable(QString cCuentaContable);
    void setcCuentaIvaRepercutido(QString cCuentaIvaRepercutido);
    void setcCuentaDeudas(QString cCuentaDeudas);
    void setcCuentaCobros(QString cCuentaCobros);
    void setcFormaPago(QString cFormaPago);
    void setnDiaPago1(int nDiaPago1);
    void setnDiaPago2(int nDiaPago2);
    void setnTarifaCliente(int nTarifaCliente);
    void setrImporteaCuenta(double rImporteACuenta);
    void setrVales(double rVales);
    void setcEntidadBancaria(QString cEntidadBancaria);
    void setcOficinaBancaria(QString cOficinaBancaria);
    void setcDC(QString cDC);
    void setcCuentaCorriente(QString cCuentaCorriente);
    void setdFechaNacimiento(QDate dFechaNacimiento);
    void setrImportePendiente(double rImportePendiente);
    void setcAccesoWeb(QString cAccesoWeb);
    void setcPasswordWeb(QString cPasswordWeb);
    void setlIRPF(bool lIRPF);
};

#endif // CLIENTE_H
