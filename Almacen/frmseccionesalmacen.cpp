#include "frmseccionesalmacen.h"
#include "ui_frmseccionesalmacen.h"

frmSeccionesAlmacen::frmSeccionesAlmacen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSeccionesAlmacen)
{
    ui->setupUi(this);
}

frmSeccionesAlmacen::~frmSeccionesAlmacen()
{
    delete ui;
}
