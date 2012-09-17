#ifndef FACTURA_H
#define FACTURA_H

#include <QObject>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

class Factura : public QObject
{
    Q_OBJECT
public:
    explicit Factura(QObject *parent = 0);

private:
    int id;
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
    QString cPais;
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
    QSqlDatabase db;
    QSqlQuery *cab_fac;


    
signals:

public slots:
    // Metodos de trabajo Clase Factura
    void AnadirFactura();
    void RecuperarFactura(QString cSQL);
    void GuardarFactura(int nId_Factura, bool FacturaLegal);
    QString NuevoNumeroFactura();
    void AnadirLineaFactura(int id_cab, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                            double porcdto, double dto, double total, double nPorcIva);
    void ModificarLineaFactura(int id_lin, QString cCodigo, double nCantidad, QString cDescripcion, double pvp, double subtotal,
                            double porcdto, double dto, double total, double nPorcIva);
    void BorrarLineaFactura(int id_lin);
    void calcularFactura();
    void CobrarFactura();

    // getters
    int Getid();
    QString getcCodigoCliente();
    QString getcFactura();
    QDate getdFecha();
    QDate getdFechaCobro();
    int getiId_Cliente();
    QString getcCliente();
    QString getcDireccion();
    QString getcDireccion2();
    QString getcCp();
    QString getcPoblacion();
    QString getcProvincia();
    QString getcPais();
    QString getcCif();
    int getlRecargoEquivalencia();
    double getrSubtotal();
    int getnDto();
    int getnDtoPP();
    double getrImporteDescuento();
    double getrImporteDescuentoPP();
    double getrBase();
    int getnIva();
    double getrImporteIva();
    double getrTotal();
    int getlImpresa();
    int getlCobrada();
    int getlContablilizada();
    int getid_FormaPago();
    QString getcFormaPago();
    QString gettComentario();
    double getrBase1();
    double getrBase2();
    double getrBase3();
    double getrBase4();
    int getnPorcentajeIVA1();
    int getnPorcentajeIVA2();
    int getnPorcentajeIVA3();
    int getnPorcentajeIVA4();
    double getrIVA1();
    double getrIVA2();
    double getrIVA3();
    double getrIVA4();
    double getrTotal1();
    double getrTotal2();
    double getrTotal3();
    double getrTotal4();
    double getnRec1();
    double getnRec2();
    double getnRec3();
    double getnRec4();
    double getrRecargoEq1();
    double getrRecargoEq2();
    double getrRecargoEq3();
    double getrRecargoEq4();
    double getrTotalRecargoEq();
    double getrEntregadoaCuenta();
    double getrImportePendiente();
    QString getcCodigoEntidad();
    QString getcOficinaEntidad();
    QString getcDCCuenta();
    QString getcNumeroCuenta();
    QString getcPedidoCliente();
    int getnIRPF();
    double getrImporteIRPF();


    // setters
    void setid( int iID_Factura);
    void setcCodigoCliente(QString cCodigoCliente);
    void setcFactura( QString cFactura);
    void setdFecha(QDate dFecha);
    void setdFechaCobro(QDate dFechaCobro);
    void setiId_Cliente(int iId_Cliente);
    void setcCliente(QString cCliente);
    void setcDireccion(QString cDireccion);
    void setcDireccion2(QString cDireccion2);
    void setcCp(QString cCp);
    void setcPoblacion(QString cPoblacion);
    void setcProvincia(QString cProvincia);
    void setcPais(QString cPais);
    void setcCif(QString cCif);
    void setlRecargoEquivalencia(int lRecargoEquivalencia);
    void setrSubtotal(double rSubtotal);
    void setnDto(int nDto);
    void setnDtoPP(int nDtoPP);
    void setrImporteDescuento(double rImporteDescuento);
    void setrImporteDescuentoPP(double rImporteDescuentoPP);
    void setrBase(double rBase);
    void setnIva(int nIva);
    void setrImporteIva(double rImporteIva);
    void setrTotal(double rTotal);
    void setlImpresa(int lImpresa);
    void setlCobrada(int lCobrada);
    void setlContablilizada(int lContablilizada);
    void setid_FormaPago(int id_FormaPago);
    void setcFormaPago(QString cFormaPago);
    void settComentario(QString tComentario);
    void setrBase1(double rBase1);
    void setrBase2(double rBase2);
    void setrBase3(double rBase3);
    void setrBase4(double rBase4);
    void setnPorcentajeIVA1(int nPorcentajeIVA1);
    void setnPorcentajeIVA2(int nPorcentajeIVA2);
    void setnPorcentajeIVA3(int nPorcentajeIVA3);
    void setnPorcentajeIVA4(int nPorcentajeIVA4);
    void setrIVA1( double rIVA1);
    void setrIVA2( double rIVA2);
    void setrIVA3( double rIVA3);
    void setrIVA4( double rIVA4);
    void setrTotal1(double rTotal1);
    void setrTotal2(double rTotal2);
    void setrTotal3(double rTotal3);
    void setrTotal4(double rTotal4);
    void setnRec1(int nRec1);
    void setnRec2(int nRec2);
    void setnRec3(int nRec3);
    void setnRec4(int nRec4);
    void setrRecargoEq1(double rRecargoEq1);
    void setrRecargoEq2(double rRecargoEq2);
    void setrRecargoEq3(double rRecargoEq3);
    void setrRecargoEq4(double rRecargoEq4);
    void setrTotalRecargoEq(double rTotalRecargoEq);
    void setrEntregadoaCuenta(double rEntregadoaCuenta);
    void setrImportePendiente(double rImportePendiente);
    void setcCodigoEntidad(QString cCodigoEntidad);
    void setcOficinaEntidad(QString cOficinaEntidad);
    void setcDCCuenta(QString cDCCuenta);
    void setcNumeroCuenta(QString cNumeroCuenta);
    void setcPedidoCliente(QString cPedidoCliente);
    void setnIRPF(int nIRPF);
    void setrImporteIRPF(double rImporteIRPF);
    
};

#endif // FACTURA_H
