#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"

FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmPresupuestosCli)
{
    ui->setupUi(this);
}

FrmPresupuestosCli::~FrmPresupuestosCli()
{
    delete ui;
}

void FrmPresupuestosCli::on_chklEnviado_stateChanged(int arg1)
{
    if(arg1 == 0) {
        ui->lblFechaAprovacin->setVisible(false);
        ui->txtdFechaAprovacion->setVisible(false);
    } else {
        ui->lblFechaAprovacin->setVisible(true);
        ui->txtdFechaAprovacion->setVisible(true);
    }
}
