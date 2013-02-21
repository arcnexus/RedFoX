#ifndef CLIENTE_H
#define CLIENTE_H

#include "../Auxiliares/Globlal_Include.h"
class Cliente : public QObject
{
    Q_OBJECT

public:
    explicit Cliente(QObject *parent = 0);
        QSqlQueryModel *model;
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
    int idTarifa;

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
    void AnadirPersonaContacto(int Id,QString Nombre, QString descTelefono1, QString Telefono1, QString descTelefono2,
                               QString Telefono2,QString descTelefono3, QString Telefono3,
                               QString descMovil1, QString Movil1, QString descMovil2, QString Movil2);
    void GuardarDireccion(bool Anadir, QString Descripcion, QString Direccion1, QString Direccion2, QString CP, QString Poblacion,
                         QString Provincia, QString Pais, int idcliente, int id);
    void DescontarDeuda(int id_deuda,double rPagado);
    void Borrar(int id_cliente);
    QString NuevoCodigoCliente();
    int BuscaridPais(QString Pais);
    QString RecuperarPais(int nid);

};

#endif // CLIENTE_H
