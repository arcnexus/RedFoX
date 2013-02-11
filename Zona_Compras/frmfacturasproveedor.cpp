#include "frmfacturasproveedor.h"
#include "ui_frmfacturasproveedor.h"

FrmFacturasProveedor::FrmFacturasProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFacturasProveedor)
{
    ui->setupUi(this);
}

FrmFacturasProveedor::~FrmFacturasProveedor()
{
    delete ui;
}
