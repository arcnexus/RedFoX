#include "frmtarifas.h"
#include "ui_frmtarifas.h"

FrmTarifas::FrmTarifas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTarifas)
{
    ui->setupUi(this);
}

FrmTarifas::~FrmTarifas()
{
    delete ui;
}
