#ifndef FRMPEDIDOSPROVEEDOR_H
#define FRMPEDIDOSPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
#include "pedidoproveedor.h"
namespace Ui {
class FrmPedidosProveedor;
}

class FrmPedidosProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmPedidosProveedor(QWidget *parent = 0);
    ~FrmPedidosProveedor();
private slots:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

private:
    PedidoProveedor pedido;
    Ui::FrmPedidosProveedor *ui;
    Table_Helper helper;
};

#endif // FRMPEDIDOSPROVEEDOR_H
