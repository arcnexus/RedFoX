#include "frmcajas.h"
#include "ui_frmcajas.h"

FrmCajas::FrmCajas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmCajas),
    menuButton(QIcon(":/Iconos/PNG/avisos.png"),tr("Cajas"),this)

{
    ui->setupUi(this);
}

FrmCajas::~FrmCajas()
{
    delete ui;
}
