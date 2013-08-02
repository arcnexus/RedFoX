#include "frminventario.h"
#include "ui_frminventario.h"

frmInventario::frmInventario(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmInventario),
  toolButton(tr("Almacen"),":/Icons/PNG/inventario.png",this),
  menuButton(QIcon(":/Icons/PNG/inventario.png"),tr("Inventario"),this),
  shortCut(new QPushButton(QIcon(":/Icons/PNG/inventario.png"),"",this))
{
    ui->setupUi(this);
}

frmInventario::~frmInventario()
{
    delete ui;
}
