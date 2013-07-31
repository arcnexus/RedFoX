#ifndef PROVEEDOR_H
#define PROVEEDOR_H

#include "../Auxiliares/Globlal_Include.h"

class Proveedor : public QObject
{
    Q_OBJECT
public:
    explicit Proveedor(QObject *parent = 0);

    int id;
    QString codigo;
    QString proveedor;
    QString cif;
    QString direccion1;
    QString direccion2;
    QString cp;
    QString poblacion;
    QString provincia;
    int id_pais;
    QString pais;
    QString telefono1;
    QString telefono2;
    QString telefono3;
    QString fax;
    QString movil;
    QString email;
    QString web;
    QString persona_contacto;
    int dia_cobro;
    QString direccion_almacen;
    QString cp_almacen;
    QString poblacion_almacen;
    QString provincia_almacen;
    int id_pais_almacen;
    QString paisAlmacen;
    QString telefono_almacen;
    QString fax_almacen;
    int idFormadePago;
    QString codigoFormaPago;
    QDate fecha_ultima_compra;
    double importe_acumulado_compras;
    QString entidad_bancaria_proveedor;
    QString oficina_bancaria_proveedor;
    QString dc_proveedor;
    QString cc_proveedor;
    QString entidad_pago_proveedor;
    QString oficina_pago_proveedor;
    QString dc_pago_proveedor;
    double retencion_irpf;
    int tipo_retencion;
    QString cuenta_aplicacion;
    QString comentarios;
    double dto;
    QDate fecha_alta;
    double deuda_maxima;
    double deuda_actual;
    int recargo_equivalencia;
    QString texto_para_pedidos;
    double entregado_a_cuenta;

    // Acumulados
    double enero,febrero,marzo,abril,mayo,junio,julio,agosto,septiembre,octubre,noviembre,diciembre;
    void cargaracumulados(int id_proveedor);
    bool acumular(int id_proveedor, int mes, double importe);
    void anadir_persona_contacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                 QString desc_telefono2, QString Telefono2, QString desc_telefono3, QString Telefono3,
                                 QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2, QString cargo, QString email);
    void guardar_persona_contacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1,
                                  QString desc_telefono2, QString Telefono2, QString desc_telefono3, QString Telefono3,
                                  QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2,QString cargo,QString email);

signals:
    
public slots:
    // Funcionamiento Base
    void Anadir();
    void Recuperar(int id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Cargar(QSqlRecord &rProveedor);
    void Guardar();
    void Vaciar();
    void Borrar(int nid);
    void clear();
    QString Nuevocodigo_proveedor();


};

#endif // PROVEEDOR_H
