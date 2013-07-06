#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class AlbaranProveedor : public QObject
{
    Q_OBJECT
public:
    explicit AlbaranProveedor(QObject *parent = 0);
    int anadir();
    void Recuperar(int id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL, int val_accion);
    void Cargar(QSqlQuery query);
    void guardar();
    void Recuperar();
    bool borrar(int id_alb);
    void llenarCampos();

    // -------
    // CAMPOS
    //--------
    int id;
    QString albaran;
    QDate fecha;
    int id_proveedor;
    QString proveedor;
    QString cif_proveedor;
    double total1,total2,total3,total4;
    double iva1,iva2,iva3,iva4;
    double rec1,rec2,rec3,rec4;
    float porc_iva1,porc_iva2,porc_iva3,porc_iva4;
    float porc_rec1,porc_rec2,porc_rec3,porc_rec4;
    double base1,base2,base3,base4;
    QString factura;
    double base_total,iva_total,total,importe_rec_total;
    QString comentario;
    int pedido;
signals:
    
public slots:
private:
    int accion;
    
};

#endif // ALBARANPROVEEDOR_H
