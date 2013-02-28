#ifndef TICKET_H
#define TICKET_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
#include "lineaticket.h"
class Ticket : public QObject
{
    Q_OBJECT
public:
    explicit Ticket(QObject *parent = 0);

    void set_table(QTableWidget * table);
    void add_linea(
            QString cCodigo,
            QString cDescripcion,
            double 	rPvp,
            double 	nCantidad,
            double 	rImporte,
            double 	nPorcentajeIva,
            double 	nPorcDto,
            double 	nTotal,
            double 	rSubTotal,
            QDate 	dFechaLinea);

    void remove_linea(int row);

    bool save_Ticket(int ID);
    
    int 	Id;
    int 	nCaja;
    int 	nTicket;
    QDate 	dFecha;
    QString 	cHora;
    int 	idpersonalemp;
    int 	id_Cliente;
    QString 	cCliente;
    QString 	cNombreCliente;
    int 	lticketEnEpera;
    int 	lPendienteCobro;
    int 	idFormaPago;
    QString 	cTipoTarjeta;
    int 	lImpreso;
    double 	rImporte;
    double 	rBase1;
    double 	rBase2;
    double 	rBase3;
    double 	rBase4;
    double 	nPorcentajeIva1;
    double 	nPorcentajeIva2;
    double 	nPorcentajeIva3;
    double 	nPorcentajeIva4;
    double 	rImporteIva1;
    double 	rImporteIva2;
    double 	rImporteIva3;
    double 	rImporteIva4;
    double 	rTotal1;
    double 	rTotal2;
    double 	rTotal3;
    double 	rTotal4;
    double 	rSubotal;
    double 	rDto;
    double 	nPorcentajeRe1;
    double 	nPorcentajeRe2;
    double 	nPorcentajeRe3;
    double 	nPorcentajeRe4;
    double 	rImporteRecargoEq1;
    double 	rImporteRecargoEq2;
    double 	rImporteRecargoEq3;
    double 	rImporteRecargoEq4;
    double 	rTotalRecargoEq;
    double 	rImporteTotalIva;
    double 	rCobrado;
    double 	rPendienteCobro;
    double 	rImporteEfectivo;
    double 	rImporteTarjeta;
    double 	rImporteCheque;
    double 	rImporteCredito;
    double 	rImporteDtoRedondeo;
    double 	rImporteCambio;
    double 	rEntregado;
    int 	lPasadoAlbaranFactura;
    int 	nNumeroAlbaran;
    int 	nNumeroFactura;
    int 	id_cierre;
    int 	id_Dependient;
signals:
    
public slots:
    bool next();
    bool prev();
private:
    QVector<LineaTicket*> lineas;
    QTableWidget * table;
};

#endif // TICKET_H
