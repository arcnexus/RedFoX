#include "frmfacturaralabaranes.h"
#include "ui_frmfacturaralabaranes.h"

FrmFacturarAlabaranes::FrmFacturarAlabaranes(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmFacturarAlabaranes),
    toolButton(tr("Fact. \nalbaranes"),":Icons/PNG/factmult.png",this),
    menuButton(QIcon(":Icons/PNG/factmult.png"),tr("Fact. albaranes"),this)
{
    ui->setupUi(this);
}

FrmFacturarAlabaranes::~FrmFacturarAlabaranes()
{
    delete ui;
}
