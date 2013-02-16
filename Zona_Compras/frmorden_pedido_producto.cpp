#include "frmorden_pedido_producto.h"
#include "ui_frmorden_pedido_producto.h"

FrmOrden_Pedido_Producto::FrmOrden_Pedido_Producto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmOrden_Pedido_Producto)
{
    ui->setupUi(this);
}

FrmOrden_Pedido_Producto::~FrmOrden_Pedido_Producto()
{
    delete ui;
}
