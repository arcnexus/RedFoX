#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <QDate>
#include<QString>
#include <QSqlDatabase>
#include <QSqlQuery>

class Presupuesto
{
public:
    Presupuesto();
    ~Presupuesto();
private:
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
    QString cPais;
    QString cTelefono;
    QString cMovil;
    QString cFax;
    double nDto;
    QString tComentarios;
    double rImporte;
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
    QSqlQuery *qCab_pre;

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

    //setters

    void setId(int id);
    void setnPresupuesto(int nPresupuesto);
    void setdFecha(QDate dFecha);
    void setdValidoHasta(QDate dValidoHasta);
    void setidCliente(int id_cliente);
    void setcCodigoCliente(QString cCodigoCliente);
    void setcCliente(QString cCliente);
    void setcCif(QString cCif);
    void setcDireccion(QString cDireccion);
    void setcDireccion2(QString cDireccion2);
    void setcCP(QString cCP);
    void setcPoblacion(QString cPoblacion);
    void setcProvincia(QString cProvincia);
    void setcPais(QString cPais);
    void setcTelefono(QString cTelefono);
    void setcMovil(QString cMovil);
    void setcFax(QString cFax);
    void setnDto(double nDto);
    void settComentarios(QString tComentarios);
    void setrImporte(double rImporte);
    void setrSubtotal(double rSubTotal);
    void setrDescuento(double rDescuento);
    void setrTotal(double rTotal);
    void setlImpreso(int lImpreso);
    void setlAprobado(int lAprobado);
    void setdFechaAprobacion(QDate dFechaAprobacion);
    void setrImporteFactura(double rImporteFactura);
    void setrImportePendiente(double rImportePendiente);
    void setcFactura(QString cFactura);
    void setnAlbaran(int nAlbaran);
    void setnPedido(int nPedido);
    void setid_FormaPago(int id_FormaPago);
    void setcCodigoFormaPago(QString cCodigoFormaPago);
    void setcDescripcionFormaPago(QString cDescripcionFormaPago);
    void settLugarEntrega(QString tLugarEntrega);
    void setcAtencionde(QString cAtencionde);
    void setrbase1(double rBase1);
    void setrbase2(double rBase2);
    void setrbase3(double rBase3);
    void setrbase4(double rBase4);
    void setnIva1(double nIva1);
    void setnIva2(double nIva2);
    void setnIva3(double nIva3);
    void setnIva4(double nIva4);
    void setrIva1(double rIva1);
    void setrIva2(double rIva2);
    void setrIva3(double rIva3);
    void setrIva4(double rIva4);
    void setnRecargoEquivalencia1(double nRecargoEquivalencia1);
    void setnRecargoEquivalencia2(double nRecargoEquivalencia2);
    void setnRecargoEquivalencia3(double nRecargoEquivalencia3);
    void setnRecargoEquivalencia4(double nRecargoEquivalencia4);
    void setrRec1(double rRec1);
    void setrRec2(double rRec2);
    void setrRec3(double rRec3);
    void setrRec4(double rRec4);
    void setrTotal1(double rTotal1);
    void setrTotal2(double rTotal2);
    void setrTotal3(double rTotal3);
    void setrTotal4(double rTotal4);
    void setlRecargoEquivalencia(int lRecargoEquivalencia);
    void setcEmail(QString cEmail);
    void setrTotalIva(double rTotalIva);
    void setrTotalRec(double rTotalRec);


// getters
    int getId();
    int getnPresupuesto();
    QDate getdFecha();
    QDate getdValidoHasta();
    int getid_cliente();
    QString getcCodigoCliente();
    QString getcCliente();
    QString getcCif();
    QString getcDireccion();
    QString getcDireccion2();
    QString getcCP();
    QString getcPoblacion();
    QString getcProvincia();
    QString getcPais();
    QString getcTelefono();
    QString getcMovil();
    QString getcFax();
    double getnDto();
    QString gettComentarios();
    double getrImporte();
    double getrSubTotal();
    double getrDescuento();
    double getrTotal();
    bool getlImpreso();
    int getlImpresoInt();
    bool getlAprobado();
    int getlAprobadoInt();
    QDate getdFechaAprobacion();
    double getrImporteFactura();
    double getrImportePendiente();
    QString getcFactura();
    int getnAlbaran();
    int getnPedido();
    int getid_FormaPago();
    QString getcCodigoFormaPago();
    QString getcDescripcionFormaPago();
    QString gettLugarEntrega();
    QString getcAtencionde();
    double getrBase1();
    double getrBase2();
    double getrBase3();
    double getrBase4();
    double getnIva1();
    double getnIva2();
    double getnIva3();
    double getnIva4();
    double getrIva1();
    double getrIva2();
    double getrIva3();
    double getrIva4();
    double getnRecargoEquivalencia1();
    double getnRecargoEquivalencia2();
    double getnRecargoEquivalencia3();
    double getnRecargoEquivalencia4();
    double getrRec1();
    double getrRec2();
    double getrRec3();
    double getrRec4();
    double getrTotal1();
    double getrTotal2();
    double getrTotal3();
    double getrTotal4();
    bool getlRecargoEquivalencia();
    int getlRecargoEquivalenciaInt();
    QString getcEmail();
    double getrTotalIva();
    double getrTotalRec();
};

#endif // PRESUPUESTO_H
