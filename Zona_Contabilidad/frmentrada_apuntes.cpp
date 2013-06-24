#include "frmentrada_apuntes.h"
#include "ui_frmentrada_apuntes.h"

FrmEntrada_apuntes::FrmEntrada_apuntes(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmEntrada_apuntes),
    toolButton(tr("Diario"),":/Icons/PNG/cuentas.png",this),
    menuButton(QIcon(":/Icons/PNG/cuentas.png"),tr("Diario"),this)
{
    ui->setupUi(this);
}

FrmEntrada_apuntes::~FrmEntrada_apuntes()
{
    delete ui;
}
