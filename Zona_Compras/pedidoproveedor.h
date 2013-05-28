#ifndef PEDIDOPROVEEDOR_H
#define PEDIDOPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
class PedidoProveedor : public QObject
{
    Q_OBJECT
public:
    explicit PedidoProveedor(QObject *parent = 0);
    
    int	id;
    int 	nPedido;
    int     nEjercicio;
    QDate	dFecha;
    QDate	dRecepcion;
    int 	Id_Proveedor;
    QString	cCodigoProveedor;
    QString	cProveedor;
    QString	cDireccion1;
    QString	cDireccion2;
    QString	cCP;
    QString	cPoblacion;
    QString	cProvincia;
    int  	idpais;
    QString	cCifNif;
    double	rBaseTotal;
    double	rSubotal;
    double	rDto;
    double	nIVA;
    double	rRecTotal;
    double	rTotal;
    bool	lEnviado;
    bool	lRecibido;
    bool	lRecibidoCompleto;
    bool    lGeneroPendiente;
    double	rBase1;
    double	rBase2;
    double	rBase3;
    double	rBase4;
    double	nPorcIva1;

    bool lTraspasado;
    int nPedidoCliente;
    int Id_FormaPago;
    QString tComentario;
    QDate dFechaEntrega;
    QString cDireccion1Entrega;
    QString cDireccion2Entrega;
    QString cCPEntrega;
    QString cPoblacionEntrega;
    QString cProvinciaEntrega;
    int id_paisEntrega;
    QString cNombreCliente;
    QString cHorarioActivo;

    int niva1;
    int niva2;
    int niva3;
    int niva4;
    double riva1;
    double riva2;
    double riva3;
    double riva4;
    double rdto1;
    double rdto2;
    double rdto3;
    double rdto4;
    int nrec1;
    int nrec2;
    int nrec3;
    int nrec4;
    double rrec1;
    double rrec2;
    double rrec3;
    double rrec4;
    double rtotal1;
    double rtotal2;
    double rtotal3;
    double rtotal4;


signals:
    
public slots:
    int nuevo_pedido_proveedor();
    void guardar();
    void recuperar(int id);
    void recuperar(QString cadenaSQL);
    void recuperar(QString cadenaSQL,int accion);
    void cargar(QSqlQuery *queryPedido, int accion);
    void clear();
    void convertir_en_albaran();
    void convertir_en_factura();
    bool borrar(int id);

    
};

#endif // PEDIDOPROVEEDOR_H
