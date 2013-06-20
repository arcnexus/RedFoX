#include "frmorden_pedido_producto.h"
#include "ui_frmorden_pedido_producto.h"

FrmOrden_Pedido_Producto::FrmOrden_Pedido_Producto(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmOrden_Pedido_Producto),
    toolButton(tr("Orden \nde pedido"),":/Icons/PNG/opedido.png",this),
    menuButton(QIcon(":/Icons/PNG/opedido.png"),tr("Orden de pedido"),this)
{
    ui->setupUi(this);
}

FrmOrden_Pedido_Producto::~FrmOrden_Pedido_Producto()
{
    delete ui;
}
