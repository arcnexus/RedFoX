#include "frmalbaranproveedor.h"
#include "ui_frmalbaranproveedor.h"

FrmAlbaranProveedor::FrmAlbaranProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAlbaranProveedor)
{
    ui->setupUi(this);
}

FrmAlbaranProveedor::~FrmAlbaranProveedor()
{
    delete ui;
}
