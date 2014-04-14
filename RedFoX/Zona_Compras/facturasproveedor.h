#ifndef FACTURASPROVEEDOR_H
#define FACTURASPROVEEDOR_H

#include <QObject>
#include "Auxiliares/Globlal_Include.h"

class FacturasProveedor : public QObject
{
    Q_OBJECT
public:
    explicit FacturasProveedor(QObject *parent = 0);

    bool anadir_factura();
    bool guardar_factura();
    bool recuperar_factura(QString cSQL,int accion);
    bool Recuperar(int id_factura);


    // Campos
    int	id;
    QString	factura;
    QDate	fecha;
    QDate	fecha_recepcion;
    QString	pedido;
    int	    id_proveedor;
    QString	proveedor;
    QString	cif_proveedor;
    QString	direccion1;
    QString	direccion2;
    QString	cp;
    QString	poblacion;
    QString	provincia;
    int	    id_pais;
    QString	telefono;
    QString	movil;
    QString	fax;
    double	porc_dto;
    double	porc_irpf;
    double	recargo_equivalencia;
    double	base1;
    double	base2;
    double	base3;
    double	base4;
    double	porc_iva1;
    double	porc_iva2;
    double	porc_iva3;
    double	porc_iva4;
    double	iva1;
    double	iva2;
    double	iva3;
    double	iva4;
    double	porc_rec1;
    double	porc_rec2;
    double	porc_rec3;
    double	porc_rec4;
    double	rec1;
    double	rec2;
    double	rec3;
    double	rec4;
    double	total1;
    double	total2;
    double	total3;
    double	total4;
    double	base_total;
    double	iva_total;
    double	irpf;
    double	total_recargo;
    double	total;
    int     id_forma_pago;
    int     id_tipo_gasto;
    QString	comentario;
    bool	pagado;
    QString	albaran;
    QString	desc_gasto1;
    QString	desc_gasto2;
    QString	desc_gasto3;
    double	imp_gasto1;
    double	imp_gasto2;
    double	imp_gasto3;
    bool	gasto_to_coste;
    double	porc_iva_gasto1;
    double	porc_iva_gasto2;
    double	porc_iva_gasto3;
    double	iva_gasto1;
    double	iva_gasto2;
    double	iva_gasto3;
    double	dto;
    int 	ejercicio;
    bool	contabilizada;
    double	subtotal;
    QString	codigo_proveedor;
    bool	impreso;
    bool	editable;
 private:
    bool cargar_factura(QSqlQuery queryFact, int accion);
    
signals:
    
public slots:
    
};

#endif // FACTURASPROVEEDOR_H
