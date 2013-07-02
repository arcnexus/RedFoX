#ifndef PEDidOPROVEEDOR_H
#define PEDidOPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
class PedidoProveedor : public QObject
{
    Q_OBJECT
public:
    explicit PedidoProveedor(QObject *parent = 0);
    
    int	id;
    int 	pedido;
    int     ejercicio;
    QDate	fecha;
    QDate	recepcion;
    int 	id_proveedor;
    QString	codigoProveedor;
    QString	proveedor;
    QString	direccion1;
    QString	direccion2;
    QString	cp;
    QString	poblacion;
    QString	provincia;
    int  	id_pais;
    QString	cif_nif;
    double	base_total;
    double	subtotal;
    double	dto;
    double	iva;
    double	recTotal;
    double	total;
    bool	enviado;
    bool	recibido;
    bool	recibido_completo;
    bool    genero_pendiente;
    bool    recargo_equivalencia;
    double	base1;
    double	base2;
    double	base3;
    double	base4;

    bool traspasado;
    int pedido_cliente;
    int id_forma_pago;
    QString comentario;
    QDate fecha_entrega;
    QString direccion1_entrega;
    QString direccion2_entrega;
    QString cp_entrega;
    QString poblacion_entrega;
    QString provincia_entrega;
    int id_pais_entrega;
    QString nombre_cliente;
    QString horario_activo;

    float porc_iva1;
    float porc_iva2;
    float porc_iva3;
    float porc_iva4;
    double iva1;
    double iva2;
    double iva3;
    double iva4;
    double dto1;
    double dto2;
    double dto3;
    double dto4;
    float porc_rec1;
    float porc_rec2;
    float porc_rec3;
    float porc_rec4;
    double rec1;
    double rec2;
    double rec3;
    double rec4;
    double total1;
    double total2;
    double total3;
    double total4;


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
    void imprimir(int id);

    

    bool get(int id);
    bool next();
    bool prev();
    long save();
    bool update();
signals:
    
public slots:
private:
    void fillPedido(QSqlRecord r);

};

#endif // PEDidOPROVEEDOR_H
