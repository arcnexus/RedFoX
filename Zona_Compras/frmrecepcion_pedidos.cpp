#include "frmrecepcion_pedidos.h"
#include "ui_frmrecepcion_pedidos.h"

Frmrecepcion_pedidos::Frmrecepcion_pedidos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Frmrecepcion_pedidos)
{
    ui->setupUi(this);
}

Frmrecepcion_pedidos::~Frmrecepcion_pedidos()
{
    delete ui;
}
