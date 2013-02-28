#ifndef FRMORDEN_PEDIDO_PRODUCTO_H
#define FRMORDEN_PEDIDO_PRODUCTO_H

#include <QDialog>

namespace Ui {
class FrmOrden_Pedido_Producto;
}

class FrmOrden_Pedido_Producto : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmOrden_Pedido_Producto(QWidget *parent = 0);
    ~FrmOrden_Pedido_Producto();
    
private:
    Ui::FrmOrden_Pedido_Producto *ui;
};

#endif // FRMORDEN_PEDIDO_PRODUCTO_H
