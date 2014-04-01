#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class AlbaranProveedor : public QObject
{
    Q_OBJECT
public:
    explicit AlbaranProveedor(QObject *parent = 0);
    bool anadir();
    void Recuperar(int id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL, int val_accion);
    void Cargar(QSqlQuery query);
    bool guardar();
    void Recuperar();
    bool borrar(int id_alb);
    void llenarCampos();

    // CAMPOS
    int id;
    QString	albaran;
    QDate	fecha;
    int	    id_proveedor;
    QString	proveedor;
    QString	cif_proveedor;
    double	total1;
    double	total2;
    double	total3;
    double	total4;
    double	porc_iva1;
    double	porc_iva2;
    double	porc_iva3;
    double	porc_iva4;
    double	iva1;
    double	iva2;
    double	iva3;
    double	iva4;
    double	base1;
    double	base2;
    double	base3;
    double	base4;
    QString	factura;
    double	base_total;
    double	iva_total;
    double	total;
    QString	comentario;
    int 	pedido;
    bool	recargo_equivalencia;
    double	porc_rec1;
    double	porc_rec2;
    double	porc_rec3;
    double	porc_rec4;
    double	rec1;
    double	rec2;
    double	rec3;
    double	rec4;
    double	total_recargo;
    int 	ejercicio;
    double	subtotal;
    QString	direccion1;
    QString	direccion2;
    QString	cp;
    QString	poblacion;
    QString	provincia;
    int 	id_pais;
    double	porc_dto;
    double	dto;
    QString	telefono;
    QString	fax;
    QString	movil;
    QString	codigo_proveedor;
    int 	id_forma_pago;
    bool	impreso;
    QString	desc_gasto1;
    QString	desc_gasto2;
    QString	desc_gasto3;
    double	imp_gasto1;
    double	imp_gasto2;
    double	imp_gasto3;
    double	porc_iva_gasto1;
    double	porc_iva_gasto2;
    double	porc_iva_gasto3;
    double	iva_gasto1;
    double	iva_gasto2;
    double	iva_gasto3;
    bool	gasto_to_coste;
    bool	editable;

signals:
    
public slots:

private:
    int accion;
    
};

#endif // ALBARANPROVEEDOR_H

/*
albaran
fecha
id_proveedor
proveedor
cif_proveedor
total1
total2
total3
total4
porc_iva1
porc_iva2
porc_iva3
porc_iva4
iva1
iva2
iva3
iva4
base1
base2
base3
base4
factura
base_total
iva_total
total
comentario
pedido
recargo_equivalencia
porc_rec1
porc_rec2
porc_rec3
porc_rec4
rec1
rec2
rec3
rec4
total_recargo
ejercicio
subtotal
direccion1
direccion2
cp
poblacion
provincia
id_pais
porc_dto
dto
telefono
fax
movil
codigo_proveedor
id_forma_pago
impreso
desc_gasto1
desc_gasto2
desc_gasto3
imp_gasto1
imp_gasto2
imp_gasto3
porc_iva_gasto1
porc_iva_gasto2
porc_iva_gasto3
iva_gasto1
iva_gasto2
iva_gasto3
gasto_to_coste
editable


*/
