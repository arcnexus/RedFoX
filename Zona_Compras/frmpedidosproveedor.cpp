#include "frmpedidosproveedor.h"
#include "ui_frmpedidosproveedor.h"

FrmPedidosProveedor::FrmPedidosProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmPedidosProveedor)
{
    ui->setupUi(this);
}

FrmPedidosProveedor::~FrmPedidosProveedor()
{
    delete ui;
}
