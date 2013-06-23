#include "frmcuadro_cuentas.h"
#include "ui_frmcuadro_cuentas.h"

frmCuadro_cuentas::frmCuadro_cuentas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCuadro_cuentas)
{
    ui->setupUi(this);
}

frmCuadro_cuentas::~frmCuadro_cuentas()
{
    delete ui;
}
