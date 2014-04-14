#include "frmfacturaralabaranes.h"
#include "ui_frmfacturaralabaranes.h"

FrmFacturarAlabaranes::FrmFacturarAlabaranes(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmFacturarAlabaranes),
    menuButton(QIcon(":Icons/PNG/factmult.png"),tr("Fact. albaranes"),this)
{
    ui->setupUi(this);
}

FrmFacturarAlabaranes::~FrmFacturarAlabaranes()
{
    delete ui;
}
