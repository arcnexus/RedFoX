#ifndef PEDIDOPROVEEDOR_H
#define PEDIDOPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
class PedidoProveedor : public QObject
{
    Q_OBJECT
public:
    explicit PedidoProveedor(QObject *parent = 0);
    
    unsigned long	id;
    unsigned long	nPedido;
    QString	cSerie;
    QDate	dFecha;
    QDate	dRecepcion;
    unsigned long	Id_Proveedor;
    QString	cCodigoProveedor;
    QString	cProveedor;
    QString	cDireccion1;
    QString	cDireccion2;
    QString	cCP;
    QString	cPoblacion;
    QString	cProvincia;
    unsigned long	idpais;
    QString	cCifNif;
    double	rBase;
    double	rSubotal;
    double	rDto;
    double	nIVA;
    double	rRecTotal;
    double	rTotal;
    unsigned long	lEnviado;
    unsigned long	lRecibido;
    unsigned long	lRecibidoCompleto;
    unsigned long	lGeneroPendiente;
    double	rBase1;
    double	rBase2;
    double	rBase3;
    double	rBase4;
    double	nPorcIva1;
signals:
    
public slots:
    
};

#endif // PEDIDOPROVEEDOR_H
