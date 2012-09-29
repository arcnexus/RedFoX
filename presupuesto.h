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

    int nAlbaran;

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
