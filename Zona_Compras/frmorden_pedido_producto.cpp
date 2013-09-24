#include "frmorden_pedido_producto.h"
#include "ui_frmorden_pedido_producto.h"

FrmOrden_Pedido_Producto::FrmOrden_Pedido_Producto(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmOrden_Pedido_Producto),
    menuButton(QIcon(":/Icons/PNG/opedido.png"),tr("Orden de pedido"),this)
{
    ui->setupUi(this);
}

FrmOrden_Pedido_Producto::~FrmOrden_Pedido_Producto()
{
    delete ui;
}
