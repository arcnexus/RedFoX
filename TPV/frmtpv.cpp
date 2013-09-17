#include "frmtpv.h"
#include "ui_frmtpv.h"
#include <QGraphicsOpacityEffect>

FrmTPV::FrmTPV(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTPV),
    toolButton(tr("TPV"),":/Icons/PNG/tpv.png",this),
    menuButton(QIcon(":/Icons/PNG/tpv.png"),tr("TPV"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/tpv.png"),"",this))
{
    ui->setupUi(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de Caja"));
  //  ui->frameBuscar->setVisible(false);
}

FrmTPV::~FrmTPV()
{
    delete ui;
}

void FrmTPV::on_btnClientes_clicked()
{

}



void FrmTPV::on_btnBuscar_clicked()
{




}



void FrmTPV::on_btnCambioModo_clicked()
{
    if(ui->frmcontrol->currentIndex() == 1)
        ui->frmcontrol->setCurrentIndex(0);
    else
        ui->frmcontrol->setCurrentIndex(1);
}
